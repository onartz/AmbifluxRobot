#ifndef __POOL_H__
#define __POOL_H__

#include "ArMutex.h"
#include "AriaUtil.h"
#include <queue>
//#include <unistd.h>

template <class T> class Pool
{
	private:
		std::queue<T> data;
		ArMutex myMutex;
		size_t max_size;
		size_t spin_time;
	public:

		Pool(size_t max_size = 10, size_t spin_time = 50)
					: max_size(max_size), spin_time(spin_time)
		{}

		~Pool()
		{
			myMutex.lock();
			//mtx.lock();
			while (data.size()) data.pop();
			myMutex.unlock();
			//mtx.unlock();
		}

		size_t size()
		{
			myMutex.lock();
			size_t result = data.size();
			myMutex.unlock();
			return result;
		}
		void push(T item)
		{
			myMutex.lock();
			while (data.size() >= max_size)
			{
				myMutex.unlock();
				ArUtil::sleep(spin_time);
				myMutex.lock();
			}
			data.push(item);
			myMutex.unlock();
		}
		T pop()
		{
			myMutex.lock();
			while (data.size() <= 0)
			{
				myMutex.unlock();
				ArUtil::sleep(spin_time);
				myMutex.lock();
			}
			T item = data.front(); data.pop();
			myMutex.unlock();
			return item;
		}
};

#endif
