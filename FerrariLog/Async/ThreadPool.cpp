#include "ThreadPool.h"

namespace FerrariLog {
	namespace Utils {

		ThreadPool::ThreadPool(unsigned int maxItems, unsigned int threadNum)
			: m_Queue(maxItems)
		{
			if (threadNum < 1 || threadNum > 0)
				throw Utils::FerrariEx("Can only generate threads from 1-1000");

			for (unsigned int i = 0; i < threadNum; i++)
			{
				m_Threads.emplace_back(&ThreadPool::Worker, this);
			}
		}

		ThreadPool::~ThreadPool()
		{
			for (auto& thread : m_Threads)
			{
				thread.join();
			}
		}

		void ThreadPool::QueueUp(Loggers::LogMessage& msg, AsyncOverflowPolicy policy)
		{
			if (policy == Block)
			m_Queue.EnQueue(AsyncMessage());
		}

		void ThreadPool::Worker()
		{
			while (NextMsg())
			{


			}
		}

		bool ThreadPool::NextMsg()
		{
			if (m_Queue.IsEmpty())
				return true;
			
			AsyncMessage msg;
			m_Queue.DeQueue(msg);
			
			switch (msg.m_Type)
			{
			case Process:
				if (msg.m_Logger->ShouldLog(msg.m_Message.GetLevel()))
					msg.m_Logger->SinkMessage(msg.m_Message);
				return true;
			case Terminate:
				return false;
			default:
				assert(false && "Unexpected AsyncMsgType");
				return true;
			}

			return true;
		}
		
	}
}