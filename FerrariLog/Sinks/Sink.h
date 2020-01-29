#pragma once

#include <mutex>
#include <atomic>

#include "../LogMessage.h"
#include "../Formatters/PatternFormatter.h"
#include "../Level.h"
#include "../Utils/Utils.h"
#include "../Utils/OSGlobals.h"

namespace FerrariLog {
	namespace Sinks {

		class Sink
		{
		protected:
			std::atomic<Levels::Level::LevelEnum> m_Level;
			std::unique_ptr<Formatters::Formatter> m_Formatter;
		public:
			Sink()
				: /*m_Level(Levels::Level::TRACE),*/ m_Formatter(std::make_unique<Formatters::PatternFormatter>()) {}
			
			explicit Sink(std::unique_ptr<Formatters::PatternFormatter> formatter)
				: /*m_Level(Levels::Level::TRACE),*/ m_Formatter(std::move(formatter)) {}

			virtual ~Sink() = default;
			Sink(const Sink &) = delete;
			Sink &operator=(const Sink &) = delete;


			inline bool ShouldLog(Levels::Level::LevelEnum messageLevel)
			{
				return (m_Level.load(std::memory_order_relaxed) < messageLevel);
			}


			//inline Levels::Level::LevelEnum GetLevel() const { return m_Level.load(std::memory_order_relaxed); }
			
			//virtual inline void LogMessages(Loggers::LogMessage &msg) final
			//{
			//	std::lock_guard<Mutex> lock(m_Mutex);
			//	SinkLog(msg);
			//}
			//
			//virtual inline void FlushMessages() final
			//{
			//	std::lock_guard<Mutex> lock(m_Mutex);
			//	Flush();
			//}
			//
			//virtual inline void SetDefPattern(const std::string &pattern) final
			//{
			//	std::lock_guard<Mutex> lock(m_Mutex);
			//	SetPattern(pattern);
			//}
			//
			//inline void SetDefFormatter(std::unique_ptr<Formatters::Formatter> sinkFormatter) final
			//{
			//	std::lock_guard<Mutex> lock(m_Mutex);
			//	SetFormatter(std::move(sinkFormatter));
			//}

			//inline Formatters::Formatter* GetFormatter()
			//{
			//	return m_Formatter.get();
			//}

			virtual void Flush() = 0;
			virtual void SinkLog(Loggers::LogMessage& msg) = 0; 
			virtual void SetPattern(const std::string& pattern) = 0;
			virtual void SetFormatter(std::unique_ptr<Formatters::Formatter> sinkFormatter) = 0;
		};
	}
}