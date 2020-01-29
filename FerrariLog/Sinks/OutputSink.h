#pragma once

#include "Sink.h"

namespace FerrariLog {
	namespace Sinks {

		template<typename TargetStream, typename Mutex>
		class OutputSink : public Sink
		{
		private:
			FILE* m_FP;
			Mutex m_Mutex;
		public:
			OutputSink()
				:m_FP(TargetStream::Stream()) {}

			~OutputSink() = default;

			OutputSink(const OutputSink& other) = delete;
			OutputSink& operator=(const OutputSink& other) = delete;

			void SinkLog(Loggers::LogMessage& message) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				std::string messageText = std::move(Sink::m_Formatter->Format(message));
				fwrite(messageText.data(), sizeof(char), messageText.size(), m_FP);
				fflush(m_FP);
			}

			void Flush() override 
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				fflush(m_FP);
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

		using output_stdout_st = OutputSink<OS::ConsoleOutput, OS::NullMutex>;
		using output_stdout_mt = OutputSink<OS::ConsoleOutput, std::mutex>;
			  					 
		using output_stderr_st = OutputSink<OS::ConsoleError, OS::NullMutex>;
		using output_stderr_mt = OutputSink<OS::ConsoleError, std::mutex>;
	}
}