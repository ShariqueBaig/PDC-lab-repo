#include <immintrin.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <cstring>
#include <algorithm> // For std::min and std::max

class Timer {
    std::chrono::high_resolution_clock::time_point start_time;
public:
    void start() { start_time = std::chrono::high_resolution_clock::now(); }
    double elapsed_ms() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start_time).count();
    }
};

// Scalar version
void convolve_1d_scalar(const float* input, int input_size, 
                        const float* kernel, int kernel_size,
                        float* output) {
    int output_size = input_size - kernel_size + 1;
    
    for (int i = 0; i < output_size; i++) {
        float sum = 0.0f;
        for (int j = 0; j < kernel_size; j++) {
            sum += input[i + j] * kernel[j];
        }
        output[i] = sum;
    }
}

// SIMD version
void convolve_1d_simd(const float* input, int input_size,
                      const float* kernel, int kernel_size,
                      float* output) {
    int output_size = input_size - kernel_size + 1;
    
    int i = 0;
    
    // Process 8 outputs at a time
    for (; i <= output_size - 8; i += 8) {
        __m256 sum = _mm256_setzero_ps();
        
        // For each kernel element
        for (int j = 0; j < kernel_size; j++) {
            // Load 8 consecutive input values starting at position (i + j)
            // Note: input is unaligned, so use loadu
            __m256 input_vec = _mm256_loadu_ps(&input[i + j]);
            
            // Broadcast current kernel value to all 8 lanes
            __m256 kernel_vec = _mm256_set1_ps(kernel[j]);
            
            // Accumulate: sum += input_vec * kernel_vec
            sum = _mm256_fmadd_ps(input_vec, kernel_vec, sum);
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
    const int KERNEL_SIZE = 5; // Must be odd for symmetric convolution usually, but works for any size herewith this logic
    const int OUTPUT_SIZE = INPUT_SIZE - KERNEL_SIZE + 1;
    const int ITERATIONS = 1000;
    
    // Allocate arrays with 32-byte alignment for better performance (though loadu handles unaligned)
    // Using simple new for portability, but aligned_alloc is better in practice
    float* input = new float[INPUT_SIZE];
    float* kernel = new float[KERNEL_SIZE];
    float* output_scalar = new float[OUTPUT_SIZE];
    float* output_simd = new float[OUTPUT_SIZE];
    
    // Initialize input (simulated audio signal)
    for (int i = 0; i < INPUT_SIZE; i++) {
        input[i] = std::sin(i * 0.01f) + 0.5f * std::cos(i * 0.05f);
    }
    
    // Initialize kernel (Gaussian smoothing filter)
    // kernel size 5: [0.06, 0.24, 0.40, 0.24, 0.06] - sum is 1.0
    if (KERNEL_SIZE >= 5) {
        kernel[0] = 0.06f;
        kernel[1] = 0.24f;
        kernel[2] = 0.40f;
        kernel[3] = 0.24f;
        kernel[4] = 0.06f;
        // Fill rest with 0 if kernel size > 5
        for (int i = 5; i < KERNEL_SIZE; i++) kernel[i] = 0.0f;
    } else {
         // Fallback for smaller kernels
         for (int i = 0; i < KERNEL_SIZE; i++) kernel[i] = 1.0f / KERNEL_SIZE;
    }
    
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
        // Prevent compiler optimization by modifying input
        input[0] += 0.000001f;
    }
    double scalar_time = timer.elapsed_ms();
    
    // Reset input for fairness
    input[0] -= ITERATIONS * 0.000001f;
    
    // Benchmark SIMD version
    timer.start();
    for (int iter = 0; iter < ITERATIONS; iter++) {
        convolve_1d_simd(input, INPUT_SIZE, kernel, KERNEL_SIZE, output_simd);
        // Prevent compiler optimization
        input[0] += 0.000001f;
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
