#pragma once

#include <cstdint>
#include <mutex>
#include <PotatoEngine/Util/Assert.h>


namespace potato
{
	template <class BaseInterface, typename CommandQueueType>
	class BaseRenderDeviceModern : public BaseInterface
	{
		public:

			BaseRenderDeviceModern(size_t commandQueueCount, CommandQueueType** commandQueues)
				: m_commandQueueCount(commandQueueCount)
			{
				for (size_t i = 0; i < m_commandQueueCount; i++)
				{
					m_commandQueues = new CommandQueue(commandQueues[i]);
				}
			}

			virtual ~BaseRenderDeviceModern() 
            {
                destroyCommandQueues();
            }

            const CommandQueueType& getCommandQueue(uint32_t queueIndex) const
            {
                POTATO_ASSERT(queueIndex < m_cmdQueueCount);
                return *m_commandQueues[queueIndex].cmdQueue;
            }

		protected:

			void destroyCommandQueues()
			{
                if (m_commandQueues != nullptr)
                {
                    for (size_t i = 0; i < m_commandQueueCount; ++i)
                    {
                        auto& queue = m_commandQueues[q];
                        //DEV_CHECK_ERR(Queue.ReleaseQueue.GetStaleResourceCount() == 0, "All stale resources must be released before destroying a command queue");
                        //DEV_CHECK_ERR(Queue.ReleaseQueue.GetPendingReleaseResourceCount() == 0, "All resources must be released before destroying a command queue");
                        queue.~CommandQueue();
                    }
                    m_commandQueues = nullptr;
                }
			}

            struct CommandQueue
            {
                CommandQueue(CommandQueueType* commandQueue) noexcept :
                    this.commandQueue(commandQueue),
                    ReleaseQueue{ Allocator }
                {
                    NextCmdBufferNumber = 0;
                }

                std::mutex mutex;
                //Atomics::AtomicInt64 NextCmdBufferNumber;
                CommandQueueType* commandQueue;
                //ResourceReleaseQueue<DynamicStaleResourceWrapper> ReleaseQueue;
            };

			const size_t m_commandQueueCount = 0;
            CommandQueue* m_commandQueues = nullptr;

	};
}