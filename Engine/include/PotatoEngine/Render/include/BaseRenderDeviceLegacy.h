#pragma once

namespace potato
{
	template <class BaseInterface>
	class BaseRenderDeviceLegacy : public BaseInterface
	{
		public:

			virtual ~BaseRenderDeviceLegacy() {}
	};
}