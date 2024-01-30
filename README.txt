# README for XV6 Threading and Scheduling Enhancement Project

## Introduction
This project significantly enhances the xv6 operating system by introducing advanced kernel threading support and a new scheduler. These features elevate xv6's multitasking capabilities and optimize its performance on multi-core systems.

## Features
1. **Kernel Threading Support**: Implementation of `clone()` and `join()` system calls, and user-space functions like `thread_create()`, mirroring Linux's pthreads functionality.
2. **New Scheduler**: A sophisticated scheduling algorithm is introduced to balance fairness between processes and threads, maximizing efficiency and performance.
3. **Enhanced Synchronization and Memory Management**: Improved mechanisms for handling shared memory and ensuring thread safety, addressing challenges in synchronization and error handling.

## Installation and Usage
Detailed steps for installing and using the enhanced xv6 on various systems (Ubuntu VM, WSL2, Mac) are provided, ensuring a seamless setup process.

## Contributions
- **System Calls and User-Space Functions**: Implementation of threading support, making xv6 more versatile in handling concurrent processes.
- **New Scheduler**: Development of a new scheduling strategy to improve system responsiveness and fairness.
- **Testing and Documentation**: Rigorous testing to ensure stability and comprehensive documentation for ease of use.

## Conclusion
This project marks a significant milestone in the evolution of the xv6 operating system, adding robust multi-threading capabilities and an advanced scheduler, thus making it more suitable for modern multi-core environments.
