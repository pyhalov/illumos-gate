/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright (c) 2010, Oracle and/or its affiliates. All rights reserved.
 */

#include <sys/fm/protocol.h>
#include <sys/wait.h>
#include <fm/fmd_msg.h>
#include <fm/libfmevent.h>
#include <pwd.h>
#include <errno.h>
#include <signal.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <unistd.h>
#include "libfmnotify.h"

#define TIMEOUT 30000
#define MAXTIMESIZE 21
#define TIMEFORMAT "%Y-%m-%d %H:%M:%S"

/*
 * Debug messages can be enabled by setting the debug property to true
 *
 * # svccfg -s svc:/system/fm/cmd-notify setprop config/debug=true
 */
#define	SVCNAME		"system/fm/cmd-notify"

typedef struct notify_env {
	char *class;
	char *msgid;
	char *desc;
	time_t tstamp;
	char *fmri;
	char *from_state;
	char *to_state;
	char *reason;
	char *url;
} notify_env_t;

static nd_hdl_t *nhdl;
static const char optstr[] = "dfR:";
static char script[PATH_MAX + 1];
static char user[LOGIN_NAME_MAX + 1];

static int
usage(const char *pname)
{
	(void) fprintf(stderr, "Usage: %s [-df] [-R <altroot>] [-S <script>]\n", pname);

	(void) fprintf(stderr,
	    "\t-d  enable debug mode\n"
	    "\t-f  stay in foreground\n"
	    "\t-R  specify alternate root\n");

	return (1);
}

/*
 * If someone does an "svcadm refresh" on us, then this function gets called,
 * which rereads our service configuration.
 */
static void
get_svc_config()
{
	int s = 0;
	uint8_t val;

	s = nd_get_boolean_prop(nhdl, SVCNAME, "config", "debug", &val);
	nhdl->nh_debug = val;

	s += nd_get_astring_prop(nhdl, SVCNAME, "config", "rootdir",
	    &(nhdl->nh_rootdir));

	if (s != 0)
		nd_error(nhdl, "Failed to read retrieve service "
		    "properties");
}

static void
nd_sighandler(int sig)
{
	if (sig == SIGHUP)
		get_svc_config();
	else
		nd_cleanup(nhdl);
}

static int
get_cmd_prefs(nd_hdl_t *nhdl, nvlist_t **pref_nvl, uint_t npref)
{
	boolean_t *a1, *a2;
	uint_t n, n1, n2;
        char **u, **s;
	int r;

	/*
	 * For SMF state transition events, pref_nvl contain two sets of
	 * preferences, which will have to be merged.
	 * 
	 * The "cmd" nvlist currently only supports:
         * -  a single boolean member, "active" which will be set to true,
         *    if it is true in either set;
         * -  "user" property, determining what user id should be used to
         *    run script;
         * -  "script" property, a path to script to be run.
         * "active" will be set to true, if it is true in either set;
         * other properties will be taken from the first set.
	 */
	
	r = nvlist_lookup_boolean_array(pref_nvl[0], "active", &a1, &n);
        if (npref == 2) {
		r += nvlist_lookup_boolean_array(pref_nvl[1], "active", &a2,
		    &n);
        }

	r += nvlist_lookup_string_array(pref_nvl[0], "user", &u, &n1);
	r += nvlist_lookup_string_array(pref_nvl[0], "script", &s, &n2);
	if (r != 0) {
		nd_debug(nhdl, "Malformed cmd notification "
		    "preferences");
		nd_dump_nvlist(nhdl, pref_nvl[0]);
		nd_dump_nvlist(nhdl, pref_nvl[1]);
		return (-1);
	} else if (!a1[0] && !a2[0]) {
		nd_debug(nhdl, "CMD notification is disabled");
		return (-1);
	}

	(void) strncpy(user,u[0],LOGIN_NAME_MAX);
	(void) strncpy(script,s[0],PATH_MAX);
	return (0);
}

