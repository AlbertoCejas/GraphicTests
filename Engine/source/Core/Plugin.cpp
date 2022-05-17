#include "Core/Plugin.h"

#include "Util/Assert.h"

namespace potato
{
	Plugin::Plugin(int priority) :
		m_priority(priority)
	{
		POTATO_ASSERT_MSG(priority >= 0, "Priority must be a positive number");
	}
}