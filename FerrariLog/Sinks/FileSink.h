#pragma once

#include "Sink.h"

namespace FerrariLog {
	namespace Sinks {

		template<typename Mutex>
		class FileSink final : public Sink
		{
		private:
			Utils::FileMaker m_File;
		public:
			explicit FileSink(const std::string& filename, bool truncate = false)
			{
				m_File.OpenFile(filename, truncate);
			}

			inline void SinkLog(Loggers::LogMessage& logMsg) override
			{
				std::lock_guard<Mutex> lock(m_Mutex)
				m_File.Write(Sink::m_Formatter->Format(logMsg));
			}

			inline void Flush() override
			{
				std::lock_guard<Mutex> lock(m_Mutex)
				m_File.Flush();
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
		};

		using file_st = FileSink<OS::NullMutex>;
		using file_mt = FileSink<std::mutex>;
	}
}