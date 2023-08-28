#pragma once

#include <cstdint>
#include <PotatoEngine/Render/interface/IShader.h>
#include <vector>

namespace potato
{
	namespace vk
	{
		class VKIShader : public IShader
		{
			public:

				virtual const std::vector<uint32_t>& getSPIRV() const = 0;
		};
	}
}