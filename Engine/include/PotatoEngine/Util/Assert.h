#pragma once

#include <PotatoEngine/PotatoEngineExport.h>
#include <PotatoEngine/Util/Macros.h>

namespace potato
{
	using AssertHandler = void(*)(const char* condition, const char* message, const char* file, int line);
	POTATOENGINE_EXPORT void setHandler(AssertHandler assertHandler);

	void reportFailure(const char* condition, const char* file, int line, const char* msg, ...);
}

#ifdef POTATO_ASSERTS_ENABLED

	#define POTATO_ASSERT(condition)                                    \
		do                                                              \
		{                                                               \
			if ((condition) == false)                                   \
			{                                                           \
				reportFailure(#condition, __FILE__, __LINE__, nullptr); \
			}                                                           \
		} while (0)

	#define POTATO_ASSERT_MSG(condition, msg, ...)                                   \
		do                                                                           \
		{                                                                            \
			if ((condition) == false)                                                \
			{                                                                        \
				reportFailure(#condition, __FILE__, __LINE__, (msg), ##__VA_ARGS__); \
			}                                                                        \
		} while (0)

	#define POTATO_FATAL_ASSERT(condition)                              \
		do                                                              \
		{                                                               \
			if ((condition) == false)                                   \
			{                                                           \
				reportFailure(#condition, __FILE__, __LINE__, nullptr); \
			}                                                           \
		} while (0)

	#define POTATO_FATAL_ASSERT_MSG(condition, msg, ...)                             \
		do                                                                           \
		{                                                                            \
			if ((condition) == false)                                                \
			{                                                                        \
				reportFailure(#condition, __FILE__, __LINE__, (msg), ##__VA_ARGS__); \
			}                                                                        \
		} while (0)


	#define POTATO_FAIL_MSG(msg, ...)												 \
		do                                                                           \
		{                                                                            \
			reportFailure("fail", __FILE__, __LINE__, (msg), ##__VA_ARGS__);		 \
		} while (0)

#else

	#define POTATO_ASSERT(condition)                                        \
			do { POTATO_UNUSED(condition); } while(0)
	#define POTATO_ASSERT_MSG(condition, msg, ...)                          \
			do { POTATO_UNUSED(condition); POTATO_UNUSED(msg); } while(0)
	#define POTATO_FATAL_ASSERT(condition)                                  \
			do { POTATO_UNUSED(condition); } while(0)
	#define POTATO_FATAL_ASSERT_MSG(condition, msg, ...)                    \
			do { POTATO_UNUSED(condition); POTATO_UNUSED(msg); } while(0)

	#define POTATO_FAIL_MSG(msg, ...)										\
			do { POTATO_UNUSED(msg); } while(0)

#endif