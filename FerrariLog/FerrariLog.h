#pragma once

#include <iostream>

#include "LoggerRegistry.h"

namespace FerrariLog {

	inline void SetDefaultLogger(std::unique_ptr<Loggers::Logger> defaultLogger)
	{
		Registry::LoggerRegistry::RegisterInstance().SetDefaultLogger(std::move(defaultLogger));
	}

	inline std::unique_ptr<Loggers::Logger> GetLogger(const std::string& loggerName)
	{
		return std::move(Registry::LoggerRegistry::RegisterInstance().GetLogger(loggerName));
	}

	inline void SetFormatter(std::unique_ptr<Formatters::Formatter> formatter)
	{
		Registry::LoggerRegistry::RegisterInstance().SetFormatter(std::move(formatter));
	}

	inline void SetPattern(std::string& pattern)
	{
		SetFormatter(std::make_unique<Formatters::PatternFormatter>(pattern));
	}

	inline void FlushOn(Levels::Level::LevelEnum logLevel)
	{
		Registry::LoggerRegistry::RegisterInstance().FlushLevel(logLevel);
	}

	template<typename... Args>
	inline void Log(Loggers::SourceLock source, Levels::Level::LevelEnum level, const std::string& message, const Args&... args)
	{
		Registry::LoggerRegistry::RegisterInstance().GetDefaultLogger()->Log(source, level, message, args...);
	}

	template<typename...Args>
	inline void Log(Levels::Level::LevelEnum level, const std::string& message, const Args&... args)
	{
		Registry::LoggerRegistry::RegisterInstance().GetDefaultLogger()->Log(Loggers::SourceLock{}, level, message, args...);
	}

	template<typename... Args>
	inline void Trace(const std::string& message, const Args&... args)
	{
		Registry::LoggerRegistry::RegisterInstance().GetDefaultLogger()->Log(Levels::Level::TRACE, (std::string)message, args...);
	}
	
	template<typename... Args>
	inline void Info(const std::string& message, const Args&... args)
	{
		Registry::LoggerRegistry::RegisterInstance().GetDefaultLogger()->Log(Levels::Level::INFO, (std::string)message, args...);
	}

	template<typename... Args>
	inline void Debug(const std::string& message, const Args&... args)
	{
		Registry::LoggerRegistry::RegisterInstance().GetDefaultLogger()->Log(Levels::Level::DEBUG, (std::string)message, args...);
	}

	template<typename... Args>
	inline void Release(const std::string& message, bool multithreaded = false, const Args&... args)
	{
		Registry::LoggerRegistry::RegisterInstance().GetDefaultLogger()->Log(Levels::Level::RELEASE, (std::string)message, args...);
	}

	template<typename... Args>
	inline void Warning(const std::string& message, const Args&... args)
	{
		Registry::LoggerRegistry::RegisterInstance().GetDefaultLogger()->Log(Levels::Level::WARNING, (std::string)message, args...);
	}

	template<typename... Args>
	inline void Error(const std::string& message, const Args&... args)
	{
		Registry::LoggerRegistry::RegisterInstance().GetDefaultLogger()->Log(Levels::Level::ERR, (std::string)message, args...);
	}

	template<typename... Args>
	inline void Fatal(const std::string& message, const Args&... args)
	{
		Registry::LoggerRegistry::RegisterInstance().GetDefaultLogger()->Log(Levels::Level::FATAL, (std::string)message, args...);
	}

	template<typename Sink, typename... SinkArgs>
	inline std::unique_ptr<Sinks::Sink> CreateSink(SinkArgs&... args)
	{
		return std::make_unique<Sink>(args...);
	}

	template<typename Sink, typename... Args>
	inline std::unique_ptr<Loggers::Logger> CreateLogger(const std::string& name, const Args&... args)
	{
		auto logger = std::make_unique<Loggers::Logger>(name, std::move(CreateSink<Sink>(args...)));
		return std::make_unique<Loggers::Logger>(name, std::move(CreateSink<Sink>(args...)));
	}

	template<typename Sink, typename... Args>
	inline void Factory(const std::string& name, const Args&... args)
	{
		auto logger = std::make_unique<Loggers::Logger>(name, std::move(CreateSink<Sink>(args...)));
		Registry::LoggerRegistry::RegisterInstance().InitLogger(std::make_unique<Loggers::Logger>(name, std::move(CreateSink<Sink>(args...))););
	}

	class SynchFactory
	{
		template<typename Sink, typename... SinkArgs>
		static std::unique_ptr<Loggers::Logger> CreateLogger(std::string loggerName, SinkArgs &&... args)
		{
			auto new_logger = std::make_shared<Loggers::Logger>(std::move(loggerName), std::make_unqiue<Sink>(std::forward<SinkArgs>(args)...));
			//Registry::LoggerRegistry::RegisterInstance.initialize_logger(new_logger);
			return new_logger;
		}
	};
}