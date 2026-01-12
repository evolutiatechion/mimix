/* System Limits Header for MIMIX 3.1.2 Microkernel
 *
 * Functional Paradigm: Immutable constant definitions
 * Big O Complexity: O(1) - Compile-time constant evaluation
 * Memory Optimization: Cache-line aligned structure boundaries
 * Architecture: x86_64 with AVX-256 vector register constraints
 */

#ifndef _MIMIX_LIMITS_H
#define _MIMIX_LIMITS_H

#include <headers/ansi.h>  /* For _ALIGNED_32 and optimization macros */

/* Architecture Detection with Fallback */
#if defined(__x86_64__) || defined(__amd64__)
#define _MIMIX_X86_64          1
#define _MIMIX_64BIT           1
#define _MIMIX_REGISTER_WIDTH  64
#elif defined(__i386__) || defined(__i686__)
#define _MIMIX_X86             1
#define _MIMIX_32BIT           1
#define _MIMIX_REGISTER_WIDTH  32
#else
#define _MIMIX_GENERIC         1
#define _MIMIX_REGISTER_WIDTH  64
#endif

/* Word Size Detection - Compatible with all architectures */
#if defined(__SIZEOF_POINTER__)
#define _MIMIX_POINTER_SIZE    __SIZEOF_POINTER__
#elif defined(__x86_64__) || defined(__amd64__)
#define _MIMIX_POINTER_SIZE    8
#elif defined(__i386__) || defined(__i686__)
#define _MIMIX_POINTER_SIZE    4
#else
#define _MIMIX_POINTER_SIZE    8  /* Default to 64-bit */
#endif

/* SIMD Register Constraints for AVX-256 */
#define _MIMIX_YMM_REGISTER_BITS     256
#define _MIMIX_YMM_REGISTER_BYTES    32
#define _MIMIX_ZMM_REGISTER_BITS     512  /* Future AVX-512 compatibility */
#define _MIMIX_ZMM_REGISTER_BYTES    64

/* Character Type Limits with Cache Optimization */
#define MIMIX_CHAR_BIT               8   /* Constant: O(1) */
#define MIMIX_CHAR_MIN               (-128)
#define MIMIX_CHAR_MAX               127
#define MIMIX_SCHAR_MIN              (-128)
#define MIMIX_SCHAR_MAX              127
#define MIMIX_UCHAR_MAX              255
#define MIMIX_MB_LEN_MAX             6   /* UTF-8 maximum bytes per character */

/* Short Integer Limits */
#define MIMIX_SHRT_MIN               (-32767-1)
#define MIMIX_SHRT_MAX               32767
#define MIMIX_USHRT_MAX              0xFFFF

/* Integer Limits - Architecture Dependent */
#if defined(__SIZEOF_INT__)
#define MIMIX_INT_MIN                (-2147483647-1)
#define MIMIX_INT_MAX                2147483647
#define MIMIX_UINT_MAX               0xFFFFFFFFU
#elif defined(__INT_MAX__)
#define MIMIX_INT_MIN                ((-__INT_MAX__)-1)
#define MIMIX_INT_MAX                __INT_MAX__
#define MIMIX_UINT_MAX               (__INT_MAX__ * 2U + 1U)
#else
/* Fallback values for 32-bit architectures */
#define MIMIX_INT_MIN                (-2147483647-1)
#define MIMIX_INT_MAX                2147483647
#define MIMIX_UINT_MAX               0xFFFFFFFFU
#endif

/* Long Integer Limits - Handle 32-bit overflow */
#if defined(__SIZEOF_LONG__)
#if __SIZEOF_LONG__ == 8
#define MIMIX_LONG_MIN               (-9223372036854775807L-1)
#define MIMIX_LONG_MAX               9223372036854775807L
#define MIMIX_ULONG_MAX              0xFFFFFFFFFFFFFFFFUL
#else
#define MIMIX_LONG_MIN               (-2147483647L-1)
#define MIMIX_LONG_MAX               2147483647L
#define MIMIX_ULONG_MAX              0xFFFFFFFFUL
#endif
#else
#define MIMIX_LONG_MIN               (-2147483647L-1)
#define MIMIX_LONG_MAX               2147483647L
#define MIMIX_ULONG_MAX              0xFFFFFFFFUL
#endif

