#pragma once

#include <cstdint>
#include <PotatoEngine/Render/include/BaseDeviceObject.h>
#include <PotatoEngine/Render/include/GraphicsTypes.h>
#include <PotatoEngine/Render/interface/IRenderPass.h>

namespace potato
{

	template <class BaseInterface, class RenderDeviceImplType>
	class BaseRenderPass : public BaseDeviceObject<BaseInterface, RenderDeviceImplType, RenderPassDesc>
	{
		public:

			using Base = BaseDeviceObject<BaseInterface, RenderDeviceImplType, RenderPassDesc>;

			BaseRenderPass(RenderDeviceImplType& device, const RenderPassDesc& desc, bool isDeviceInternal = false)
				: Base(device, desc, isDeviceInternal)
			{
				// TODO: check there are no errors in RenderPassDesc

				m_desc = desc;
			}

			virtual ~BaseRenderPass() override {}


		protected:

            static void countSubpassAttachmentReferences(const RenderPassDesc& desc, uint32_t& totalAttachmentReferencesCount/*, uint32_t& totalPreserveAttachmentsCount*/)
            {
                totalAttachmentReferencesCount = 0;
                //totalPreserveAttachmentsCount = 0;
                for (uint32_t i = 0; i < desc.subpasses.size(); ++i)
                {
                    const auto& subpass = desc.subpasses[i];
                    //TotalAttachmentReferencesCount += Subpass.InputAttachmentCount;
                    totalAttachmentReferencesCount += subpass.RenderTargetAttachmentCount;
                    totalAttachmentReferencesCount += subpass.resolveAttachments.size();
                    if (subpass.depthStencilAttachment != nullptr)
                        totalAttachmentReferencesCount += 1;
                    //totalPreserveAttachmentsCount += subpass.PreserveAttachmentCount;
                }
            }

	};
}