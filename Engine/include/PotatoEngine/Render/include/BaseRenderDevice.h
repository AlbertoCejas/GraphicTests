#pragma once

#include <PotatoEngine/Util/Assert.h>

namespace potato
{
	class IRenderContext;

	template <class BaseInterface>
	class BaseRenderDevice : public BaseInterface
	{
		public:

			virtual ~BaseRenderDevice() {}

			IRenderContext* getImmediateContext() const { return m_immediateRenderContext; }

			void setImmediateContext(IRenderContext& immediateContext)
			{
				m_immediateRenderContext = &immediateContext;
			}

		protected:

			IRenderContext* m_immediateRenderContext = nullptr;
	};
}