/* Long Long Integer Limits (C99 compatibility) */
#define MIMIX_LLONG_MIN              (-9223372036854775807LL-1)
#define MIMIX_LLONG_MAX              9223372036854775807LL
#define MIMIX_ULLONG_MAX             0xFFFFFFFFFFFFFFFFULL

/* MIMIX-specific Filesystem Limits */
#ifndef _MIMIX_NAME_MAX
#define _MIMIX_NAME_MAX       255
#endif

/* System Limits with Conditional Definitions */
#ifdef _POSIX_SOURCE

/* Only define POSIX limits if they're not already defined */
#ifndef _POSIX_ARG_MAX
#define _POSIX_ARG_MAX         4096
#endif

#ifndef _POSIX_CHILD_MAX
#define _POSIX_CHILD_MAX       6
#endif

#ifndef _POSIX_LINK_MAX
#define _POSIX_LINK_MAX        8
#endif

#ifndef _POSIX_MAX_CANON
#define _POSIX_MAX_CANON       255
#endif

#ifndef _POSIX_MAX_INPUT
#define _POSIX_MAX_INPUT       255
#endif

#ifndef _POSIX_NAME_MAX
#define _POSIX_NAME_MAX        _MIMIX_NAME_MAX
#endif

#ifndef _POSIX_NGROUPS_MAX
#define _POSIX_NGROUPS_MAX     0
#endif

#ifndef _POSIX_OPEN_MAX
#define _POSIX_OPEN_MAX        16
#endif

#ifndef _POSIX_PATH_MAX
#define _POSIX_PATH_MAX        255
#endif

#ifndef _POSIX_PIPE_BUF
#define _POSIX_PIPE_BUF        512
#endif

#ifndef _POSIX_STREAM_MAX
#define _POSIX_STREAM_MAX      8
#endif

#ifndef _POSIX_TZNAME_MAX
#define _POSIX_TZNAME_MAX      3
#endif

#ifndef _POSIX_SSIZE_MAX
#define _POSIX_SSIZE_MAX       32767
#endif

/* MIMIX 3.1.2 Enhanced Limits */
#define MIMIX_NO_LIMIT          0x7FFFFFFF

/* Process and Thread Limits */
#ifndef NGROUPS_MAX
#define NGROUPS_MAX            32  /* Enhanced from POSIX minimum */
#endif

#ifndef ARG_MAX
#define ARG_MAX                (128 * 1024)  /* 128KB for exec() */
#endif

#ifndef CHILD_MAX
#define CHILD_MAX              MIMIX_NO_LIMIT  /* Process forking unlimited */
#endif

#ifndef OPEN_MAX
#define OPEN_MAX               1024  /* Enhanced file descriptors */
#endif

/* Filesystem Limits */
#ifndef LINK_MAX
#define LINK_MAX               65535  /* Maximum hard links */
#endif

#ifndef MAX_CANON
#define MAX_CANON              4096  /* Enhanced canonical input */
#endif

#ifndef MAX_INPUT
#define MAX_INPUT              4096  /* Enhanced type-ahead buffer */
#endif

#ifndef NAME_MAX
#define NAME_MAX               255  /* Extended filename support */
#endif

#ifndef PATH_MAX
#define PATH_MAX               4096  /* Extended path support */
#endif

#ifndef PIPE_BUF
#define PIPE_BUF               (16 * 1024)  /* 16KB atomic writes */
#endif

#define MIMIX_PIPE_MAX         (1 * 1024 * 1024)  /* 1MB pipe capacity */

/* Stream and I/O Limits */
#ifndef STREAM_MAX
#define STREAM_MAX             1024  /* Enhanced FILE* limit */
#endif

