#include "LoggerRegistry.h"
#include "Logger.h"

namespace FerrariLog {
	namespace Registry {

		LoggerRegistry::LoggerRegistry()
			: m_DefaultLogger(std::make_unique<Loggers::Logger>("Default Logger", std::make_unique<Sinks::wincolor_stdout_st>())),
			  m_Formatter(std::make_unique<Formatters::PatternFormatter>())
		{}

		LoggerRegistry& LoggerRegistry::RegisterInstance()
		{
			static LoggerRegistry m_Registry;
			return m_Registry;
		}
		
		void LoggerRegistry::RegisterLogger(std::unique_ptr<Loggers::Logger> logger)
		{
			std::lock_guard<std::mutex> lock(m_LoggerMutex);

			if (m_Loggers.find(logger->GetLoggerName()) != m_Loggers.end())
				throw Utils::FerrariEx("Logger with the name '" + logger->GetLoggerName() + "' already exists in the registry. Choose another logger name");
			else
				m_Loggers[logger->GetLoggerName()] = std::move(logger);
		}

		void LoggerRegistry::InitLogger(std::unique_ptr<Loggers::Logger> logger)
		{
			std::lock_guard<std::mutex> lock(m_LoggerMutex);
			logger->SetFormatter(m_Formatter->Clone());
			logger->SetLogLevel(m_LogLevel);
			logger->SetFlushLevel(m_FlushLevel);

			RegisterLogger(std::move(logger));
		}

		void LoggerRegistry::SetDefaultLogger(std::unique_ptr<Loggers::Logger> defaultLogger)
		{
			if (m_DefaultLogger != nullptr)
				m_Loggers.erase(m_DefaultLogger->GetLoggerName());
			
			m_Loggers[defaultLogger->GetLoggerName()] = std::move(defaultLogger);
			m_DefaultLogger = std::move(defaultLogger);
		}

		std::unique_ptr<Loggers::Logger> LoggerRegistry::GetLogger(const std::string& loggerName)
		{
			std::lock_guard<std::mutex> lock(m_LoggerMutex);

			auto logger = m_Loggers.find(loggerName);
			return logger == m_Loggers.end() ? nullptr : std::move(logger->second);
		}

		inline std::unique_ptr<Loggers::Logger> LoggerRegistry::GetDefLogger()
		{
			std::lock_guard<std::mutex> lock(m_LoggerMutex);
			return std::move(m_DefaultLogger);
		}

		void LoggerRegistry::SetFormatter(std::unique_ptr<Formatters::Formatter> formatter)
		{
			std::lock_guard<std::mutex> lock(m_LoggerMutex);
			
			m_Formatter = std::move(formatter);
			for (auto& logger : m_Loggers)
			{
				logger.second->SetFormatter(formatter->Clone());
			}
		}

		void LoggerRegistry::SetLevel(Levels::Level::LevelEnum logLevel)
		{
			std::lock_guard<std::mutex> lock(m_LoggerMutex);
		
			for (auto& logger : m_Loggers)
			{
				logger.second->SetLogLevel(logLevel);
			}
		}

		void LoggerRegistry::FlushLevel(Levels::Level::LevelEnum logLevel)
		{
			std::lock_guard<std::mutex> lock(m_LoggerMutex);
			
			for (auto& logger : m_Loggers)
			{
				logger.second->SetFlushLevel(logLevel);
			}
		}

		void LoggerRegistry::RemoveLogger(const std::string& loggerName)
		{
			std::lock_guard<std::mutex> lock(m_LoggerMutex);

			m_Loggers.erase(loggerName);

			if (m_DefaultLogger->GetLoggerName() == loggerName)
				m_DefaultLogger.reset();
		}

		void LoggerRegistry::ClearRegistry()
		{
			std::lock_guard<std::mutex> lock(m_LoggerMutex);

			m_Loggers.clear();
			m_DefaultLogger.reset();
		}
	}
}