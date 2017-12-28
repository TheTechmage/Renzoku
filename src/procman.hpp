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
#pragma once
#include "process.hpp"
#include "log.hpp"
#include <vector>

class ProcessManager
{
	private:
		std::vector<Process*> mProcesses; // Procs that run for build before launch
		Process* mProgram; // The actual program
		iLogger* logger;
	public:
		ProcessManager(iLogger*);
		~ProcessManager();
		Process* getProgram();
		bool runProcesses();
		void haltConstructionProcs();
		void startProgram();
		void haltProgram();
		void restartAll();
		void killAll();
		void addProcess(Process*);
		void setProgram(Process*);
		void finalize();
};
