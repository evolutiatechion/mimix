# MIMIX 3.1.2 Build System with AVX-256 Optimization
CC = gcc
CFLAGS = -std=c90 -ansi -pedantic -Wall -Wextra -Werror \
         -O2 -march=x86-64-v3 -mtune=generic -mavx2 -mfma \
         -mprefer-vector-width=256 \
         -falign-functions=32 -falign-loops=32 \
         -pthread -D_MIMIX_MICROKERNEL \
         -D_POSIX_C_SOURCE=200809L \
         -D_GNU_SOURCE -D_MIMIX_PTHREADS_OPTIMIZED \
         -D_MIMIX_OPENCL_SUPPORT -D_MIMIX_OPENSSL_CRYPTO

# Memory Alignment and Security
CFLAGS += -fstack-protector-strong -D_FORTIFY_SOURCE=2

# Include paths
INCLUDES = -I./src/headers

# Library paths and linking
LIBS = -lm -lssl -lcrypto -lOpenCL -pthread

# Target Architecture
TARGET = mimix-test
SRCDIR = src
HEADERDIR = $(SRCDIR)/headers
TESTDIR = $(SRCDIR)/testcase

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(TESTDIR)/main.c $(HEADERDIR)/ansi.h $(HEADERDIR)/limits.h
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@ $(LIBS)

optimize:
	@echo "Optimization Report for MIMIX 3.1.2:"
	@echo "------------------------------------"
	@echo "Standard: C90 with POSIX.1-2008"
	@echo "SIMD: AVX2 with FMA instructions"
	@echo "Threading: PThreads optimized"
	@echo "Security: Stack protection enabled"

test: $(TARGET)
	@echo "Running MIMIX 3.1.2 Test Suite..."
	@./$(TARGET)
	@echo "Test completed"

clean:
	rm -f $(TARGET) *.o *.i *.s *.log
	find . -name "*.d" -delete

# Debug build
debug: CFLAGS += -O0 -g3 -DDEBUG
debug: $(TARGET)

# Release build with maximum optimization
release: CFLAGS += -O3 -flto -fno-semantic-interposition -funroll-loops
release: $(TARGET)

# Generate assembly output
asm:
	$(CC) $(CFLAGS) $(INCLUDES) -S $(TESTDIR)/main.c -o main.s

# Check for vectorization
vec-report:
	$(CC) $(CFLAGS) $(INCLUDES) -fopt-info-vec-missed \
	      $(TESTDIR)/main.c -o $(TARGET) $(LIBS) 2> vectorization.log
	@echo "Vectorization report written to vectorization.log"

# Build with OpenCL support
opencl: CFLAGS += -DCL_TARGET_OPENCL_VERSION=300
opencl: $(TARGET)
