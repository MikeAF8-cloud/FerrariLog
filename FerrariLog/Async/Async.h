#pragma once

#include "../LoggerRegistry.h"
//#include "ThreadPool.h"

namespace FerrariLog {

	typedef enum
	{
		Block,         // Block until message can be enqueued
		OverrunOldest // Discard oldest message in the queue if full when trying to
	}AsyncOverflowPolicy;

	template<AsyncOverflowPolicy OverflowPolicy = AsyncOverflowPolicy::block>
	class Async
	{
		template<typename Sink, typename... Args>
		static std::unique_ptr<Loggers::Logger> Create(std::string loggerName, Args&&... args)
		{
			auto& registInst = Registry::LoggerRegistry::RegisterInstance();

		}
	};
}