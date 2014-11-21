#include "MessagePool.h"


Pool::Pool(size_t max_size = 10, size_t spin_time = 50)
			: max_size(max_size), spin_time(spin_time)
			{}

		Pool::~Pool()
			{
				myMutex.lock();
				//mtx.lock();
				while (data.size()) data.pop();
				myMutex.unlock();
				//mtx.unlock();
			}

		size_t Pool::size() const
			{
				myMutex.lock();
				size_t result = data.size();
				myMutex.unlock();
				return result;
			}
		void Pool::push(T item)
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
		T Pool::pop()
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
}