#include "FlagFormatter.h"

namespace FerrariLog {
	namespace Formatters {

		FormatDetails FlagFormatter::m_Details{ nullptr, nullptr, "" };

		const char* const WeekdayFormatter::m_Weekdays[] = FERRARI_WEEKDAYS;
		const char* const ShortWeekdayFormatter::m_ShortWeekdays[] = FERRARI_SHORT_WEEKDAYS;
		const char* const MonthFormatter::m_Month[] = FERRARI_MONTHS;
		const char* const ShortMonthFormatter::m_ShortMonth[] = FERRARI_SHORT_MONTHS;
		const char* const LevelFormatter::m_Level[] = FERRARI_LEVELS;
		const char* const AbbrevLevelFormatter::m_AbbrevLevel[] = ABBREV_FERRARI_LEVELS;

		//FlagFormatter(FormatDetails details)
		//: m_Details(details) {}

		CharFormatter::CharFormatter(std::string& text)
			: m_Text(text) {}
		
		WeekdayFormatter::WeekdayFormatter() {}
		
		ShortWeekdayFormatter::ShortWeekdayFormatter() {}
		
		MonthFormatter::MonthFormatter() {}
		
		ShortMonthFormatter::ShortMonthFormatter() {}
		
		MonthDayFormatter::MonthDayFormatter() {}
		
		YearDayFormatter::YearDayFormatter() {}
		
		FourDigitYearFormatter::FourDigitYearFormatter() {}
		
		TwoDigitYearFormatter::TwoDigitYearFormatter() {}
		
		MonthNumFormatter::MonthNumFormatter() {}
		
		DateFormatter::DateFormatter() {}
		
		MilitaryHourFormatter::MilitaryHourFormatter() {}
		
		RegHourFormatter::RegHourFormatter() {}
		
		MinuteFormatter::MinuteFormatter() {}
		
		SecondFormatter::SecondFormatter() {}
		
		EpochSecondFormatter::EpochSecondFormatter() {}
		
		MilliSecondFormatter::MilliSecondFormatter() {}
		
		EpochMilliSecondFormatter::EpochMilliSecondFormatter() {}
		
		MicroSecondFormatter::MicroSecondFormatter() {}
		
		EpochMicroSecondFormatter::EpochMicroSecondFormatter() {}
		
		NanoSecondFormatter::NanoSecondFormatter() {}
		
		EpochNanoSecondFormatter::EpochNanoSecondFormatter() {}
		
		MeridiemFormatter::MeridiemFormatter() {}
		
		ClockFormatter::ClockFormatter() {}
		
		ArmyClockFormatter::ArmyClockFormatter() {}
		
		SecondArmyClockFormatter::SecondArmyClockFormatter() {}
		
		MilliArmyClockFormatter::MilliArmyClockFormatter() {}
		
		MicroArmyClockFormatter::MicroArmyClockFormatter() {}
		
		NanoArmyClockFormatter::NanoArmyClockFormatter() {}
		
		ISO8601Formatter::ISO8601Formatter() {}
		
		ColorStartFormatter::ColorStartFormatter() {}
		
		ColorEndFormatter::ColorEndFormatter() {}
		
		LoggerNameFormatter::LoggerNameFormatter() {}

		LevelFormatter::LevelFormatter() {}

		AbbrevLevelFormatter::AbbrevLevelFormatter() {}

		MessageFormatter::MessageFormatter() {}

		DefaultFormatter::DefaultFormatter() {}

		inline std::string CharFormatter::Format()
		{
			return m_Text;
		}
		
		inline std::string WeekdayFormatter::Format()
		{
			return m_Weekdays[m_Details.m_Time->tm_wday];
		}
		
		inline std::string ShortWeekdayFormatter::Format()
		{
			return m_ShortWeekdays[m_Details.m_Time->tm_wday];
		}
		
		inline std::string MonthFormatter::Format()
		{
			return m_Month[m_Details.m_Time->tm_mon];
		}
		
		inline std::string ShortMonthFormatter::Format()
		{
			return  m_ShortMonth[m_Details.m_Time->tm_mon];
		}
		
		inline std::string MonthDayFormatter::Format()
		{
			return m_Details.m_Time->tm_mday < 10 ? '0' + std::to_string(m_Details.m_Time->tm_mday) : std::to_string(m_Details.m_Time->tm_mday);
		}
		
		inline std::string YearDayFormatter::Format()
		{
			if (m_Details.m_Time->tm_yday < 100)
			{
				if (m_Details.m_Time->tm_yday < 10)
					return "00" + std::to_string(m_Details.m_Time->tm_yday);
				else
					return '0' + std::to_string(m_Details.m_Time->tm_yday);
			}
			else
				return std::to_string(m_Details.m_Time->tm_yday);
		}
		
		inline std::string FourDigitYearFormatter::Format()
		{
			return std::to_string(1900 + m_Details.m_Time->tm_year);
		}
		
		inline std::string TwoDigitYearFormatter::Format()
		{
			return std::to_string(m_Details.m_Time->tm_year % 100);
		}
		
