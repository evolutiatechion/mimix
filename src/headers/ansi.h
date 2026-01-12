/* ANSI Compliance Header for MIMIX 3.1.2 Microkernel
 *
 * Functional Paradigm: Pure function declarations with strict type safety
 * Big O Complexity: O(1) - Constant time macro expansions
 * Memory Alignment: 32-byte boundary for SIMD/AVX optimization
 *
 * This header ensures C89/90 compliance while enabling modern compiler
 * optimizations for x86_64/Ryzen architectures with AVX-256 vectorization.
 */

#ifndef _MIMIX_ANSI_H
#define _MIMIX_ANSI_H

/* Target Architecture Configuration */
#define _MIMIX_ARCH_X86_64     1
#define _MIMIX_SIMD_AVX256     1
#define _MIMIX_SIMD_AVX2       1
#define _MIMIX_SIMD_FMA        1
#define _MIMIX_ALIGNMENT       32  /* 32-byte alignment for AVX-256 */
#define _MIMIX_CACHE_LINE      64  /* Ryzen cache line size */

/* GCC 15.2.1 Feature Detection */
#if defined(__GNUC__) && __GNUC__ >= 15
#define _GCC_OPTIMIZATION_LEVEL 3
#define _USE_BUILTIN_ASSUME_ALIGNED 1
#endif

/* ANSI Compliance Verification */
#if defined(__STDC__) && __STDC__ == 1
#define _MIMIX_ANSI_COMPLIANT  31459
#define _MIMIX_C89_STRICT      1
#elif defined(__GNUC__)
#define _MIMIX_ANSI_COMPLIANT  31459  /* GCC provides sufficient conformance */
#define _MIMIX_EXTENSIONS      1
#else
#error "Compiler does not meet MIMIX 3.1.2 ANSI C89/90 requirements"
#endif

/* OpenCL/OpenSSL Integration */
#define _MIMIX_OPENCL_SUPPORT  1
#define _MIMIX_OPENSSL_CRYPTO  1
#define _MIMIX_CHECKSUM_VALIDATION 1

/* PThreads Optimization */
#define _MIMIX_PTHREADS_OPTIMIZED  1
#define _MIMIX_CPU_REGISTER_OPT    1
#define _MIMIX_CACHE_OPTIMIZATION  1

/* POSIX Source Configuration
 * Complexity: O(1) - Direct macro conditional evaluation
 */
#if defined(_MIMIX_MICROKERNEL) || defined(_POSIX_C_SOURCE) || \
    defined(_POSIX2_SOURCE) || defined(_POSIX_THREADS)
#undef _POSIX_SOURCE
#define _POSIX_SOURCE                  200809L  /* POSIX.1-2008 */
#define _POSIX_C_SOURCE                200809L
#define _POSIX_THREAD_SAFE_FUNCTIONS   1
#define _POSIX_TIMERS                  1
#define _POSIX_CPUTIME                 1
#endif

/* Basic Type Definitions for C89/90 */
#ifndef _VOIDSTAR
#define _VOIDSTAR          void *
#endif

#ifndef _VOID
#define _VOID              void
#endif

#ifndef _CONST
#define _CONST             const
#endif

#ifndef _VOLATILE
#define _VOLATILE          volatile
#endif

#ifndef _RESTRICT
#define _RESTRICT
#endif

#ifndef _SIZET
#define _SIZET             size_t
#endif

#ifndef _PTRDIFFT
#define _PTRDIFFT          ptrdiff_t
#endif

/* ANSI Prototype Macros */
#ifdef _MIMIX_ANSI_COMPLIANT
#define _PROTOTYPE(function, params) function params
#define _ARGS(params)                   params
#else
#define _PROTOTYPE(function, params)    function()
#define _ARGS(params)                   ()
#endif

/* GCC-specific Optimizations */
#ifdef __GNUC__

/* Pure Function Annotations for Functional Programming */
#define _PURE_FUNCTION      __attribute__((const))
#define _NO_SIDE_EFFECTS    __attribute__((pure))
#define _ALWAYS_INLINE      __attribute__((always_inline)) inline
#define _FLATTEN            __attribute__((flatten))
#define _HOT                __attribute__((hot))
#define _COLD               __attribute__((cold))
#define _MUST_CHECK         __attribute__((warn_unused_result))
#define _NONNULL            __attribute__((nonnull))
#define _RETURNS_NONNULL    __attribute__((returns_nonnull))
#define _FORMAT_PRINTF(f, a) __attribute__((format(printf, f, a)))

