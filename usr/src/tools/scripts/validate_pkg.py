#! PYTHON
#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

#
# Compare the content generated by a build to a set of manifests
# describing how that content is to be delivered.
#


import getopt
import os
import stat
import sys

from pkg import actions
from pkg import manifest


#
# Dictionary used to map action names to output format.  Each entry is
# indexed by action name, and consists of a list of tuples that map
# FileInfo class members to output labels.
#
OUTPUTMAP = {
    "dir": [
        ("group", "group="),
        ("mode", "mode="),
        ("owner", "owner="),
        ("path", "path=")
    ],
    "file": [
        ("hash", ""),
        ("group", "group="),
        ("mode", "mode="),
        ("owner", "owner="),
        ("path", "path=")
    ],
    "link": [
        ("mediator", "mediator="),
        ("path", "path="),
        ("target", "target=")
    ],
    "hardlink": [
        ("path", "path="),
        ("hardkey", "target=")
    ],
}

# Mode checks used to validate safe file and directory permissions
ALLMODECHECKS = frozenset(("m", "w", "s", "o"))
DEFAULTMODECHECKS = frozenset(("m", "w", "o"))

class FileInfo(object):
    """Base class to represent a file.

    Subclassed according to whether the file represents an actual filesystem
    object (RealFileInfo) or an IPS manifest action (ActionInfo).
    """

    def __init__(self):
        self.path = None
        self.isdir = False
        self.target = None
        self.owner = None
        self.group = None
        self.mode = None
        self.hardkey = None
        self.hardpaths = set()
        self.editable = False

    def name(self):
        """Return the IPS action name of a FileInfo object.
        """
        if self.isdir:
            return "dir"

        if self.target:
            return "link"

        if self.hardkey:
            return "hardlink"

        return "file"

    def checkmodes(self, modechecks):
        """Check for and report on unsafe permissions.

        Returns a potentially empty list of warning strings.
        """
        w = []

        t = self.name()
        if t in ("link", "hardlink"):
            return w
        m = int(self.mode, 8)
        o = self.owner
        p = self.path

        if "s" in modechecks and t == "file":
            if m & (stat.S_ISUID | stat.S_ISGID):
                if m & (stat.S_IRGRP | stat.S_IROTH):
                    w.extend(["%s: 0%o: setuid/setgid file should not be " \
                        "readable by group or other" % (p, m)])

        if "o" in modechecks and o != "root" and ((m & stat.S_ISUID) == 0):
            mu = (m & stat.S_IRWXU) >> 6
            mg = (m & stat.S_IRWXG) >> 3
            mo = m & stat.S_IRWXO
            e = self.editable

            if (((mu & 02) == 0 and (mo & mg & 04) == 04) or
                (t == "file" and mo & 01 == 1) or
                (mg, mo) == (mu, mu) or
                ((t == "file" and not e or t == "dir" and o == "bin") and
                (mg & 05 == mo & 05)) or
                (t == "file" and o == "bin" and mu & 01 == 01) or
                (m & 0105 != 0 and p.startswith("etc/security/dev/"))):
                w.extend(["%s: owner \"%s\" may be safely " \
                    "changed to \"root\"" % (p, o)])

        if "w" in modechecks and t == "file" and o != "root":
            uwx = stat.S_IWUSR | stat.S_IXUSR
            if m & uwx == uwx:
                w.extend(["%s: non-root-owned executable should not " \
                    "also be writable by owner." % p])

        if ("m" in modechecks and
            m & (stat.S_IWGRP | stat.S_IWOTH) != 0 and
            m & stat.S_ISVTX == 0):
            w.extend(["%s: 0%o: should not be writable by group or other" %
                (p, m)])

        return w

    def __ne__(self, other):
        """Compare two FileInfo objects.

        Note this is the "not equal" comparison, so a return value of False
        indicates that the objects are functionally equivalent.
        """
        #
        # Map the objects such that the lhs is always the ActionInfo,
        # and the rhs is always the RealFileInfo.
        #
        # It's only really important that the rhs not be an
        # ActionInfo; if we're comparing FileInfo the RealFileInfo, it
        # won't actually matter what we choose.
        #
        if isinstance(self, ActionInfo):
            lhs = self
            rhs = other
        else:
            lhs = other
            rhs = self

        #
        # Because the manifest may legitimately translate a relative
        # path from the proto area into a different path on the installed
        # system, we don't compare paths here.  We only expect this comparison
        # to be invoked on items with identical relative paths in
        # first place.
        #

        #
        # All comparisons depend on type.  For symlink and directory, they
        # must be the same.  For file and hardlink, see below.
        #
        typelhs = lhs.name()
        typerhs = rhs.name()
        if typelhs in ("link", "dir"):
            if typelhs != typerhs:
                return True

        #
        # For symlinks, all that's left is the link target.
        # For mediated symlinks targets can differ.
        #
        if typelhs == "link":
            return (lhs.mediator is None) and (lhs.target != rhs.target)

        #
        # For a directory, it's important that both be directories,
        # the modes be identical, and the paths are identical.  We already
        # checked all but the modes above.
        #
        # If both objects are files, then we're in the same boat.
        #
        if typelhs == "dir" or (typelhs == "file" and typerhs == "file"):
            return lhs.mode != rhs.mode

        #
        # For files or hardlinks:
        #
        # Since the key space is different (inodes for real files and
        # actual link targets for hard links), and since the proto area will
        # identify all N occurrences as hardlinks, but the manifests as one
        # file and N-1 hardlinks, we have to compare files to hardlinks.
        #

        #
        # If they're both hardlinks, we just make sure that
        # the same target path appears in both sets of
        # possible targets.
        #
        if typelhs == "hardlink" and typerhs == "hardlink":
            return len(lhs.hardpaths.intersection(rhs.hardpaths)) == 0

        #
        # Otherwise, we have a mix of file and hardlink, so we
        # need to make sure that the file path appears in the
        # set of possible target paths for the hardlink.
        #
        # We already know that the ActionInfo, if present, is the lhs
        # operator.  So it's the rhs operator that's guaranteed to
        # have a set of hardpaths.
        #
        return lhs.path not in rhs.hardpaths

    def __str__(self):
        """Return an action-style representation of a FileInfo object.

        We don't currently quote items with embedded spaces.  If we
        ever decide to parse this output, we'll want to revisit that.
        """
        name = self.name()
        out = name

        for member, label in OUTPUTMAP[name]:
            out += " " + label + str(getattr(self, member))

        return out

    def protostr(self):
        """Return a protolist-style representation of a FileInfo object.
        """
        target = "-"
        major = "-"
        minor = "-"

        mode = self.mode
        owner = self.owner
        group = self.group

        name = self.name()
        if name == "dir":
            ftype = "d"
        elif name in ("file", "hardlink"):
            ftype = "f"
        elif name == "link":
            ftype = "s"
            target = self.target
            mode = "777"
            owner = "root"
            group = "other"

        out = "%c %-30s %-20s %4s %-5s %-5s %6d %2ld  -  -" % \
            (ftype, self.path, target, mode, owner, group, 0, 1)

        return out


