#pragma once

#include <PotatoEngine/Render/interface/IRenderContext.h>
#include <VKRenderer/Core/Types.h>

namespace potato
{

	namespace vk
	{
		class VKIRenderContext : public IRenderContext
		{
			public:

				virtual ~VKIRenderContext() {}
		};
	}
}