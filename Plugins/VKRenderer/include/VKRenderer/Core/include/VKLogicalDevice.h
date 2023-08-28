#pragma once

#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>


namespace potato
{
	namespace vk
	{
        struct VKCreateInfo;

        class FenceWrapper;
		class VKPhysicalDevice;


        enum class VKHandleTypeId : uint32_t
        {
            CommandPool,
//            CommandBuffer,
//            Buffer,
//            BufferView,
//            Image,
            ImageView,
//            DeviceMemory,
            Fence,
            RenderPass,
//            Pipeline,
            ShaderModule,
//            PipelineLayout,
//            Sampler,
//            Framebuffer,
//            DescriptorPool,
//            DescriptorSetLayout,
//            DescriptorSet,
//            Semaphore,
//            Queue,
//            Event,
//            QueryPool,
//            AccelerationStructureKHR
        };

        template <typename VKObjectType, VKHandleTypeId>
        class VKObjectWrapper;

#define DEFINE_VULKAN_OBJECT_WRAPPER(Type) VKObjectWrapper<Vk##Type, VKHandleTypeId::Type>
        using CommandPoolWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(CommandPool);
//        using BufferWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(Buffer);
//        using BufferViewWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(BufferView);
//        using ImageWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(Image);
        using ImageViewWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(ImageView);
//        using DeviceMemoryWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(DeviceMemory);
        using FenceWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(Fence);
        using RenderPassWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(RenderPass);
//        using PipelineWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(Pipeline);
        using ShaderModuleWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(ShaderModule);
//        using PipelineLayoutWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(PipelineLayout);
//        using SamplerWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(Sampler);
//        using FramebufferWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(Framebuffer);
//        using DescriptorPoolWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(DescriptorPool);
//        using DescriptorSetLayoutWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(DescriptorSetLayout);
//        using SemaphoreWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(Semaphore);
//        using QueryPoolWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(QueryPool);
//        using AccelStructWrapper = DEFINE_VULKAN_OBJECT_WRAPPER(AccelerationStructureKHR);
#undef DEFINE_VULKAN_OBJECT_WRAPPER

		class VKLogicalDevice
		{
			public:

				VKLogicalDevice(const VKPhysicalDevice& physicalDevice, const VkDeviceCreateInfo& deviceCreateInfo, const VKCreateInfo& createInfo);
				~VKLogicalDevice();

				VkQueue getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex) const;
				VkDevice getVkDevice() const { return m_device; }

                CommandPoolWrapper createCommandPool(const VkCommandPoolCreateInfo& cmdPoolCI, const char* debugName = "") const;
                FenceWrapper createFence(const VkFenceCreateInfo& fenceCI, const char* debugName = "") const;
                ImageViewWrapper createImageView(const VkImageViewCreateInfo& imageViewCI, const char* debugName = "") const;
                RenderPassWrapper createRenderPass(const VkRenderPassCreateInfo& renderPassCI, const char* debugName = "") const;
                ShaderModuleWrapper createShaderModule(const VkShaderModuleCreateInfo& shaderModuleCI, const char* debugName = "") const;

                void releaseVulkanObject(CommandPoolWrapper&& CmdPool) const;
                //void releaseVulkanObject(BufferWrapper&& Buffer) const;
                //void releaseVulkanObject(BufferViewWrapper&& BufferView) const;
                //void releaseVulkanObject(ImageWrapper&& Image) const;
                void releaseVulkanObject(ImageViewWrapper&& ImageView) const;
                //void releaseVulkanObject(SamplerWrapper&& Sampler) const;
                void releaseVulkanObject(FenceWrapper&& Fence) const;
                void releaseVulkanObject(RenderPassWrapper&& RenderPass) const;
                //void releaseVulkanObject(DeviceMemoryWrapper&& Memory) const;
                //void releaseVulkanObject(PipelineWrapper&& Pipeline) const;
                void releaseVulkanObject(ShaderModuleWrapper&& ShaderModule) const;
                //void releaseVulkanObject(PipelineLayoutWrapper&& PipelineLayout) const;
                //void releaseVulkanObject(FramebufferWrapper&& Framebuffer) const;
                //void releaseVulkanObject(DescriptorPoolWrapper&& DescriptorPool) const;
                //void releaseVulkanObject(DescriptorSetLayoutWrapper&& DescriptorSetLayout) const;
                //void releaseVulkanObject(SemaphoreWrapper&& Semaphore) const;
                //void releaseVulkanObject(QueryPoolWrapper&& QueryPool) const;
                //void releaseVulkanObject(AccelStructWrapper&& AccelStruct) const;

                VkResult getFenceStatus(VkFence fence) const;
                VkResult resetFence(VkFence fence) const;
                VkResult waitForFences(uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) const;

                VkPipelineStageFlags getEnabledShaderStages() const { return m_enabledShaderStages; }

			private:

                template <typename VkObjectType, VKHandleTypeId VkTypeId, typename VkCreateObjectFuncType, typename VkObjectCreateInfoType>
                VKObjectWrapper<VkObjectType, VkTypeId> createVulkanObject(
                    VkCreateObjectFuncType VkCreateObject, 
                    const VkObjectCreateInfoType& CreateInfo,
                    const char* DebugName,
                    const char* ObjectType) const;

				VkDevice m_device = VK_NULL_HANDLE;
                VkPipelineStageFlags m_enabledShaderStages = 0;
		};
	}
}