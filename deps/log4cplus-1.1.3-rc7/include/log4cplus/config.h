/* include/log4cplus/config.h.  Generated from config.h.in by configure.  */
/* include/log4cplus/config.h.in.  Generated from configure.ac by autoheader.  */

#ifndef LOG4CPLUS_CONFIG_H

#define LOG4CPLUS_CONFIG_H

/* Defined if the compiler supports C99 style variadic macros with
   __VA_ARGS__. */
/* #undef HAS_C99_VARIADIC_MACROS */

/* Defined if the compiler supports GNU style variadic macros. */
/* #undef HAS_GNU_VARIADIC_MACROS */

/* Define to 1 if you have the `clock_gettime' function. */
#define HAVE_CLOCK_GETTIME 1

/* Define to 1 if you have the `clock_nanosleep' function. */
/* #undef HAVE_CLOCK_NANOSLEEP */

/* Defined if the compiler provides C++11 <atomic> header and increment,
   decrement operations. */
#define HAVE_CXX11_ATOMICS 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the `fcntl' function. */
/* #undef HAVE_FCNTL */

/* Define to 1 if you have the `flock' function. */
/* #undef HAVE_FLOCK */

/* Define to 1 if you have the `ftime' function. */
#define HAVE_FTIME 1

/* Define to 1 if the system has the `constructor' function attribute */
#define HAVE_FUNC_ATTRIBUTE_CONSTRUCTOR 1

/* */
/* #undef HAVE_GETADDRINFO */

/* */
/* #undef HAVE_GETHOSTBYNAME_R */

/* Define to 1 if you have the `getpid' function. */
#define HAVE_GETPID 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the `gmtime_r' function. */
/* #undef HAVE_GMTIME_R */

/* Define to 1 if you have the `htonl' function. */
/* #undef HAVE_HTONL */

/* Define to 1 if you have the `htons' function. */
/* #undef HAVE_HTONS */

/* Define to 1 if you have the `iconv' function. */
/* #undef HAVE_ICONV */

/* Define to 1 if you have the `iconv_close' function. */
/* #undef HAVE_ICONV_CLOSE */

/* Define to 1 if you have the `iconv_open' function. */
/* #undef HAVE_ICONV_OPEN */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `advapi32' library (-ladvapi32). */
#define HAVE_LIBADVAPI32 1

/* Define to 1 if you have the `libiconv' function. */
/* #undef HAVE_LIBICONV */

/* Define to 1 if you have the `libiconv_close' function. */
/* #undef HAVE_LIBICONV_CLOSE */

/* Define to 1 if you have the `libiconv_open' function. */
/* #undef HAVE_LIBICONV_OPEN */

/* Define to 1 if you have the `kernel32' library (-lkernel32). */
#define HAVE_LIBKERNEL32 1

/* Define to 1 if you have the `ws2_32' library (-lws2_32). */
#define HAVE_LIBWS2_32 1

/* Define to 1 if you have the `localtime_r' function. */
/* #undef HAVE_LOCALTIME_R */

/* Define to 1 if you have the `lockf' function. */
/* #undef HAVE_LOCKF */

/* Define to 1 if you have the `lstat' function. */
/* #undef HAVE_LSTAT */

/* Define to 1 if you have the `mbstowcs' function. */
#define HAVE_MBSTOWCS 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `nanosleep' function. */
#define HAVE_NANOSLEEP 1

/* Define to 1 if you have the `ntohl' function. */
/* #undef HAVE_NTOHL */

/* Define to 1 if you have the `ntohs' function. */
/* #undef HAVE_NTOHS */

/* Define to 1 if you have the `pipe' function. */
/* #undef HAVE_PIPE */

/* Define to 1 if you have the `pipe2' function. */
/* #undef HAVE_PIPE2 */

/* Define to 1 if you have the `poll' function. */
/* #undef HAVE_POLL */

/* Define if you have POSIX threads libraries and header files. */
/* #undef HAVE_PTHREAD */

/* Have PTHREAD_PRIO_INHERIT. */
/* #undef HAVE_PTHREAD_PRIO_INHERIT */

/* Define to 1 if you have the `shutdown' function. */
/* #undef HAVE_SHUTDOWN */

/* Define to 1 if you have the `stat' function. */
#define HAVE_STAT 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Defined if the compiler understands __thread or __declspec(thread)
   construct. */
#define HAVE_TLS_SUPPORT 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if the system has the `init_priority' variable attribute */
#define HAVE_VAR_ATTRIBUTE_INIT_PRIORITY 1

/* Define to 1 if you have the `vfprintf_s' function. */
#define HAVE_VFPRINTF_S 1

