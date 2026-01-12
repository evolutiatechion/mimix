/*
 * MIMIX 3.1.2 Microkernel - White Box Test Suite
 * Strictly C89/C90 compatible
 */

#include <stdio.h>
#include <assert.h>
#include <headers/ansi.h>

/* Test pure function: square all elements in array */
static void* test_square_transform(const void *input) {
	const int *data = (const int*) input;
	int *result;
	int i;

	result = (int*) malloc(32 * sizeof(int));
	if (result == NULL) {
		return NULL;
	}

	for (i = 0; i < 32; i++) {
		result[i] = data[i] * data[i];
	}

	return result;
}

/* Test predicate: is even */
static int test_is_even(const void *element) {
	const int *val = (const int*) element;
	return (*val % 2) == 0;
}

static void test_atomic_operations(void) {
	printf("Testing atomic operations...\n");

	atomic_int_t atom;
	atomic_init(&atom, 0);

	assert(atomic_inc(&atom) == 0);
	assert(atomic_inc(&atom) == 1);
	assert(atomic_inc(&atom) == 2);

	assert(atomic_cas(&atom, 3, 10) == 3);

	printf("Atomic operations: PASS\n");
}

static void test_lockfree_ipc(void) {
	printf("Testing lock-free IPC...\n");

	lockfree_ringbuf_t ringbuf;
	int i;

	ringbuf.capacity = 16;
	ringbuf.buffer = (pure_task_t*) malloc(16 * sizeof(pure_task_t));
	if (ringbuf.buffer == NULL) {
		printf("Failed to allocate ring buffer\n");
		return;
	}

	ringbuf.head = 0;
	ringbuf.tail = 0;
	atomic_init(&ringbuf.lock, 0);

	pure_task_t task;
	task.id = 1;
	task.priority = 10;
	task.transform = test_square_transform;

	int test_data[32];
	for (i = 0; i < 32; i++) {
		test_data[i] = i;
	}

	task.input.data = test_data;
	task.input.size = sizeof(test_data);
	task.input.checksum = 0;

	int messages_sent = 0;
	for (i = 0; i < 10; i++) {
		task.id = i;
		if (ipc_send(&ringbuf, &task) == 0) {
			messages_sent++;
		}
	}

	assert(messages_sent == 10);
	printf("Lock-free IPC: PASS (%d messages sent)\n", messages_sent);

	free(ringbuf.buffer);
}

static void test_cache_aware_scheduler(void) {
	printf("Testing cache-aware scheduler...\n");

	scheduler_t scheduler;
	int i;

	scheduler.max_priority = 32;
	scheduler.current_highest = -1;

	scheduler.queues = (pure_task_t**) malloc(32 * sizeof(pure_task_t*));
	if (scheduler.queues == NULL) {
		printf("Failed to allocate scheduler queues\n");
		return;
	}

	scheduler.sizes = (size_t*) malloc(32 * sizeof(size_t));
	if (scheduler.sizes == NULL) {
		free(scheduler.queues);
		printf("Failed to allocate scheduler sizes\n");
		return;
	}

	for (i = 0; i < 32; i++) {
		scheduler.queues[i] = NULL;
		scheduler.sizes[i] = 0;
	}

	pure_task_t *test_task_array = (pure_task_t*) malloc(sizeof(pure_task_t));
	if (test_task_array == NULL) {
		free(scheduler.queues);
		free(scheduler.sizes);
		printf("Failed to allocate test task\n");
		return;
	}

	test_task_array[0].id = 100;
	test_task_array[0].priority = 10;

	scheduler.sizes[10] = 1;
	scheduler.queues[10] = test_task_array;

	pure_task_t *scheduled = (pure_task_t*) cache_aware_scheduler(&scheduler);
	assert(scheduled != NULL);
	assert(scheduled->id == 100);

	printf("Cache-aware scheduler: PASS\n");

	free(test_task_array);
	free(scheduler.queues);
	free(scheduler.sizes);
}

static void benchmark_operations(void) {
	printf("Benchmarking operations...\n");

	int data[1024];
	int result[1024];
	int i;
	int j;
	long operations = 0;

	for (i = 0; i < 1024; i++) {
		data[i] = i;
	}

	for (j = 0; j < 1000; j++) {
		for (i = 0; i < 1024; i++) {
			result[i] = data[i] * data[i];
			operations++;
		}
	}

	printf("Operations completed: %ld\n", operations);

	for (i = 0; i < 1024; i++) {
		assert(result[i] == data[i] * data[i]);
	}
}

static void test_functional_paradigms(void) {
	printf("Testing functional programming paradigms...\n");

	int input[32];
	int i;
	for (i = 0; i < 32; i++) {
		input[i] = i;
	}

	void *mapped = map_task_transform(input);
	assert(mapped != NULL);

	int *mapped_array = (int*) mapped;
	for (i = 0; i < 32; i++) {
		assert(mapped_array[i] == input[i] + 1);
	}

	free(mapped);

	void *filtered = filter_task(input, test_is_even);
	assert(filtered != NULL);

	int even_count = 0;
	for (i = 0; i < 32; i++) {
		if (input[i] % 2 == 0) {
			even_count++;
		}
	}

	int *filtered_array = (int*) filtered;
	int actual_count = 0;
	for (i = 0; i < even_count; i++) {
		if (filtered_array[i] % 2 == 0) {
			actual_count++;
		}
	}

	assert(actual_count == even_count);

	free(filtered);

	printf("Functional paradigms: PASS\n");
}

static void test_compare_tasks(void) {
	printf("Testing task comparison...\n");

	pure_task_t task1, task2;

	task1.id = 1;
	task1.priority = 10;
	task1.deadline.tv_sec = 100;
	task1.deadline.tv_nsec = 0;

	task2.id = 2;
	task2.priority = 5;
	task2.deadline.tv_sec = 100;
	task2.deadline.tv_nsec = 0;

	assert(compare_tasks(&task1, &task2) < 0);

	task2.priority = 10;
	task2.deadline.tv_sec = 90;
	assert(compare_tasks(&task1, &task2) > 0);

	printf("Task comparison: PASS\n");
}

int main(void) {
	printf("========================================\n");
	printf("MIMIX 3.1.2 Microkernel Test Suite      \n");
	printf("========================================\n\n");

	test_atomic_operations();
	printf("\n");

	test_lockfree_ipc();
	printf("\n");

	test_cache_aware_scheduler();
	printf("\n");

	benchmark_operations();
	printf("\n");

	test_functional_paradigms();
	printf("\n");

	test_compare_tasks();
	printf("\n");

	printf("========================================\n");
	printf("All tests passed successfully!\n");
	printf("========================================\n");

	return 0;
}
