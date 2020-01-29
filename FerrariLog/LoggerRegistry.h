#pragma once

#include <vector>
#include <mutex>
#include <unordered_map>

#include "LogMessage.h"
#include "Logger.h"

namespace FerrariLog {
	namespace Registry {

		class LoggerRegistry
		{
		private:
			std::unique_ptr<Loggers::Logger> m_DefaultLogger;
			std::string m_LoggerName;
			std::unique_ptr<Formatters::Formatter> m_Formatter;

			Levels::Level::LevelEnum m_LogLevel, m_FlushLevel;

			std::mutex m_LoggerMutex;
			std::unordered_map<std::string, std::unique_ptr<Loggers::Logger>> m_Loggers;
		public:
			LoggerRegistry();

			LoggerRegistry(const LoggerRegistry &) = delete;
			LoggerRegistry &operator=(const LoggerRegistry &) = delete;

			static LoggerRegistry& RegisterInstance();
			
			inline void RegisterLogger(std::unique_ptr<Loggers::Logger> logger);
		
			void InitLogger(std::unique_ptr<Loggers::Logger> logger);

			inline void SetDefaultLogger(std::unique_ptr<Loggers::Logger> Logger);

			inline Loggers::Logger* GetDefaultLogger() { return m_DefaultLogger.get(); }

			std::unique_ptr<Loggers::Logger> GetLogger(const std::string& loggerName);

			inline std::unique_ptr<Loggers::Logger> GetDefLogger();

			void SetFormatter(std::unique_ptr<Formatters::Formatter> formatter);

			void SetLevel(Levels::Level::LevelEnum logLevel);

			void FlushLevel(Levels::Level::LevelEnum logLevel);

			void RemoveLogger(const std::string& loggerName);

			void ClearRegistry();
		};
	}
}