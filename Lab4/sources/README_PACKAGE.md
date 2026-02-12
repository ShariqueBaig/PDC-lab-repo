# SIMD Programming Lab 

## 🎯 Lab Overview

### Prerequisites
- Basic C++ (loops, functions, pointers)
- Linux environment (or WSL2/Docker for Windows users)
- CPU with AVX2 support (Intel 2013+, AMD 2015+)

### Learning Outcomes
Students will be able to:
1. Write SIMD code using Intel AVX2 intrinsics
2. Measure performance improvements
3. Apply SIMD to real-world problems
4. Understand when and why to use SIMD

---

## 📚 Lab Structure

### Part 1: SIMD Basics
Five foundational examples with complete code:
1. Importing SIMD headers
2. Initializing vectors
3. Using unions for easy access
4. Loading/storing data
5. Data types and conversions

### Part 2: SIMD Computations
Two computational examples:
1. Simple arithmetic operations
2. Dot product with horizontal sum

### Part 3: Advanced Examples
Two practical applications:
1. Conditional code with masks
2. Image processing (RGB to grayscale)

### Part 4: Challenge
Students implement **1D convolution** on their own:
- Scalar version (guided)
- SIMD version (challenging)
- Performance comparison
- Expected speedup: 4-6x

---

## 🚀 Quick Start

1. **Check CPU support:**
   ```bash
   lscpu | grep avx2
   ```

2. **Install dependencies:**
   ```bash
   sudo apt update
   sudo apt install build-essential
   ```

3. **Clone repository:**
   ```bash
   git clone https://github.com/yuninxia/hands-on-simd-programming.git
   ```

4. **Follow SIMD_Lab_README.md**

---

## 🔧 Troubleshooting

### Common Issues

**"AVX2 not supported"**
- Check CPU: `lscpu | grep avx2`
- Use Docker with AVX2 image
- Fall back to SSE (modify flags to `-msse4.2`)

**"Illegal instruction"**
- Code was compiled with AVX2 but CPU doesn't support it
- Recompile without `-mavx2` or run on different machine

**"No speedup observed"**
- Check optimization: `-O3` flag
- Verify large enough array size (> 10,000 elements)
- Confirm SIMD code is actually running (add print statement)

**"Segmentation fault"**
- Using `_mm256_load_ps()` on unaligned data
- Solution: Use `_mm256_loadu_ps()` instead

---

## 📊 Expected Results

### Performance Metrics

Students should observe:

| Example | Typical Speedup | Notes |
|---------|----------------|-------|
| Simple Math | 5-7x | Best case for arithmetic |
| Dot Product | 4-6x | Horizontal sum overhead |
| Conditional | 2-4x | Mask overhead |
| Image Processing | 3-5x | Memory bandwidth limited |
| Convolution (Challenge) | 4-6x | Depends on kernel size |

**Note:** Actual speedups vary based on:
- CPU model and generation
- Memory bandwidth
- Compiler version
- System load

---

## 🤝 Credits

This lab is based on the excellent repository:
**hands-on-simd-programming** by yuninxia
https://github.com/yuninxia/hands-on-simd-programming

Lab materials created to provide:
- Beginner-friendly introduction
- Complete working examples
- Hands-on challenge
- Comprehensive teaching guide