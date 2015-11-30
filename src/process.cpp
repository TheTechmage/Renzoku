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
/*
 * =====================================================================================
 *
 *       Filename:  process.cpp
 *
 *    Description:  Create and manage subprocesses
 *
 *        Version:  1.0
 *        Created:  05/23/2015 08:29:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Colton Wolkins (frostyfrog), frostyfrog2@gmail.com
 *        Company:
 *
 * =====================================================================================
 */
#include "process.hpp"
#include "log.hpp"
#include "util.hpp"
#include <string>
#include <vector>
#include <cstddef>
#include <cstring>
#include <sys/wait.h>

Process::Process(iLogger* logger, const std::string &command, bool in_path) :
	mProc(0),
	mSearchInPath(in_path),
	mGeneratedCommand(true),
	mEnabled(true),
	logger(logger)
{
	mCommand = Util::parseCommand(command);
}

Process::Process(iLogger* logger, char** command, bool in_path) :
	mProc(0),
	mSearchInPath(in_path),
	mGeneratedCommand(false),
	mEnabled(true),
	mCommand(command),
	logger(logger)
{
}

Process::~Process()
{
	this->kill();
	if(mGeneratedCommand)
	{
		for(size_t i = 0; mCommand[i]; i++)
			delete [] mCommand[i];
		delete [] mCommand;
	}
}

bool Process::run()
{
	if(mCommand == NULL || !mEnabled)
		return false;
	mProc = fork();
	if(mProc == -1)
	{
		LOG(logger, ERROR, "Failed to launch fork!");
		LOG_ERROR(logger, "fork");
		//perror("fork");
		return false;
	}
	else if(mProc == 0)
	{
		LOG(logger, DEBUG, "In child process.");
		size_t size = 0, i = 1;
		while(mCommand[i] != NULL)
		{
			size += strlen(mCommand[i]) + 1;
			i++;
		}
		char* args_string = new char[size + 1];
		//strcpy(args_string, mCommand[0]);
		args_string[0] = '\0';
		for(size_t i2 = 1; mCommand[i2] != NULL; i2++) {
			strcat(args_string, mCommand[i2]);
			strcat(args_string, " ");
		}
		LOG(logger, DEBUG, "running command %s with %s arguments", mCommand[0], args_string);
		//delete args_string;
		delete [] args_string;
		if(mSearchInPath)
			execvp(mCommand[0], (char**)mCommand);
		else
			execv(mCommand[0], (char**)mCommand);
		LOG(logger, ERROR, "Failed to run command!");
		exit(1);
	}
	else
	{
		LOG(logger, DEBUG, "The child has a pid of %d", mProc);
		usleep(10);
		int status = 0;
		pid_t pid = waitpid(mProc, &status, WNOHANG);
		if(pid < 0 && errno != ECHILD)
			LOG_ERROR(logger, "waitpid");
		//perror("waitpid");
		if(pid > 0 && status != 0)
		{
			LOG(logger, ERROR, "An error has occured while attempting to run: %s", mCommand[0]);
			return false;
		}
		return true;
	}
	//Logger::getLogger()->LOG(logger, DEBUG, "");
	//Logger::getLogger()->LOG(logger, DEBUG, "");
}
bool Process::runAndWait()
{
	if(!this->run())
		return false;
	int status = 0;
	pid_t pid = waitpid(mProc, &status, 0);
	if(pid < 0 && errno != ECHILD)
		LOG_ERROR(logger, "waitpid");
	if(pid > 0 && status != 0)
	{
		LOG(logger, ERROR, "An error has occured while attempting to run: %s", mCommand[0]);
		return false;
	}
	return true;
}
bool Process::kill()
{
	if(mProc <= 1)
		return true;
	if((!::kill(mProc, 0)) && ::kill(mProc, SIGTERM))
	{
		LOG_ERROR(logger, "kill");
		return false;
	}
	int status = 0;
	pid_t pid = waitpid(mProc, &status, 0);
	if(pid < 0 && errno != ECHILD)
		LOG_ERROR(logger, "waitpid");
	if(status != 0)
		LOG(logger, SUCCESS, "Process [%d] exited with %d", mProc, status);
	return true;
}
int Process::status() const
{
	return 0;
}

#ifdef NOBUILD
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
#endif
