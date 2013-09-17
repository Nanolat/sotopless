#ifndef _NANOLAT_STACK_TRACE_H_
#define _NANOLAT_STACK_TRACE_H_ (1)

#include <boost/log/trivial.hpp>

#if defined(NDEBUG)
#  define NL_RETURN(rc) return (rc)
#else
#  define NL_RETURN(rc) \
   {\
	  if ((rc)) {\
        printf("Error(code:%d) At : %s (File:%s,Line:%d)\n", (rc), __FUNCTION__, __FILE__, __LINE__);              \
        BOOST_LOG_TRIVIAL(error) << "Error(code:"<<rc<< ") At :" << __FUNCTION__ << "(" << __FILE__ << "," << __LINE__ << ")"; \
      }\
      return (rc);\
   }
#endif // NDEBUG


#endif //_NANOLAT_STACK_TRACE_H_