/* Define to 1 if you have the `vfwprintf_s' function. */
#define HAVE_VFWPRINTF_S 1

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Define to 1 if you have the `vsnwprintf' function. */
#define HAVE_VSNWPRINTF 1

/* Define to 1 if you have the `vsprintf_s' function. */
#define HAVE_VSPRINTF_S 1

/* Define to 1 if you have the `vswprintf_s' function. */
#define HAVE_VSWPRINTF_S 1

/* Define to 1 if you have the `wcstombs' function. */
#define HAVE_WCSTOMBS 1

/* Define to 1 if you have the `_vsnprintf_s' function. */
#define HAVE__VSNPRINTF_S 1

/* Define to 1 if you have the `_vsnwprintf' function. */
#define HAVE__VSNWPRINTF 1

/* Define to 1 if you have the `_vsnwprintf_s' function. */
#define HAVE__VSNWPRINTF_S 1

/* Defined if the compiler supports __FUNCTION__ macro. */
/* #undef HAVE___FUNCTION___MACRO */

/* Defined if the compiler supports __func__ symbol. */
/* #undef HAVE___FUNC___SYMBOL */

/* Defined if the compiler supports __PRETTY_FUNCTION__ macro. */
/* #undef HAVE___PRETTY_FUNCTION___MACRO */

/* Defined if the compiler provides __sync_add_and_fetch(). */
#define HAVE___SYNC_ADD_AND_FETCH 1

/* Defined if the compiler provides __sync_sub_and_fetch(). */
#define HAVE___SYNC_SUB_AND_FETCH 1

/* Defined for --enable-debugging builds. */
/* #undef LOG4CPLUS_DEBUGGING */

/* Defined if the compiler understands __declspec(dllimport) or
   __attribute__((visibility("default"))) or __global construct. */
#define LOG4CPLUS_DECLSPEC_EXPORT __declspec(dllexport)

/* Defined if the compiler understands __declspec(dllimport) or
   __attribute__((visibility("default"))) or __global construct. */
#define LOG4CPLUS_DECLSPEC_IMPORT __declspec(dllimport)

/* Defined if the compiler understands __attribute__((visibility("hidden")))
   or __hidden construct. */
#define LOG4CPLUS_DECLSPEC_PRIVATE /* empty */

/* */
/* #undef LOG4CPLUS_HAVE_ARPA_INET_H */

/* */
#define LOG4CPLUS_HAVE_C99_VARIADIC_MACROS 1

/* */
#define LOG4CPLUS_HAVE_CLOCK_GETTIME 1

/* */
/* #undef LOG4CPLUS_HAVE_CLOCK_NANOSLEEP */

/* */
#define LOG4CPLUS_HAVE_CXX11_ATOMICS 1

/* */
#define LOG4CPLUS_HAVE_ENAMETOOLONG 1

/* */
#define LOG4CPLUS_HAVE_ERRNO_H 1

/* */
/* #undef LOG4CPLUS_HAVE_FCNTL */

/* */
#define LOG4CPLUS_HAVE_FCNTL_H 1

/* */
/* #undef LOG4CPLUS_HAVE_FLOCK */

/* */
#define LOG4CPLUS_HAVE_FTIME 1

/* */
#define LOG4CPLUS_HAVE_FUNCTION_MACRO 1

/* */
#define LOG4CPLUS_HAVE_FUNC_ATTRIBUTE_CONSTRUCTOR 1

/* */
#define LOG4CPLUS_HAVE_FUNC_SYMBOL 1

/* */
/* #undef LOG4CPLUS_HAVE_GETADDRINFO */

/* */
/* #undef LOG4CPLUS_HAVE_GETHOSTBYNAME_R */

/* */
#define LOG4CPLUS_HAVE_GETPID 1

/* */
/* #undef LOG4CPLUS_HAVE_GETTID */

/* */
#define LOG4CPLUS_HAVE_GETTIMEOFDAY 1

/* */
/* #undef LOG4CPLUS_HAVE_GMTIME_R */

/* */
#define LOG4CPLUS_HAVE_GNU_VARIADIC_MACROS 1

/* */
/* #undef LOG4CPLUS_HAVE_HTONL */

/* */
/* #undef LOG4CPLUS_HAVE_HTONS */

/* */
/* #undef LOG4CPLUS_HAVE_ICONV */

/* */
/* #undef LOG4CPLUS_HAVE_ICONV_CLOSE */

/* */
/* #undef LOG4CPLUS_HAVE_ICONV_H */

/* */
/* #undef LOG4CPLUS_HAVE_ICONV_OPEN */

/* */
#define LOG4CPLUS_HAVE_LIMITS_H 1

/* */
/* #undef LOG4CPLUS_HAVE_LOCALTIME_R */

