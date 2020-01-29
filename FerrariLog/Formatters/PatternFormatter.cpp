#include "PatternFormatter.h"

namespace FerrariLog {
	namespace Formatters {

		PatternFormatter::PatternFormatter(std::string pattern, TimeType ttype, const char* eol)
			: m_Pattern(pattern), m_TimeType(ttype), m_Eol(eol)
		{
			FormatPattern(pattern);
		}
		
		PatternFormatter::PatternFormatter(TimeType ttype, const char* eol)
			: m_Pattern("%0"), m_TimeType(ttype), m_Eol(eol) 
		{
			m_Formatters.push_back(Utils::make_unique<DefaultFormatter>());
		}

		std::string PatternFormatter::Format(Loggers::LogMessage& message)
		{
			FlagFormatter::m_Details = { &FERRARI_GETTIME, &message, "" };

			for (auto& formatter : m_Formatters)
			{
				FlagFormatter::m_Details.m_Text += formatter->Format();
			}
			FlagFormatter::m_Details.m_Text += FERRARI_EOL;
			
			return FlagFormatter::m_Details.m_Text;
		}

		void PatternFormatter::FormatPattern(std::string& pattern)
		{
			if (!m_Formatters.empty())
				m_Formatters.clear();

			bool flag_found = false;
			std::string temp_str;
			for (char& ch : pattern)
			{
				if (flag_found == true)
				{
					FindFlag(ch);
					flag_found = false;
					continue;
				}
				else if (ch == '%')
				{
					if (!temp_str.empty())
					{
						m_Formatters.push_back(std::make_unique<CharFormatter>(temp_str));
						temp_str = "";
					}
					flag_found = true;
					continue;
				}
				else
				{
					temp_str += ch;
				}
			}
		}

		void PatternFormatter::FindFlag(char& ch)
		{
			switch (ch)
			{
			case 'A':
				m_Formatters.push_back(std::make_unique<MonthFormatter>());
				break;
			case 'a':
				m_Formatters.push_back(std::make_unique<ShortMonthFormatter>());
				break;
			case 'B':
				m_Formatters.push_back(std::make_unique<MonthDayFormatter>());
				break;
			case 'b':
				m_Formatters.push_back(std::make_unique<YearDayFormatter>());
				break;
			case 'C':
				m_Formatters.push_back(std::make_unique<FourDigitYearFormatter>());
				break;
			case 'c':
				m_Formatters.push_back(std::make_unique<TwoDigitYearFormatter>());
				break;
			case 'D':
				m_Formatters.push_back(std::make_unique<MonthNumFormatter>());
				break;
			case 'd':
				m_Formatters.push_back(std::make_unique<DateFormatter>());
				break;
			case 'E':
				m_Formatters.push_back(std::make_unique<ClockFormatter>());
				break;
			case 'F':
				m_Formatters.push_back(std::make_unique<MessageFormatter>());
				break;
			case 'H':
				m_Formatters.push_back(std::make_unique<MilitaryHourFormatter>());
				break;
			case 'h':
				m_Formatters.push_back(std::make_unique<RegHourFormatter>());
				break;
			case 'I':
				m_Formatters.push_back(std::make_unique<MeridiemFormatter>());
				break;
			case 'k':
				m_Formatters.push_back(std::make_unique<MinuteFormatter>());
				break;
			case 'L':
				m_Formatters.push_back(std::make_unique<LevelFormatter>());
				break;
			case 'l':
				m_Formatters.push_back(std::make_unique<AbbrevLevelFormatter>());
			case 'M':
				m_Formatters.push_back(std::make_unique<EpochMilliSecondFormatter>());
				break;
			case 'm':
				m_Formatters.push_back(std::make_unique<MilliSecondFormatter>());
				break;
			case 'N':
				m_Formatters.push_back(std::make_unique<EpochNanoSecondFormatter>());
				break;
			case 'n':
				m_Formatters.push_back(std::make_unique<NanoSecondFormatter>());
				break;
			case 'O':
				m_Formatters.push_back(std::make_unique<SecondArmyClockFormatter>());
				break;
			case 'P':
				m_Formatters.push_back(std::make_unique<MilliArmyClockFormatter>());
				break;
			case 'Q':
				m_Formatters.push_back(std::make_unique<MicroArmyClockFormatter>());
				break;
			case 'R':
				m_Formatters.push_back(std::make_unique<NanoArmyClockFormatter>());
				break;
			case 'S':
				m_Formatters.push_back(std::make_unique<EpochSecondFormatter>());
				break;
			case 's':
				m_Formatters.push_back(std::make_unique<SecondFormatter>());
				break;
			case 'T':
				m_Formatters.push_back(std::make_unique<LoggerNameFormatter>());
				break;
			case 'U':
				m_Formatters.push_back(std::make_unique<EpochMicroSecondFormatter>());
				break;
			case 'u':
				m_Formatters.push_back(std::make_unique<MicroSecondFormatter>());
				break;
			case 'W':
				m_Formatters.push_back(std::make_unique<WeekdayFormatter>());
				break;
			case 'w':
				m_Formatters.push_back(std::make_unique<ShortWeekdayFormatter>());
				break;
			case '0':
				m_Formatters.push_back(std::make_unique<DefaultFormatter>());
				break;
			case '^':
				m_Formatters.push_back(std::make_unique<ColorStartFormatter>());
				break;
			case '$':
				m_Formatters.push_back(std::make_unique<ColorEndFormatter>());
				break;
			default:
				std::string cha = std::to_string(ch);
				m_Formatters.push_back(std::make_unique<CharFormatter>("{(%" + std::string(1,ch) + ")}"));
			}
			
		}

		std::unique_ptr<Formatter> PatternFormatter::Clone() const
		{
			return std::make_unique<PatternFormatter>(m_Pattern, m_TimeType, m_Eol);
		}
	}
}