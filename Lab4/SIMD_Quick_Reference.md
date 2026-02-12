# SIMD Quick Reference Card

## Common AVX2 Data Types

| Type | Description | Size | Elements |
|------|-------------|------|----------|
| `__m256` | 32-bit floats | 256 bits | 8 floats |
| `__m256d` | 64-bit doubles | 256 bits | 4 doubles |
| `__m256i` | Integers | 256 bits | 8×32-bit, 16×16-bit, or 32×8-bit |
| `__m128` | 32-bit floats | 128 bits | 4 floats |

---

## Initialization

```cpp
// Set all to zero
__m256 zeros = _mm256_setzero_ps();

// Broadcast single value to all lanes
__m256 fives = _mm256_set1_ps(5.0f);

// Set individual values (REVERSE order: 7,6,5,4,3,2,1,0)
__m256 vec = _mm256_set_ps(7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f);

// Set individual values (NORMAL order: 0,1,2,3,4,5,6,7)
__m256 vec = _mm256_setr_ps(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f);
```

---

## Load/Store

```cpp
float data[8];

// Unaligned load (safe, works anywhere)
__m256 vec = _mm256_loadu_ps(data);

// Aligned load (faster, requires 32-byte alignment)
__m256 vec = _mm256_load_ps(data);

// Unaligned store
_mm256_storeu_ps(data, vec);

// Aligned store
_mm256_store_ps(data, vec);
```

---

## Arithmetic Operations

```cpp
__m256 a, b, c;

// Addition: c = a + b
c = _mm256_add_ps(a, b);

// Subtraction: c = a - b
c = _mm256_sub_ps(a, b);

// Multiplication: c = a * b
c = _mm256_mul_ps(a, b);

// Division: c = a / b
c = _mm256_div_ps(a, b);

// Fused Multiply-Add: c = a*b + c (fastest!)
c = _mm256_fmadd_ps(a, b, c);

// Square root: c = sqrt(a)
c = _mm256_sqrt_ps(a);

// Reciprocal: c = 1/a (approximate)
c = _mm256_rcp_ps(a);
```

---

## Comparisons (Create Masks)

```cpp
__m256 a, b, mask;

// Compare operations (result is a mask)
mask = _mm256_cmp_ps(a, b, _CMP_EQ_OQ);  // a == b
mask = _mm256_cmp_ps(a, b, _CMP_LT_OQ);  // a < b
mask = _mm256_cmp_ps(a, b, _CMP_LE_OQ);  // a <= b
mask = _mm256_cmp_ps(a, b, _CMP_GT_OQ);  // a > b
mask = _mm256_cmp_ps(a, b, _CMP_GE_OQ);  // a >= b
mask = _mm256_cmp_ps(a, b, _CMP_NEQ_OQ); // a != b
```

---

## Conditional Operations (Masking)

```cpp
__m256 a, b, mask, result;

// Select: pick b where mask is true, else a
result = _mm256_blendv_ps(a, b, mask);

// Bitwise AND with mask
result = _mm256_and_ps(a, mask);

// Bitwise OR
result = _mm256_or_ps(a, b);

// Bitwise XOR
result = _mm256_xor_ps(a, b);

// Bitwise AND-NOT: a & ~b
result = _mm256_andnot_ps(b, a);
```

---

## Horizontal Operations

```cpp
__m256 vec;

// Horizontal add (add adjacent pairs)
vec = _mm256_hadd_ps(vec, vec);

// Extract upper/lower 128-bit halves
__m128 upper = _mm256_extractf128_ps(vec, 1);
__m128 lower = _mm256_castps256_ps128(vec);

// Convert 128-bit to scalar
float value = _mm_cvtss_f32(lower);
```

---

## Type Conversions

```cpp
__m256 float_vec;
__m256i int_vec;

// Float to int (truncate)
int_vec = _mm256_cvttps_epi32(float_vec);

// Float to int (round)
int_vec = _mm256_cvtps_epi32(float_vec);

// Int to float
float_vec = _mm256_cvtepi32_ps(int_vec);

// Store int vector
int values[8];
_mm256_storeu_si256((__m256i*)values, int_vec);
```

---

## Common Patterns

### Pattern 1: Element-wise Operation
```cpp
void add_arrays(const float* a, const float* b, float* c, int size) {
    int i = 0;
    // SIMD loop
    for (; i <= size - 8; i += 8) {
        __m256 va = _mm256_loadu_ps(&a[i]);
        __m256 vb = _mm256_loadu_ps(&b[i]);
        __m256 vc = _mm256_add_ps(va, vb);
        _mm256_storeu_ps(&c[i], vc);
    }
    // Scalar tail
    for (; i < size; i++) {
        c[i] = a[i] + b[i];
    }
}
```

