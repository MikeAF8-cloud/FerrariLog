#pragma once

#include "Formatter.h"
#include "../OS.h"

namespace FerrariLog {
	namespace Formatters {

#ifndef FERRARI_WEEKDAYS
#define FERRARI_WEEKDAYS															\
	{																				\
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"	\
	}
#endif

#ifndef FERRARI_SHORT_WEEKDAYS
#define FERRARI_SHORT_WEEKDAYS							\
	{													\
	"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"	\
	}
#endif

#ifndef FERRARI_MONTHS
#define FERRARI_MONTHS																											\
	{																															\
	"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"	\
	}
#endif

#ifndef FERRARI_SHORT_MONTHS
#define FERRARI_SHORT_MONTHS																\
	{																						\
	"Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec"	\
	}
#endif

		struct FormatDetails
		{
			std::tm* m_Time;
			Loggers::LogMessage* m_Message;
			std::string m_Text;
		};

		class FlagFormatter
		{
		public:
			static FormatDetails m_Details;
		public:
			FlagFormatter() = default;
			inline virtual std::string Format() = 0;
		};

		class CharFormatter final: public FlagFormatter
		{
		private:
			std::string m_Text;
		public:
			
			CharFormatter(std::string& text);
			inline std::string Format() override;
		};
		
		class WeekdayFormatter final : public FlagFormatter
		{
		private:
			static const char* const m_Weekdays[7];
		public:
			WeekdayFormatter();
			inline std::string Format() override;
		};
		
		class ShortWeekdayFormatter final : public FlagFormatter
		{
		private:
			static const char* const m_ShortWeekdays[7];
		public:
			ShortWeekdayFormatter();
			inline std::string Format() override;
		};
		
		class MonthFormatter final : public FlagFormatter
		{
		private:
			static const char* const m_Month[12];
		public:
			MonthFormatter();
			inline std::string Format() override;
		};
		
		class ShortMonthFormatter final : public FlagFormatter
		{
		private:
			static const char* const m_ShortMonth[12];
		public:
			ShortMonthFormatter();
			inline std::string Format() override;
		};
		
		class MonthDayFormatter final : public FlagFormatter
		{
		public:
			MonthDayFormatter();
			inline std::string Format() override;
		};
		
		class YearDayFormatter final : public FlagFormatter
		{
		public:
			YearDayFormatter();
			inline std::string Format() override;
		};
		
		class FourDigitYearFormatter final : public FlagFormatter
		{
		public:
			FourDigitYearFormatter();
			inline std::string Format() override;
		};
		
		class TwoDigitYearFormatter final : public FlagFormatter
		{
		public:
			TwoDigitYearFormatter();
			inline std::string Format() override;
		};
		
		class MonthNumFormatter final : public FlagFormatter
		{
		public:
			MonthNumFormatter();
			inline std::string Format() override;
		};
		
		class DateFormatter final : public FlagFormatter
		{
		private:
			FourDigitYearFormatter m_Year;
			MonthNumFormatter m_Month;
			MonthDayFormatter m_Day;
		public:
			DateFormatter();
			inline std::string Format() override;
		};
		
		class MilitaryHourFormatter final : public FlagFormatter
		{
		public:
			MilitaryHourFormatter();
			inline std::string Format() override;
		};
		
		class RegHourFormatter final : public FlagFormatter
		{
		public:
			RegHourFormatter();
			inline std::string Format() override;
		};
		
		class MinuteFormatter final : public FlagFormatter
		{
		public:
			MinuteFormatter();
			inline std::string Format() override;
		};
		
		class SecondFormatter final : public FlagFormatter
		{
		public:
			SecondFormatter();
			inline std::string Format() override;
		};
		
		class EpochSecondFormatter final : public FlagFormatter
		{
		public:
			EpochSecondFormatter();
			inline std::string Format() override;
		};
		
		class MilliSecondFormatter final : public FlagFormatter
		{
		public:
			MilliSecondFormatter();
			inline std::string Format() override;
		};
		
		class EpochMilliSecondFormatter final : public FlagFormatter
		{
		public:
			EpochMilliSecondFormatter();
			inline std::string Format() override;
		};
		
		class MicroSecondFormatter final : public FlagFormatter
		{
		public:
			MicroSecondFormatter();
			inline std::string Format() override;
		};
		
		class EpochMicroSecondFormatter final : public FlagFormatter
		{
		public:
			EpochMicroSecondFormatter();
			inline std::string Format() override;
		};
		
		class NanoSecondFormatter final : public FlagFormatter
		{
		public:
			NanoSecondFormatter();
			inline std::string Format() override;
		}; 
		
		class EpochNanoSecondFormatter final : public FlagFormatter
		{
		public:
			EpochNanoSecondFormatter();
			inline std::string Format() override;
		};
		
		class MeridiemFormatter final : public FlagFormatter
		{
		public:
			MeridiemFormatter();
			inline std::string Format() override;
		};
		
		class ClockFormatter final : public FlagFormatter
		{
		private:
			RegHourFormatter m_Hour;
			MinuteFormatter m_Minute;
			SecondFormatter m_Second;
			MeridiemFormatter m_Meridiam;
		public:
			ClockFormatter();
			inline std::string Format() override;
		};
		
		class ArmyClockFormatter final : public FlagFormatter
		{
		private:
			MilitaryHourFormatter m_Hour;
			MinuteFormatter m_Minute;
		public:
			ArmyClockFormatter();
			inline std::string Format() override;
		};
		
		class SecondArmyClockFormatter final : public FlagFormatter
		{
		private:
			ArmyClockFormatter m_ArmyClock;
			SecondFormatter m_Second;
		public:
			SecondArmyClockFormatter();
			inline std::string Format() override;
		};
		
		class MilliArmyClockFormatter final : public FlagFormatter
		{
		private:
			SecondArmyClockFormatter m_ArmyClock;
			MilliSecondFormatter m_Milli;
		public:
			MilliArmyClockFormatter();
			inline std::string Format() override;
		};
		
		class MicroArmyClockFormatter final : public FlagFormatter
		{
		private:
			SecondArmyClockFormatter m_ArmyClock;
			MicroSecondFormatter m_Micro;
		public:
			MicroArmyClockFormatter();
			inline std::string Format() override;
		};
		
		class NanoArmyClockFormatter final : public FlagFormatter
		{
		private:
			SecondArmyClockFormatter m_ArmyClock;
			NanoSecondFormatter m_Nano;
		public:
			NanoArmyClockFormatter();
			inline std::string Format() override;
		};
		
		class ISO8601Formatter final : public FlagFormatter
		{
		private:
			MilitaryHourFormatter m_Hour;
			MinuteFormatter m_Minute;
			SecondFormatter m_Second;
		public:
			ISO8601Formatter();
			inline std::string Format() override;
		};
		
		class TimeZoneFormatter final : public FlagFormatter
		{
		private:
		};
		
		class ColorStartFormatter final : public FlagFormatter
		{
		public:
			ColorStartFormatter();
			inline std::string Format() override;
		};
		
		class ColorEndFormatter final : public FlagFormatter
		{
		public:
			ColorEndFormatter();
			inline std::string Format() override;
		};

		class LoggerNameFormatter final : public FlagFormatter
		{
		public:
			LoggerNameFormatter();
			inline std::string Format() override;
		};

		class LevelFormatter final : public FlagFormatter
		{
		private:
			static const char* const m_Level[8];
		public:
			LevelFormatter();
			inline std::string Format() override;
		};

		class AbbrevLevelFormatter final : public FlagFormatter
		{
		private:
			static const char* const m_AbbrevLevel[8];
		public:
			AbbrevLevelFormatter();
			inline std::string Format() override;
		};

		class MessageFormatter final : public FlagFormatter
		{
		public:
			MessageFormatter();
			inline std::string Format() override;
		};

		class DefaultFormatter final : public FlagFormatter
		{
		private:
			DateFormatter m_Date;
			MilliArmyClockFormatter m_Milli;
			LoggerNameFormatter m_LoggerName;
			LevelFormatter m_Level;
			MessageFormatter m_LoggerMessage;
		public:
			DefaultFormatter();
			inline std::string Format() override;
		};
	}
}