class ActionInfo(FileInfo):
    """Object to track information about manifest actions.

    This currently understands file, link, dir, and hardlink actions.
    """

    def __init__(self, action):
        FileInfo.__init__(self)
        #
        # Currently, all actions that we support have a "path"
        # attribute.  If that changes, then we'll need to
        # catch a KeyError from this assignment.
        #
        self.path = action.attrs["path"]

        if action.name == "file":
            self.owner = action.attrs["owner"]
            self.group = action.attrs["group"]
            self.mode = action.attrs["mode"]
            self.hash = action.hash
            if "preserve" in action.attrs:
                self.editable = True
        elif action.name == "link":
            target = action.attrs["target"]
            self.target = os.path.normpath(target)
            self.mediator = action.attrs.get("mediator")
        elif action.name == "dir":
            self.owner = action.attrs["owner"]
            self.group = action.attrs["group"]
            self.mode = action.attrs["mode"]
            self.isdir = True
        elif action.name == "hardlink":
            target = os.path.normpath(action.get_target_path())
            self.hardkey = target
            self.hardpaths.add(target)

    @staticmethod
    def supported(action):
        """Indicates whether the specified IPS action time is
        correctly handled by the ActionInfo constructor.
        """
        return action in frozenset(("file", "dir", "link", "hardlink"))


