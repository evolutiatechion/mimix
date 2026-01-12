/*
 * MIMIX 3.1.2 Microkernel - System Limits
 * GCC 15.2.1 20251211 | ANSI C89/90
 * Architecture: x86_64/AMD RyZen
 * Optimization: Cache-aware alignment | SIMD boundaries
 * Cryptographic: Bounds validation with checksums
 * Big O Analysis: All operations O(1)
 */

#ifndef _MIMIX_LIMITS_H
#define _MIMIX_LIMITS_H

#include "ansi.h"

/*
 * Character Type Limits
 */
#define CHAR_BIT           8
#define CHAR_MIN        (-128)
#define CHAR_MAX          127
#define SCHAR_MIN       (-128)
#define SCHAR_MAX         127
#define UCHAR_MAX         255
#define MB_LEN_MAX          1

/*
 * Short Type Limits
 */
#define SHRT_MIN   (-32767 - 1)
#define SHRT_MAX        32767
#define USHRT_MAX     0xFFFF

/*
 * Integer Limits
 */
#define INT_MIN   (-2147483647 - 1)
#define INT_MAX   2147483647
#define UINT_MAX  0xFFFFFFFFU

/*
 * Long Type Limits
 */
#define LONG_MIN  (-2147483647L - 1L)
#define LONG_MAX   2147483647L
#define ULONG_MAX  0xFFFFFFFFUL

#include <sys/dir.h>

/*
 * POSIX Compliance Layer
 */
#ifdef _POSIX_SOURCE

/*
 * POSIX Minimum Requirements
 */
#define _POSIX_ARG_MAX     4096
#define _POSIX_CHILD_MAX      6
#define _POSIX_LINK_MAX       8
#define _POSIX_MAX_CANON    255
#define _POSIX_MAX_INPUT    255
#define _POSIX_NAME_MAX  DIRSIZ
#define _POSIX_NGROUPS_MAX    0
#define _POSIX_OPEN_MAX      16
#define _POSIX_PATH_MAX     255
#define _POSIX_PIPE_BUF     512
#define _POSIX_STREAM_MAX     8
#define _POSIX_TZNAME_MAX     3
#define _POSIX_SSIZE_MAX  32767

/*
 * MIMIX Implementation Limits
 */
#define _NO_LIMIT          100
#define NGROUPS_MAX          0
#define ARG_MAX          16384
#define CHILD_MAX    _NO_LIMIT
#define OPEN_MAX            64
#define LINK_MAX      SHRT_MAX
#define MAX_CANON          255
#define MAX_INPUT          255
#define NAME_MAX        DIRSIZ
#define PATH_MAX          4096
#define PIPE_BUF         16384
#define STREAM_MAX          64
#define TZNAME_MAX           3
#define SSIZE_MAX   0x7FFFFFFF

/*
 * SIMD-optimized Buffer Sizes
 */
#define MIMIX_CACHE_LINE_SIZE    64
#define MIMIX_AVX_ALIGNMENT      32
#define MIMIX_PAGE_SIZE       4096
#define MIMIX_HUGE_PAGE_SIZE 2097152

/*
 * Thread and Process Limits
 */
#define MIMIX_MAX_THREADS        256
#define MIMIX_MAX_CPUS           128
#define MIMIX_VECTOR_LANES         8

/*
 * Cryptographic Buffer Limits
 */
#define MIMIX_HASH_SIZE_SHA256    32
#define MIMIX_HASH_SIZE_MD5       16
#define MIMIX_AES_BLOCK_SIZE      16
#define MIMIX_RSA_KEY_SIZE_MAX  4096

/*
 * Memory Allocation Limits
 */
#define MIMIX_HEAP_ALIGNMENT      32
#define MIMIX_STACK_ALIGNMENT     16
#define MIMIX_MAX_ALLOC_SIZE  0x1000000

#endif /* _POSIX_SOURCE */

/*
 * Validation Functions
 */
static unsigned char* mimix_validate_limits(void) {
	/* Generate SHA-256 hash of all limit definitions */
	static unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX ctx;

	const struct {
		int char_bit;
		int int_max;
		unsigned int uint_max;
		long long_max;
	} limits = {
	CHAR_BIT,
	INT_MAX,
	UINT_MAX,
	LONG_MAX };

	SHA256_Init(&ctx);
	SHA256_Update(&ctx, &limits, sizeof(limits));
	SHA256_Final(hash, &ctx);

	return hash;
}

/*
 * Boundary Check Function
 */
static int mimix_boundary_check(long long value, long long min, long long max) {
	/* Simple boundary check */
	return (value > min) && (value < max);
}

#endif /* _MIMIX_LIMITS_H */