### Pattern 2: Reduction (Sum)
```cpp
float sum_array(const float* data, int size) {
    __m256 sum_vec = _mm256_setzero_ps();
    
    int i = 0;
    for (; i <= size - 8; i += 8) {
        __m256 vec = _mm256_loadu_ps(&data[i]);
        sum_vec = _mm256_add_ps(sum_vec, vec);
    }
    
    // Horizontal sum
    __m128 sum_high = _mm256_extractf128_ps(sum_vec, 1);
    __m128 sum_low = _mm256_castps256_ps128(sum_vec);
    __m128 sum128 = _mm_add_ps(sum_high, sum_low);
    sum128 = _mm_hadd_ps(sum128, sum128);
    sum128 = _mm_hadd_ps(sum128, sum128);
    float result = _mm_cvtss_f32(sum128);
    
    // Add remaining
    for (; i < size; i++) {
        result += data[i];
    }
    
    return result;
}
```

### Pattern 3: Conditional Processing
```cpp
void conditional_op(const float* in, float* out, int size) {
    __m256 zero = _mm256_setzero_ps();
    
    int i = 0;
    for (; i <= size - 8; i += 8) {
        __m256 vec = _mm256_loadu_ps(&in[i]);
        
        // Create mask (vec > 0)
        __m256 mask = _mm256_cmp_ps(vec, zero, _CMP_GT_OQ);
        
        // Compute both paths
        __m256 path_a = _mm256_sqrt_ps(vec);
        __m256 path_b = _mm256_setzero_ps();
        
        // Select based on mask
        __m256 result = _mm256_blendv_ps(path_b, path_a, mask);
        
        _mm256_storeu_ps(&out[i], result);
    }
    
    for (; i < size; i++) {
        out[i] = (in[i] > 0.0f) ? std::sqrt(in[i]) : 0.0f;
    }
}
```

---

## Memory Alignment

```cpp
// Allocate aligned memory (32 bytes for AVX2)
float* data = (float*)aligned_alloc(32, size * sizeof(float));

// Check alignment
bool is_aligned = ((uintptr_t)data % 32) == 0;

// Use aligned operations (faster)
__m256 vec = _mm256_load_ps(data);   // Aligned load
_mm256_store_ps(data, vec);          // Aligned store

// Free aligned memory
free(data);
```

---

## Compiler Flags

```bash
# Essential flags
-mavx2          # Enable AVX2 instructions
-O3             # Maximum optimization
-std=c++17      # C++17 standard

# Optional optimization flags
-march=native   # Optimize for your CPU
-ffast-math     # Fast math (may reduce precision)
-funroll-loops  # Unroll loops
```

---

## Debugging Tips

```cpp
// Print a vector for debugging
void print_m256(__m256 vec, const char* name) {
    float values[8];
    _mm256_storeu_ps(values, vec);
    printf("%s: [", name);
    for (int i = 0; i < 8; i++) {
        printf("%.2f%s", values[i], i < 7 ? ", " : "");
    }
    printf("]\n");
}
```

---

## Performance Tips

### ✅ DO:
- Use aligned memory when possible
- Process multiples of 8 elements
- Keep data in SIMD registers
- Use FMA instructions
- Minimize memory traffic
- Use Structure of Arrays (SoA)

### ❌ DON'T:
- Mix aligned and unaligned loads unnecessarily
- Store/load in every iteration
- Use Array of Structures (AoS) for SIMD data
- Ignore the tail loop
- Branch inside SIMD loops

---

## Common Errors

### Segmentation Fault
- **Cause:** Using aligned load on unaligned data
- **Fix:** Use `_mm256_loadu_ps()` or align your data

### Illegal Instruction
- **Cause:** CPU doesn't support AVX2
- **Fix:** Check with `lscpu | grep avx2`

### Wrong Results
- **Cause:** Wrong loop bounds or missing tail
- **Fix:** Ensure `i <= size - 8` and handle remainder

### Poor Performance
- **Cause:** Memory-bound or unaligned access
- **Fix:** Use aligned memory, check bandwidth

---

## Useful Resources

- **Intel Intrinsics Guide:** https://www.intel.com/content/www/us/en/docs/intrinsics-guide/
- **Agner Fog's Optimization Manuals:** https://www.agner.org/optimize/
- **GCC SIMD Extensions:** https://gcc.gnu.org/onlinedocs/gcc/Vector-Extensions.html

---

**Happy SIMD Programming! 🚀**