static void
call_handler_script(notify_env_t *t)
{
	pid_t pid;
	
	pid = fork();
	if (pid == 0) {
		char tstamp[MAXTIMESIZE];
		uid_t uid;
		gid_t gid;
		struct passwd *pw;

		if (!(pw = getpwnam(user))) {
			nd_debug(nhdl, "failed to get user properties for %s", user);
			return ;
		}

		uid = pw->pw_uid;
		gid = pw->pw_uid;
		
		 if (setgid(gid)) {
			nd_debug(nhdl, "Couldn't set gid to %d", gid);
			return;
		}

		if (setuid(uid)) {
			nd_debug(nhdl, "Couldn't set uid to %d", uid);
			return;
		}
		
		(void) setenv("CLASS", t->class ? t->class: "", 1);
		(void) setenv("MSGID", t->msgid ? t->msgid: "", 1);
		(void) setenv("DESC", t->desc ? t->desc: "", 1);
		(void) setenv("FMRI", t->fmri ? t->fmri: "", 1);
		(void) setenv("FROM_STATE", t->from_state ? t->from_state: "", 1);
		(void) setenv("TO_STATE", t->to_state ? t->to_state: "", 1);
		(void) setenv("REASON", t->reason ? t->reason: "", 1);
		(void) setenv("URL", t->url ? t->url: "", 1);

		(void) strftime(tstamp, MAXTIMESIZE, TIMEFORMAT, localtime(&(t->tstamp)));
		(void) setenv("TIMESTAMP", tstamp, 1);
		(void) execl(script, script, NULL);
		nd_debug(nhdl, "Couldn't exec sctipt");
	} else if (pid>0) {
		int ret;
		time_t t, cur;
		
		(void) time(&t);
		(void) time(&cur);
		while (waitpid(pid, &ret, WNOHANG) != pid &&
		    (cur - t < TIMEOUT)) {
			(void) sleep(2);
			(void) time(&cur);
		}
		if (cur - t >= TIMEOUT) {
			(void) kill(pid, SIGKILL);
		}
	} else {
		nd_debug(nhdl, "Couldn't fork");
	}
}

/*ARGSUSED*/
static void
ireport_cb(fmev_t ev, const char *class, nvlist_t *nvl, void *arg)
{
	nvlist_t **pref_nvl = NULL;
	nd_ev_info_t *ev_info = NULL;
	notify_env_t nenv;
	uint_t npref;
	int ret;

	nd_debug(nhdl, "Received event of class %s", class);

	ret = nd_get_notify_prefs(nhdl, "cmd", ev, &pref_nvl, &npref);
	if (ret == SCF_ERROR_NOT_FOUND) {
		/*
		 * No cmd notification preferences specified for this type of
		 * event, so we're done
		 */
		return;
	} else if (ret != 0) {
		nd_error(nhdl, "Failed to retrieve notification preferences "
		    "for this event");
		return;
	}

	if (get_cmd_prefs(nhdl, pref_nvl, npref) != 0)
		goto irpt_done;

	if (nd_get_event_info(nhdl, class, ev, &ev_info) != 0)
		goto irpt_done;

	bzero(&nenv, sizeof (struct notify_env));

	nenv.class = (char *)ev_info->ei_class;
	nenv.msgid = ev_info->ei_diagcode;
	nenv.desc = ev_info->ei_descr;
	nenv.tstamp = (time_t)fmev_time_sec(ev);

	if (strncmp(class, "ireport.os.smf", 14) == 0) {
		nenv.fmri = ev_info->ei_fmri;
		nenv.from_state=ev_info->ei_from_state;
		nenv.to_state=ev_info->ei_to_state;
		nenv.reason = ev_info->ei_reason;
	}
	call_handler_script(&nenv);
irpt_done:
	if (ev_info)
		nd_free_event_info(ev_info);
	nd_free_nvlarray(pref_nvl, npref);
}

/*ARGSUSED*/
static void
list_cb(fmev_t ev, const char *class, nvlist_t *nvl, void *arg)
{
	char *uuid;
	uint8_t version;
	nd_ev_info_t *ev_info = NULL;
	nvlist_t **pref_nvl = NULL;
	uint_t npref;
	int ret;
	boolean_t domsg;
	notify_env_t nenv;

	nd_debug(nhdl, "Received event of class %s", class);

	ret = nd_get_notify_prefs(nhdl, "cmd", ev, &pref_nvl, &npref);
	if (ret == SCF_ERROR_NOT_FOUND) {
		/*
		 * No cmd notification preferences specified for this type of
		 * event, so we're done
		 */
		return;
	} else if (ret != 0) {
		nd_error(nhdl, "Failed to retrieve notification preferences "
		    "for this event");
		return;
	}

	if (get_cmd_prefs(nhdl, pref_nvl, npref) != 0)
		goto listcb_done;

	if (nd_get_event_info(nhdl, class, ev, &ev_info) != 0)
		goto listcb_done;

	/*
	 * If the message payload member is set to 0, then it's an event we
	 * typically suppress messaging on
	 */
	if (nvlist_lookup_boolean_value(ev_info->ei_payload, FM_SUSPECT_MESSAGE,
	    &domsg) == 0 && !domsg) {
		nd_debug(nhdl, "Messaging suppressed for this event");
		goto listcb_done;
	}

	if (nvlist_lookup_uint8(ev_info->ei_payload, FM_VERSION, &version)
	    != 0 || version > FM_SUSPECT_VERSION) {
		nd_error(nhdl, "invalid event version: %u", version);
		goto listcb_done;
	}

	(void) nvlist_lookup_string(ev_info->ei_payload, FM_SUSPECT_UUID,
	    &uuid);

	if (strcmp(ev_info->ei_url, ND_UNKNOWN) != 0) {
		bzero(&nenv, sizeof (struct notify_env));

		nenv.class = (char *) ev_info->ei_class;
		nenv.msgid = ev_info->ei_diagcode;
		nenv.desc = ev_info->ei_descr;
		nenv.tstamp = (time_t)fmev_time_sec(ev);
		nenv.url=ev_info->ei_url;
		call_handler_script(&nenv);
	} else
		nd_error(nhdl, "failed to format url for %s", uuid);
listcb_done:
	nd_free_nvlarray(pref_nvl, npref);
	if (ev_info)
		nd_free_event_info(ev_info);
}

