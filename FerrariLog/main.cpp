#include <iostream>

#include "FerrariLog.h"
#include "Sinks\RotatingFileSink.h"
#include "Sinks\OstreamSink.h"
#include "Sinks\MSVCSink.h"
#include "Sinks\DistibutionSink.h"
#include "Sinks\DailyFileSink.h"
#include "Sinks\AnsiColorSink.h"
#include "Async\CircularQueue.h"
#include <random>

int main()
{
	using namespace FerrariLog;
	using namespace Levels;
	using namespace Formatters;
	using namespace Sinks;

	Utils::CircularQueue<int> cq(5);

	std::thread threads[4];

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, 1);

	//threads[0] = std::thread(&Utils::CircularQueue<int>::EnQueue, &cq, 5);
	//threads[2] = std::thread(&Utils::CircularQueue<int>::DeQueue, &cq);
	//threads[1] = std::thread(&Utils::CircularQueue<int>::EnQueue, &cq, 4);
	//threads[3] = std::thread(&Utils::CircularQueue<int>::EnQueue, &cq, 8);

	//thread.join();
	//for (auto& th : threads) th.join();
	//cq.DiplayQueue();
	//cq.EnQueue(3);
	//cq.EnQueue(2);
	//cq.EnQueue(4);
	//cq.EnQueue(6);
	//cq.EnQueue(5);
	//cq.DeQueue();
	//cq.EnQueue(2);
	//cq.EnQueue(8);
	//
	//cq.DiplayQueue();

	//DailyFileSink dp("totally.txt");
	
	//FileSink fs("testFile.txt");

	//std::cout << sizeof(unsigned short) << std::endl;
	
	//auto testSink = FerrariLog::CreateSink<wincolor_stdout_st>();
	//testSink->SinkLog(Loggers::LogMessage(Loggers::SourceLock{}, "LoggerName", Levels::Level::INFO, (std::string)"Hello There"));
	//
	//auto testLogger = FerrariLog::CreateLogger<rotating_file_st>("logz", "logger.txt", 80, 2);

	//auto logger = std::make_unique<Loggers::Logger>("Logger1", std::make_unique<Sinks::wincolor_stdout_st>());
	//logger->SetPattern("%^%0 %$%L");
	//logger->AddSink(std::make_unique<rotating_file_st>("logger.txt", 1024 * 8 * 5, 10));
	//logger->Info("Hello There {} {} {}!", 2, "Hello", 3.14);

	//std::cout << logger->GetLoggerName() << std::endl;

	//SinkMessage(Loggers::LogMessage(Loggers::SourceLock{}, "LoggerName", Levels::Level::INFO, (std::string)"Hello There"));

	//auto logger2 = Loggers::Logger("Logger1", Sinks::wincolor_stdout_st());
	//logger2.SinkMessage(Loggers::LogMessage(Loggers::SourceLock{}, "LoggerName", Levels::Level::INFO, (std::string)"Hello There"));

	//auto dist = std::make_unique<distrib_st>();
	//dist->AddSink(std::make_unique<wincolor_stdout_st>());
	//auto dailysink = std::make_unique<dailyfile_st>("logs.txt", 3, 11, 0);
	//
	//dailysink->SinkLog(Loggers::LogMessage(Loggers::SourceLock{}, "LoggerName", Levels::Level::INFO, (std::string)"Hello There"));
	//dailysink->SinkLog(Loggers::LogMessage(Loggers::SourceLock{}, "LoggerName", Levels::Level::INFO, (std::string)"Hello There"));
	//dailysink->SinkLog(Loggers::LogMessage(Loggers::SourceLock{}, "LoggerName", Levels::Level::INFO, (std::string)"Hello There"));


	//auto ansiSink = std::make_unique<ansicolor_stdout_st>();
	//ansiSink->SetPattern("%^%0 %$ %L");
	//ansiSink->SetAnsiColor(Levels::Level::TRACE, AnsiColors::BLUE, AnsiColors::BRIGHT_GREEN, true, true, true);
	//ansiSink->SinkLog(Loggers::LogMessage(Loggers::SourceLock{}, "LoggerName", Levels::Level::INFO, (std::string)"Hello There"));
	//std::cout << sizeof(std::atomic<Levels::Level::LevelEnum>) << std::endl;

	//auto wcs = std::make_unique<wincolor_stdout_st>();
	

	//auto sink1 = std::make_unique<wincolor_stdout_st>();
	//
	//auto sink2 = std::make_unique<rotating_file_st>("logger.txt", 80, 2);
	//auto sink3 = std::make_unique<ostream_st>();
	
	//sink1->SetPattern("%^%0 %$ %L");
	//sink1->SinkLog(Loggers::LogMessage(Loggers::SourceLock{}, "LoggerName", Levels::Level::INFO, (std::string)"Hello There"));
	
	
	//sink2->SinkLog(Loggers::LogMessage(Loggers::SourceLock{}, "LoggerName", Levels::Level::INFO, (std::string)"Hello There"));
	//sink2->SinkLog(Loggers::LogMessage(Loggers::SourceLock{}, "LoggerName", Levels::Level::INFO, (std::string)"Hello There"));
	//sink2->SinkLog(Loggers::LogMessage(Loggers::SourceLock{}, "LoggerName", Levels::Level::INFO, (std::string)"Hello There"));
	//dist->AddSink(*sink1);
	//dist->AddSink(*sink2);

	//dist->SinkLog(Loggers::LogMessage(Loggers::SourceLock{}, "LoggerName", Levels::Level::INFO, (std::string)"Hello There"));
	//dist->SinkLog(Loggers::LogMessage(Loggers::SourceLock{}, "LoggerName", Levels::Level::INFO, (std::string)"Hello There"));

	//dist->SinkLog(Loggers::LogMessage(Loggers::SourceLock{}, "LoggerName", Levels::Level::INFO, (std::string)"Hello There"));

	FerrariLog::Trace("Hello There!");
	//FerrariLog::Fatal("This will work", false);
	//FerrariLog::Info("What is going on {} {} {} ", false, "Hello", 3.5, false);


	//std::cout << format.GetMessageText() << std::endl;

	system("pause");
	return 0;
}
	//PatternFormatter format(" Weekday %W \n Weekday(Short) %w \n Month %A \n Month (Short) %a \n Month Day %B \n Year Day %b \n Year (4 Digits) %C \n Year (2 Digits) %c \n Month Number %D \n Military Hour %H \n Regular Hour %h \n Minutes %l \n Seconds (Epoch) %S \n Milliseconds (Epoch) %M \n MicroSeconds (Epoch) %U \n NanoSeconds (Epoch) %n \n Seconds %s \n MilliSeconds %m \n MicroSeconds %u \n NanoSeconds %n \n Meridiam %I \n Clock Time %E \n Date %d \n Army Clock (Seconds) %O \n Army Clock (Milli) %P \n Army Clock (Micro) %Q \n Army Clock (Nano) %R \n Default Mode %0 %% /n", Formatter::Local, "/n");
	//Logger::LogMessage m;
	//
	//format.Format(m);