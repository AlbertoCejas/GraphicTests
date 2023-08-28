#pragma once

#include <PotatoEngine/Render/include/BaseDeviceObject.h>
#include <PotatoEngine/Render/include/GraphicsTypes.h>
#include <PotatoEngine/Render/interface/IShader.h>

namespace potato
{

	template <class BaseInterface, class RenderDeviceImplType>
	class BaseShader : public BaseDeviceObject<BaseInterface, RenderDeviceImplType, ShaderDesc>
	{
		public:

			using Base = BaseDeviceObject<BaseInterface, RenderDeviceImplType, ShaderDesc>;

			BaseShader(RenderDeviceImplType& device, const ShaderDesc& desc, bool isDeviceInternal = false)
				: Base(device, desc, isDeviceInternal)
			{
				// TODO: check device supports shadertype
			}

			virtual ~BaseShader() override {}


		protected:


	};
}