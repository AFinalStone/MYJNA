#ifndef ROUTER_PLUGIN_BASICTYPES_H__
#define ROUTER_PLUGIN_BASICTYPES_H__

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>


#define GG_LONGLONG(x) x##LL
#define GG_ULONGLONG(x) x##ULL

#define GG_INT8_C(x)    (x)
#define GG_INT16_C(x)   (x)
#define GG_INT32_C(x)   (x)
#define GG_INT64_C(x)   GG_LONGLONG(x)

#define AIOCBMAX 2048

#define DUMPPACKBUF 4096

#define _LOG_LEN 128
#define _LOG_LENGTH 4096
#define BASE_ASSERT(cond)  assert (cond)

#define _USEC_PER_SEC 1000000

typedef signed char                    schar;
typedef signed char                    int8;
typedef short                          int16;

#ifndef _INT32
#define _INT32
typedef int                            int32;
#endif

#if __LP64__
typedef long                           int64;
#else
typedef long long                      int64;
#endif

typedef unsigned char                  uint8;
typedef unsigned short                 uint16;

#ifndef _UINT32
#define _UINT32
typedef unsigned int                   uint32;
#endif

#if __LP64__
typedef unsigned long                  uint64;
#else
typedef unsigned long long             uint64;
#endif

typedef signed int                     char32;

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
    void operator=(const TypeName&);

// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
//
// This should be used in the private: declarations for a class
// that wants to prevent anyone from instantiating it. This is
// especially useful for classes containing only static methods.
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName();                                    \
  DISALLOW_COPY_AND_ASSIGN(TypeName)

int32 GetRandomTime(void);

//template <typename T, size_t N>
//char (&ArraySizeHelper(T (&array)[N]))[N];
//
//#define arraysize(array) (sizeof(ArraySizeHelper(array)))

void LogAssert(const char * function, const char * file, int line, const char * expression);

inline void MigAssert(bool result, const char * function, const char * file, int line, const char * expression) {
  if (!result) {
    LogAssert(function, file, line, expression);
    abort();
  }
}

#ifdef _DEBUG

#if defined(_MSC_VER) && _MSC_VER < 1300
#define __FUNCTION__ ""
#endif

#ifndef ASSERT
#define ASSERT(x) MigAssert((x),__FUNCTION__,__FILE__,__LINE__,#x)
#endif

#else // !ENABLE_DEBUG

#ifndef ASSERT
#define ASSERT(x) (void)0
#endif

#ifndef VERIFY
#define VERIFY(x) (void)(x)
#endif

#endif // !ENABLE_DEBUG
#endif
