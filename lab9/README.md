# Lab 9: Inter-Process Communication (IPC)

## Objective
The objective of this lab is to explore various mechanisms for communication between processes in a distributed or multi-process environment.

## Contents
The lab is divided into several sections:
- **Section A: Pipes and FIFOs**
    - `task_a1.c`: Basic anonymous pipes.
    - `task_a2.c`: Bi-directional communication with pipes.
    - `task_a3_p1.c`, `task_a3_p2.c`: Named pipes (FIFOs).
- **Section B: Shared Memory**
    - `task_b1.c`: Process synchronization using shared memory.
- **Section C: Sockets**
    - `task_c1.c`: Local socket communication.
    - `task_c2_server.c`, `task_c2_client.c`: TCP Socket Server/Client.
    - `task_c3_server.c`, `task_c3_client.c`: UDP Socket Server/Client.
- **Section D: Message Queues**
    - `task_d1.c`, `task_d2.c`: POSIX/System V Message Queues.

## Key Concepts
- Process Forking and execution.
- Anonymous Pipes vs. Named Pipes (FIFOs).
- Socket Programming (TCP/UDP, Server/Client model).
- Message Passing vs. Shared Memory.

## How to Build and Run
Compile each C file using GCC:
```bash
gcc task_a1.c -o a1
./a1
```
For Server/Client tasks, run the server in one terminal and the client in another.
