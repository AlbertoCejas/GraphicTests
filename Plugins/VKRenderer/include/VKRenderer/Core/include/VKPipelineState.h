#pragma once


#include <Core/include/VKLogicalDevice.h>
#include <Core/include/VKRenderDevice.h>
#include <Core/include/VKShaderResourceLayout.h>
#include <Core/interface/VKIPipelineState.h>
#include <PotatoEngine/Render/include/BasePipelineState.h>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace potato
{
	namespace vk
	{
		class VKPipelineState final : public BasePipelineState<VKIPipelineState, VKRenderDevice>
		{
			public:

				using Base = BasePipelineState<VKIPipelineState, VKRenderDevice>;

				VKPipelineState(VKRenderDevice& renderDevice, const GraphicsPipelineStateCreateInfo& createInfo);
				~VKPipelineState() override;


			private:

				using TShaderStages = ShaderResourceLayoutVk::TShaderStages;

				template <typename PSOCreateInfoType>
				TShaderStages initInternalObjects(
					const PSOCreateInfoType& createInfo, 
					std::vector<VkPipelineShaderStageCreateInfo>& vkShaderStages, 
					std::vector<ShaderModuleWrapper>& shaderModules);

				void initResourceLayouts(const PipelineStateCreateInfo& createInfo, TShaderStages& shaderStages);


				// Resource layout index in m_shaderResourceLayouts array for every shader stage,
				// indexed by the shader type pipeline index (returned by getShaderTypePipelineIndex)
				std::array<int8_t, MAX_SHADERS_IN_PIPELINE> m_resourceLayoutIndex = { -1, -1, -1, -1, -1, -1 };
				static_assert(MAX_SHADERS_IN_PIPELINE == 6, "Please update the initializer list above");

		};
	}
}