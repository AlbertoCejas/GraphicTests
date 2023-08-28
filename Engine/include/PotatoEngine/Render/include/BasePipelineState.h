#pragma once

#include <cstdint>
#include <PotatoEngine/Render/include/BaseDeviceObject.h>
#include <PotatoEngine/Render/include/GraphicsTypes.h>
#include <PotatoEngine/Render/include/GraphicsAccessories.h>
#include <PotatoEngine/Render/include/InputLayout.h>
#include <PotatoEngine/Render/interface/IPipelineState.h>

namespace potato
{
    class IRenderPass;

	template <class BaseInterface, class RenderDeviceImplType>
	class BasePipelineState : public BaseDeviceObject<BaseInterface, RenderDeviceImplType, PipelineStateDesc>
	{
		public:

			BasePipelineState(RenderDeviceImplType& device, const GraphicsPipelineStateCreateInfo& graphicsPipelineStateCI, bool isDeviceInternal = false)
				: Base(device, graphicsPipelineStateCI.PSODesc, isDeviceInternal)
			{
				// TODO: validate graphicsPipelineStateCI contains valid data
			}

			virtual ~BasePipelineState() override {}

            uint32_t getNumShaderStages() const { return m_numShaderStages; }

            const GraphicsPipelineDesc& getGraphicsPipelineDesc() const final
            {
                //POTATO_ASSERT(this->m_desc.IsAnyGraphicsPipeline());
                POTATO_ASSERT(m_graphicsPipelineDesc != nullptr);
                return *m_graphicsPipelineDesc;
            }


		protected:

            template <typename ShaderImplType, typename TShaderStages>
            void extractShaders(const GraphicsPipelineStateCreateInfo& createInfo, TShaderStages& shaderStages)
            {
                POTATO_ASSERT_MSG(m_numShaderStages == 0, "The number of shader stages is not zero! ExtractShaders must only be called once.");
                POTATO_ASSERT(this->m_desc.isAnyGraphicsPipeline());

                shaderStages.clear();
                auto addShaderStage = [&](IShader* shader) {
                    if (shader != nullptr)
                    {
                        const auto shaderType = shader->getDesc().shaderType;
                        shaderStages.emplace_back(static_cast<ShaderImplType>(shader));
                        POTATO_ASSERT_MSG(m_shaderStageTypes[m_numShaderStages] == ShaderType::SHADER_TYPE_UNKNOWN, "This shader stage has already been initialized.");
                        m_shaderStageTypes[m_numShaderStages++] = shaderType;
                    }
                    };

                switch (createInfo.PSODesc.pipelineType)
                {
                case PIPELINE_TYPE_GRAPHICS:
                {
                    addShaderStage(createInfo.vsPtr);
                    //addShaderStage(createInfo.hsPtr);
                    //addShaderStage(createInfo.dsPtr);
                    //addShaderStage(createInfo.gsPtr);
                    addShaderStage(createInfo.fsPtr);
                    POTATO_ASSERT_MSG(createInfo.vsPtr != nullptr, "Vertex shader must not be null");
                    break;
                }
                /*
                case PIPELINE_TYPE_MESH:
                {
                    addShaderStage(CreateInfo.asPtr);
                    addShaderStage(CreateInfo.msPtr);
                    addShaderStage(CreateInfo.psPtr);
                    POTATO_ASSERT_MSG(createInfo.msPtr != nullptr, "Mesh shader must not be null");
                    break;
                }
                */

                default:
                    throw std::runtime_error("unknown pipeline type");
                }

                POTATO_ASSERT(!shaderStages.empty() && shaderStages.size() == m_numShaderStages);
            }

