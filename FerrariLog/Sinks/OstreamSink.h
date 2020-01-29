#pragma once

#include "Sink.h"

namespace FerrariLog {
	namespace Sinks {

		template<typename Mutex>
		class OstreamSink : public Sink
		{
		private:
			std::ostream& m_Ostream;
			Mutex m_Mutex;
			bool m_ForceFlush;
		public:
			explicit OstreamSink(std::ostream& os, bool forceFlush = false)
				: m_Ostream(os), m_ForceFlush(forceFlush) {}

			OstreamSink(const OstreamSink &) = delete;
			OstreamSink &operator=(const OstreamSink &) = delete;

			void SinkLog(Loggers::LogMessage& msg) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);

				std::string messageText = Sink::m_Formatter->Format(msg);
				m_Ostream.write(messageText.c_str(), messageText.size());

				if (m_ForceFlush)
					m_Ostream.flush();
			}

			void Flush() override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_Ostream.flush();
			}

			void SetPattern(const std::string& pattern) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_Formatter = std::unique_ptr<Formatters::Formatter>(new Formatters::PatternFormatter(pattern));
			}

			void SetFormatter(std::unique_ptr <Formatters::Formatter>sinkFormatter) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_Formatter = std::move(sinkFormatter);
			}

			inline void SetFlush(bool forceFlush) { m_ForceFlush = forceFlush; }
		};

		using ostream_st = Sinks::OstreamSink<OS::NullMutex>;
		using ostream_mt = Sinks::OstreamSink<OS::ConsoleMutex>;

	}
}