*This project has been created as part of the 42 curriculum by phenry.*

## Description

**Codexion** is a multi-threaded simulation project designed to explore concurrent programming, resource allocation, and real-time scheduling constraints in C.

The project models a software development floor where a team of programmers (**Coders**) shares a finite set of hardware keys (**USB dongles**) to compile their code using a single central resource (**Quantum compiler**). The architecture mirrors the classic *Dining Philosophers Problem* but introduces advanced execution constraints:

* **Circular Seating:** Coders sit in a ring. Coder $N$ sits between Coder $N - 1$ and Coder $N + 1$, while Coder 1 sits next to the last Coder.
* **Resource Contention:** A dongle is placed between each coder. To compile, a coder must simultaneously hold **two adjacent dongles** (left and right).
* **The Development Cycle:** Each coder continuously cycles through three phases: **Compile**, **Debug**, and **Refactor**. A coder can only perform one task at a time.
* **The Burnout Lifecycle:** Coders work under a strict biological deadline. If a coder does not successfully start a compilation within their allocated `time_to_burnout` window, they suffer a fatal burnout, and the entire simulation immediately terminates.

The objective is to implement a robust scheduling engine that guarantees liveness, prevents deadlocks, tracks precise time intervals, and ensures clean termination without resource leaks.

---

## Instructions

### Compilation

The project compiles into an executable named `codexion` using a standard GNU `Makefile`. Run the following command at the root of the repository:

```bash
make
make codexion
```

### Execution & Arguments

The executable accepts exactly 8 arguments:

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

| Argument | Type | Description |
| --- | --- | --- |
| `number_of_coders` | `int` | Total number of coders (and matching number of available USB dongles). |
| `time_to_burnout` | `int` (ms) | Max duration a coder can survive without compiling before burning out. |
| `time_to_compile` | `int` (ms) | Time required to complete a compilation phase (requires 2 dongles). |
| `time_to_debug` | `int` (ms) | Time spent in the debugging phase. |
| `time_to_refactor` | `int` (ms) | Time spent in the refactoring phase. |
| `number_of_compiles_required` | `int` | Number of successful compilations each coder must hit to complete the program. |
| `dongle_cooldown` | `int` (ms) | Mandatory downtime for a dongle after release before it can be reused. |
| `scheduler` | `string` | Selection of the scheduling algorithm: `fifo` or `edf`. |

### Scheduling Strategies

* **`fifo` (First In, First Out):** Dongles are granted to coders in the chronological order of their requests.
* **`edf` (Earliest Deadline First):** Priority is dynamically calculated. The scheduler serves the coder closest to their burnout threshold first, where the deadline equals `last_compile_start + time_to_burnout`.

### Example Usage

```bash
./codexion 20 5000 2 1 100 2 10 fifo
```

### Cleaning

Makefile includes two cleaning rules: `clean` and `fclean`. First one removes the object files, the second does the same, then removes the `codexion` executable file.

```bash
make clean
make fclean
```

### Re-compile

To fully update the `codexion` executable file (`fclean` + `make codexion`), use:

```bash
make re
```

---

## Blocking Cases Handled

### Deadlock Prevention & Coffman's Conditions

To eliminate deadlocks, our architecture directly targets and breaks **Coffman's Circular Wait condition**. Instead of letting threads grab adjacent dongles arbitrarily, a deterministic ordering strategy is enforced:

* Each coder identifies their low-indexed and high-indexed dongle.
* Coders always attempt to lock their **lower-indexed dongle first**, followed by their higher-indexed dongle.
* *Edge Case (1 Coder):* If `number_of_coders == 1`, both the left and right positions point to the same dongle index. Attempting to lock the same normal mutex twice creates an absolute self-deadlock. The engine explicitly catches this case at thread entry, safely bypassing the locking pipeline and letting the single coder drop into a clean `sliced_sleep` until burnout occurs naturally.

### Starvation Prevention

Under the `edf` scheduler, resource allocation favors threads on the brink of failure. To prevent healthy threads from starving completely during high-load simulations, a custom priority queue structured as a min-heap tracks thread states. Since no standard template libraries are allowed, this heap structure guarantees that no thread is passed over when its parameters are mathematically feasible.

### Cooldown Handling

After a coder finishes compiling and drops their keys, the dongles enter a strict hardware lock state for the duration of `dongle_cooldown`. Coder threads cannot instantly re-acquire their released tools; timestamps are tracked atomically inside the dongle structures to ensure they remain isolated until the cooldown clears.

### Precise Burnout Detection