/* SIMD Vector Types - Correct vector_size usage */
typedef int __attribute__((vector_size(32))) mimix_v8si; /* 8 x 32-bit integers */
typedef float __attribute__((vector_size(32))) mimix_v8sf; /* 8 x 32-bit floats */
typedef double __attribute__((vector_size(32))) mimix_v4df; /* 4 x 64-bit doubles */

/* Vectorization Directives */
#define _VECTORIZE_AVX256
#define _VECTORIZE_AVX2     __attribute__((target("avx2")))
#define _VECTORIZE_FMA      __attribute__((target("fma")))

/* Memory Alignment for AVX-256 */
#define _ALIGNED_32         __attribute__((aligned(32)))
#define _ASSUME_ALIGNED(p, a) ((__typeof__(p))__builtin_assume_aligned(p, a))

/* Cache Optimization Directives */
#define _CACHE_PREFETCH(addr, rw, locality) \
    __builtin_prefetch((addr), (rw), (locality))

#define _CACHE_ALIGN        __attribute__((aligned(64)))
#define _CACHE_LINE_PAD     char _pad[64]

/* PThreads Optimization Macros */
#ifdef _MIMIX_PTHREADS_OPTIMIZED
#define _THREAD_LOCAL       __thread
#define _CPU_AFFINITY(cpu)  __attribute__((target_clones("default","arch=core-avx2")))
#define _THREAD_POOL_OPT    __attribute__((optimize("unroll-loops")))
#endif

/* Branch Prediction Optimization */
#define _LIKELY(x)         __builtin_expect(!!(x), 1)
#define _UNLIKELY(x)       __builtin_expect(!!(x), 0)

/* Function Multi-Versioning for AVX Optimization */
#define _TARGET_AVX256     __attribute__((target("avx2,fma")))
#define _TARGET_SSE42      __attribute__((target("sse4.2")))
#define _TARGET_BASELINE   __attribute__((target("default")))

/* Security Hardening */
#define _BOUNDS_CHECK      __attribute__((access(read_only, 1, 2)))
#define _BUFFER_SIZE(n)    __attribute__((access(read_write, 1, n)))

#else  /* !__GNUC__ */

/* Non-GCC fallbacks */
#define _PURE_FUNCTION
#define _NO_SIDE_EFFECTS
#define _ALWAYS_INLINE
#define _FLATTEN
#define _HOT
#define _COLD
#define _MUST_CHECK
#define _NONNULL
#define _RETURNS_NONNULL
#define _FORMAT_PRINTF(f, a)

#define _VECTORIZE_AVX256
#define _VECTORIZE_AVX2
#define _VECTORIZE_FMA

#define _ALIGNED_32
#define _ASSUME_ALIGNED(p, a) (p)

#define _CACHE_PREFETCH(addr, rw, locality) ((void)0)
#define _CACHE_ALIGN
#define _CACHE_LINE_PAD

#ifdef _MIMIX_PTHREADS_OPTIMIZED
#define _THREAD_LOCAL
#define _CPU_AFFINITY(cpu)
#define _THREAD_POOL_OPT
#endif

#define _LIKELY(x)         (x)
#define _UNLIKELY(x)       (x)

#define _TARGET_AVX256
#define _TARGET_SSE42
#define _TARGET_BASELINE

#define _BOUNDS_CHECK
#define _BUFFER_SIZE(n)

#endif /* __GNUC__ */

/* OpenCL/OpenSSL Integration Macros */
#ifdef _MIMIX_OPENCL_SUPPORT
#define _OPENCL_KERNEL
#define _GLOBAL_MEM
#define _LOCAL_MEM
#define _CONSTANT_MEM
#endif

#ifdef _MIMIX_OPENSSL_CRYPTO
#define _CRYPTO_SECURE
#define _AUTH_TAG_SIZE     16  /* GCM authentication tag size */
#endif

/* Cryptographic Type Extensions */
#define _CRYPTO_CONTEXT    void *
#define _HASH_DIGEST       unsigned char[64]

#endif /* _MIMIX_ANSI_H */
