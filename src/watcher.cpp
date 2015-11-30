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
#include "watcher.hpp"
#include "process.hpp"
#include "util.hpp"
#include "log.hpp"

Watcher::Watcher(iLogger* logger, std::string dir, Config& conf, ProcessManager* pm, bool recursive) :
	logger(logger),
	mDirectory(dir),
	mRecursive(recursive),
	mConfig(conf),
	mTimer(time(0)-15),
	mBuilder(pm->getBuildStep()),
	procman(pm)
{
	mINotify = inotify_init();
	watchDirectory();

}

Watcher::~Watcher()
{
	procman->killAll();
	this->removeAllWatches();
	close(mINotify);
}

void Watcher::removeAllWatches()
{
	for(int watch : this->mFDs)
		inotify_rm_watch(mINotify, watch);
}

void Watcher::watchDirectory()
{

	mINotify = inotify_init();
	if( mINotify  < 0 )
		perror("inotify_init");
	mFDs.push_back(inotify_add_watch(mINotify, Util::cwd().c_str(), IN_MODIFY));
	if(mRecursive)
	{
		this->recursiveWatch(Util::cwd());
	}
}
void Watcher::recursiveWatch(std::string maindir)
{
	Util::DirList dirs = Util::listDir(maindir, Util::DIRECTORY);
	if(dirs.size() > 0)
	{
		for(std::string dir : dirs)
		{
			if(dir.compare(".") == 0 || dir.compare("..") == 0)
				continue;
			//LOG(logger, DEBUG, "Watching %s", (maindir + '/' + dir).c_str());
			mFDs.push_back(inotify_add_watch(mINotify, (maindir + '/' +
							dir).c_str(), IN_MODIFY));
			//LOG(logger, DEBUG, "%s => %s", dir.c_str(), ".");
			this->recursiveWatch(maindir + '/' + dir);
		}
	}
}
void Watcher::removeWatch(std::string dir)
{
	// /proc/self/fd/#
}
void Watcher::watchFileType(std::string ft)
{
}
void Watcher::listen()
{
	// Setup select() call
	FD_ZERO(&mReadFDs);
	FD_SET(mINotify, &mReadFDs);
	mTimeout.tv_sec = 1;
	mTimeout.tv_usec = 0;
	int selval = select(mINotify+1, &mReadFDs, NULL, NULL, &mTimeout);
	if(selval == 1) {
		int length, i;
		struct tm timediff = {0};
		length = read( mINotify, mBuffer, EVENT_BUF_LEN);
		i = 0;
		if( length <= 0 )
			LOG_ERROR(logger, "read");

		LOG(logger, DEBUG, "Event buffer size: %d", /*mBuffer,*/length);
		while( i < length ) {
			struct inotify_event *event = ( struct inotify_event * ) &mBuffer[i];
			LOG(logger, DEBUG, "File %s -> 0x%x!", event->name, event->mask);
			LOG(logger, DEBUG, "%d && %X & %X && ! %X & %X", event->len, event->mask, IN_MODIFY, event->mask, IN_ISDIR);
			if( event->len &&
					(event->mask & IN_MODIFY) &&
					(! (event->mask & IN_ISDIR))
				){
				for(auto str : mConfig.getWatchConfig().filters)
				{
					if(Util::strMatch(str, std::string(event->name))) {
						LOG(logger, INFO, "\033[0;32m=> File %s was modified!\033[0m", event->name);
						//Logger::getLogger()->LOG(logger, DEBUG, "%ju", timer);
						//Logger::getLogger()->LOG(logger, DEBUG, "%ju", timer+5);
						//Logger::getLogger()->LOG(logger, DEBUG, "%ju", time(0));
						timediff = *localtime(&mTimer);
						timediff.tm_sec += 5;
						if(mktime(&timediff) <= time(0)) {
							if(this->rebuild()) {
								LOG(logger, DEBUG, "%ju", mktime(&timediff));
								mTimer = time(0);
								this->restartProgram();
							}
						}
					}
				}
			}
			i += EVENT_SIZE + event->len;
		}
	} else if(selval < 0) {
		LOG_ERROR(logger, "select");
	} else {
		// Timout reached, nothing to read!
		//LOG(logger, DEBUG, "Timout reached: %d", mINotify);
	}
}

bool Watcher::rebuild()
{
	bool status = mBuilder->runAndWait();
	if(status)
		LOG(logger, SUCCESS, "Successfully rebuilt project");
	else
		LOG(logger, ERROR, "Project failed to rebuild");
	return status;
}

void Watcher::restartProgram()
{
	mConfig.restartProcesses();
}

#ifdef DEBUG
void watcher() {
	int length, i = 0;
	int fd;
	int wd;
	char buffer[EVENT_BUF_LEN];
	time_t mTimer = time(0)-15;
	struct tm timediff = {0};

	pid_t progpid;
	progpid = runprog();

	char cwd[1024];
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
							timediff = *localtime(&mTimer);
							timediff.tm_sec += 5;
							if(mktime(&timediff) <= time(0)) {
								if(gRunning && !rebuild()) {
									//printf("%ju\n", mktime(&timediff));
									mTimer = time(0);
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

int main(int argc, char** argv) {
	struct sigaction int_catcher;
	int_catcher.sa_handler = catch_int;
	//signal(SIGINT, catch_int);
	sigaction(SIGINT,&int_catcher,0);
	watcher();
	return EXIT_SUCCESS;
}
#endif