A dedicated asynchronous **Monitor Thread** runs continuously alongside the coder cluster. By decoupling lifecycle monitoring from the coders' operational loops, the system evaluates time deltas using `gettimeofday()`. This setup guarantees that if a coder crosses their burnout threshold, the event log is flushed to standard output well within the strict **10ms constraint** required by the system specifications.

### Log Serialization

Because multiple POSIX threads concurrently attempt to write to standard output, unmanaged logs would result in interleaved, garbled characters. The application routes all logging activities through a centralized, thread-safe logger protected by a dedicated output mutex, ensuring chronological clarity and formatting integrity.

---

## Thread Synchronization Mechanisms

```
+-------------------------------------------------------------+
|                        MONITOR THREAD                       |
+-------------------------------------------------------------+
                               |
                   Updates shared state flag
                   & broadcasts teardown
                               |
                               v
+-----------------------+              +----------------------+
|     CODER THREADS     | ===Locks===> |    DONGLE MUTEXES    |
| (Heap-based tracking) | <==Signals== | (pthread_cond_wait)  |
+-----------------------+              +----------------------+
                               |
                       Routes output to
                               v
+-------------------------------------------------------------+
|                  SERIALIZED LOGGER MUTEX                    |
+-------------------------------------------------------------+

```

### Threading Primitives Used

* **`pthread_mutex_t`:** Used to enforce mutual exclusion across distinct areas:
* Individual mutexes per dongle to guard structural states and cooldown clocks.
* A status mutex guarding `table->status->reason` to prevent concurrent modifications during abrupt shutdowns.
* A logging mutex ensuring atomic terminal output.


* **`pthread_cond_t`:** Utilized to coordinate efficient thread sleeping. Instead of busy-waiting (which wastes CPU cycles), coder threads call `pthread_cond_wait` when waiting for a specific dongle to become available or when waiting for the master startup barrier.

### Preventing Race Conditions

Race conditions during state initialization and teardown are eliminated by enforcing a strict hierarchy of operations:

1. All synchronization primitives, global structures, and the monitor infrastructure are fully initialized **before** launching the coder threads.
2. If an initialization error occurs midway through startup (e.g., `create_team` fails), the program signals `STOP_FATAL` to the active monitor, executes a `pthread_join` on its threads, and only then tears down the allocated table structures. This order ensures no thread is abandoned to access unmapped heap allocations.

### Thread-Safe Communication

Communication between the coders and the monitor is handled via an atomic status structure:

```c
pthread_mutex_lock(&table->status->lock);
if (table->status->reason == STOP_NONE)
{
    table->status->reason = reason;
    table->monitor->run = 0;
    pthread_cond_broadcast(&table->monitor->logger->has_log);
}
pthread_mutex_unlock(&table->status->lock);

```

When the monitor registers a fatal burnout, it safely obtains the status lock, alters the runtime state, and fires a broadcast signal. All operational coder threads check this state via an inline `is_running()` wrapper during their cycle boundaries, allowing them to release any partial resources and exit their routines cleanly.

---

## Resources

### References

* **Time** - https://openclassrooms.com/forum/sujet/temps-d-execution-en-c?__cf_chl_f_tk=mhTkTVgvh6uidiOR7KHIexhCYaQ4mLJyPWjttVZWl30-1783305968-1.0.1.-WAZ6aJmOC4dddQaCXGg8faTOUB9IY62HcOoVagSg0Os

**Threads in C**
* https://perso.ens-lyon.fr/francois.schwarzentruber/teaching/l3-prog/book/c_thread.html
* https://franckh.developpez.com/tutoriels/posix/pthreads/#LIX-A
* **Mutex** https://stackoverflow.com/questions/34524/what-is-a-mutex
* **Cond signals** https://stackoverflow.com/questions/52960662/trying-to-understand-pthread-cond-lock-and-pthread-cond-signal#comment92859253_52963925

### AI Usage Disclosure

Artificial Intelligence was used as a development assistant throughout this project, focusing on the following areas:

* **Architectural Layout & File Structure:** Designing a clean separation of concerns across directories (`srcs/coders/`, `srcs/dongle/`, `srcs/logger/` ...) to keep code modular.
* **Edge-Case Troubleshooting:** Isolating and fixing concurrency anomalies, such as handling the single-coder scenario without crashing the mutex pipeline.
* **Memory Safety & Verification:** Analyzing Valgrind tracing histories to catch hidden memory leaks involving unjoined thread stacks when partial initialization errors occurred.
* **Documentation:** Generating and structuring this `README.md` file according to the technical requirements of the curriculum.
