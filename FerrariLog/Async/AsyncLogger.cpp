#include "AsyncLogger.h"

namespace FerrariLog {

	AsyncLogger::AsyncLogger(const std::string& name, std::unique_ptr<Sinks::Sink> initSink, std::weak_ptr<Utils::ThreadPool> tp, AsyncOverflowPolicy policy)
		: Loggers::Logger(name, std::move(initSink)), m_TP(std::move(tp)), m_Policy(policy) {}

	inline void AsyncLogger::SinkMessage(Loggers::LogMessage& message)
	{
		if (auto ptr = m_TP.lock())
		{
		}

		AsyncMessage msg(shared_from_this(), m_Policy, message);
			//ptr->QueueUp();
	}

	inline void AsyncLogger::FlushLogger()
	{

	}
}