class UnsupportedFileFormatError(Exception):
    """This means that the stat.S_IFMT returned something we don't
    support, ie a pipe or socket.  If it's appropriate for such an
    object to be in the proto area, then the RealFileInfo constructor
    will need to evolve to support it, or it will need to be in the
    exception list.
    """
    def __init__(self, path, mode):
        Exception.__init__(self)
        self.path = path
        self.mode = mode

    def __str__(self):
        return '%s: unsupported S_IFMT %07o' % (self.path, self.mode)


class RealFileInfo(FileInfo):
    """Object to track important-to-packaging file information.

    This currently handles regular files, directories, and symbolic links.

    For multiple RealFileInfo objects with identical hardkeys, there
    is no way to determine which of the hard links should be
    delivered as a file, and which as hardlinks.
    """

    def __init__(self, root=None, path=None):
        FileInfo.__init__(self)
        self.path = path
        path = os.path.join(root, path)
        lstat = os.lstat(path)
        mode = lstat.st_mode

        #
        # Per stat.py, these cases are mutually exclusive.
        #
        if stat.S_ISREG(mode):
            self.hash = self.path
        elif stat.S_ISDIR(mode):
            self.isdir = True
        elif stat.S_ISLNK(mode):
            self.target = os.path.normpath(os.readlink(path))
            self.mediator = None
        else:
            raise UnsupportedFileFormatError(path, mode)

        if not stat.S_ISLNK(mode):
            self.mode = "%04o" % stat.S_IMODE(mode)
            #
            # Instead of reading the group and owner from the proto area after
            # a non-root build, just drop in dummy values.  Since we don't
            # compare them anywhere, this should allow at least marginally
            # useful comparisons of protolist-style output.
            #
            self.owner = "owner"
            self.group = "group"

        #
        # refcount > 1 indicates a hard link
        #
        if lstat.st_nlink > 1:
            #
            # This could get ugly if multiple proto areas reside
            # on different filesystems.
            #
            self.hardkey = lstat.st_ino


class DirectoryTree(dict):
    """Meant to be subclassed according to population method.
    """
    def __init__(self, name):
        dict.__init__(self)
        self.name = name

    def compare(self, other):
        """Compare two different sets of FileInfo objects.
        """
        keys1 = frozenset(self.keys())
        keys2 = frozenset(other.keys())

        common = keys1.intersection(keys2)
        onlykeys1 = keys1.difference(common)
        onlykeys2 = keys2.difference(common)

        if onlykeys1:
            print "Entries present in %s but not %s:" % \
                (self.name, other.name)
            for path in sorted(onlykeys1):
                print("\t%s" % str(self[path]))
            print ""

        if onlykeys2:
            print "Entries present in %s but not %s:" % \
                (other.name, self.name)
            for path in sorted(onlykeys2):
                print("\t%s" % str(other[path]))
            print ""

        nodifferences = True
        for path in sorted(common):
            if self[path] != other[path]:
                if nodifferences:
                    nodifferences = False
                    print "Entries that differ between %s and %s:" \
                        % (self.name, other.name)
                print("%14s %s" % (self.name, self[path]))
                print("%14s %s" % (other.name, other[path]))
        if not nodifferences:
            print ""


class BadProtolistFormat(Exception):
    """This means that the user supplied a file via -l, but at least
    one line from that file doesn't have the right number of fields to
    parse as protolist output.
    """
    def __str__(self):
        return 'bad proto list entry: "%s"' % Exception.__str__(self)


