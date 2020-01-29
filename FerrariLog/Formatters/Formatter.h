#pragma once

#include <memory>
#include "../LogMessage.h"

namespace FerrariLog {
	namespace Formatters {

		struct PaddingInfo
		{
		protected:
			typedef enum
			{ 
			left,
			right,
			corner
			} PadSide;

			size_t m_Width;
			PadSide m_Info;
		public:
			PaddingInfo() = default;
			PaddingInfo(size_t width, PadSide info)
				: m_Width(width), m_Info(info) {}

			inline bool Enabled() const { return m_Width != 0; }
		};

		class Formatter
		{
		public:
			typedef enum
			{
				Local,
				UTC,
				GMT
			} TimeType;

			Formatter() = default;
			virtual ~Formatter() = default;
			virtual std::string Format(Loggers::LogMessage& message) = 0;
			virtual std::unique_ptr<Formatter> Clone() const = 0;
		};
	}
}