#pragma once

#include "CircularQueue.h"
//#include "..\Utils\Utils.h"
#include "AsyncMessage.h"

namespace FerrariLog {
	namespace Utils {


		class ThreadPool
		{
		private:
			CircularQueue<AsyncMessage> m_Queue;
			std::vector<std::thread> m_Threads;
		public:
			ThreadPool(unsigned int maxItems, unsigned int threads);
			~ThreadPool();

			ThreadPool(const ThreadPool&) = delete;
			ThreadPool& operator=(ThreadPool&&) = delete;

			void QueueUp(Loggers::LogMessage& msg, AsyncOverflowPolicy policy);
		private:
			void Worker();

			bool NextMsg();
		};
	}
}