class ProtoTree(DirectoryTree):
    """Describes one or more proto directories as a dictionary of
    RealFileInfo objects, indexed by relative path.
    """

    def adddir(self, proto, exceptions):
        """Extends the ProtoTree dictionary with RealFileInfo
        objects describing the proto dir, indexed by relative
        path.
        """
        newentries = {}

        pdir = os.path.normpath(proto)
        strippdir = lambda r, n: os.path.join(r, n)[len(pdir)+1:]
        for root, dirs, files in os.walk(pdir):
            for name in dirs + files:
                path = strippdir(root, name)
                if path not in exceptions:
                    try:
                        newentries[path] = RealFileInfo(pdir, path)
                    except OSError, e:
                        sys.stderr.write("Warning: unable to stat %s: %s\n" %
                            (path, e))
                        continue
                else:
                    exceptions.remove(path)
                    if name in dirs:
                        dirs.remove(name)

        #
        # Find the sets of paths in this proto dir that are hardlinks
        # to the same inode.
        #
        # It seems wasteful to store this in each FileInfo, but we
        # otherwise need a linking mechanism.  With this information
        # here, FileInfo object comparison can be self contained.
        #
        # We limit this aggregation to a single proto dir, as
        # represented by newentries.  That means we don't need to care
        # about proto dirs on separate filesystems, or about hardlinks
        # that cross proto dir boundaries.
        #
        hk2path = {}
        for path, fileinfo in newentries.iteritems():
            if fileinfo.hardkey:
                hk2path.setdefault(fileinfo.hardkey, set()).add(path)
        for fileinfo in newentries.itervalues():
            if fileinfo.hardkey:
                fileinfo.hardpaths.update(hk2path[fileinfo.hardkey])
        self.update(newentries)

    def addprotolist(self, protolist, exceptions):
        """Read in the specified file, assumed to be the
        output of protolist.

        This has been tested minimally, and is potentially useful for
        comparing across the transition period, but should ultimately
        go away.
        """

        try:
            plist = open(protolist)
        except IOError, exc:
            raise IOError("cannot open proto list: %s" % str(exc))

        newentries = {}

        for pline in plist:
            pline = pline.split()
            #
            # Use a FileInfo() object instead of a RealFileInfo()
            # object because we want to avoid the RealFileInfo
            # constructor, because there's nothing to actually stat().
            #
            fileinfo = FileInfo()
            try:
                if pline[1] in exceptions:
                    exceptions.remove(pline[1])
                    continue
                if pline[0] == "d":
                    fileinfo.isdir = True
                fileinfo.path = pline[1]
                if pline[2] != "-":
                    fileinfo.target = os.path.normpath(pline[2])
                fileinfo.mode = int("0%s" % pline[3])
                fileinfo.owner = pline[4]
                fileinfo.group = pline[5]
                if pline[6] != "0":
                    fileinfo.hardkey = pline[6]
                newentries[pline[1]] = fileinfo
            except IndexError:
                raise BadProtolistFormat(pline)

        plist.close()
        hk2path = {}
        for path, fileinfo in newentries.iteritems():
            if fileinfo.hardkey:
                hk2path.setdefault(fileinfo.hardkey, set()).add(path)
        for fileinfo in newentries.itervalues():
            if fileinfo.hardkey:
                fileinfo.hardpaths.update(hk2path[fileinfo.hardkey])
        self.update(newentries)


class ManifestParsingError(Exception):
    """This means that the Manifest.set_content() raised an
    ActionError.  We raise this, instead, to tell us which manifest
    could not be parsed, rather than what action error we hit.
    """
    def __init__(self, mfile, error):
        Exception.__init__(self)
        self.mfile = mfile
        self.error = error

    def __str__(self):
        return "unable to parse manifest %s: %s" % (self.mfile, self.error)


