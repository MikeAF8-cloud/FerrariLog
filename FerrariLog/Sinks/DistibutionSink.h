#pragma once
#include <algorithm>

#include "Sink.h"

namespace FerrariLog {
	namespace Sinks {

		template<typename Mutex>
		class DistributionSink : public Sink
		{
		private:
			std::vector<std::unique_ptr<Sink>> m_SinkList;
			Mutex m_Mutex;
		public:
			DistributionSink() = default;
			DistributionSink(const DistributionSink&) = delete;
			DistributionSink& operator=(const DistributionSink&) = delete;

			void AddSink(std::unique_ptr<Sink> sink)
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_SinkList.push_back(std::move(sink));
			}
			
			void SetSink(std::vector<std::unique_ptr<Sink>> sink)
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_SinkList = std::move(sink);
			}
			
			void RemoveSink(std::unique_ptr<Sink> sink)
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_SinkList.erase(std::remove(m_SinkList.begin(), m_SinkList.end(), sink), m_SinkList.end());
			}
			
			void SinkLog(Loggers::LogMessage& msg) override
			{
				for (auto& sink : m_SinkList)
				{
					sink->SinkLog(msg);
				}
			}
			
			void Flush() override
			{
				for (auto& sink : m_SinkList)
				{
					sink->Flush();
				}
			}
			
			void SetPattern(const std::string& pattern) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				SetFormatter(std::make_unique<Formatters::PatternFormatter>(pattern));
			}
			
			void SetFormatter(std::unique_ptr<Formatters::Formatter> sinkFormatter) override
			{
				Sink::m_Formatter = std::move(sinkFormatter);
				for (auto& sink : m_SinkList)
				{
					sink->SetFormatter(Sink::m_Formatter->Clone());
				}
			}
		};
		using distrib_st = DistributionSink<OS::NullMutex>;
		using distrib_mt = DistributionSink<std::mutex>;

	}
}