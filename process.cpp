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
#include <sys/wait.h>

Process::Process(std::string command, bool in_path) :
	mSearchInPath(in_path)
{
	std::vector<std::string> command_vector;
	std::size_t firstChar = command.find_first_not_of(" \t\n\r");
	std::string* tmpstr = new std::string;
	for(std::size_t i = firstChar; i < command.length(); i++){

		char c = command.at(i);
		if( c == ' ' || c == '\n' || c == '\r' )
		{
			if(!tmpstr->empty())
			{
				command_vector.push_back(*tmpstr);
				*tmpstr = "";
			}
		}
		else if(c == '\"' )
		{
			i++;
			while( command.at(i) != '\"' )
			{
				*tmpstr += command.at(i);
				i++;
			}
		}
		else
			*tmpstr += c;
	}
	const char** argv = new const char* [command_vector.size()+1];
	for (std::size_t j = 0; j <= command_vector.size(); ++j)
		argv[j] = command_vector[j].c_str();

	argv[command_vector.size()+1] = NULL;
	mCommand = argv;
}

Process::~Process()
{
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
		pid_t pid = waitpid(pid, &status, WNOHANG);
		if(pid < 0)
			Logger::getLogger()->logCError("waitpid");
			//perror("waitpid");
		if(pid > 0)
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
	pid_t pid = waitpid(pid, &status, 0);
	if(pid < 0)
		Logger::getLogger()->logCError("waitpid");
	if(pid > 0)
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
	pid_t pid = waitpid(pid, &status, 0);
	if(pid < 0)
		Logger::getLogger()->logCError("waitpid");
	if(status != 0)
		Logger::getLogger()->log(SUCCESS, "Process [%d] exited with %d", mProc, status);
	return true;
}
int Process::status()
{
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
