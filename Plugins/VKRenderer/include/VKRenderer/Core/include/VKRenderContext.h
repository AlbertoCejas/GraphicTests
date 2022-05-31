#pragma once

#include <Core/interface/VKIRenderContext.h>
#include <PotatoEngine/Render/include/BaseRenderContext.h>
#include <PotatoEngine/Render/include/BaseRenderContextLegacy.h>
#include <VKRenderer/Core/Types.h>

namespace potato
{
	namespace vk
	{
		class VKRenderContext final : public BaseRenderContext<BaseRenderContextLegacy<VKIRenderContext>>
		{
			public:

				using VKRenderContextBaseType = BaseRenderContext<BaseRenderContextLegacy<VKIRenderContext>>;

				VKRenderContext();
				~VKRenderContext();

		};
	}
}