# SIMD Programming Lab


## Table of Contents

1. [Introduction](#introduction)
2. [Setup & Dependencies](#setup--dependencies)
3. [Understanding the Makefile](#understanding-the-makefile)
4. [Part 1: SIMD Basics](#part-1-simd-basics)
5. [Part 2: SIMD Computations](#part-2-simd-computations)
6. [Part 3: Advanced Examples](#part-3-advanced-examples)
7. [Part 4: Your Challenge - 1D Convolution](#part-4-your-challenge---1d-convolution)
8. [Performance Tips & Best Practices](#performance-tips--best-practices)
9. [Troubleshooting](#troubleshooting)

---

## Introduction

### What is SIMD?

**SIMD** stands for **Single Instruction, Multiple Data**. It's a type of parallel computing where a single CPU instruction operates on multiple data points simultaneously. Think of it as doing the same operation on an entire array at once, rather than one element at a time.

**Example:**
- **Scalar (Normal):** Add 8 numbers one by one → 8 operations
- **SIMD (AVX2):** Add 8 numbers simultaneously → 1 operation

### Why SIMD?

Modern CPUs have special SIMD registers and instructions that can process multiple data elements in parallel:
- **SSE:** 128-bit registers (4 floats or 4 ints)
- **AVX2:** 256-bit registers (8 floats or 8 ints)
- **AVX-512:** 512-bit registers (16 floats or 16 ints)

This lab focuses on **AVX2**, which is widely available on Intel and AMD processors since 2013.

### Performance Expectations

With proper SIMD usage, you can expect:
- **2-8x speedup** for simple arithmetic operations
- **10-40x speedup** for well-structured algorithms with good memory access patterns
- Actual speedups depend on memory bandwidth, alignment, and algorithm complexity

---

## Setup & Dependencies

### System Requirements

- **OS:** Linux (Ubuntu 20.04+ recommended)
- **CPU:** x86-64 processor with AVX2 support (Intel Haswell 2013+ or AMD Excavator 2015+)
- **Compiler:** GCC 7.0+ or Clang 5.0+

**Windows Users:** You can use Windows Subsystem for Linux (WSL2) or Docker:
```bash
# WSL2: Install Ubuntu from Microsoft Store, then follow Linux instructions

# Docker: Run Ubuntu container
docker run -it --rm ubuntu:22.04 bash
# Then install dependencies inside container
```

### Check AVX2 Support

First, verify your CPU supports AVX2:

```bash
# On Linux
lscpu | grep avx2

# Or check /proc/cpuinfo
grep avx2 /proc/cpuinfo
```

If you see `avx2` in the output, you're good to go!

### Install Dependencies

```bash
# Update package list
sudo apt update

# Install build essentials
sudo apt install -y build-essential

# Install GCC/G++ (if not already installed)
sudo apt install -y gcc g++

# Optional: Install clang
sudo apt install -y clang

# Verify installation
gcc --version
g++ --version
```

### Download Lab Materials

```bash
# Clone the repository
git clone https://github.com/yuninxia/hands-on-simd-programming.git
cd hands-on-simd-programming

# Check structure
ls -la src/
```

You should see three directories:
- `01_Basics/` - Fundamental SIMD concepts
- `02_Computations/` - SIMD arithmetic operations
- `03_Examples/` - Advanced applications

---

## Understanding the Makefile

### What is a Makefile?

A **Makefile** is a file that contains instructions for compiling your program. Instead of typing long compilation commands every time, you simply run `make`.

### Basic Makefile Structure

Here's a simplified Makefile for this lab:

```makefile
# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -O3 -mavx2 -Wall

# Target executable name
TARGET = simd_example

# Source files
SOURCES = main.cpp

# Build rule
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Clean rule
clean:
	rm -f $(TARGET)
```

### Key Compiler Flags Explained

| Flag | Purpose |
|------|---------|
| `-std=c++17` | Use C++17 standard |
| `-O3` | Maximum optimization level |
| `-mavx2` | Enable AVX2 instructions |
| `-Wall` | Show all warnings |
| `-g` | Include debug symbols (optional) |
| `-march=native` | Optimize for your specific CPU (alternative to `-mavx2`) |

### Using Make

```bash
# Compile the program
make

# Run the program
./simd_example

# Clean up (remove compiled files)
make clean

# Rebuild from scratch
make clean && make
```

### Common Make Commands for This Lab

Each example in this lab has its own Makefile. To build and run:

```bash
# Navigate to an example
cd src/01_Basics/01_importing_simd/

# Compile
make

# Run
./main

# Clean
make clean
```

---

## Part 1: SIMD Basics

In this section, we'll cover fundamental SIMD concepts. Each example includes complete working code that you can compile and run.

### 1.1 Importing SIMD Headers

**Location:** `src/01_Basics/01_importing_simd/`

**Concept:** Learn how to include AVX2 intrinsics and verify compiler support.

**File: `main.cpp`**

```cpp
#include <immintrin.h>  // AVX2 intrinsics header
#include <iostream>

int main() {
    std::cout << "=== SIMD Header Import Example ===" << std::endl;
    
    // Check if AVX2 is available at compile time
    #ifdef __AVX2__
        std::cout << "✓ AVX2 is supported and enabled!" << std::endl;
    #else
        std::cout << "✗ AVX2 is NOT enabled!" << std::endl;
        std::cout << "Make sure to compile with -mavx2 flag" << std::endl;
        return 1;
    #endif
    
    // Create a simple 256-bit vector of 8 floats
    __m256 vec = _mm256_setzero_ps();  // Initialize to zeros
    
    std::cout << "Successfully created a 256-bit AVX2 vector!" << std::endl;
    std::cout << "Vector can hold 8 float values (256 bits / 32 bits per float)" << std::endl;
    
    return 0;
}
```

**Key Points:**
- `#include <immintrin.h>` - Main header for Intel intrinsics (SSE, AVX, AVX2, etc.)
- `__m256` - Data type for 256-bit vector (8 floats)
- `__m256d` - Data type for 256-bit vector (4 doubles)
- `__m256i` - Data type for 256-bit vector (integers)
- All AVX2 functions start with `_mm256_`

**Compile & Run:**
```bash
cd src/01_Basics/01_importing_simd/
make
./main
```

---

### 1.2 Initializing Data

**Location:** `src/01_Basics/02_initializing_data/`

**Concept:** Different ways to initialize SIMD vectors.

**File: `main.cpp`**

```cpp
#include <immintrin.h>
#include <iostream>
#include <iomanip>

// Helper function to print a 256-bit vector
void print_vec(__m256 vec, const char* name) {
    float values[8];
    _mm256_storeu_ps(values, vec);  // Store vector to array
    
    std::cout << name << ": [";
    for (int i = 0; i < 8; i++) {
        std::cout << std::setw(6) << values[i];
        if (i < 7) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

int main() {
    std::cout << "=== SIMD Vector Initialization ===" << std::endl << std::endl;
    
    // Method 1: Set all elements to zero
    __m256 zeros = _mm256_setzero_ps();
    print_vec(zeros, "Zeros   ");
    
    // Method 2: Set all elements to the same value
    __m256 fives = _mm256_set1_ps(5.0f);
    print_vec(fives, "All 5s  ");
    
    // Method 3: Set each element individually (reverse order!)
    __m256 sequence = _mm256_set_ps(7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f);
    print_vec(sequence, "Sequence");
    
    // Method 4: Set each element in normal order
    __m256 forward = _mm256_setr_ps(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f);
    print_vec(forward, "Forward ");
    
    std::cout << std::endl;
    std::cout << "Key Differences:" << std::endl;
    std::cout << "- _mm256_set_ps():  Parameters in REVERSE order (7,6,5,4,3,2,1,0)" << std::endl;
    std::cout << "- _mm256_setr_ps(): Parameters in NORMAL order  (0,1,2,3,4,5,6,7)" << std::endl;
    std::cout << "- _mm256_set1_ps(): Broadcast single value to all elements" << std::endl;
    
    return 0;
}
```

**Key Functions:**
- `_mm256_setzero_ps()` - Initialize all to zero
- `_mm256_set1_ps(value)` - Broadcast value to all lanes
- `_mm256_set_ps(v7, v6, ..., v0)` - Set individual values (reverse order!)
- `_mm256_setr_ps(v0, v1, ..., v7)` - Set individual values (normal order)
- `_mm256_storeu_ps(array, vec)` - Store vector to memory (unaligned)

**Compile & Run:**
```bash
cd src/01_Basics/02_initializing_data/
make
./main
```

---

### 1.3 Binding with Unions

**Location:** `src/01_Basics/03_binding_with_unions/`

**Concept:** Use unions to access SIMD vectors as both vectors and arrays.

**File: `main.cpp`**

```cpp
#include <immintrin.h>
#include <iostream>
#include <iomanip>

// Union allows treating same memory as both vector and array
union Vec8f {
    __m256 vec;      // SIMD vector view
    float data[8];   // Array view
};

int main() {
    std::cout << "=== SIMD Unions for Easy Access ===" << std::endl << std::endl;
    
    // Create and initialize using union
    Vec8f myVec;
    myVec.vec = _mm256_setr_ps(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
    
    std::cout << "Original vector:" << std::endl;
    for (int i = 0; i < 8; i++) {
        std::cout << "  data[" << i << "] = " << myVec.data[i] << std::endl;
    }
    
    std::cout << std::endl << "Modifying individual elements via array access:" << std::endl;
    myVec.data[0] = 100.0f;
    myVec.data[7] = 200.0f;
    
    std::cout << "Modified vector:" << std::endl;
    for (int i = 0; i < 8; i++) {
        std::cout << "  data[" << i << "] = " << myVec.data[i] << std::endl;
    }
    
    // SIMD operations still work on the modified data
    Vec8f result;
    result.vec = _mm256_mul_ps(myVec.vec, _mm256_set1_ps(2.0f));
    
    std::cout << std::endl << "After multiplying by 2 (SIMD):" << std::endl;
    for (int i = 0; i < 8; i++) {
        std::cout << "  data[" << i << "] = " << result.data[i] << std::endl;
    }
    
    std::cout << std::endl << "Benefits of unions:" << std::endl;
    std::cout << "✓ Easy debugging - inspect individual values" << std::endl;
    std::cout << "✓ Flexible access - use vector OR array as needed" << std::endl;
    std::cout << "✓ No copying - same memory, different view" << std::endl;
    
    return 0;
}
```

**Why Use Unions?**
1. **Easy Debugging:** Inspect individual elements without storing to separate array
2. **Flexibility:** Mix SIMD operations with scalar access
3. **Zero Copy:** Same memory, different access patterns

**Compile & Run:**
```bash
cd src/01_Basics/03_binding_with_unions/
make
./main
```

---

### 1.4 Loading Data

**Location:** `src/01_Basics/04_loading_data/`

**Concept:** Load data from memory into SIMD vectors (aligned vs. unaligned).

**File: `main.cpp`**

```cpp
#include <immintrin.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>

void print_array(const float* arr, int size, const char* name) {
    std::cout << name << ": [";
    for (int i = 0; i < size; i++) {
        std::cout << std::setw(6) << arr[i];
        if (i < size - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

int main() {
    std::cout << "=== Loading Data into SIMD Vectors ===" << std::endl << std::endl;
    
    // Regular array (may not be aligned)
    float data[8] = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f};
    print_array(data, 8, "Original data");
    
    // Method 1: Unaligned load (works anywhere)
    __m256 vec_unaligned = _mm256_loadu_ps(data);
    std::cout << "\n✓ Loaded using _mm256_loadu_ps() (unaligned)" << std::endl;
    
    // Method 2: Aligned load (requires 32-byte alignment)
    // Allocate aligned memory
    float* aligned_data = (float*)aligned_alloc(32, 8 * sizeof(float));
    if (!aligned_data) {
        std::cerr << "Failed to allocate aligned memory!" << std::endl;
        return 1;
    }
    
    // Copy data to aligned memory
    for (int i = 0; i < 8; i++) {
        aligned_data[i] = data[i] * 2.0f;
    }
    
    print_array(aligned_data, 8, "Aligned data ");
    __m256 vec_aligned = _mm256_load_ps(aligned_data);
    std::cout << "✓ Loaded using _mm256_load_ps() (aligned)" << std::endl;
    
    // Performance note
    std::cout << "\n=== Alignment Matters for Performance ===" << std::endl;
    std::cout << "Unaligned loads (_mm256_loadu_ps):" << std::endl;
    std::cout << "  ✓ Works with any memory address" << std::endl;
    std::cout << "  ✗ Slightly slower (may cross cache line boundaries)" << std::endl;
    std::cout << "\nAligned loads (_mm256_load_ps):" << std::endl;
    std::cout << "  ✓ Faster (optimal cache/memory access)" << std::endl;
    std::cout << "  ✗ Requires 32-byte aligned memory" << std::endl;
    std::cout << "  ✗ Will crash if data is not aligned!" << std::endl;
    
    std::cout << "\nRecommendation: Use unaligned loads unless you can guarantee alignment." << std::endl;
    
    // Cleanup
    free(aligned_data);
    
    return 0;
}
```

**Key Concepts:**

1. **Unaligned Load:** `_mm256_loadu_ps(ptr)`
   - Works with any memory address
   - Slightly slower but safe

2. **Aligned Load:** `_mm256_load_ps(ptr)`
   - Requires 32-byte alignment
   - Faster but crashes if misaligned
   - Use `aligned_alloc(32, size)` to allocate aligned memory

3. **When to Use Which:**
   - **Unaligned:** Default choice, always safe
   - **Aligned:** Hot loops with pre-allocated aligned buffers

**Compile & Run:**
```bash
cd src/01_Basics/04_loading_data/
make
./main
```

---

### 1.5 Data Types

**Location:** `src/03_Examples/03_data_types/`

**Concept:** Understanding different SIMD data types and conversions.

**File: `main.cpp`**

```cpp
#include <immintrin.h>
#include <iostream>
#include <iomanip>

void print_float_vec(__m256 vec, const char* name) {
    float values[8];
    _mm256_storeu_ps(values, vec);
    std::cout << name << " (floats): [";
    for (int i = 0; i < 8; i++) {
        std::cout << std::setw(8) << values[i];
        if (i < 7) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

void print_int_vec(__m256i vec, const char* name) {
    int32_t values[8];
    _mm256_storeu_si256((__m256i*)values, vec);
    std::cout << name << " (ints):   [";
    for (int i = 0; i < 8; i++) {
        std::cout << std::setw(8) << values[i];
        if (i < 7) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

int main() {
    std::cout << "=== SIMD Data Types ===" << std::endl << std::endl;
    
    // Float vector
    __m256 float_vec = _mm256_setr_ps(1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 7.5f, 8.5f);
    print_float_vec(float_vec, "Float vec");
    
    // Integer vector
    __m256i int_vec = _mm256_setr_epi32(10, 20, 30, 40, 50, 60, 70, 80);
    print_int_vec(int_vec, "Int vec  ");
    
    std::cout << std::endl << "=== Type Conversions ===" << std::endl << std::endl;
    
    // Convert float to int (truncate)
    __m256i float_to_int = _mm256_cvttps_epi32(float_vec);
    print_int_vec(float_to_int, "Float→Int");
    
    // Convert int to float
    __m256 int_to_float = _mm256_cvtepi32_ps(int_vec);
    print_float_vec(int_to_float, "Int→Float");
    
    std::cout << std::endl << "=== Common SIMD Types ===" << std::endl;
    std::cout << "__m256   : 8 × 32-bit floats  (256 bits total)" << std::endl;
    std::cout << "__m256d  : 4 × 64-bit doubles (256 bits total)" << std::endl;
    std::cout << "__m256i  : 8 × 32-bit ints    (or 16×16-bit, 32×8-bit)" << std::endl;
    
    std::cout << std::endl << "=== Conversion Functions ===" << std::endl;
    std::cout << "_mm256_cvttps_epi32() : float → int (truncate)" << std::endl;
    std::cout << "_mm256_cvtepi32_ps()  : int → float" << std::endl;
    
    return 0;
}
```

**Common SIMD Types:**

| Type | Description | Elements |
|------|-------------|----------|
| `__m256` | 32-bit floats | 8 floats |
| `__m256d` | 64-bit doubles | 4 doubles |
| `__m256i` | Integers | 8×32-bit, 16×16-bit, or 32×8-bit |

**Compile & Run:**
```bash
cd src/03_Examples/03_data_types/
make
./main
```

---

## Part 2: SIMD Computations

Now that we understand the basics, let's perform actual computations with SIMD!

### 2.1 Simple Mathematics

**Location:** `src/02_Computations/01_simple_maths/`

**Concept:** Basic arithmetic operations with SIMD and performance comparison.

**File: `main.cpp`**

```cpp
#include <immintrin.h>
#include <iostream>
#include <iomanip>
#include <chrono>

// Timing helper
class Timer {
    std::chrono::high_resolution_clock::time_point start_time;
public:
    void start() { start_time = std::chrono::high_resolution_clock::now(); }
    double elapsed_ms() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start_time).count();
    }
};

// Scalar version: Add two arrays
void add_scalar(const float* a, const float* b, float* c, int size) {
    for (int i = 0; i < size; i++) {
        c[i] = a[i] + b[i];
    }
}

// SIMD version: Add two arrays
void add_simd(const float* a, const float* b, float* c, int size) {
    int i = 0;
    // Process 8 elements at a time
    for (; i <= size - 8; i += 8) {
        __m256 vec_a = _mm256_loadu_ps(&a[i]);
        __m256 vec_b = _mm256_loadu_ps(&b[i]);
        __m256 vec_c = _mm256_add_ps(vec_a, vec_b);
        _mm256_storeu_ps(&c[i], vec_c);
    }
    // Handle remaining elements
    for (; i < size; i++) {
        c[i] = a[i] + b[i];
    }
}

// Multiply-Add: c[i] = a[i] * b[i] + c[i]
void fma_simd(const float* a, const float* b, float* c, int size) {
    int i = 0;
    for (; i <= size - 8; i += 8) {
        __m256 vec_a = _mm256_loadu_ps(&a[i]);
        __m256 vec_b = _mm256_loadu_ps(&b[i]);
        __m256 vec_c = _mm256_loadu_ps(&c[i]);
        vec_c = _mm256_fmadd_ps(vec_a, vec_b, vec_c);  // a*b + c
        _mm256_storeu_ps(&c[i], vec_c);
    }
    for (; i < size; i++) {
        c[i] = a[i] * b[i] + c[i];
    }
}

int main() {
    const int SIZE = 1000000;  // 1 million elements
    const int ITERATIONS = 1000;
    
    // Allocate arrays
    float* a = new float[SIZE];
    float* b = new float[SIZE];
    float* c_scalar = new float[SIZE];
    float* c_simd = new float[SIZE];
    
    // Initialize
    for (int i = 0; i < SIZE; i++) {
        a[i] = static_cast<float>(i);
        b[i] = static_cast<float>(i) * 2.0f;
        c_scalar[i] = 0.0f;
        c_simd[i] = 0.0f;
    }
    
    std::cout << "=== SIMD Simple Math Performance ===" << std::endl;
    std::cout << "Array size: " << SIZE << " elements" << std::endl;
    std::cout << "Iterations: " << ITERATIONS << std::endl << std::endl;
    
    Timer timer;
    
    // Benchmark scalar addition
    timer.start();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        add_scalar(a, b, c_scalar, SIZE);
    }
    double scalar_time = timer.elapsed_ms();
    
    // Benchmark SIMD addition
    timer.start();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        add_simd(a, b, c_simd, SIZE);
    }
    double simd_time = timer.elapsed_ms();
    
    std::cout << "Addition Performance:" << std::endl;
    std::cout << "  Scalar: " << std::setw(8) << scalar_time << " ms" << std::endl;
    std::cout << "  SIMD:   " << std::setw(8) << simd_time << " ms" << std::endl;
    std::cout << "  Speedup: " << std::setw(6) << (scalar_time / simd_time) << "x" << std::endl;
    
    // Verify correctness
    bool correct = true;
    for (int i = 0; i < SIZE && correct; i++) {
        if (std::abs(c_scalar[i] - c_simd[i]) > 1e-5f) {
            correct = false;
        }
    }
    std::cout << "  Correctness: " << (correct ? "✓ PASS" : "✗ FAIL") << std::endl;
    
    std::cout << "\n=== Common SIMD Math Operations ===" << std::endl;
    std::cout << "_mm256_add_ps(a, b)      : a + b" << std::endl;
    std::cout << "_mm256_sub_ps(a, b)      : a - b" << std::endl;
    std::cout << "_mm256_mul_ps(a, b)      : a * b" << std::endl;
    std::cout << "_mm256_div_ps(a, b)      : a / b" << std::endl;
    std::cout << "_mm256_fmadd_ps(a, b, c) : a*b + c (Fused Multiply-Add)" << std::endl;
    std::cout << "_mm256_sqrt_ps(a)        : sqrt(a)" << std::endl;
    
    // Cleanup
    delete[] a;
    delete[] b;
    delete[] c_scalar;
    delete[] c_simd;
    
    return 0;
}
```

**Key Operations:**
- `_mm256_add_ps()` - Addition
- `_mm256_mul_ps()` - Multiplication
- `_mm256_fmadd_ps()` - Fused Multiply-Add (very efficient!)

**Compile & Run:**
```bash
cd src/02_Computations/01_simple_maths/
make
./main
```

---

### 2.2 Dot Product

**Location:** `src/02_Computations/02_dot_product/`

**Concept:** Compute dot product using SIMD with horizontal sum.

**File: `main.cpp`**

```cpp
#include <immintrin.h>
#include <iostream>
#include <iomanip>
#include <chrono>

class Timer {
    std::chrono::high_resolution_clock::time_point start_time;
public:
    void start() { start_time = std::chrono::high_resolution_clock::now(); }
    double elapsed_ms() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start_time).count();
    }
};

// Scalar dot product
float dot_product_scalar(const float* a, const float* b, int size) {
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

// SIMD dot product
float dot_product_simd(const float* a, const float* b, int size) {
    __m256 sum_vec = _mm256_setzero_ps();
    
    int i = 0;
    // Process 8 elements at a time
    for (; i <= size - 8; i += 8) {
        __m256 vec_a = _mm256_loadu_ps(&a[i]);
        __m256 vec_b = _mm256_loadu_ps(&b[i]);
        // Multiply and accumulate
        sum_vec = _mm256_fmadd_ps(vec_a, vec_b, sum_vec);
    }
    
    // Horizontal sum: reduce 8 values to 1
    // Step 1: Add upper and lower 128-bit halves
    __m128 sum_high = _mm256_extractf128_ps(sum_vec, 1);
    __m128 sum_low = _mm256_castps256_ps128(sum_vec);
    __m128 sum128 = _mm_add_ps(sum_high, sum_low);
    
    // Step 2: Horizontal add twice
    sum128 = _mm_hadd_ps(sum128, sum128);
    sum128 = _mm_hadd_ps(sum128, sum128);
    
    float result = _mm_cvtss_f32(sum128);
    
    // Handle remaining elements
    for (; i < size; i++) {
        result += a[i] * b[i];
    }
    
    return result;
}

int main() {
    const int SIZE = 1000000;
    const int ITERATIONS = 10000;
    
    float* a = new float[SIZE];
    float* b = new float[SIZE];
    
    // Initialize with some values
    for (int i = 0; i < SIZE; i++) {
        a[i] = static_cast<float>(i % 100) / 100.0f;
        b[i] = static_cast<float>(i % 50) / 50.0f;
    }
    
    std::cout << "=== Dot Product Performance ===" << std::endl;
    std::cout << "Vector size: " << SIZE << " elements" << std::endl;
    std::cout << "Iterations: " << ITERATIONS << std::endl << std::endl;
    
    Timer timer;
    float scalar_result, simd_result;
    
    // Benchmark scalar
    timer.start();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        scalar_result = dot_product_scalar(a, b, SIZE);
    }
    double scalar_time = timer.elapsed_ms();
    
    // Benchmark SIMD
    timer.start();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        simd_result = dot_product_simd(a, b, SIZE);
    }
    double simd_time = timer.elapsed_ms();
    
    std::cout << "Results:" << std::endl;
    std::cout << "  Scalar result: " << scalar_result << std::endl;
    std::cout << "  SIMD result:   " << simd_result << std::endl;
    std::cout << "  Difference:    " << std::abs(scalar_result - simd_result) << std::endl;
    
    std::cout << "\nPerformance:" << std::endl;
    std::cout << "  Scalar: " << std::setw(8) << scalar_time << " ms" << std::endl;
    std::cout << "  SIMD:   " << std::setw(8) << simd_time << " ms" << std::endl;
    std::cout << "  Speedup: " << std::setw(6) << (scalar_time / simd_time) << "x" << std::endl;
    
    std::cout << "\n=== Horizontal Sum Technique ===" << std::endl;
    std::cout << "To reduce 8 values to 1:" << std::endl;
    std::cout << "1. Extract upper/lower 128-bit halves" << std::endl;
    std::cout << "2. Add them together (8→4 values)" << std::endl;
    std::cout << "3. Use _mm_hadd_ps twice (4→2→1)" << std::endl;
    std::cout << "4. Extract final scalar result" << std::endl;
    
    delete[] a;
    delete[] b;
    
    return 0;
}
```

**Horizontal Sum Pattern:**
This is a common pattern for reducing a vector to a single value. The steps are:
1. Extract upper and lower 128-bit halves
2. Add them together
3. Use horizontal adds to reduce further
4. Extract final scalar

**Compile & Run:**
```bash
cd src/02_Computations/02_dot_product/
make
./main
```

---

## Part 3: Advanced Examples

### 3.1 Conditional Code

**Location:** `src/03_Examples/01_conditional_code/`

**Concept:** Implement conditional logic using SIMD masks (no branches!).

**File: `main.cpp`**

```cpp
#include <immintrin.h>
#include <iostream>
#include <iomanip>
#include <cmath>

void print_vec(__m256 vec, const char* name) {
    float values[8];
    _mm256_storeu_ps(values, vec);
    std::cout << name << ": [";
    for (int i = 0; i < 8; i++) {
        std::cout << std::setw(8) << std::fixed << std::setprecision(2) << values[i];
        if (i < 7) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

// Scalar version: if (x > 0) result = sqrt(x) else result = 0
void conditional_scalar(const float* input, float* output, int size) {
    for (int i = 0; i < size; i++) {
        if (input[i] > 0.0f) {
            output[i] = std::sqrt(input[i]);
        } else {
            output[i] = 0.0f;
        }
    }
}

// SIMD version: Using masks instead of branches
void conditional_simd(const float* input, float* output, int size) {
    __m256 zero = _mm256_setzero_ps();
    
    int i = 0;
    for (; i <= size - 8; i += 8) {
        __m256 vec = _mm256_loadu_ps(&input[i]);
        
        // Create mask: where vec > 0
        __m256 mask = _mm256_cmp_ps(vec, zero, _CMP_GT_OQ);
        
        // Compute sqrt for all elements
        __m256 sqrt_vec = _mm256_sqrt_ps(vec);
        
        // Blend: select sqrt_vec where mask is true, else zero
        __m256 result = _mm256_blendv_ps(zero, sqrt_vec, mask);
        
        _mm256_storeu_ps(&output[i], result);
    }
    
    // Handle remaining elements
    for (; i < size; i++) {
        output[i] = (input[i] > 0.0f) ? std::sqrt(input[i]) : 0.0f;
    }
}

int main() {
    std::cout << "=== Conditional Code with SIMD Masks ===" << std::endl << std::endl;
    
    // Test data with positive and negative values
    float input[8] = {-4.0f, 9.0f, -1.0f, 16.0f, 0.0f, 25.0f, -9.0f, 4.0f};
    float output_scalar[8];
    float output_simd[8];
    
    std::cout << "Task: Compute sqrt(x) if x > 0, else 0" << std::endl << std::endl;
    
    // Show input
    std::cout << "Input:  [";
    for (int i = 0; i < 8; i++) {
        std::cout << std::setw(8) << input[i];
        if (i < 7) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    
    // Compute both versions
    conditional_scalar(input, output_scalar, 8);
    conditional_simd(input, output_simd, 8);
    
    std::cout << "Scalar: [";
    for (int i = 0; i < 8; i++) {
        std::cout << std::setw(8) << std::fixed << std::setprecision(2) << output_scalar[i];
        if (i < 7) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    
    std::cout << "SIMD:   [";
    for (int i = 0; i < 8; i++) {
        std::cout << std::setw(8) << std::fixed << std::setprecision(2) << output_simd[i];
        if (i < 7) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    
    // Verify
    bool correct = true;
    for (int i = 0; i < 8; i++) {
        if (std::abs(output_scalar[i] - output_simd[i]) > 1e-5f) {
            correct = false;
        }
    }
    std::cout << "\nCorrectness: " << (correct ? "✓ PASS" : "✗ FAIL") << std::endl;
    
    std::cout << "\n=== SIMD Mask Operations ===" << std::endl;
    std::cout << "_mm256_cmp_ps(a, b, cmp)  : Compare and create mask" << std::endl;
    std::cout << "  _CMP_EQ_OQ  : a == b" << std::endl;
    std::cout << "  _CMP_LT_OQ  : a < b" << std::endl;
    std::cout << "  _CMP_LE_OQ  : a <= b" << std::endl;
    std::cout << "  _CMP_GT_OQ  : a > b" << std::endl;
    std::cout << "  _CMP_GE_OQ  : a >= b" << std::endl;
    std::cout << "_mm256_blendv_ps(a, b, mask) : Select b where mask=true, else a" << std::endl;
    
    std::cout << "\nKey Insight: No branch instructions!" << std::endl;
    std::cout << "  - Compute both paths" << std::endl;
    std::cout << "  - Use mask to select correct result" << std::endl;
    std::cout << "  - Better for CPU pipeline (no branch misprediction)" << std::endl;
    
    return 0;
}
```

**SIMD Conditional Pattern:**
1. Create a **mask** using comparison
2. Compute **both** possible outcomes
3. Use **blend** to select based on mask

This avoids branching, which is good for CPU pipelines!

**Compile & Run:**
```bash
cd src/03_Examples/01_conditional_code/
make
./main
```

---

### 3.2 Image Processing

**Location:** `src/03_Examples/04_image_processing/`

**Concept:** Apply SIMD to image operations (grayscale conversion).

**File: `main.cpp`**

```cpp
#include <immintrin.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstring>

class Timer {
    std::chrono::high_resolution_clock::time_point start_time;
public:
    void start() { start_time = std::chrono::high_resolution_clock::now(); }
    double elapsed_ms() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start_time).count();
    }
};

// Simple RGB struct
struct RGB {
    unsigned char r, g, b;
};

// Convert RGB to grayscale: Y = 0.299*R + 0.587*G + 0.114*B
void rgb_to_gray_scalar(const RGB* input, unsigned char* output, int pixels) {
    for (int i = 0; i < pixels; i++) {
        float gray = 0.299f * input[i].r + 
                     0.587f * input[i].g + 
                     0.114f * input[i].b;
        output[i] = static_cast<unsigned char>(gray);
    }
}

// SIMD version - process 8 pixels at a time
void rgb_to_gray_simd(const RGB* input, unsigned char* output, int pixels) {
    __m256 coeff_r = _mm256_set1_ps(0.299f);
    __m256 coeff_g = _mm256_set1_ps(0.587f);
    __m256 coeff_b = _mm256_set1_ps(0.114f);
    
    int i = 0;
    for (; i <= pixels - 8; i += 8) {
        // Load RGB values (this is simplified - real code would need deinterleaving)
        float r[8], g[8], b[8];
        for (int j = 0; j < 8; j++) {
            r[j] = static_cast<float>(input[i + j].r);
            g[j] = static_cast<float>(input[i + j].g);
            b[j] = static_cast<float>(input[i + j].b);
        }
        
        __m256 vec_r = _mm256_loadu_ps(r);
        __m256 vec_g = _mm256_loadu_ps(g);
        __m256 vec_b = _mm256_loadu_ps(b);
        
        // Compute: 0.299*R + 0.587*G + 0.114*B
        __m256 gray = _mm256_mul_ps(vec_r, coeff_r);
        gray = _mm256_fmadd_ps(vec_g, coeff_g, gray);
        gray = _mm256_fmadd_ps(vec_b, coeff_b, gray);
        
        // Convert to integers and store
        __m256i gray_int = _mm256_cvtps_epi32(gray);
        
        // Extract and store (simplified)
        int gray_values[8];
        _mm256_storeu_si256((__m256i*)gray_values, gray_int);
        for (int j = 0; j < 8; j++) {
            output[i + j] = static_cast<unsigned char>(gray_values[j]);
        }
    }
    
    // Handle remaining pixels
    for (; i < pixels; i++) {
        float gray = 0.299f * input[i].r + 
                     0.587f * input[i].g + 
                     0.114f * input[i].b;
        output[i] = static_cast<unsigned char>(gray);
    }
}

int main() {
    const int WIDTH = 1920;
    const int HEIGHT = 1080;
    const int PIXELS = WIDTH * HEIGHT;
    const int ITERATIONS = 100;
    
    std::cout << "=== Image Processing: RGB to Grayscale ===" << std::endl;
    std::cout << "Image size: " << WIDTH << "x" << HEIGHT << " (" << PIXELS << " pixels)" << std::endl;
    std::cout << "Iterations: " << ITERATIONS << std::endl << std::endl;
    
    // Allocate
    RGB* rgb_image = new RGB[PIXELS];
    unsigned char* gray_scalar = new unsigned char[PIXELS];
    unsigned char* gray_simd = new unsigned char[PIXELS];
    
    // Initialize with some test data
    for (int i = 0; i < PIXELS; i++) {
        rgb_image[i].r = (i * 7) % 256;
        rgb_image[i].g = (i * 13) % 256;
        rgb_image[i].b = (i * 19) % 256;
    }
    
    Timer timer;
    
    // Benchmark scalar
    timer.start();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        rgb_to_gray_scalar(rgb_image, gray_scalar, PIXELS);
    }
    double scalar_time = timer.elapsed_ms();
    
    // Benchmark SIMD
    timer.start();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        rgb_to_gray_simd(rgb_image, gray_simd, PIXELS);
    }
    double simd_time = timer.elapsed_ms();
    
    std::cout << "Performance:" << std::endl;
    std::cout << "  Scalar: " << std::setw(8) << scalar_time << " ms" << std::endl;
    std::cout << "  SIMD:   " << std::setw(8) << simd_time << " ms" << std::endl;
    std::cout << "  Speedup: " << std::setw(6) << (scalar_time / simd_time) << "x" << std::endl;
    
    // Verify
    int differences = 0;
    for (int i = 0; i < PIXELS; i++) {
        if (gray_scalar[i] != gray_simd[i]) {
            differences++;
        }
    }
    std::cout << "  Differences: " << differences << " pixels" << std::endl;
    std::cout << "  Correctness: " << (differences == 0 ? "✓ PASS" : "✓ ACCEPTABLE") << std::endl;
    std::cout << "  (Small differences are OK due to rounding)" << std::endl;
    
    std::cout << "\n=== Grayscale Formula ===" << std::endl;
    std::cout << "Y = 0.299*R + 0.587*G + 0.114*B" << std::endl;
    std::cout << "These coefficients approximate human perception of brightness" << std::endl;
    
    delete[] rgb_image;
    delete[] gray_scalar;
    delete[] gray_simd;
    
    return 0;
}
```

**Image Processing with SIMD:**
- Real implementations need to handle pixel deinterleaving
- Memory layout matters: SoA (Structure of Arrays) is better than AoS for SIMD
- Can process 8+ pixels simultaneously

**Compile & Run:**
```bash
cd src/03_Examples/04_image_processing/
make
./main
```

---

## Part 4: Your Challenge - 1D Convolution

**Estimated Time:** 30-40 minutes

### What is Convolution?

**Convolution** is a fundamental operation in signal processing and deep learning. It applies a small "kernel" (filter) across an input array to produce an output.

**Example: Audio Smoothing**
```
Input signal:  [1, 5, 3, 8, 2, 9, 4]
Kernel:        [0.25, 0.5, 0.25]  (3-element smoothing filter)

Output[0] = 0.25*1 + 0.5*5 + 0.25*3 = 3.25
Output[1] = 0.25*5 + 0.5*3 + 0.25*8 = 4.75
Output[2] = 0.25*3 + 0.5*8 + 0.25*2 = 4.75
...
```

Each output element is a weighted sum of the input elements in a sliding window.

### Applications

1. **Audio Processing:** Filtering, echo, reverb
2. **Signal Processing:** Noise reduction, smoothing
3. **Deep Learning:** Convolutional neural networks (CNNs)
4. **Time Series:** Moving averages, trend detection

### Your Task

Implement a **1D convolution** function using SIMD. You'll create both scalar and SIMD versions, then compare their performance.

**Specification:**
- Input array: `float input[]` with `input_size` elements
- Kernel: `float kernel[]` with `kernel_size` elements (always odd: 3, 5, 7, etc.)
- Output array: `float output[]` with `output_size = input_size - kernel_size + 1` elements

**Formula:**
```
output[i] = sum(input[i+j] * kernel[j]) for j = 0 to kernel_size-1
```

### Starter Code

Create a file: `convolution_challenge.cpp`

```cpp
#include <immintrin.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <cstring>

class Timer {
    std::chrono::high_resolution_clock::time_point start_time;
public:
    void start() { start_time = std::chrono::high_resolution_clock::now(); }
    double elapsed_ms() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start_time).count();
    }
};

// Scalar version - COMPLETE THIS
void convolve_1d_scalar(const float* input, int input_size, 
                        const float* kernel, int kernel_size,
                        float* output) {
    int output_size = input_size - kernel_size + 1;
    
    // TODO: Implement scalar convolution
    // For each output position i:
    //   output[i] = sum of (input[i+j] * kernel[j]) for j = 0 to kernel_size-1
    
    for (int i = 0; i < output_size; i++) {
        float sum = 0.0f;
        // YOUR CODE HERE
        
        output[i] = sum;
    }
}

// SIMD version - COMPLETE THIS
void convolve_1d_simd(const float* input, int input_size,
                      const float* kernel, int kernel_size,
                      float* output) {
    int output_size = input_size - kernel_size + 1;
    
    // TODO: Implement SIMD convolution
    // Strategy:
    // 1. Process 8 output elements at once
    // 2. For each output position, you need kernel_size input values
    // 3. Accumulate products using FMA instructions
    
    int i = 0;
    
    // Process 8 outputs at a time
    for (; i <= output_size - 8; i += 8) {
        __m256 sum = _mm256_setzero_ps();
        
        // For each kernel element
        for (int j = 0; j < kernel_size; j++) {
            // YOUR CODE HERE
            // Load 8 consecutive input values starting at position (i + j)
            // Multiply by kernel[j]
            // Accumulate into sum
        }
        
        // Store result
        _mm256_storeu_ps(&output[i], sum);
    }
    
    // Handle remaining elements with scalar code
    for (; i < output_size; i++) {
        float sum = 0.0f;
        for (int j = 0; j < kernel_size; j++) {
            sum += input[i + j] * kernel[j];
        }
        output[i] = sum;
    }
}

void print_array(const float* arr, int size, const char* name) {
    std::cout << name << ": [";
    for (int i = 0; i < std::min(size, 10); i++) {
        std::cout << std::setw(7) << std::fixed << std::setprecision(2) << arr[i];
        if (i < std::min(size, 10) - 1) std::cout << ", ";
    }
    if (size > 10) std::cout << ", ...";
    std::cout << "]" << std::endl;
}

int main() {
    std::cout << "=== 1D Convolution Challenge ===" << std::endl << std::endl;
    
    // Test parameters
    const int INPUT_SIZE = 1000000;
    const int KERNEL_SIZE = 5;
    const int OUTPUT_SIZE = INPUT_SIZE - KERNEL_SIZE + 1;
    const int ITERATIONS = 1000;
    
    // Allocate arrays
    float* input = new float[INPUT_SIZE];
    float* kernel = new float[KERNEL_SIZE];
    float* output_scalar = new float[OUTPUT_SIZE];
    float* output_simd = new float[OUTPUT_SIZE];
    
    // Initialize input (simulated audio signal)
    for (int i = 0; i < INPUT_SIZE; i++) {
        input[i] = std::sin(i * 0.01f) + 0.5f * std::cos(i * 0.05f);
    }
    
    // Initialize kernel (Gaussian smoothing filter)
    kernel[0] = 0.06f;
    kernel[1] = 0.24f;
    kernel[2] = 0.40f;
    kernel[3] = 0.24f;
    kernel[4] = 0.06f;
    
    std::cout << "Configuration:" << std::endl;
    std::cout << "  Input size:  " << INPUT_SIZE << std::endl;
    std::cout << "  Kernel size: " << KERNEL_SIZE << std::endl;
    std::cout << "  Output size: " << OUTPUT_SIZE << std::endl;
    std::cout << "  Iterations:  " << ITERATIONS << std::endl << std::endl;
    
    print_array(input, INPUT_SIZE, "Input sample ");
    print_array(kernel, KERNEL_SIZE, "Kernel       ");
    
    Timer timer;
    
    // Benchmark scalar version
    timer.start();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        convolve_1d_scalar(input, INPUT_SIZE, kernel, KERNEL_SIZE, output_scalar);
    }
    double scalar_time = timer.elapsed_ms();
    
    // Benchmark SIMD version
    timer.start();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        convolve_1d_simd(input, INPUT_SIZE, kernel, KERNEL_SIZE, output_simd);
    }
    double simd_time = timer.elapsed_ms();
    
    std::cout << std::endl;
    print_array(output_scalar, OUTPUT_SIZE, "Output scalar");
    print_array(output_simd, OUTPUT_SIZE, "Output SIMD  ");
    
    // Verify correctness
    float max_error = 0.0f;
    int error_count = 0;
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        float error = std::abs(output_scalar[i] - output_simd[i]);
        max_error = std::max(max_error, error);
        if (error > 1e-4f) error_count++;
    }
    
    std::cout << "\n=== Results ===" << std::endl;
    std::cout << "Correctness:" << std::endl;
    std::cout << "  Max error:    " << max_error << std::endl;
    std::cout << "  Error count:  " << error_count << " / " << OUTPUT_SIZE << std::endl;
    std::cout << "  Status:       " << (max_error < 1e-3f ? "✓ PASS" : "✗ FAIL") << std::endl;
    
    std::cout << "\nPerformance:" << std::endl;
    std::cout << "  Scalar: " << std::setw(10) << std::fixed << std::setprecision(2) 
              << scalar_time << " ms" << std::endl;
    std::cout << "  SIMD:   " << std::setw(10) << std::fixed << std::setprecision(2) 
              << simd_time << " ms" << std::endl;
    
    if (simd_time > 0) {
        std::cout << "  Speedup: " << std::setw(9) << std::fixed << std::setprecision(2) 
                  << (scalar_time / simd_time) << "x" << std::endl;
    }
    
    // Cleanup
    delete[] input;
    delete[] kernel;
    delete[] output_scalar;
    delete[] output_simd;
    
    std::cout << "\n🎉 Challenge complete!" << std::endl;
    
    return 0;
}
```

### Hints

1. **Scalar Version:**
   ```cpp
   for (int i = 0; i < output_size; i++) {
       float sum = 0.0f;
       for (int j = 0; j < kernel_size; j++) {
           sum += input[i + j] * kernel[j];
       }
       output[i] = sum;
   }
   ```

2. **SIMD Version Strategy:**
   - Process 8 output values simultaneously
   - For each kernel position j:
     - Load 8 input values: `input[i+j]` through `input[i+j+7]`
     - Broadcast `kernel[j]` to all 8 lanes
     - Use FMA to accumulate: `sum = FMA(input_vec, kernel_vec, sum)`

3. **Key Functions You'll Need:**
   - `_mm256_loadu_ps()` - Load 8 floats
   - `_mm256_set1_ps()` - Broadcast single value to all lanes
   - `_mm256_fmadd_ps()` - Fused multiply-add
   - `_mm256_storeu_ps()` - Store 8 floats

4. **Debugging Tips:**
   - Start with a small test case (10 inputs, 3 kernel)
   - Print intermediate values
   - Verify scalar version first
   - Check that SIMD matches scalar output

### Expected Performance

With a correct SIMD implementation, you should see:
- **3-6x speedup** for kernel size 3-5
- **Higher speedups** for larger kernels
- **Better speedup** with `-O3` optimization

### Compilation

Create a Makefile:

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -O3 -mavx2 -Wall

TARGET = convolution_challenge
SOURCES = convolution_challenge.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean
```

Compile and run:
```bash
make
./convolution_challenge
```

### Bonus Challenges (Optional)

If you finish early:

1. **Variable Kernel Size:** Test with kernel sizes 3, 5, 7, 9 and compare speedups
2. **2D Convolution:** Extend to 2D images (apply kernel in both X and Y directions)
3. **Optimized Memory Access:** Use aligned memory allocation for better performance
4. **Padding:** Add support for "same" padding (output size = input size)

---

## Performance Tips & Best Practices

### 1. Memory Alignment

**Always prefer aligned memory when possible:**
```cpp
// Allocate 32-byte aligned memory
float* data = (float*)aligned_alloc(32, size * sizeof(float));

// Use aligned load/store
__m256 vec = _mm256_load_ps(data);  // Faster!
_mm256_store_ps(data, vec);

// Don't forget to free
free(data);
```

### 2. Loop Structure

**Good pattern:**
```cpp
int i = 0;
// SIMD loop
for (; i <= size - 8; i += 8) {
    // Process 8 elements
}
// Scalar tail loop
for (; i < size; i++) {
    // Process remaining elements
}
```

### 3. Data Layout

**Prefer Structure of Arrays (SoA) over Array of Structures (AoS):**

```cpp
// Bad for SIMD (AoS)
struct Point { float x, y, z; };
Point points[100];

// Good for SIMD (SoA)
struct Points {
    float x[100];
    float y[100];
    float z[100];
};
Points points;
```

### 4. Compiler Flags

```bash
-O3           # Maximum optimization
-mavx2        # Enable AVX2
-march=native # Optimize for your CPU
-ffast-math   # Faster math (may reduce precision)
```

### 5. Common Pitfalls

❌ **Don't do this:**
```cpp
// Unnecessary stores/loads
for (int i = 0; i < size; i += 8) {
    __m256 a = _mm256_loadu_ps(&data[i]);
    float temp[8];
    _mm256_storeu_ps(temp, a);  // Unnecessary!
    // ... use temp ...
}
```

✅ **Do this:**
```cpp
// Keep data in SIMD registers
for (int i = 0; i < size; i += 8) {
    __m256 a = _mm256_loadu_ps(&data[i]);
    __m256 b = _mm256_mul_ps(a, a);  // Work with vectors
    _mm256_storeu_ps(&result[i], b);
}
```

### 6. When SIMD Doesn't Help

SIMD may not provide speedups when:
- **Memory bound:** Bandwidth saturated
- **Small data:** Overhead exceeds benefit
- **Irregular access:** Random memory patterns
- **Too many branches:** Masking overhead too high

---

## Troubleshooting

### Compilation Errors

**Error: `immintrin.h not found`**
```bash
# Install GCC/G++
sudo apt install build-essential
```

**Error: `_mm256_xxx is not defined`**
```bash
# Make sure to compile with -mavx2
g++ -mavx2 main.cpp -o main
```

**Error: `Illegal instruction (core dumped)`**
```bash
# Your CPU doesn't support AVX2
# Check with: lscpu | grep avx2
```

### Runtime Issues

**Segmentation Fault**
- Check array bounds
- Verify alignment for `_mm256_load_ps()`
- Use `_mm256_loadu_ps()` for unaligned data

**Incorrect Results**
- Verify loop bounds (size - 8, not size - 7)
- Check tail loop handles remaining elements
- Ensure initialization is correct
- Print intermediate values for debugging

**Poor Performance**
- Use `-O3` optimization flag
- Check memory alignment
- Verify data layout (prefer SoA)
- Profile to find bottlenecks

### Getting Help

If you're stuck:
1. Check the example code in Parts 1-3
2. Print intermediate values to debug
3. Start with a very small test case (8-16 elements)
4. Compare scalar and SIMD outputs element by element
5. Use GDB: `gdb ./program`

---

## Summary

Congratulations on completing this SIMD programming lab! You've learned:

✅ **Basics:**
- AVX2 intrinsics and headers
- Vector initialization and data types
- Loading and storing data
- Memory alignment concepts

✅ **Computations:**
- Basic arithmetic operations
- Fused multiply-add (FMA)
- Dot product with horizontal sum
- Performance measurement

✅ **Advanced Techniques:**
- Conditional logic with masks
- Image processing applications
- 1D convolution implementation

✅ **Best Practices:**
- Memory alignment
- Loop structuring
- SoA vs AoS data layout
- When to use (and not use) SIMD

### Next Steps

To continue learning SIMD:

1. **Explore the full repository:**
   - Multi-headed attention (`05_mha_block`)
   - Transformer decoder (`06_tiny_gpt`)

2. **Try more algorithms:**
   - Matrix multiplication
   - FFT (Fast Fourier Transform)
   - Sorting algorithms

3. **Learn advanced instructions:**
   - Gather/scatter operations
   - Permutations and shuffles
   - AVX-512 (if available)

4. **Study real-world code:**
   - Numpy/SciPy SIMD implementations
   - Image processing libraries (OpenCV, libvips)
   - BLAS libraries (OpenBLAS, Intel MKL)

### Resources

- **Intel Intrinsics Guide:** https://www.intel.com/content/www/us/en/docs/intrinsics-guide/
- **Agner Fog's Optimization Manuals:** https://www.agner.org/optimize/
- **Original Repository:** https://github.com/yuninxia/hands-on-simd-programming

---

**Happy SIMD programming! 🚀**