/* */
/* #undef LOG4CPLUS_HAVE_LOCKF */

/* */
/* #undef LOG4CPLUS_HAVE_LSTAT */

/* */
#define LOG4CPLUS_HAVE_MBSTOWCS 1

/* */
#define LOG4CPLUS_HAVE_NANOSLEEP 1

/* */
/* #undef LOG4CPLUS_HAVE_NETDB_H */

/* */
/* #undef LOG4CPLUS_HAVE_NETINET_IN_H */

/* */
/* #undef LOG4CPLUS_HAVE_NETINET_TCP_H */

/* */
/* #undef LOG4CPLUS_HAVE_NTOHL */

/* */
/* #undef LOG4CPLUS_HAVE_NTOHS */

/* */
/* #undef LOG4CPLUS_HAVE_PIPE */

/* */
/* #undef LOG4CPLUS_HAVE_PIPE2 */

/* */
/* #undef LOG4CPLUS_HAVE_POLL */

/* */
/* #undef LOG4CPLUS_HAVE_POLL_H */

/* */
#define LOG4CPLUS_HAVE_PRETTY_FUNCTION_MACRO 1

/* */
/* #undef LOG4CPLUS_HAVE_SHUTDOWN */

/* */
#define LOG4CPLUS_HAVE_STAT 1

/* */
#define LOG4CPLUS_HAVE_STDARG_H 1

/* */
#define LOG4CPLUS_HAVE_STDIO_H 1

/* */
#define LOG4CPLUS_HAVE_STDLIB_H 1

/* */
/* #undef LOG4CPLUS_HAVE_SYSLOG_H */

/* */
#define LOG4CPLUS_HAVE_SYS_FILE_H 1

/* */
/* #undef LOG4CPLUS_HAVE_SYS_SOCKET_H */

/* */
#define LOG4CPLUS_HAVE_SYS_STAT_H 1

/* */
/* #undef LOG4CPLUS_HAVE_SYS_SYSCALL_H */

/* */
#define LOG4CPLUS_HAVE_SYS_TIMEB_H 1

/* */
#define LOG4CPLUS_HAVE_SYS_TIME_H 1

/* */
#define LOG4CPLUS_HAVE_SYS_TYPES_H 1

/* */
#define LOG4CPLUS_HAVE_TIME_H 1

/* */
#define LOG4CPLUS_HAVE_TLS_SUPPORT 1

/* */
#define LOG4CPLUS_HAVE_UNISTD_H 1

/* */
#define LOG4CPLUS_HAVE_VAR_ATTRIBUTE_INIT_PRIORITY 1

/* */
#define LOG4CPLUS_HAVE_VFPRINTF_S 1

/* */
#define LOG4CPLUS_HAVE_VFWPRINTF_S 1

/* */
#define LOG4CPLUS_HAVE_VSNPRINTF 1

/* */
#define LOG4CPLUS_HAVE_VSNWPRINTF 1

/* */
#define LOG4CPLUS_HAVE_VSPRINTF_S 1

/* */
#define LOG4CPLUS_HAVE_VSWPRINTF_S 1

/* */
#define LOG4CPLUS_HAVE_WCHAR_H 1

/* */
#define LOG4CPLUS_HAVE_WCSTOMBS 1

/* */
#define LOG4CPLUS_HAVE__VSNPRINTF_S 1

/* */
#define LOG4CPLUS_HAVE__VSNWPRINTF 1

/* */
#define LOG4CPLUS_HAVE__VSNWPRINTF_S 1

/* */
#define LOG4CPLUS_HAVE___SYNC_ADD_AND_FETCH 1

/* */
#define LOG4CPLUS_HAVE___SYNC_SUB_AND_FETCH 1

/* Define if this is a single-threaded library. */
/* #undef LOG4CPLUS_SINGLE_THREADED */

/* */
#define LOG4CPLUS_THREAD_LOCAL_VAR thread_local

/* */
/* #undef LOG4CPLUS_USE_PTHREADS */

/* Define when iconv() is available. */
/* #undef LOG4CPLUS_WITH_ICONV */

/* Define for C99 compilers/standard libraries that support more than just the
   "C" locale. */
/* #undef LOG4CPLUS_WORKING_C_LOCALE */

/* Define for compilers/standard libraries that support more than just the "C"
   locale. */
/* #undef LOG4CPLUS_WORKING_LOCALE */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "log4cplus"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "log4cplus 1.1.3"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "log4cplus"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.1.3"

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Defined to the actual TLS support construct. */
#define TLS_SUPPORT_CONSTRUCT thread_local

/* Substitute for socklen_t */
#define socklen_t int

#endif // LOG4CPLUS_CONFIG_H