            void initializePipelineDesc(const GraphicsPipelineStateCreateInfo& createInfo)
            {
                this->m_graphicsPipelineDesc = createInfo.graphicsPipeline;

                // TODO

                m_renderPass = this->m_graphicsPipelineDesc.renderPass;
                if (m_renderPass != nullptr)
                {
                    // TODO
                }
                const auto& inputLayout = this->m_graphicsPipelineDesc.inputLayout;
                LayoutElement* layoutElements = new LayoutElement[inputLayout.numElements];
                for (size_t elem = 0; elem < inputLayout.numElements; ++elem)
                {
                    const auto& srcElem = inputLayout.layoutElements[elem];
                    layoutElements[elem] = srcElem;
                    POTATO_ASSERT(srcElem.HLSLSemantic != nullptr);
                    //layoutElements[elem].HLSLSemantic = srcElem.HLSLSemantic; -- not required if using std::string
                }
                this->m_graphicsPipelineDesc.inputLayout.layoutElements = layoutElements;

                static const constexpr uint32_t MAX_BUFFER_SLOTS = 32; // TODO - move it to constants file
                // Correct description and compute offsets and tight strides
                std::array<uint32_t, MAX_BUFFER_SLOTS> strides, tightStrides = {};
                // Set all strides to an invalid value because an application may want to use 0 stride
                strides.fill(LAYOUT_ELEMENT_AUTO_STRIDE);

                for (uint32_t i = 0; i < inputLayout.numElements; ++i)
                {
                    auto& layoutElem = layoutElements[i];

                    if (layoutElem.valueType == VT_FLOAT32 || layoutElem.valueType == VT_FLOAT16)
                        layoutElem.isNormalized = false; // Floating point values cannot be normalized

                    auto buffSlot = layoutElem.bufferSlot;
                    if (buffSlot >= strides.size())
                    {
                        //UNEXPECTED("Buffer slot (", BuffSlot, ") exceeds the maximum allowed value (", Strides.size() - 1, ")");
                        continue;
                    }
                    POTATO_ASSERT(buffSlot < m_bufferSlotsUsed);

                    auto& currAutoStride = tightStrides[buffSlot];
                    // If offset is not explicitly specified, use current auto stride value
                    if (layoutElem.relativeOffset == LAYOUT_ELEMENT_AUTO_OFFSET)
                    {
                        layoutElem.relativeOffset = currAutoStride;
                    }

                    // If stride is explicitly specified, use it for the current buffer slot
                    if (layoutElem.stride != LAYOUT_ELEMENT_AUTO_STRIDE)
                    {
                        // Verify that the value is consistent with the previously specified stride, if any
                        /*if (strides[buffSlot] != LAYOUT_ELEMENT_AUTO_STRIDE && strides[buffSlot] != layoutElem.stride)
                        {
                            LOG_ERROR_MESSAGE("Inconsistent strides are specified for buffer slot ", BuffSlot,
                                ". Input element at index ", LayoutElem.InputIndex, " explicitly specifies stride ",
                                LayoutElem.Stride, ", while current value is ", Strides[BuffSlot],
                                ". Specify consistent strides or use LAYOUT_ELEMENT_AUTO_STRIDE to allow "
                                "the engine compute strides automatically.");
                        }*/
                        POTATO_ASSERT(strides[buffSlot] == LAYOUT_ELEMENT_AUTO_STRIDE || strides[buffSlot] == layoutElem.stride);
                        strides[buffSlot] = layoutElem.stride;
                    }

                    currAutoStride = std::max(currAutoStride, layoutElem.relativeOffset + layoutElem.numComponents * getValueSize(layoutElem.valueType));
                }

                for (uint32_t i = 0; i < inputLayout.numElements; ++i)
                {
                    auto& layoutElem = layoutElements[i];

                    auto buffSlot = layoutElem.bufferSlot;
                    // If no input elements explicitly specified stride for this buffer slot, use automatic stride
                    if (strides[buffSlot] == LAYOUT_ELEMENT_AUTO_STRIDE)
                    {
                        strides[buffSlot] = tightStrides[buffSlot];
                    }
                    else
                    {
                        POTATO_ASSERT(strides[buffSlot] >= tightStrides[buffSlot]);
                        /*if (strides[buffSlot] < tightStrides[buffSlot])
                        {
                            LOG_ERROR_MESSAGE("Stride ", Strides[BuffSlot], " explicitly specified for slot ", BuffSlot,
                                " is smaller than the minimum stride ", TightStrides[BuffSlot],
                                " required to accomodate all input elements.");
                        }*/
                    }
                    if (layoutElem.stride == LAYOUT_ELEMENT_AUTO_STRIDE)
                        layoutElem.stride = strides[buffSlot];
                }

                m_strides = new uint32_t[m_bufferSlotsUsed]; // TODO: delete

                // Set strides for all unused slots to 0
                for (uint32_t i = 0; i < m_bufferSlotsUsed; ++i)
                {
                    auto stride = strides[i];
                    m_strides[i] = stride != LAYOUT_ELEMENT_AUTO_STRIDE ? stride : 0;
                }
            }

            void reserveSpaceForPipelineDesc(const GraphicsPipelineStateCreateInfo& createInfo) noexcept
            {
                //reserveResourceLayout(createInfo.PSODesc.resourceLayout);

                const auto& inputLayout = createInfo.graphicsPipeline.inputLayout;
                for (uint32_t i = 0; i < inputLayout.numElements; ++i)
                {
                    auto& layoutElem = inputLayout.layoutElements[i];
                    m_bufferSlotsUsed = std::max(m_bufferSlotsUsed, static_cast<uint8_t>(layoutElem.bufferSlot + 1));
                }

                //static_assert(std::is_trivially_destructible<decltype(*inputLayout.layoutElements)>::value, "Add destructor for this object to Destruct()");
            }



            uint32_t* strides = nullptr; 
            uint8_t m_bufferSlotsUsed = 0;

            uint8_t m_numShaderStages = 0; ///< Number of shader stages in this PSO

            constexpr static const uint32_t MAX_SHADERS_IN_PIPELINE = 6; // TODO: move it somewhere
            /// Array of shader types for every shader stage used by this PSO
            std::array<ShaderType, MAX_SHADERS_IN_PIPELINE> m_shaderStageTypes = {};

            GraphicsPipelineDesc m_graphicsPipelineDesc;

            IRenderPass* m_renderPass;

		private:

			using Base = BaseDeviceObject<BaseInterface, RenderDeviceImplType, PipelineStateDesc>;

			BasePipelineState(RenderDeviceImplType& device, const PipelineStateDesc& desc, bool isDeviceInternal = false)
				: Base(device, desc, isDeviceInternal)
			{
				
			}


	};
}