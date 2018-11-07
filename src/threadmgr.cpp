#include "threadmgr.hpp"

Threadmgr::Threadmgr() :
	mWatchers(),
	mThreads()
{
}

Threadmgr::~Threadmgr()
{
	for (auto th : mThreads) {
		if(th) {
			th->join();
			delete th;
		}
	}
}

void Threadmgr::AddWatcher(Watcher* w) {
	mWatchers.push_back(w);
}

void Threadmgr::Start() {
	std::thread* thread;
	for (auto w : mWatchers) {
		thread = new std::thread(&Watcher::Start, w);
		if (thread) {
			mThreads.push_back(thread);
		} else {
			fprintf(stderr, "Failed to create new thread for %s\n",
					w->getName().c_str());
		}
	}
}

void Threadmgr::Stop() {
	for (auto w : mWatchers) {
		w->Stop();
	}
}
