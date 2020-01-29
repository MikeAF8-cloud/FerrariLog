#pragma once

#include <Windows.h>
#include <wincon.h>
#include <unordered_map>
#include <array>

#include "Sink.h"
#include "../Utils/OSGlobals.h"
#include "../Level.h"

namespace FerrariLog {
	namespace Sinks {

		struct WinColors
		{
			typedef enum
			{
				FG_BLACK = 0,
				FG_BLUE = FOREGROUND_BLUE,
				FG_GREEN = FOREGROUND_GREEN,
				FG_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN,
				FG_RED = FOREGROUND_RED,
				FG_PURPLE = FOREGROUND_BLUE | FOREGROUND_RED,
				FG_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
				FG_WHITE = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
				FG_GRAY = FOREGROUND_INTENSITY,
				FG_BRIGHT_BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
				FG_BRIGHT_GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
				FG_BRIGHT_CYAN = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN,
				FG_BRIGHT_RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
				FG_BRIGHT_PURPLE = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED,
				FG_BRIGHT_YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
				FG_BRIGHT_WHITE = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
				BG_BLUE = BACKGROUND_BLUE,
				BG_GREEN = BACKGROUND_GREEN,
				BG_CYAN = BACKGROUND_BLUE | BACKGROUND_GREEN,
				BG_RED = BACKGROUND_RED,
				BG_PURPLE = BACKGROUND_BLUE | BACKGROUND_RED,
				BG_YELLOW = BACKGROUND_RED | BACKGROUND_GREEN,
				BG_WHITE = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED,
				BG_GRAY = BACKGROUND_INTENSITY,
				BG_BRIGHT_BLUE = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
				BG_BRIGHT_GREEN = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
				BG_BRIGHT_CYAN = BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN,
				BG_BRIGHT_RED = BACKGROUND_INTENSITY | BACKGROUND_RED,
				BG_BRIGHT_PURPLE = BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_RED,
				BG_BRIGHT_YELLOW = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
				BG_BRIGHT_WHITE = BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED
			} Colors;
		};
		template<typename OutHandle, typename Mutex>
		class WinColorSink : public Sink
		{
		private:
			CONSOLE_SCREEN_BUFFER_INFO m_BufferInfo;
			HANDLE m_OutputHandle;
			Mutex m_Mutex;
			std::array<unsigned int, 7> m_ColorMap;
		public:

			WinColorSink()
				:m_OutputHandle(OutHandle::Handle()),	
				m_ColorMap({ { WinColors::FG_BLUE, WinColors::FG_CYAN, WinColors::FG_PURPLE, WinColors::FG_GREEN, WinColors::FG_BRIGHT_YELLOW, WinColors::FG_BRIGHT_RED, WinColors::BG_BRIGHT_RED | WinColors::FG_GRAY } })
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				::GetConsoleScreenBufferInfo(m_OutputHandle, &m_BufferInfo);
			}

			WinColorSink(const WinColorSink &other) = delete;
			WinColorSink &operator=(const WinColorSink &other) = delete;

			void SetWinColor(Levels::Level::LevelEnum level, int WinColor_color)
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_ColorMaps[level] = WinColor_color;
			}

			void SinkLog(Loggers::LogMessage& message) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				std::string messageText = std::move(Sink::m_Formatter->Format(message));

				if (message.m_AnsiColorEnd > message.m_AnsiColorStart)
				{
					::WriteConsole(m_OutputHandle, messageText.data(), message.m_AnsiColorStart, nullptr, nullptr);
					
					WORD bgColor = m_BufferInfo.wAttributes;
					bgColor &= static_cast<WORD>(~(FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED));
					::SetConsoleTextAttribute(m_OutputHandle, m_ColorMap[message.GetLevel()] | bgColor);
					
					::WriteConsole(m_OutputHandle, messageText.data() + message.m_AnsiColorStart, message.m_AnsiColorEnd - message.m_AnsiColorStart, nullptr, nullptr);
					
					::SetConsoleTextAttribute(m_OutputHandle, m_BufferInfo.wAttributes);
					
					::WriteConsole(m_OutputHandle, messageText.data() + message.m_AnsiColorEnd, messageText.size() - message.m_AnsiColorEnd, nullptr, nullptr);
				}
				else
					::WriteConsole(m_OutputHandle, messageText.data(), messageText.size(), nullptr, nullptr);
			}

			void Flush() override {}

			void SetPattern(const std::string& pattern) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_Formatter = std::make_unique<Formatters::PatternFormatter>(pattern);
			}

			void SetFormatter(std::unique_ptr<Formatters::Formatter> sinkFormatter) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_Formatter = std::move(sinkFormatter);
			}
		};

		using wincolor_stdout_st = WinColorSink<OS::ConsoleOutput, OS::NullMutex>;
		using wincolor_stdout_mt = WinColorSink<OS::ConsoleOutput, std::mutex>;
	
		using wincolor_stderr_st = WinColorSink<OS::ConsoleError, OS::NullMutex>;
		using wincolor_stderr_mt = WinColorSink<OS::ConsoleError, std::mutex>;
	}
}