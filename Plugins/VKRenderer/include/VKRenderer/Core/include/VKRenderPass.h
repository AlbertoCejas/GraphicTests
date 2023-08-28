#pragma once

#include <Core/include/VKObjectWrapper.h>
#include <Core/include/VKRenderDevice.h>
#include <Core/interface/VKIRenderPass.h>
#include <PotatoEngine/Render/include/BaseRenderPass.h>


namespace potato
{
	namespace vk
	{

		class VKRenderPass final : public BaseRenderPass<VKIRenderPass, VKRenderDevice>
		{
			public:

				using Base = BaseRenderPass<VKIRenderPass, VKRenderDevice>;

				VKRenderPass(VKRenderDevice& renderDevice, const RenderPassDesc& desc, bool isDeviceInternal = false);
				~VKRenderPass() override;

				VkRenderPass getVkRenderPass() const final { return m_VkRenderPass; }

			private:

				RenderPassWrapper m_VkRenderPass;
		};
	}
}