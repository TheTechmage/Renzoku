/*
 * Renzoku - Re-build, re-test, and re-run a program whenever the code changes
 * Copyright (C) 2015  Colton Wolkins
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
// This file contains the original prototype version of Renzoku. It works
// decently well, has some quirks, and is hard coded. This version of the code
// can only watch a single directory instead of multiple. Renzoku has been
// written from the ground up (with some help from this template and
// third-party libraries) with the idea that it could be turned into a real
// utility that is useful. Renzoku wasn't just built to compile code then run a
// daemon (well, the prototype was but that's besides the point), it should
// also be used when setting up a config file and continuously testing out new
// changes. Currently Renzoku only supports Linux (and quite possibly, only GCC
// 5.1+ since I use Archlinux near exclusively), but there are plans to support
// the Windows platform as well.
//
// I am sticking the prototype version of Renzoku into the repository so that I
// can mainly reference it while I am writing code for Renzoku, but also
// because there are times that I need Renzoku "now" and the prototype is the
// next best thing
//
// -- Colton Wolkins
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <linux/inotify.h>

#define EVENT_SIZE	( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN	( 1024 * ( EVENT_SIZE + 16 ) )

volatile sig_atomic_t gRunning = 1;
void catch_int(int sig) {
	gRunning = 0;
	fprintf(stderr, "=> Caught deadly SIGINT, termination immenent!\n");
	fflush(stderr);
	signal(sig, catch_int);
}
int ends_with(const char* str, const char* suffix) {
	if(!str || !suffix)
		return 0;
	size_t lenstr = strlen(str);
	size_t lensuffix = strlen(suffix);
	if(lensuffix > lenstr)
		return 0;
	return strncmp(str+lenstr - lensuffix, suffix, lensuffix) == 0;
}

int killprog(pid_t pid) {
	if(pid<=1)
		return 1;
	if(kill(pid, SIGTERM)) {
		perror("kill");
		return 0;
	}
	int status = 0;
	pid = waitpid(pid, &status, 0);
	if(pid < 0)
		perror("waitpid");
	if(status != 0)
		printf("=> Process [%d] exited with %d\n", pid, status);
	return 1;
}
pid_t runprog() {
	pid_t pid;
	pid = fork();
	if(pid == -1) {
		fprintf(stderr, "Failed to run program!\n");
		perror("fork");
		return;
	} else if(pid == 0) {
		printf("[%d] -> In child process!\n", getpid());
		char* args[] = {
			"./a.out",
			"--renzoku",
			0
		};
		execvp(args[0], args);
		perror("ERROR: Failed to run rebuild command");
	} else {
		printf("-> The child has a pid of %d\n", pid);
	}
	return pid;
}

int rebuild() {
	pid_t pid;
	pid = fork();
	if(pid == -1) {
		fprintf(stderr, "Failed to rebuild!\n");
		perror("fork");
		return 1;
	} else if(pid == 0) {
		printf("[%d] -> In child process!\n", getpid());
		char* gccargs[] = {
			"make",
			0
		};
		execvp(gccargs[0], gccargs);
		//fprintf(stderr, "%s"
		perror("ERROR: Failed to run rebuild command");
		exit(1);
	} else {
		printf("-> The child has a pid of %d\n", pid);
		int status = 0;
		pid = waitpid(pid, &status, 0);
		if(pid < 0)
			perror("waitpid");
		if(status != 0)
		{
			fprintf(stderr, "=> An error has occured during the build\n");
			return 1;
		}
	}
	return 0;
}

void watcher() {
	int length, i = 0;
	int fd;
	int wd;
	char buffer[EVENT_BUF_LEN];
	char cwd[1024];
	time_t timer = time(0)-15;
	struct tm timediff = {0};

	pid_t progpid;
	progpid = runprog();

	if( getcwd(cwd, sizeof(cwd)) != NULL )
		fprintf(stdout, "Current working dir: %s\n", cwd);
	else
		perror("getcwd");

	fd = inotify_init();
	if( fd < 0 )
		perror("inotify_init");
	wd = inotify_add_watch(fd, cwd, IN_MODIFY);
	while(gRunning) {
		length = read( fd, buffer, EVENT_BUF_LEN);
		i = 0;
		if( length <= 0 )
			perror("read");

		//printf("File %s!%d!\n", buffer,length);
		while( i < length ) {
			struct inotify_event *event = ( struct inotify_event * ) &buffer[i];
			//printf("File %s -> 0x%x!\n", event->name, event->mask);
			if( event->len ) {
				if(event->mask & IN_MODIFY) {
					if(! (event->mask & IN_ISDIR)) {
						if(ends_with(event->name, ".sh")) {
							printf(" => File %s was modified!\n", event->name);
							//printf("%ju\n", timer);
							//printf("%ju\n", timer+5);
							//printf("%ju\n", time(0));
							timediff = *localtime(&timer);
							timediff.tm_sec += 5;
							if(mktime(&timediff) <= time(0)) {
								if(gRunning && !rebuild()) {
									//printf("%ju\n", mktime(&timediff));
									timer = time(0);
									if(killprog(progpid))
										progpid = runprog();
								}
							}
						}
					}
				}
			}
			i += EVENT_SIZE + event->len;
		}
	}
	killprog(progpid);
	inotify_rm_watch(fd, wd);
	close(fd);
}

int main(int argc, char* argv) {
	struct sigaction int_catcher = {.sa_handler=catch_int};
	//signal(SIGINT, catch_int);
	sigaction(SIGINT,&int_catcher,0);
	watcher();
	return EXIT_SUCCESS;
}
