#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <csignal>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define EVENT_SIZE	( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN	( 1024 * ( EVENT_SIZE + 16 ) )

volatile sig_atomic_t gRunning = 1;
void catch_int(int sig) {
	gRunning = 0;
	fprintf(stderr, "\033[0;31m=> Caught deadly SIGINT, termination immenent!\033[0m\n");
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
		printf("\033[0;31m=> Process [%d] exited with %d\033[0m\n", pid, status);
	return 1;
}
pid_t runprog() {
	pid_t pid;
	pid = fork();
	if(pid == -1) {
		fprintf(stderr, "\033[0;31m=> Failed to run program!\033[0m\n");
		perror("fork");
		return 0;
	} else if(pid == 0) {
		printf("\033[1;30m[%d] -> In child process!\033[0m\n", getpid());
		char* args[] = {
			(char*)"./led",
			0
		};
		execv("led", args);
		perror("ERROR: Failed to run rebuild command");
	} else {
		printf("\033[1;30m-> The child has a pid of %d\033[0m\n", pid);
	}
	return pid;
}

int rebuild() {
	pid_t pid;
	pid = fork();
	if(pid == -1) {
		fprintf(stderr, "=\033[0;31m> Failed to rebuild!\033[0m\n");
		perror("fork");
		return 1;
	} else if(pid == 0) {
		printf("\033[1;30m[%d] -> In child process!\033[0m\n", getpid());
		char* gccargs[] = {
			(char*)"make",
			0
		};
		execvp("make", gccargs);
		//fprintf(stderr, "%s"
		perror("ERROR: Failed to run rebuild command");
		exit(1);
	} else {
		printf("\033[1;30m-> The child has a pid of %d\033[0m\n", pid);
		int status = 0;
		pid = waitpid(pid, &status, 0);
		if(pid < 0)
			perror("waitpid");
		if(status != 0)
		{
			fprintf(stderr, "\033[0;31m=> An error has occured during the build\033[0m\n");
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
		fprintf(stdout, "\033[0;32m=>Current working dir: %s\033[0m\n", cwd);
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
						if(ends_with(event->name, ".c") || ends_with(event->name, ".cpp")) {
							printf("\033[0;32m=> File %s was modified!\033[0m\n", event->name);
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

int main(int argc, char** argv) {
	struct sigaction int_catcher;
	int_catcher.sa_handler = catch_int;
	//signal(SIGINT, catch_int);
	sigaction(SIGINT,&int_catcher,0);
	watcher();
	return EXIT_SUCCESS;
}