		inline std::string MonthNumFormatter::Format()
		{
			return std::to_string(m_Details.m_Time->tm_mon + 1);
		}
		
		inline std::string DateFormatter::Format()
		{
			return m_Month.Format() + '-' + m_Day.Format() + '-' + m_Year.Format();
		}
		
		inline std::string MilitaryHourFormatter::Format()
		{
			return m_Details.m_Time->tm_hour < 10 ? '0' + std::to_string(m_Details.m_Time->tm_hour) : std::to_string(m_Details.m_Time->tm_hour);
		}
		
		inline std::string RegHourFormatter::Format()
		{
			int hour = m_Details.m_Time->tm_hour;
			if (hour == 0)
				int hour = 12;
			else if (hour > 12)
				hour -= 12;
			
			return hour < 10 ? '0' + std::to_string(hour) : std::to_string(hour);
		}
		
		inline std::string MinuteFormatter::Format()
		{
			return m_Details.m_Time->tm_min < 10 ? '0' + std::to_string(m_Details.m_Time->tm_min) : std::to_string(m_Details.m_Time->tm_min);
		}
		
		inline std::string SecondFormatter::Format()
		{
			return  m_Details.m_Time->tm_sec < 10 ? '0' + std::to_string(m_Details.m_Time->tm_sec) : std::to_string(m_Details.m_Time->tm_sec);
		}
		
		inline std::string EpochSecondFormatter::Format()
		{
			return std::to_string(OS::OS::GetEpochSecond<std::chrono::seconds>());
		}
		
		inline std::string MilliSecondFormatter::Format()
		{
			std::string milliseconds = std::to_string(OS::OS::GetEpochSecond<std::chrono::milliseconds>() % 1000);
			return milliseconds.size() < 3 ? milliseconds.size() < 2 ? "00" + milliseconds : '0' + milliseconds : milliseconds;
		}
		
		inline std::string EpochMilliSecondFormatter::Format()
		{
			return std::to_string(OS::OS::GetEpochSecond<std::chrono::milliseconds>());
		}
		
		inline std::string MicroSecondFormatter::Format()
		{
			return std::to_string(OS::OS::GetEpochSecond<std::chrono::microseconds>() % 1000000);
		}
		
		inline std::string EpochMicroSecondFormatter::Format()
		{
			return std::to_string(OS::OS::GetEpochSecond<std::chrono::microseconds>());
		}
		
		inline std::string NanoSecondFormatter::Format()
		{
			return std::to_string(OS::OS::GetEpochSecond<std::chrono::nanoseconds>() % 1000000000);
		}
		
		inline std::string EpochNanoSecondFormatter::Format()
		{
			return std::to_string(OS::OS::GetEpochSecond<std::chrono::nanoseconds>());
		}
		
		inline std::string MeridiemFormatter::Format()
		{
			return m_Details.m_Time->tm_hour > 11 ? "PM" : "AM";
		}
		
		inline std::string ClockFormatter::Format()
		{
			return m_Hour.Format() + ':' + m_Minute.Format() + ':' + m_Second.Format() + ' ' + m_Meridiam.Format();
		}
		
		inline std::string ArmyClockFormatter::Format()
		{
			return m_Hour.Format() + ':' + m_Minute.Format();
		}
		
		inline std::string SecondArmyClockFormatter::Format()
		{
			return m_ArmyClock.Format() + ':' + m_Second.Format();
		}
		
		inline std::string MilliArmyClockFormatter::Format()
		{
			return m_ArmyClock.Format() + '.' + m_Milli.Format();
		}
		
		inline std::string MicroArmyClockFormatter::Format()
		{
			return m_ArmyClock.Format() + '.' + m_Micro.Format();
		}
		
		inline std::string NanoArmyClockFormatter::Format()
		{
			return m_ArmyClock.Format() + '.' + m_Nano.Format();
		}
		
		inline std::string ISO8601Formatter::Format()
		{
			return m_Hour.Format() + ':' + m_Minute.Format() + ':' + m_Second.Format();
		}
		
		inline std::string ColorStartFormatter::Format()
		{
			m_Details.m_Message->m_AnsiColorStart = m_Details.m_Text.size();
			return "";
		}
		
		inline std::string ColorEndFormatter::Format()
		{
			m_Details.m_Message->m_AnsiColorEnd = m_Details.m_Text.size();
			return "";
		}

		inline std::string LoggerNameFormatter::Format()
		{
			return m_Details.m_Message->GetLoggerName();
		}

		inline std::string LevelFormatter::Format()
		{
			return m_Level[m_Details.m_Message->GetLevel()];
		}

		inline std::string AbbrevLevelFormatter::Format()
		{
			return m_AbbrevLevel[m_Details.m_Message->GetLevel()];
		}

		inline std::string MessageFormatter::Format()
		{
			return m_Details.m_Message->GetLogMessage();
		}

		inline std::string DefaultFormatter::Format()
		{
			return '[' + m_Date.Format() + ' ' + m_Milli.Format() + "] [" + m_LoggerName.Format() + "] [" + m_Level.Format() + "] " + m_LoggerMessage.Format();
		}
	}
}