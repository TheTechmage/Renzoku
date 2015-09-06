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
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <csignal>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/inotify.h>

#include <string>
#include <vector>

#define EVENT_SIZE	( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN	( 1024 * ( EVENT_SIZE + 16 ) )

class Watcher {
	private:
		std::string mDirectory;
		std::vector<std::string> mFiletypes;
		std::vector<int> mFDs;
		bool mRecursive;
		int mINotify;
		char mBuffer[EVENT_BUF_LEN];
		time_t mTimer;

		void watchDirectory();
		void removeWatch(std::string);
		void recurse(std::string);
		bool filterName(char* name);
		bool rebuild();
	public:
		Watcher(std::string, bool recursive);
		void watchFileType(std::string);
		void listen();
};
