#ifndef _NANOLAT_ASSERT_H_
#define _NANOLAT_ASSERT_H_ (1)

#if defined (NDEBUG)
#  define IT_WAS_RELEAE_MODE (1)
#endif

#undef NDEBUG
#include <assert.h>

#if defined(IT_WAS_RELEAE_MODE)
#  define NDEBUG (1)
#endif

#define NL_RELEASE_ASSERT assert

#if defined(NDEBUG)
#  define NL_ASSERT(...)
#else
#  define NL_ASSERT assert
#endif

#endif /* _NANOLAT_ASSERT_H_ */
