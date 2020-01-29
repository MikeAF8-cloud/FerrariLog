#include "Level.h"

namespace FerrariLog {
	namespace Levels {

		const char* const Level::m_LevelStringNames[] = FERRARI_LEVELS;
		const char* const Level::m_AbbrevLevelStringNames[] = ABBREV_FERRARI_LEVELS;
		const char* const Level::days[] = FERRARI_LEVELS;

		const Level::LevelEnum Level::GetLevelEnum(std::string& name)
		{
			for (int i = 0; i < FERRARI_LEVEL_SIZE - 1; i++)
			{
				if (name == m_LevelStringNames[i])
					return static_cast<Level::LevelEnum>(i);
			}
			return Level::LevelEnum::DISABLED;
		}

		const Level::LevelEnum Level::GetLevelEnum(const char* name)
		{
			std::string str = name;
			return Level::GetLevelEnum(str);
		}
	}
}