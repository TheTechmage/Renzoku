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
#include <string>
#include <vector>
#include <cstddef>
#include <cstring>
#include <sys/wait.h>

Process::Process(std::string &command, bool in_path) :
	mSearchInPath(in_path)
{
	// TODO: Write string parser for parsing out the command line arguments so
	// that we can push it through the C based functions.
	size_t position = 0;
	size_t tmppos = 0;
	bool inQuote = false;
	std::vector<std::string> args;
	while(position < command.length())
	{
		switch(command[position + tmppos])
		{
			case '"':
				inQuote = !inQuote;
				break;
			case ' ':
				if(inQuote)
					break;
				args.push_back(command.substr(position, tmppos));
				position += tmppos + 1;
				tmppos = 0;
				break;
			default:
				break;
		}
		tmppos++;
		if(position + tmppos >= command.size())
		{
			args.push_back(command.substr(position));
			break;
		}
	}
	mCommand = new char*[args.size() + 1];
	size_t i = 0;
	for(std::string &item : args)
	{
		mCommand[i] = new char[item.length()+1];
		strcpy(mCommand[i++], item.c_str());
	}
	mCommand[i] = NULL;
}

Process::~Process()
{
	for(size_t i = 0; mCommand[i]; i++)
		delete [] mCommand[i];
	delete [] mCommand;
}

bool Process::run()
{
	mProc = fork();
	if(mProc == -1)
	{
		Logger::getLogger()->log(ERROR, "Failed to launch fork!");
		Logger::getLogger()->logCError("fork");
		//perror("fork");
		return false;
	}
	else if(mProc == 0)
	{
		Logger::getLogger()->log(DEBUG, "In child process.");
		size_t size = 0, i = 1;
		while(mCommand[i] != NULL)
		{
			size += strlen(mCommand[i]);
			i++;
		}
		char* args_string = new char[size + 1];
		//strcpy(args_string, mCommand[0]);
		args_string[0] = '\0';
		for(size_t i2 = 1; mCommand[i2] != NULL; i2++)
			strcat(args_string, mCommand[i2]);
		Logger::getLogger()->log(DEBUG, "running command %s with %s arguments", mCommand[0], args_string);
		//delete args_string;
		delete [] args_string;
		if(mSearchInPath)
			execvp(mCommand[0], (char**)mCommand);
		else
			execv(mCommand[0], (char**)mCommand);
		Logger::getLogger()->log(ERROR, "Failed to run command!");
		exit(1);
	}
	else
	{
		Logger::getLogger()->log(DEBUG, "The child has a pid of %d", mProc);
		usleep(10);
		int status = 0;
		pid_t pid = waitpid(mProc, &status, WNOHANG);
		if(pid < 0)
			Logger::getLogger()->logCError("waitpid");
			//perror("waitpid");
		if(pid > 0 && status != 0)
		{
			Logger::getLogger()->log(ERROR, "An error has occured while attempting to run: %s", mCommand[0]);
			return false;
		}
		return true;
	}
		//Logger::getLogger()->log(DEBUG, "");
		//Logger::getLogger()->log(DEBUG, "");
}
bool Process::runAndWait()
{
	if(!this->run())
		return false;
	int status = 0;
	pid_t pid = waitpid(mProc, &status, 0);
	if(pid < 0)
		Logger::getLogger()->logCError("waitpid");
	if(pid > 0 && status != 0)
	{
		Logger::getLogger()->log(ERROR, "An error has occured while attempting to run: %s", mCommand[0]);
		return false;
	}
	return true;
}
bool Process::kill()
{
	if(mProc <= 1)
		return true;
	if(::kill(mProc, SIGTERM))
	{
		Logger::getLogger()->logCError("kill");
		return false;
	}
	int status = 0;
	pid_t pid = waitpid(mProc, &status, 0);
	if(pid < 0)
		Logger::getLogger()->logCError("waitpid");
	if(status != 0)
		Logger::getLogger()->log(SUCCESS, "Process [%d] exited with %d", mProc, status);
	return true;
}
int Process::status()
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
