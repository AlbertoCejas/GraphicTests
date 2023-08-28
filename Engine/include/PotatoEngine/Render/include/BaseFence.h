#pragma once

#include <PotatoEngine/Render/include/BaseDeviceObject.h>
#include <PotatoEngine/Render/include/GraphicsTypes.h>
#include <PotatoEngine/Render/interface/IFence.h>

namespace potato
{

	template <class BaseInterface, class RenderDeviceImplType>
	class BaseFence : public BaseDeviceObject<BaseInterface, RenderDeviceImplType, FenceDesc>
	{
		public:

			using Base = BaseDeviceObject<BaseInterface, RenderDeviceImplType, FenceDesc>;

			BaseFence(RenderDeviceImplType& device, const FenceDesc& desc, bool isDeviceInternal = false)
				: Base(device, desc, isDeviceInternal)
			{}

			virtual ~BaseFence() override {}


		protected:


	};
}