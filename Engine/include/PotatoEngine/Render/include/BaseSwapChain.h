#pragma once

namespace potato
{
	template <class BaseInterface>
	class BaseSwapChain : public BaseInterface
	{
		public:

			virtual ~BaseSwapChain() override {}
	};
}