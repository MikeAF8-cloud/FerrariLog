#pragma once

//#include "Async.h"
#include <memory>
#include "../Logger.h"

namespace FerrariLog {

	typedef enum
	{
		Block,         // Block until message can be enqueued
		OverrunOldest // Discard oldest message in the queue if full when trying to
	}AsyncOverflowPolicy;

	class AsyncLogger: public std::enable_shared_from_this<AsyncLogger>, public Loggers::Logger
	{
	private:
		AsyncOverflowPolicy m_Policy;
		Utils::ThreadPool m_TP;
		//std::weak_ptr<Utils::ThreadPool> m_TP;
	public:
		AsyncLogger() = default;

		AsyncLogger(const std::string& name, std::unique_ptr<Sinks::Sink> initSink, std::weak_ptr<Utils::ThreadPool> tp, AsyncOverflowPolicy policy = AsyncOverflowPolicy::Block);

		inline void SinkMessage(Loggers::LogMessage& message) override;

		inline void FlushLogger() override;
	};
}