class ManifestTree(DirectoryTree):
    """Describes one or more directories containing arbitrarily
    many manifests as a dictionary of ActionInfo objects, indexed
    by the relative path of the data source within the proto area.
    That path may or may not be the same as the path attribute of the
    given action.
    """

    def addmanifest(self, root, mfile, arch, modechecks, exceptions):
        """Treats the specified input file as a pkg(5) package
        manifest, and extends the ManifestTree dictionary with entries
        for the actions therein.
        """
        mfest = manifest.Manifest()
        try:
            mfest.set_content(open(os.path.join(root, mfile)).read())
        except IOError, exc:
            raise IOError("cannot read manifest: %s" % str(exc))
        except actions.ActionError, exc:
            raise ManifestParsingError(mfile, str(exc))

        #
        # Make sure the manifest is applicable to the user-specified
        # architecture.  Assumption: if variant.arch is not an
        # attribute of the manifest, then the package should be
        # installed on all architectures.
        #
        if arch not in mfest.attributes.get("variant.arch", (arch,)):
            return

        modewarnings = set()
        for action in mfest.gen_actions():
            if "path" not in action.attrs or \
                not ActionInfo.supported(action.name):
                continue

            #
            # The dir action is currently fully specified, in that it
            # lists owner, group, and mode attributes.  If that
            # changes in pkg(5) code, we'll need to revisit either this
            # code or the ActionInfo() constructor.  It's possible
            # that the pkg(5) system could be extended to provide a
            # mechanism for specifying directory permissions outside
            # of the individual manifests that deliver files into
            # those directories.  Doing so at time of manifest
            # processing would mean that validate_pkg continues to work,
            # but doing so at time of publication would require updates.
            #

            #
            # See pkgsend(1) for the use of NOHASH for objects with
            # datastreams.  Currently, that means "files," but this
            # should work for any other such actions.
            #
            if getattr(action, "hash", "NOHASH") != "NOHASH":
                path = action.hash
            else:
                path = action.attrs["path"]

            #
            # This is the wrong tool in which to enforce consistency
            # on a set of manifests.  So instead of comparing the
            # different actions with the same "path" attribute, we
            # use the first one.
            #
            if path in self:
                continue

            #
            # As with the manifest itself, if an action has specified
            # variant.arch, we look for the target architecture
            # therein.
            #
            var = None

            #
            # The name of this method changed in pkg(5) build 150, we need to
            # work with both sets.
            #
            if hasattr(action, 'get_variants'):
                var = action.get_variants()
            else:
                var = action.get_variant_template()
            if "variant.arch" in var and arch not in var["variant.arch"]:
                return

            self[path] = ActionInfo(action)
            if modechecks is not None and path not in exceptions:
                modewarnings.update(self[path].checkmodes(modechecks))

        if len(modewarnings) > 0:
            print "warning: unsafe permissions in %s" % mfile
            for w in sorted(modewarnings):
                print w
            print ""

    def adddir(self, mdir, arch, modechecks, exceptions):
        """Walks the specified directory looking for pkg(5) manifests.
        """
        for mfile in os.listdir(mdir):
            if (mfile.endswith(".mog") and
                stat.S_ISREG(os.lstat(os.path.join(mdir, mfile)).st_mode)):
                try:
                    self.addmanifest(mdir, mfile, arch, modechecks, exceptions)
                except IOError, exc:
                    sys.stderr.write("warning: %s\n" % str(exc))

    def resolvehardlinks(self):
        """Populates mode, group, and owner for resolved (ie link target
        is present in the manifest tree) hard links.
        """
        for info in self.values():
            if info.name() == "hardlink":
                tgt = info.hardkey
                if tgt in self:
                    tgtinfo = self[tgt]
                    info.owner = tgtinfo.owner
                    info.group = tgtinfo.group
                    info.mode = tgtinfo.mode

class ExceptionList(set):
    """Keep track of an exception list as a set of paths to be excluded
    from any other lists we build.
    """

    def __init__(self, files, arch):
        set.__init__(self)
        for fname in files:
            try:
                self.readexceptionfile(fname, arch)
            except IOError, exc:
                sys.stderr.write("warning: cannot read exception file: %s\n" %
                    str(exc))

    def readexceptionfile(self, efile, arch):
        """Build a list of all pathnames from the specified file that
        either apply to all architectures (ie which have no trailing
        architecture tokens), or to the specified architecture (ie
        which have the value of the arch arg as a trailing
        architecture token.)
        """

        excfile = open(efile)

        for exc in excfile:
            exc = exc.split()
            if len(exc) and exc[0][0] != "#":
                if arch in (exc[1:] or arch):
                    self.add(os.path.normpath(exc[0]))

        excfile.close()


USAGE = """%s [-v] -a arch [-e exceptionfile]... [-L|-M [-X check]...] input_1 [input_2]

where input_1 and input_2 may specify proto lists, proto areas,
or manifest directories.  For proto lists, use one or more

    -l file

arguments.  For proto areas, use one or more

    -p dir

arguments.  For manifest directories, use one or more

    -m dir

arguments.

If -L or -M is specified, then only one input source is allowed, and
it should be one or more manifest directories.  These two options are
mutually exclusive.

The -L option is used to generate a proto list to stdout.

The -M option is used to check for safe file and directory modes.
By default, this causes all mode checks to be performed.  Individual
mode checks may be turned off using "-X check," where "check" comes
from the following set of checks:

    m   check for group or other write permissions
    w   check for user write permissions on files and directories
        not owned by root
    s   check for group/other read permission on executable files
        that have setuid/setgid bit(s)
    o   check for files that could be safely owned by root
""" % sys.argv[0]


