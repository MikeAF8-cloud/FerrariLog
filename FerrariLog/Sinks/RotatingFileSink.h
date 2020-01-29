#pragma once

#include "Sink.h"

namespace FerrariLog {
	namespace Sinks {

		template<typename Mutex>
		class RotatingFileSink : public Sink
		{
		private:
			Utils::FileMaker m_File;
			std::string m_Filename;
			Mutex m_Mutex;
			unsigned int m_MaxFileSize;
			unsigned int m_MaxFiles, m_CurrentSize;
		public:
			RotatingFileSink(const std::string& filename, unsigned int maxSize, unsigned int maxFiles)
				: m_Filename(filename), m_MaxFileSize(maxSize), m_MaxFiles(maxFiles)
			{
				m_File.OpenFile(FileTimeCalc(m_Filename, false));
			}

			void SinkLog(Loggers::LogMessage& msg) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);

				std::string messageText = Sink::m_Formatter->Format(msg);
				m_CurrentSize += messageText.size();
				if (m_CurrentSize > m_MaxFileSize)
				{
					RotateFile();
					m_CurrentSize = messageText.size();
				}
				m_File.Write(messageText);
			}

			void Flush() override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_File.Flush();
			}

			void SetPattern(const std::string& pattern) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_Formatter = std::unique_ptr<Formatters::Formatter>(new Formatters::PatternFormatter(pattern));
			}

			void SetFormatter(std::unique_ptr <Formatters::Formatter>sinkFormatter) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_Formatter = std::move(sinkFormatter);
			}

		private:
			static std::string FileTimeCalc(std::string& filename, int index)
			{
				if (index == 0)
					return filename;

				std::pair<std::string, std::string> fileSplit = Utils::SplitFilename(filename);
				return (fileSplit.first + std::to_string(index) + fileSplit.second);
			}

			void RotateFile()
			{
				m_File.CloseFile();
				for (int i = m_MaxFiles; i > 0; --i)
				{
					std::string sourceFile = FileTimeCalc(m_Filename, i - 1);
					if (!Utils::FileCheck(sourceFile))
						continue;

					std::string targetFile = FileTimeCalc(m_Filename, i);
					if (!RenameFile(sourceFile, targetFile))
					{
						m_File.ReopenFile(true);
						m_CurrentSize = 0;
						throw Utils::FerrariEx("Rotation File Failed. Could not rename " + sourceFile + " to " + targetFile + ".\n");
					}
				}
				m_File.ReopenFile(true);
			}

			bool RenameFile(const std::string& sourceFile, const std::string& targetFile)
			{
				std::remove(targetFile.c_str());
				return (std::rename(sourceFile.c_str(), targetFile.c_str()) == 0);
			}
		};

		using rotating_file_st = Sinks::RotatingFileSink<OS::NullMutex>;
		using rotating_file_mt = Sinks::RotatingFileSink<std::mutex>;
		
		//inline std::unique_ptr<Loggers::Logger> rotating_logger_st(const std::string& loggerName, const std::string& filename, bool truncate = false)
		//{
		//	return rotating_file_st(loggerName, filename, truncate);
		//}
	}
}