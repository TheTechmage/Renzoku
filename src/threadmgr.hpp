
#ifndef THREADMGR_H
#define THREADMGR_H
#include <vector>
#include <thread>
#include "watcher.hpp"

class Threadmgr
{
private:
	std::vector<Watcher*> mWatchers;
	std::vector<std::thread*> mThreads;

public:
	Threadmgr();
	virtual ~Threadmgr();
	void AddWatcher(Watcher*);
	void Start();
	void Stop();
};

#endif /* THREADMGR_H */
