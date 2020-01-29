#pragma once

#include <iostream>
#include <mutex>
#include <condition_variable>

namespace FerrariLog {
	namespace Utils {

		template<typename ItemType>
		class CircularQueue
		{
		private:
			std::mutex m_QueueMutex;
			std::condition_variable m_EnQueue;
			std::condition_variable m_DeQueue;
			unsigned int m_MaxItems;
			int m_Rear, m_Front;
			ItemType* m_ItemList;
		public:
			explicit CircularQueue(unsigned int maxItems)
				: m_MaxItems(maxItems + 1),
				  m_Rear(-1), m_Front(-1),
				  m_ItemList(new ItemType[m_MaxItems]) 
			{}

			void EnQueue(ItemType& item)
			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);
				
				if (IsFull())
					m_EnQueue.wait(lock, [this](){ return IsFull(); });

				if (m_Front == -1)
					m_Front = m_Rear = 0;

				m_ItemList[m_Rear] = std::move(item);
				m_Rear != m_MaxItems ? m_Rear++ : 0;

				m_DeQueue.notify_one();
			}

			void DeQueue(ItemType& item)
			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);

				if (IsEmpty())
					m_DeQueue.wait(lock, [this](){ return IsEmpty(); });
				 
				item = std::move(m_ItemList[m_Front]);
				m_Front != m_MaxItems ? m_Front++ : 0;

				if (m_Front == m_Rear)
					m_Front = m_Rear = -1;

				m_EnQueue.notify_one();
			}

			void DiplayQueue()
			{
				int i = m_Front;
				while (i != m_Rear)
				{
					std::cout << m_ItemList[i] << '\n';
					i = (i + 1) % m_MaxItems;
				}
				std::cout << m_ItemList[i] << '\n';
			}

			bool IsEmpty()
			{
				if (m_Rear == -1)
					return true;
				else
					return false;
			}

			bool IsFull()
			{
				if (((m_Rear + 1) % m_MaxItems) == m_Front)
					return true;
				else
					return false;
			}
		};
	}
}