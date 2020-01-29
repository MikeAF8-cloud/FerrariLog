#pragma once

namespace FerrariLog {
	namespace Utils {

		template<typename T>
		class RotationQueue
		{
		private:
			unsigned int m_MaxSize;
		public:
			RotationQueue(unsigned int maxSize)
				: m_MaxSize(maxSize) {}

			void PushBack(T &&item)
			{

			}
		};
	}
}