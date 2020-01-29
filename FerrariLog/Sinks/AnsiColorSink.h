#pragma once

#include <unordered_map>

#include "Sink.h"
#include "../Utils/OSGlobals.h"

namespace FerrariLog {
	namespace Sinks {
#ifndef FERRARI_FGCOLORS
#define FERRARI_FGCOLORS																								\
		{																												\
		"30;", "31;", "32;", "33;", "34;", "35;", "36;", "37;", "90;", "91;", "92;", "93;", "94;", "95;", "96;", "97;"	\
		}
#endif

#ifndef FERRARI_BGCOLORS
#define FERRARI_BGCOLORS																										\
		{																														\
		"40m", "41m", "42m", "43m", "44m", "45m", "46m", "47m", "100m", "101m", "102m", "103m", "104m", "105m", "106m", "107m"	\
		}
#endif
		
		struct AnsiColors
		{
			typedef enum
			{
				BLACK,
				RED,
				GREEN,
				YELLOW,
				BLUE,
				MAGENTA,
				CYAN,
				WHITE,
				BRIGHT_BLACK,
				BRIGHT_RED,
				BRIGHT_GREEN,
				BRIGHT_YELLOW,
				BRIGHT_BLUE,
				BRIGHT_MAGENTA,
				BRIGHT_CYAN,
				BRIGHT_WHITE,
			} Colors;
		};

		template<typename TargetStream,typename Mutex>
		class AnsiColorSink : public Sink
		{
		private:
			std::array<std::string, 7> m_ColorMaps;
			FILE* m_FP;
			Mutex m_Mutex;
			bool m_OutputColor;
		private:
			static const char* m_fgColors[16];
			static const char* m_bgColors[16];
		public:
			AnsiColorSink()
				: m_ColorMaps({ { "/033[34m", "/033[36m", "/033[35m", "/033[1;32;40m", "/033[1;33;40m", "/033[1;31;40m", "/033[1;30;41m" } }),
				m_FP(TargetStream::Stream()),
				m_OutputColor(Utils::TerminalAttached(m_FP) && Utils::SupportColor())
			{
				if (!m_OutputColor)
				{
					throw Utils::FerrariEx("Terminal does not support colors");
				}
			}

			~AnsiColorSink() override = default;
			AnsiColorSink(const AnsiColorSink& other) = delete;
			AnsiColorSink& operator=(const AnsiColorSink& other) = delete;

			void SetAnsiColor(	Levels::Level::LevelEnum level,
								AnsiColors::Colors foregroundColor = AnsiColors::WHITE,
								AnsiColors::Colors backgroundColor = AnsiColors::BLACK,
								bool bold = false,
								bool underline = false,
								bool italics = false,
								bool faint = false,
								bool slowBlink = false,
								bool fastBlink = false)
			{
				if (m_OutputColor)
				{
			
					std::lock_guard<Mutex> lock(m_Mutex);
					m_ColorMaps[level] = AnsiColorSequence(	foregroundColor,
															backgroundColor,
															bold,
															underline,
															italics,
															faint,
															slowBlink,
															fastBlink);
				}
			}

			void SinkLog(Loggers::LogMessage& message) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				
				std::string messageText = Sink::m_Formatter->Format(message);
				if (m_OutputColor && message.m_AnsiColorEnd > message.m_AnsiColorStart)
				{
					fwrite(messageText.data(), sizeof(char), message.m_AnsiColorStart, m_FP);
				
					std::string colorCode = m_ColorMaps[message.GetLevel()];
					fwrite(colorCode.data(), sizeof(char), colorCode.size(), m_FP);
					fwrite(messageText.data() + message.m_AnsiColorStart, sizeof(char), message.m_AnsiColorEnd - message.m_AnsiColorStart, m_FP);
				
					const char* reset = "/033[0m";
					fwrite("/033[0m", sizeof(char), 7, m_FP);
				
					fwrite(messageText.data() + message.m_AnsiColorEnd, sizeof(char), messageText.size() - message.m_AnsiColorEnd, m_FP);
				}
				else
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

			void SetFormatter(std::unique_ptr<Formatters::Formatter> sinkFormatter) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_Formatter = std::move(sinkFormatter);
			}

		private:
			std::string AnsiColorSequence(AnsiColors::Colors foregroundColor = WHITE,
										AnsiColors::Colors backgroundColor = BLACK,
										bool bold = false,
										bool underline = false,
										bool italics = false,
										bool faint = false,
										bool slowBlink = false,
										bool fastBlink = false)
			{
				std::string colorCode = "\\033[";

				if (bold == true)
					colorCode += m_Bold;
			
				if (underline == true)
					colorCode += m_Underline;
			
				if (italics == true)
					colorCode += m_Italic;
			
				if (faint == true)
					colorCode += m_Faint;
			
				if (slowBlink == true)
					colorCode += m_Faint;
			
				if (fastBlink == true)
					colorCode += m_FastBlink;

				colorCode += m_fgColors[foregroundColor];

				colorCode += (m_bgColors[backgroundColor]);

				return colorCode;
			}
		private:

			const char* m_Reset = "0;";

			//Text Font
			const char* m_Bold = "1;";
			const char* m_Faint = "2;";
			const char* m_Italic = "3;";
			const char* m_Underline = "4;";
			const char* m_SlowBlink = "5;";
			const char* m_FastBlink = "6;";
			const char* m_ClearLine = "K;";
		};
		template<typename TargetStream, typename Mutex>
		const char* AnsiColorSink<TargetStream, Mutex>::m_fgColors[16] = FERRARI_FGCOLORS;
		
		template<typename TargetStream, typename Mutex>
		const char* AnsiColorSink<TargetStream, Mutex>::m_bgColors[16] = FERRARI_BGCOLORS;

		using ansicolor_stdout_st = AnsiColorSink<OS::ConsoleOutput, OS::NullMutex>;
		using ansicolor_stdout_mt = AnsiColorSink<OS::ConsoleOutput, std::mutex>;

		using ansicolor_stderr_st = AnsiColorSink<OS::ConsoleError, OS::NullMutex>;
		using ansicolor_stderr_mt = AnsiColorSink<OS::ConsoleError, std::mutex>;
	}
}