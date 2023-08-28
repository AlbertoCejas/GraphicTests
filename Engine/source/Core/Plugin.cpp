#include "Core/Plugin.h"

#include "Util/Assert.h"

namespace potato
{
	Plugin::Plugin(Engine& engine, int priority) :
		m_engine(engine),
		m_priority(priority)
	{
		POTATO_ASSERT_MSG(priority >= 0, "Priority must be a positive number");
	}
}