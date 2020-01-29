#pragma once

#include <string>

namespace FerrariLog {
	namespace Levels {

#ifndef FERRARI_LEVELS
#define FERRARI_LEVELS																	\
		{																				\
		"Fatal", "Error", "Warning", "Info", "Debug", "Release", "Trace", "Disabled"	\
		}
#endif

#ifndef ABBREV_FERRARI_LEVELS
#define ABBREV_FERRARI_LEVELS															\
		{																				\
		"F", "E", "W", "I", "D", "R", "T", "O"											\
		}
#endif

#ifndef FERRARI_LEVEL_SIZE
#define FERRARI_LEVEL_SIZE 8
#endif

		class Level {
		private:
			static const char* const m_LevelStringNames[8];
			static const char* const m_AbbrevLevelStringNames[8];
			static const char* const days[];
		public:
			typedef enum
			{
				FATAL,
				ERR,
				WARNING,
				INFO,
				DEBUG,
				RELEASE,
				TRACE,
				DISABLED
			} LevelEnum;

			inline const char* GetLevelStringName(LevelEnum level) throw() { return m_LevelStringNames[level]; }
			inline const char* GetAbrrevLevelStringName(LevelEnum level) throw() { return m_AbbrevLevelStringNames[level]; }

			const LevelEnum GetLevelEnum(std::string& name);
			const LevelEnum GetLevelEnum(const char* name);

		};
	}
}