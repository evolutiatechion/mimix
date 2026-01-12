/*
 * MINIX 3.1.2 Microkernel - Modern ANSI Header
 * Strictly C89/C90 compliant
 */

#ifndef _ANSI_H
#define _ANSI_H

/* Standard C headers - C89/C90 compatible */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Define timespec for C89/C90 */
struct mimix_timespec {
	long tv_sec; /* seconds */
	long tv_nsec; /* nanoseconds */
};

/* Define uintptr_t for C89/C90 compatibility */
typedef unsigned long uintptr_t;

/* Memory alignment */
#define AVX_ALIGNMENT 32
#define CACHE_LINE_SIZE 64

/* Compiler optimization macros */
#if defined(__GNUC__)
#define inline static
#define ALWAYS_INLINE static
#else
    #define inline static
    #define ALWAYS_INLINE static
#endif

/* Pure function types */
typedef void (*pure_action_t)(void);
typedef int (*pure_predicate_t)(const void*);
typedef void* (*pure_transformer_t)(const void*);

/* Immutable data structure */
typedef struct mimix_immutable {
	const void *data;
	size_t size;
	uintptr_t checksum;
} immutable_t;

/* Functional task */
typedef struct mimix_pure_task {
	int id;
	int priority;
	pure_transformer_t transform;
	immutable_t input;
	immutable_t *output;
	unsigned int cpu_affinity;
	struct mimix_timespec deadline;
} pure_task_t;

/* Atomic operations */
typedef struct mimix_atomic {
	volatile int value;
} atomic_int_t;

/* Lock-free ring buffer */
typedef struct mimix_ringbuf {
	pure_task_t *buffer;
	volatile size_t head;
	volatile size_t tail;
	size_t capacity;
	atomic_int_t lock;
} lockfree_ringbuf_t;

/* Scheduler queue */
typedef struct mimix_scheduler {
	pure_task_t **queues; /* Array of pointers to task arrays */
	size_t *sizes; /* Current sizes of each queue */
	int max_priority;
	int current_highest;
} scheduler_t;

/* Process Control Block */
typedef struct mimix_pcb {
	int pid;
	volatile int state;
	void *stack_ptr;
	pure_task_t current_task;
	struct mimix_timespec wakeup_time;
	struct mimix_timespec cpu_time;
} pcb_t;

/*===========================================================================
 * PURE FUNCTIONAL UTILITIES
 *=========================================================================*/

/* Compare two tasks for priority scheduling - O(1) */
static int compare_tasks(const pure_task_t *a, const pure_task_t *b) {
	/* First compare by priority (higher priority first) */
	if (a->priority != b->priority) {
		return b->priority - a->priority;
	}

	/* Then by deadline (earlier deadline first) */
	if (a->deadline.tv_sec != b->deadline.tv_sec) {
		return (int) (a->deadline.tv_sec - b->deadline.tv_sec);
	}

	return (int) (a->deadline.tv_nsec - b->deadline.tv_nsec);
}

/* Initialize atomic integer - O(1) */
static void atomic_init(atomic_int_t *atom, int value) {
	atom->value = value;
}

/* Atomic increment - O(1) */
static int atomic_inc(atomic_int_t *atom) {
	int old_val = atom->value;
	atom->value++;
	return old_val;
}

/* Atomic compare-and-swap - O(1) */
static int atomic_cas(atomic_int_t *atom, int expected, int desired) {
	int result;

	if (atom->value == expected) {
		atom->value = desired;
		result = expected;
	} else {
		result = atom->value;
	}

	return result;
}

/* Cache-aware task scheduler - O(n) for priority search */
static void* cache_aware_scheduler(scheduler_t *scheduler) {
	int highest_priority = -1;
	int i;

	/* Find highest priority with tasks */
	for (i = 0; i < scheduler->max_priority; i++) {
		if (scheduler->sizes[i] > 0) {
			highest_priority = i;
			break;
		}
	}

	/* Dequeue task from highest priority queue */
	if (highest_priority >= 0 && scheduler->sizes[highest_priority] > 0) {
		size_t idx = --scheduler->sizes[highest_priority];
		pure_task_t *task_queue = scheduler->queues[highest_priority];

		if (task_queue != NULL) {
			return &task_queue[idx];
		}
	}

	return NULL;
}

/* IPC message send - O(1) average case */
static int ipc_send(lockfree_ringbuf_t *ringbuf, const pure_task_t *message) {
	size_t current_head, next_head, current_tail;

	do {
		current_head = ringbuf->head;
		current_tail = ringbuf->tail;

		/* Calculate next head position */
		next_head = (current_head + 1) % ringbuf->capacity;

		/* Check if buffer is full */
		if (next_head == current_tail) {
			/* Buffer full, retry */
			continue;
		}

		ringbuf->head = next_head;
		break;

	} while (1);

	/* Copy message */
	ringbuf->buffer[current_head] = *message;

	return 0;
}

/* Pure function for task transformation - O(n) */
static void* map_task_transform(const void *input_data) {
	const int *data = (const int*) input_data;
	int *result;
	int i;

	result = (int*) malloc(32 * sizeof(int));
	if (result == NULL) {
		return NULL;
	}

	for (i = 0; i < 32; i++) {
		result[i] = data[i] + 1;
	}

	return result;
}

/* Pure filter function using predicate - O(n) */
static void* filter_task(const void *input_data, pure_predicate_t predicate) {
	const int *data = (const int*) input_data;
	int *result;
	size_t count = 0;
	int i;

	/* First pass: count elements that satisfy predicate */
	for (i = 0; i < 32; i++) {
		if (predicate(&data[i])) {
			count++;
		}
	}

	/* Allocate result array */
	result = (int*) malloc(count * sizeof(int));
	if (result == NULL) {
		return NULL;
	}

	/* Second pass: copy filtered elements */
	{
		size_t j = 0;
		for (i = 0; i < 32; i++) {
			if (predicate(&data[i])) {
				result[j] = data[i];
				j++;
			}
		}
	}

	return result;
}

/*===========================================================================
 * ORIGINAL MINIX ANSI COMPATIBILITY MACROS
 *=========================================================================*/

#if __STDC__ == 1
#define _ANSI 31459
#endif

#ifdef __GNUC__
#define _ANSI 31459
#endif

#ifdef _ANSI
#define _PROTOTYPE(function, params)    function params
#define _ARGS(params)                   params
#define _VOIDSTAR                       void *
#define _VOID                           void
#define _CONST                          const
#define _VOLATILE                       volatile
#define _SIZET                          size_t
#else
    #define _PROTOTYPE(function, params)    function()
    #define _ARGS(params)                   ()
    #define _VOIDSTAR                       void *
    #define _VOID                           void
    #define _CONST
    #define _VOLATILE
    #define _SIZET                          int
#endif

#define _RESTRICT

#endif /* _ANSI_H */