#ifndef FOPEN_MAX
#define FOPEN_MAX              STREAM_MAX  /* Consistent with stdio */
#endif

/* Time and Locale Limits */
#ifndef TZNAME_MAX
#define TZNAME_MAX             64  /* Extended timezone support */
#endif

#define MIMIX_TIME_MAX         MIMIX_LLONG_MAX  /* 64-bit time_t support */

/* System Call Limits - Cast to int to avoid overflow warnings */
#ifndef SSIZE_MAX
#define SSIZE_MAX              2147483647  /* Maximum signed 32-bit size */
#endif

#ifndef SIZE_MAX
#define SIZE_MAX               18446744073709551615ULL  /* Maximum unsigned 64-bit size */
#endif

/* PThreads Optimization Limits */
#ifdef _MIMIX_PTHREADS_OPTIMIZED
#ifndef PTHREAD_DESTRUCTOR_ITERATIONS
#define PTHREAD_DESTRUCTOR_ITERATIONS 4
#endif

#ifndef PTHREAD_KEYS_MAX
#define PTHREAD_KEYS_MAX       1024
#endif

#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN      (64 * 1024)  /* 64KB minimum stack */
#endif

#ifndef PTHREAD_THREADS_MAX
#define PTHREAD_THREADS_MAX    MIMIX_NO_LIMIT
#endif
#endif

/* OpenCL/GP-GPU Limits */
#ifdef _MIMIX_OPENCL_SUPPORT
#define MIMIX_CL_DEVICE_MAX_WORK_GROUP_SIZE  1024
#define MIMIX_CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS 3
#define MIMIX_CL_DEVICE_MAX_COMPUTE_UNITS    64
#define MIMIX_CL_DEVICE_GLOBAL_MEM_SIZE      (8ULL * 1024 * 1024 * 1024)  /* 8GB */
#endif

/* Cryptographic Limits */
#ifdef _MIMIX_OPENSSL_CRYPTO
#define MIMIX_CRYPTO_MAX_KEY_LENGTH          64  /* 512-bit keys */
#define MIMIX_CRYPTO_MAX_IV_LENGTH           16  /* 128-bit IV */
#define MIMIX_CRYPTO_MAX_BLOCK_SIZE          32  /* 256-bit blocks */
#define MIMIX_HASH_MAX_DIGEST_SIZE           64  /* SHA-512 */
#endif

/* Cache and Memory Hierarchy Limits */
#define MIMIX_CACHE_LINE_SIZE        64
#define MIMIX_L1_CACHE_SIZE          (32 * 1024)  /* 32KB L1 */
#define MIMIX_L2_CACHE_SIZE          (512 * 1024)  /* 512KB L2 */
#define MIMIX_L3_CACHE_SIZE          (16 * 1024 * 1024)  /* 16MB L3 */

/* SIMD/AVX Vectorization Limits */
#define MIMIX_AVX256_VECTOR_ELEMENTS 8  /* 8 floats per AVX-256 register */
#define MIMIX_AVX2_VECTOR_INTEGERS   8  /* 8 ints per AVX-256 register */
#define MIMIX_FMA_PARALLEL_OPS       8  /* Fused Multiply-Add parallelism */

#endif /* _POSIX_SOURCE */

/* Compile-time Assertions for Limit Consistency */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define MIMIX_STATIC_ASSERT_LIMITS \
    _Static_assert(MIMIX_CHAR_BIT == 8, "CHAR_BIT must be 8 for MIMIX 3.1.2"); \
    _Static_assert(sizeof(int) >= 4, "int must be at least 32-bit"); \
    _Static_assert(PIPE_BUF >= 512, "PIPE_BUF must be at least POSIX minimum"); \
    _Static_assert(OPEN_MAX >= 20, "OPEN_MAX must support minimal descriptors")
#else
#define MIMIX_STATIC_ASSERT_LIMITS
#endif

#endif /* _MIMIX_LIMITS_H */
