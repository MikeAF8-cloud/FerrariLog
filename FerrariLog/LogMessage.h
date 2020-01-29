#pragma once

#include <string>

#include "Utils\Utils.h"
#include "Level.h"
#include "OS.h"

namespace FerrariLog {
	namespace Loggers {

		struct SourceLock
		{
		private:
			const char* m_Filename;
			unsigned int m_LineNum;
			const char* m_FunctionName;
		public:
			SourceLock()
				: m_Filename(""), m_LineNum(0), m_FunctionName("") {}

			SourceLock(const char* filename, unsigned int line, const char* funcname)
				: m_Filename(filename), m_LineNum(line), m_FunctionName(funcname) {}

			inline bool Empty() const throw() { return m_LineNum == 0; }
		};

		class LogMessage
		{
		private:
			std::string m_LoggerName;
			Levels::Level::LevelEnum m_Level = Levels::Level::LevelEnum::DISABLED;
			std::chrono::system_clock::time_point m_Time;
			size_t m_ThreadID, m_MessageID;
			SourceLock m_Lock;
			std::string m_Message;
		public:
			unsigned int m_AnsiColorStart;
			unsigned int m_AnsiColorEnd;
		public:
			LogMessage(SourceLock lock, const std::string& loggerName, Levels::Level::LevelEnum level, std::string& message)
				:	m_LoggerName(loggerName), 
					m_Level(level), 
					m_ThreadID(Utils::GetThreadID()),
					m_Time(OS::OS::CurrentTime()),
					m_Lock(lock),
					m_Message(message)
					{}

			LogMessage(const std::string& loggerName, Levels::Level::LevelEnum level, std::string& message)
				: LogMessage(SourceLock {}, loggerName, level, message) {}

			LogMessage() 
				: LogMessage("", Levels::Level::TRACE, (std::string)"") {}

			inline std::string GetLoggerName() { return m_LoggerName; }
			inline std::chrono::system_clock::time_point GetTime() { return m_Time; }
			inline Levels::Level::LevelEnum GetLevel() { return m_Level; }
			inline std::string GetLogMessage() { return m_Message; }
		};
	}
}