#pragma once

namespace potato
{
	template <class BaseInterface>
	class BaseRenderDevice : public BaseInterface
	{
		public:

			virtual ~BaseRenderDevice() {}
	};
}