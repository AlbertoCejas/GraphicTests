#pragma once

#include <string>

namespace potato
{
	struct FenceDesc
	{
		std::string name = nullptr;
	};

	class IFence
	{
		public:

			virtual const FenceDesc& getDesc() const = 0;
	};
}