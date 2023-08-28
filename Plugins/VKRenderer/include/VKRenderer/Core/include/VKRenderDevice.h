#pragma once

#include <Core/interface/VKIRenderDevice.h>
#include <PotatoEngine/Render/include/BaseRenderDevice.h>
#include <PotatoEngine/Render/include/BaseRenderDeviceModern.h>
#include <VKRenderer/Core/Types.h>
#include <VKRenderer/Core/include/VKPipelineState.h>
#include <vulkan/vulkan_core.h>

namespace potato
{
	class IPipelineState;
	class IRenderPass;
	class IShader;

	struct RenderPassDesc;
	struct ShaderCreateInfo;

	namespace vk
	{
		class VKICommandQueue;
		class VKInstance;
		class VKPhysicalDevice;
		class VKLogicalDevice;

		class VKRenderDevice final : public BaseRenderDeviceModern<BaseRenderDevice<VKIRenderDevice>, VKICommandQueue>
		{
			public:

				using Base = BaseRenderDeviceModern<BaseRenderDevice<VKIRenderDevice>, VKICommandQueue>;

				VKRenderDevice(VKInstance* instance, VKPhysicalDevice* physicalDevice, VKLogicalDevice* logicalDevice, size_t commandQueueCount, VKICommandQueue** commandQueues);
				~VKRenderDevice() final;

				VKInstance& getVKInstance() const { return *m_instance; }
				VKPhysicalDevice& getVKPhysicalDevice() const { return *m_physicalDevice; }
				VKLogicalDevice& getVKLogicalDevice() const { return *m_logicalDevice; }

				IRenderPass* createRenderPass(const RenderPassDesc& desc) final;
				IRenderPass* createRenderPass(const RenderPassDesc& desc, bool isDeviceInternal);

				IShader* createShader(const ShaderCreateInfo& ShaderCreateInfo) final;

			private:

				template <typename PSOCreateInfoType>
				IPipelineState* createPipelineState(const PSOCreateInfoType& PSOCreateInfo);

				VKInstance* m_instance = nullptr;
				VKPhysicalDevice* m_physicalDevice = nullptr;
				VKLogicalDevice* m_logicalDevice = nullptr;
		};

		template <typename PSOCreateInfoType>
		IPipelineState* VKRenderDevice::createPipelineState(const PSOCreateInfoType& PSOCreateInfo)
		{
			return new VKPipelineState(*this, PSOCreateInfo);
		}
	}
}