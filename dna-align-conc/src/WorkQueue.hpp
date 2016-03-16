#ifndef WORK_QUEUE
#define WORK_QUEUE

#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>
#include <functional>
#include <atomic>
#include <mutex>
#include <cstdlib>
#include "concurrentqueue.h"

class WorkQueue {
private:
	std::vector<std::thread> threadpool;
	moodycamel::ConcurrentQueue<std::function<void(void)>> tasklist;
	std::atomic<bool> cont;
	std::mutex mutex_cout;
	std::mutex mutex_task;
public:
	WorkQueue():cont(true), threadpool(), tasklist(){};
	~WorkQueue(){ stop(); };
	void start(int);
	void stop();
	void post(std::function<void(void)>);
};

void WorkQueue::start(int n)
{
	for(auto i = 0; i < n; i++)
	{
		threadpool.emplace_back(std::thread([&](){
			while(cont.load())
			{
				try
				{
					std::function<void(void)> t;
					if(tasklist.try_dequeue(t)) t();
				}
				catch(const std::exception &exc)
				{
					// std::lock_guard<std::mutex> lck(mutex_cout);
					// std::cerr << "Exception: " << exc.what() << std::endl;
				}
			}
		}));
	}
}

void WorkQueue::stop()
{
	cont = false;
	std::for_each(threadpool.begin(),threadpool.end(),[](auto& t){
		t.join();
	});

	threadpool.clear();
}

void WorkQueue::post(std::function<void(void)> f)
{
	tasklist.enqueue(f);
}

#endif
