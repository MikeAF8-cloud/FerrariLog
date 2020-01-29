#pragma once

#include <thread>
#include <memory>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <initializer_list>

#include "Macros.h"

namespace FerrariLog{
	namespace Utils {

		inline size_t GetThreadID() throw()
		{
			return static_cast<size_t>(std::hash<std::thread::id>()(std::this_thread::get_id()));
		}

#if __cplusplus >= 201402L // C++14 and beyond
		using std::make_unique;
#else
		template<typename T, typename... Args>
		inline std::unique_ptr<T> make_unique(Args &&... args)
		{
			static_assert(!std::is_array<T>::value, "Arrays are not suported");
			return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
		}
#endif

		class FerrariEx : public std::exception
		{
		private:
			std::string m_Message;
		public:
			explicit FerrariEx(const std::string& msg)
				: m_Message(msg) {}

			explicit FerrariEx(const std::string& msg, int errNum)
			{
				m_Message = msg + "(Error Number: " + std::to_string(errNum) + ")";
			}

			//virtual ~FerrariEx() throw () {}

			virtual const char* what() const throw() override
			{
				return m_Message.c_str();
			}
		};

		class FileMaker
		{
		private:
			FILE *m_FP;
			std::string m_File;
		public:
			FileMaker()
				: m_FP(nullptr) {};

			FileMaker(const FileMaker &) = delete;
			FileMaker &operator=(const FileMaker &) = delete;

			inline void OpenFile(const std::string& filename, bool truncate = false)
			{
				CloseFile();

				m_File = filename;
				const char* mode = (truncate ? "wb" : "ab");

				
				//int err = fopen_s(&m_FP, filename.c_str(), mode);

				if (!fopen_s(&m_FP, filename.c_str(), mode))
					return;
				else
					throw FerrariEx("Failed to open file [" + filename + "]", errno);
			}

			inline void CloseFile()
			{
				if (m_FP != nullptr)
				{
					std::fclose(m_FP);
					m_FP = nullptr;
				}
			}

			inline void ReopenFile(bool truncate = false)
			{
				if (m_File.empty())
					throw FerrariEx("Failed to reopen [" + m_File + "]. Was never created.");

				OpenFile(m_File, truncate);
			}

			inline void Write(std::string& text)
			{
				int err = std::fwrite(text.c_str(), 1, text.size(), m_FP);
				if (!err)
					throw FerrariEx("Failed to write to the file [" + m_File + "].");
			}

			inline void Flush()
			{
				std::fflush(m_FP);
			}

			inline const std::string GetFilename() const { return m_File; }

			~FileMaker()
			{
				CloseFile();
			}
		};

		inline bool FileCheck(const std::string& filename) FERRARI_NOEXCEPTION
		{
#ifdef _WIN32
			auto attribs = GetFileAttributes(filename.c_str());
			return (attribs != INVALID_FILE_ATTRIBUTES && !(attribs & FILE_ATTRIBUTE_DIRECTORY));
#elif LINUX_OS || APPLE_OS
			struct stat buffer;
			return (stat(filename.c_str(), &buffer) == 0);
#endif
		}

		static std::pair<std::string, std::string> SplitFilename(std::string& filename)
		{
			if (filename.empty() || filename[0] == '.')
				throw FerrariEx("Invalid Filename: No filename was given");
			
			auto extFinder = filename.find_last_of('.');
			auto dirFinder = filename.find_last_of(char('//'));

			if (extFinder < dirFinder && dirFinder != std::string::npos)
				throw FerrariEx("Invalid Filename: Cannot have a '/' character in an extension.");

			if (dirFinder != std::string::npos && filename[dirFinder + 1] == '.')
				throw FerrariEx("Invalid Filename: No filename was given");

			if (filename[filename.size() - 1] == '.')
				throw FerrariEx("Invalid Filename: No extension format after the '.' character was given");

			for (char ch : filename)
			{
				if (ch == '#' || ch == '<' || ch == '>' || ch == '&' || ch == '*' 
				 || ch == '"' || ch == '%' || ch == '|' || ch == ':')
					throw FerrariEx("Invalid Filename: File cannot contain these characters: '#' '<', '>','&', '*', '\"', '%', '|', ':'");
			}

			if (extFinder == std::string::npos)
				return std::make_pair(filename, std::string());

			return std::make_pair(filename.substr(0, extFinder), filename.substr(extFinder));
		}

		inline bool TerminalAttached(FILE* file) 
		{
#ifdef WINDOWS_OS
				return _isatty(_fileno(file)) != 0;
#else
				return isatty(fileno(file)) != 0;
#endif
		}

		inline bool SupportColor() FERRARI_NOEXCEPTION
		{
#ifdef WINDOWS_OS
			return true;
#elif defined(LINUX_OS) || defined(APPLE_OS)
			static const char* terminals[] = { "ansi", "color", "console", "cygwin", "gnome",                 
											   "konsole", "kterm", "linux", "msys", "putty",
											   "rxvt", "screen", "vt100", "xterm" };

			const char* env = std::getenv("TERM");
			if (env == nullptr)
				return false;

			return std::any_of(std::begin(terminals), std::end(terminals), [&](const char* term) { return std::strstr(env, term) != nullptr; });
#endif
		}

		template<typename... Args>
		std::vector<std::string> ArgsConvert(const Args&... args)
		{
			std::vector<std::string> argList;
			std::stringstream ss;
			(void)std::initializer_list<int> { (ss.str(""), ss << args, argList.push_back(ss.str()), 0)...};

			return argList;
		}

	}
}

