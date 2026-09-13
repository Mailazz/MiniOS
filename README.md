# MiniOS — Operating System & CPU Scheduling Simulator

A C++ simulator implementing core operating-system concepts: process management (via a Process Control Block model) and four CPU scheduling algorithms, with performance evaluation across waiting time, turnaround time, and response time.

## What it does

Each process is modeled with a PCB (`Process` struct) holding arrival time, burst time, priority, and computed metrics. The simulator runs the same workload through four scheduling algorithms and prints a Gantt chart plus performance metrics for each, followed by a side-by-side comparison.

### Implemented algorithms
- **FCFS (First-Come, First-Served)** — processes run strictly in arrival order.
- **SJF (Shortest Job First)** — non-preemptive; always picks the shortest available burst time.
- **Priority Scheduling** — non-preemptive; lower priority number = higher priority.
- **Round Robin** — preemptive, time-quantum based (default quantum = 3).

### Metrics computed
- **Waiting time** — time a process spends ready but not running.
- **Turnaround time** — total time from arrival to completion.
- **Response time** — time from arrival to first getting the CPU.

## Sample output

```
=== Shortest Job First (SJF) ===
PID   Arrival   Burst   Completion  Waiting   Turnaround  Response
1     0         8       8           0         8           0
2     1         4       14          9         13          9
3     2         9       28          17        26          17
4     3         5       19          11        16          11
5     4         2       10          4         6           4

Avg Waiting Time:    8.20
Avg Turnaround Time: 13.80
Avg Response Time:   8.20
```

Running all four algorithms on the same workload shows the expected trade-offs: SJF minimizes average waiting/turnaround time, while Round Robin gives the best (lowest) average response time at the cost of higher waiting time — exactly the trade-off scheduling theory predicts.

## Build & run

```bash
make run
```

or manually:

```bash
g++ -std=c++17 -Wall -o minios src/main.cpp src/scheduler.cpp src/process.cpp
./minios
```

## Project structure

```
MiniOS/
├── include/
│   ├── process.h      # PCB struct + state enum
│   └── scheduler.h    # Scheduling algorithm declarations
├── src/
│   ├── process.cpp
│   ├── scheduler.cpp  # FCFS, SJF, Priority, Round Robin implementations
│   └── main.cpp        # Sample workload + comparison runner
├── Makefile
└── README.md
```

## Roadmap (next phases)

This currently covers process management and CPU scheduling (Phases 1–2 of the original plan). Planned next:
- **Memory management**: paging simulation with FIFO and LRU page-replacement algorithms.
- **Synchronization**: producer–consumer problem using mutexes and threads.

## Why I built this

I wanted to move past *using* cloud/DevOps tools and actually understand the operating-system fundamentals underneath them — how scheduling decisions affect real performance metrics, not just in theory but in measurable output.
