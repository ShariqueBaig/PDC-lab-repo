## Lab 7 - GPGPU Programming

Read the article presented in the following link:

[Mapping Computational Concepts to GPUs](https://developer.nvidia.com/gpugems/gpugems2/part-iv-general-purpose-computation-gpus-primer/chapter-31-mapping-computational)

Once you've read the article, answer the following questions:

### Questions

1. What are the two main attributes of computations that map well to GPUs, according to the article?

2. Explain the concept of "arithmetic intensity." Why is it important for GPU performance?

3. Why are grid-based simulations (like the cloud simulation example) useful when understanding GPU computing concepts?

4. In the context of GPU computing, what is the difference between gather and scatter communication?

5. Name and describe the two types of programmable processors on a GPU discussed in the article.

6. What is one major limitation of fragment processors when it comes to general-purpose computation?

7. What is the GPU analogy for a CPU array and loop when programming a data-parallel computation?

8. Why is render-to-texture important for feedback in GPGPU programming?

9. What is a reduction operation in GPU computing, and why is it more challenging to implement efficiently on a GPU compared to a CPU?

10. According to Table 31-1, what are the main differences in floating-point formats supported by NVIDIA and ATI GPUs, and how do these differences affect computation precision?

## Task
The following link is the source code of the Reaction-Diffusion program presented in the article:

[Code](https://http.download.nvidia.com/developer/GPU_Gems_2/CD/Content/31.zip)

To run this code, you may require Nvidia CG Toolkit which can be downloaded from:
- [Github](https://github.com/FrozenStormInteractive/Cg-Toolkit/tree/master)
- [Docker Image](https://hub.docker.com/r/juanferreira/nvidia-cgtoolkit)

Successfully run and print the output of this code.