def usage(msg=None):
    """Try to give the user useful information when they don't get the
    command syntax right.
    """
    if msg:
        sys.stderr.write("%s: %s\n" % (sys.argv[0], msg))
    sys.stderr.write(USAGE)
    sys.exit(2)


def main(argv):
    """Compares two out of three possible data sources: a proto list, a
    set of proto areas, and a set of manifests.
    """
    try:
        opts, args = getopt.getopt(argv, 'a:e:Ll:Mm:p:vX:')
    except getopt.GetoptError, exc:
        usage(str(exc))

    if args:
        usage()

    arch = None
    exceptionlists = []
    listonly = False
    manifestdirs = []
    manifesttree = ManifestTree("manifests")
    protodirs = []
    prototree = ProtoTree("proto area")
    protolists = []
    protolist = ProtoTree("proto list")
    modechecks = set()
    togglemodechecks = set()
    trees = []
    comparing = set()
    verbose = False

    for opt, arg in opts:
        if opt == "-a":
            if arch:
                usage("may only specify one architecture")
            else:
                arch = arg
        elif opt == "-e":
            exceptionlists.append(arg)
        elif opt == "-L":
            listonly = True
        elif opt == "-l":
            comparing.add("protolist")
            protolists.append(os.path.normpath(arg))
        elif opt == "-M":
            modechecks.update(DEFAULTMODECHECKS)
        elif opt == "-m":
            comparing.add("manifests")
            manifestdirs.append(os.path.normpath(arg))
        elif opt == "-p":
            comparing.add("proto area")
            protodirs.append(os.path.normpath(arg))
        elif opt == "-v":
            verbose = True
        elif opt == "-X":
            togglemodechecks.add(arg)

    if listonly or len(modechecks) > 0:
        if len(comparing) != 1 or "manifests" not in comparing:
            usage("-L and -M require one or more -m args, and no -l or -p")
        if listonly and len(modechecks) > 0:
            usage("-L and -M are mutually exclusive")
    elif len(comparing) != 2:
        usage("must specify exactly two of -l, -m, and -p")

    if len(togglemodechecks) > 0 and len(modechecks) == 0:
        usage("-X requires -M")

    for s in togglemodechecks:
        if s not in ALLMODECHECKS:
            usage("unknown mode check %s" % s)
        modechecks.symmetric_difference_update((s))

    if len(modechecks) == 0:
        modechecks = None

    if not arch:
        usage("must specify architecture")

    exceptions = ExceptionList(exceptionlists, arch)
    originalexceptions = exceptions.copy()

    if len(manifestdirs) > 0:
        for mdir in manifestdirs:
            manifesttree.adddir(mdir, arch, modechecks, exceptions)
        if listonly:
            manifesttree.resolvehardlinks()
            for info in manifesttree.values():
                print "%s" % info.protostr()
            sys.exit(0)
        if modechecks is not None:
            sys.exit(0)
        trees.append(manifesttree)

    if len(protodirs) > 0:
        for pdir in protodirs:
            prototree.adddir(pdir, exceptions)
        trees.append(prototree)

    if len(protolists) > 0:
        for plist in protolists:
            try:
                protolist.addprotolist(plist, exceptions)
            except IOError, exc:
                sys.stderr.write("warning: %s\n" % str(exc))
        trees.append(protolist)

    if verbose and exceptions:
        print "Entries present in exception list but missing from proto area:"
        for exc in sorted(exceptions):
            print "\t%s" % exc
        print ""

    usedexceptions = originalexceptions.difference(exceptions)
    harmfulexceptions = usedexceptions.intersection(manifesttree)
    if harmfulexceptions:
        print "Entries present in exception list but also in manifests:"
        for exc in sorted(harmfulexceptions):
            print "\t%s" % exc
            del manifesttree[exc]
        print ""

    trees[0].compare(trees[1])

if __name__ == '__main__':
    try:
        main(sys.argv[1:])
    except KeyboardInterrupt:
        sys.exit(1)
    except IOError:
        sys.exit(1)
