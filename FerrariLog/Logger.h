#pragma once

#include "Level.h"
#include "LogMessage.h"
#include "Sinks\WinColorSink.h"

namespace FerrariLog {
	namespace Loggers {

		class Logger 
		{
		private:
			std::string m_LoggerName;
			std::vector<std::unique_ptr<Sinks::Sink>> m_Sinks;
			Levels::Level::LevelEnum m_FlushLevel;
			Levels::Level::LevelEnum m_LogLevel;
		public:
		
			Logger(std::string name, std::unique_ptr<Sinks::Sink> initSink)
				:m_LoggerName(name), m_FlushLevel(Levels::Level::DISABLED)
			{
				m_Sinks.push_back(std::move(initSink));
			}

			template<typename... Args>
			inline void Trace(const std::string& string, const Args&... args)
			{
				Log(Levels::Level::TRACE, string, args...);
			}
		
			template<typename... Args>
			inline void Info(const std::string& string, const Args&... args)
			{
				Log(Levels::Level::INFO, string, args...);
			}

			template<typename... Args>
			inline void Debug(const std::string& string, const Args&... args)
			{
				Log(Levels::Level::DEBUG, string, args...);
			}

			template<typename... Args>
			inline void Release(const std::string& string, const Args&... args)
			{
				Log(Levels::Level::RELEASE, string, args...);
			}

			template<typename... Args>
			inline void Warning(const std::string& string, const Args&... args)
			{
				Log(Levels::Level::WARNING, string, args...);
			}

			template<typename... Args>
			inline void Error(const std::string& string, const Args&... args)
			{
				Log(Levels::Level::ERR, string, args...);
			}

			template<typename... Args>
			inline void Fatal(const std::string& string, const Args&... args)
			{
				Log(Levels::Level::FATAL, string, args...);
			}

			template<typename... Args>
			inline void Log(Levels::Level::LevelEnum level, const std::string& string, const Args &... args)
			{
				Log(Loggers::SourceLock{}, level, std::string(string), args...);
			}

			template<typename... Args>
			inline void Log(SourceLock lock, Levels::Level::LevelEnum level, std::string& message, const Args &... args)
			{
				if (ShouldLog(level))
					return;

				bool passParam = false;
		
				auto params = Utils::ArgsConvert(args...);
				
				for (unsigned int i = 0; i < message.length(); i++)
				{
					if (message.at(i) == '{')
					{
						passParam = true;
						continue;
					}
					else if (message.at(i) == '}' && passParam == true)
					{
						if (params.empty())
							throw Utils::FerrariEx("Not enough arguments were passed.");

						message.replace(i-1, 2, params[0]);
						params.erase(params.begin());
					}
				}

				if (!params.empty())
					throw Utils::FerrariEx("Too many arguments were passed.");
				
				Loggers::LogMessage logMessage(lock, m_LoggerName, level, message);
		
				SinkMessage(logMessage);
			}
		
			virtual inline void SinkMessage(Loggers::LogMessage& message)
			{
				for (auto& sink : m_Sinks)
				{
					sink->SinkLog(message);
				}
				if (ShouldFlush(message))
					FlushLogger();
			}
		
			virtual inline void FlushLogger()
			{
				for (auto& sink : m_Sinks)
				{
					sink->Flush();
				}
			}

			inline bool ShouldLog(Levels::Level::LevelEnum level)
			{
				return m_LogLevel > level;
			}

			inline bool ShouldFlush(Loggers::LogMessage& message)
			{
				if (message.GetLevel() == Levels::Level::DISABLED)
					return false;

				return m_FlushLevel > message.GetLevel();
			}

			inline const std::string GetLoggerName() const
			{
				return m_LoggerName;
			}

			inline void SetFormatter(std::unique_ptr<Formatters::Formatter> formatter)
			{
				for (auto& sink : m_Sinks)
				{
					sink->SetFormatter(formatter->Clone());
				}
			}

			inline void SetPattern(const std::string& pattern)
			{
				SetFormatter(std::make_unique<Formatters::PatternFormatter>(pattern));
			}

			inline void SetFlushLevel(Levels::Level::LevelEnum flushLevel)
			{
				m_FlushLevel = flushLevel;
			}
			
			inline void SetLogLevel(Levels::Level::LevelEnum logLevel)
			{
				m_LogLevel = logLevel;
			}

			inline void AddSink(std::unique_ptr<Sinks::Sink> sink)
			{
				m_Sinks.push_back(std::move(sink));
			}

			inline std::vector<std::unique_ptr<Sinks::Sink>>& GetSinkList()
			{
				return m_Sinks;
			}

			inline Loggers::Logger* Clone()
			{
				return this;
			}
		};
	}
}