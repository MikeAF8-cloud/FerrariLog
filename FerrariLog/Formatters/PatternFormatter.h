#pragma once

#include <vector>

#include "../Utils/Macros.h"
#include "Formatter.h"
#include "FlagFormatter.h"

namespace FerrariLog {
	namespace Formatters {


		class PatternFormatter : public Formatter
		{
		private:
			std::string m_Pattern;
			TimeType m_TimeType;
			std::tm m_Time;
			const char* m_Eol;

			std::vector<std::unique_ptr<FlagFormatter>> m_Formatters;
		public:
			explicit PatternFormatter(std::string pattern, TimeType ttype = TimeType::Local, const char* eol = FERRARI_EOL);

			PatternFormatter(TimeType ttype = TimeType::Local, const char* eol = FERRARI_EOL);
		
			PatternFormatter(const PatternFormatter& other) = delete;
			PatternFormatter& operator=(const PatternFormatter& other) = delete;

			inline std::string GetPattern() { return m_Pattern; }
			inline void SetPattern(std::string pattern) { m_Pattern = pattern; }

			std::string Format(Loggers::LogMessage& message) override;
			std::unique_ptr<Formatter> Clone() const override;

		private:
			void FormatPattern(std::string& pattern);
			void FindFlag(char& ch);
		};
	}
}