#pragma once

#include "Sink.h"

namespace FerrariLog {
	namespace Sinks {

		template<typename Mutex>
		class DailyFileSink : public Sink
		{
		private:
			Utils::FileMaker m_File;
			std::string m_Filename;
			std::chrono::system_clock::time_point m_RotationTime;
			Mutex m_Mutex;
			int m_Hour, m_Minute, m_Second;
		public:
			DailyFileSink(const std::string& filename, int hourRotate = 0, int minuteRotate = 0, int secondRotate = 0, bool truncate = false)
				: m_Filename(filename),
				m_Hour(hourRotate < 24 ? hourRotate : 0), 
				m_Minute(minuteRotate < 60 ? minuteRotate : 0), 
				m_Second(secondRotate < 60 ? secondRotate : 0)
			{
				m_File.OpenFile(FileTimeCalc(m_Filename, OS::OS::LocalTime()));
			}

			void SinkLog(Loggers::LogMessage& msg) override
			{
				std::string messageText = Sink::m_Formatter->Format(msg);
				if (m_RotationTime < msg.GetTime())
				{
					m_File.CloseFile();
					m_File.OpenFile(FileTimeCalc(m_Filename, OS::OS::LocalTime()));
				}
				m_File.Write(messageText);
			}

			inline void Flush() override
			{
				m_File.Flush();
			}

			inline void SetPattern(const std::string& pattern) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_Formatter = std::unique_ptr<Formatters::Formatter>(new Formatters::PatternFormatter(pattern));
			}

			inline void SetFormatter(std::unique_ptr <Formatters::Formatter>sinkFormatter) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_Formatter = std::move(sinkFormatter);
			}

			void SetRotationRime(int hour, int minute, int second)
			{
				m_Hour = hour;
				m_Minute = minute;
				m_Second = second;

				RotationTime();
			}
		private:
			void RotationTime(std::tm time = OS::OS::LocalTime())
			{
				if ((m_Hour * 3600 + m_Minute * 60 + m_Second) < (time.tm_hour * 3600 + time.tm_min * 60 + time.tm_sec))
				{
					if (m_RotationTime == std::chrono::system_clock::time_point{})
					{
						time.tm_hour = m_Hour;
						time.tm_min = m_Minute;
						time.tm_sec = m_Second;
						m_RotationTime = std::chrono::system_clock::from_time_t(std::mktime(&time)) + std::chrono::hours(24);
					}
					else
						m_RotationTime += std::chrono::hours(24);
				}
				else
				{
					time.tm_hour = m_Hour;
					time.tm_min = m_Minute;
					time.tm_sec = m_Second;
					m_RotationTime = std::chrono::system_clock::from_time_t(std::mktime(&time));
				}
			}

			std::string FileTimeCalc(std::string& filename, std::tm& time)
			{
				std::pair <std::string, std::string> fileSplit = Utils::SplitFilename(filename);

				RotationTime(time);
				
				char newFilename[sizeof(m_Filename) + 12];
#ifdef _WIN32
				sprintf_s(newFilename, sizeof(newFilename), "%s(%02d-%02d-%04d)%s", fileSplit.first.c_str(), time.tm_mon + 1, time.tm_mday, time.tm_year + 1900, fileSplit.second.c_str());
#else
				sprintf(newFilename, "%s(%02d-%02d-%04d)%s", fileSplit.first.c_str(), time.tm_mon, time.tm_yday, time.tm_year + 1900, fileSplit.second.c_str());
#endif
				return std::string(newFilename);
			}
		};

		using dailyfile_st = DailyFileSink<OS::NullMutex>;
		using dailyfile_mt = DailyFileSink<std::mutex>;
	}
}