int
main(int argc, char *argv[])
{
	struct rlimit rlim;
	struct sigaction act;
	sigset_t set;
	char c;
	boolean_t run_fg = B_FALSE;

	if ((nhdl = malloc(sizeof (nd_hdl_t))) == NULL) {
		(void) fprintf(stderr, "Failed to allocate space for notifyd "
		    "handle (%s)", strerror(errno));
		return (1);
	}
	bzero(nhdl, sizeof (nd_hdl_t));
	nhdl->nh_keep_running = B_TRUE;
	nhdl->nh_log_fd = stderr;
	nhdl->nh_pname = argv[0];

	get_svc_config();

	/*
	 * In the case where we get started outside of SMF, args passed on the
	 * command line override SMF property setting
	 */
	while (optind < argc) {
		while ((c = getopt(argc, argv, optstr)) != -1) {
			switch (c) {
			case 'd':
				nhdl->nh_debug = B_TRUE;
				break;
			case 'f':
				run_fg = B_TRUE;
				break;
			case 'R':
				nhdl->nh_rootdir = strdup(optarg);
				break;
			default:
				free(nhdl);
				return (usage(nhdl->nh_pname));
			}
		}
	}

	/*
	 * Set up a signal handler for SIGTERM (and SIGINT if we'll
	 * be running in the foreground) to ensure sure we get a chance to exit
	 * in an orderly fashion.  We also catch SIGHUP, which will be sent to
	 * us by SMF if the service is refreshed.
	 */
	(void) sigfillset(&set);
	(void) sigfillset(&act.sa_mask);
	act.sa_handler = nd_sighandler;
	act.sa_flags = 0;

	(void) sigaction(SIGTERM, &act, NULL);
	(void) sigdelset(&set, SIGTERM);
	(void) sigaction(SIGHUP, &act, NULL);
	(void) sigdelset(&set, SIGHUP);

	if (run_fg) {
		(void) sigaction(SIGINT, &act, NULL);
		(void) sigdelset(&set, SIGINT);
	} else
		nd_daemonize(nhdl);

	rlim.rlim_cur = RLIM_INFINITY;
	rlim.rlim_max = RLIM_INFINITY;
	(void) setrlimit(RLIMIT_CORE, &rlim);

	/*
	 * We need to be root initialize our libfmevent handle (because that
	 * involves reading/writing to /dev/sysevent)
	 */
	nhdl->nh_evhdl = fmev_shdl_init(LIBFMEVENT_VERSION_2, NULL, NULL, NULL);
	if (nhdl->nh_evhdl == NULL) {
		(void) sleep(5);
		nd_abort(nhdl, "failed to initialize libfmevent: %s",
		    fmev_strerror(fmev_errno));
	}
	
	nhdl->nh_msghdl = fmd_msg_init(nhdl->nh_rootdir, FMD_MSG_VERSION);
	if (nhdl->nh_msghdl == NULL)
		nd_abort(nhdl, "failed to initialize libfmd_msg");

	/*
	 * Set up our event subscriptions.  We subscribe to everything and then
	 * consult libscf when we receive an event to determine what (if any)
	 * notification to send.
	 */
	nd_debug(nhdl, "Subscribing to ireport.os.smf.* events");
	if (fmev_shdl_subscribe(nhdl->nh_evhdl, "ireport.os.smf.*",
	    ireport_cb, NULL) != FMEV_SUCCESS) {
		nd_abort(nhdl, "fmev_shdl_subscribe failed: %s",
		    fmev_strerror(fmev_errno));
	}

	nd_debug(nhdl, "Subscribing to list.* events");
	if (fmev_shdl_subscribe(nhdl->nh_evhdl, "list.*", list_cb,
	    NULL) != FMEV_SUCCESS) {
		nd_abort(nhdl, "fmev_shdl_subscribe failed: %s",
		    fmev_strerror(fmev_errno));
	}

	/*
	 * We run until someone kills us
	 */
	while (nhdl->nh_keep_running)
		(void) sigsuspend(&set);

	free(nhdl->nh_rootdir);
	free(nhdl);

	return (0);
}
