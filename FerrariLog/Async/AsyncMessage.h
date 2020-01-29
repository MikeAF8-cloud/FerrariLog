#pragma once

#include "AsyncLogger.h"

namespace FerrariLog {

	typedef enum
	{
		Process,
		Terminate
	} AsyncMsgType;

	class AsyncMessage
	{
	public:
		std::shared_ptr<AsyncLogger> m_Logger;
		AsyncMsgType m_Type;
		Loggers::LogMessage m_Message;
	
		AsyncMessage() = default;
		~AsyncMessage() = default;

#if defined(_MSC_VER) && _MSC_VER <= 1800

		AsyncMessage(AsyncMessage &&other)
		: m_Logger(other.m_Logger), m_Type(other.m_Type), m_Message(other.m_Message) {}

		AsyncMessage& operator=(AsyncMessage&& other)
		{
			m_Logger = other.m_Logger;
			m_Type = other.m_Type;
			m_Message = other.m_Message;
			return *this;
		}
#else
		AsyncMessage(AsyncMessage &&) = default;
		AsyncMessage &operator=(AsyncMessage &&) = default;
#endif
		AsyncMessage(std::shared_ptr<AsyncLogger> logger, AsyncMsgType type, Loggers::LogMessage& message)
			: m_Logger(logger), m_Type(type), m_Message(message) {}
		
		AsyncMessage(std::shared_ptr<AsyncLogger> logger, AsyncMsgType type)
			: m_Logger(logger), m_Type(type), m_Message(Loggers::LogMessage()) {}
		
		//AsyncMessage(AsyncMsgType type)
		//	: m_Logger(AsyncLogger()), m_Type(type), m_Message(Loggers::LogMessage()) {}
	};
}