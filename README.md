*This project has been created as part of the 42 curriculum by abounoua.*

# 💻 Codexion: Master the race for resources before the deadline masters you

## Description

Computer programming and collaborative environments often face resource bottlenecks, where specialized hardware or tools are scarce and must be shared carefully to avoid conflicts. **Codexion** is a concurrent programming simulation written in C that models a circular co-working hub. In this environment, multiple coders compete for limited USB dongles to compile their quantum code.

The main objective of the project is to orchestrate multiple threads representing coders and a monitor, synchronizing access to shared hardware resources using POSIX threads, mutexes, and condition variables, while enforcing strict real-time scheduling policies (**FIFO** or **EDF**) and preventing developer burnout.

---

## Instructions

### Prerequisites

* C compiler (`cc`) supporting C99 standard
* `make` utility
* POSIX threads library (`-pthread`)

### Installation & Compilation

Clone the repository, enter the root directory, and compile the project using the provided Makefile:

```bash
git clone <your-repository-url>
cd codexion

# Compile the executable using the Makefile rules
make

```

### Execution

The program takes exactly 8 mandatory configuration arguments:

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler

```

#### Argument Details:

* `number_of_coders`: Number of coders in the hub and total available USB dongles.


* `time_to_burnout` (ms): Maximum time a coder can go without starting a new compilation before experiencing a burnout.


* `time_to_compile` (ms): Duration required to compile code while holding two dongles.


* `time_to_debug` (ms): Time spent debugging after compilation.


* `time_to_refactor` (ms): Time spent refactoring before immediately attempting to acquire dongles again.


* `number_of_compiles_required`: Optional completion condition; simulation stops successfully once all coders reach this compilation count.


* `dongle_cooldown` (ms): Inactive cooling period required before a released dongle can be retaken.


* `scheduler`: Arbitration policy governing dongle requests. Must be strictly either `fifo` or `edf`.



#### Example Execution:

```bash
./codexion 4 800 200 200 200 5 100 edf

```

#### Makefile Rules:

* `make`: Compiles the project without unnecessary relinking.


* `clean`: Removes object files and build directories.


* `fclean`: Removes object files and the final `codexion` executable.


* `re`: Forces a full recompilation (`fclean` followed by `all`).



---

## Thread Synchronization Mechanisms

To coordinate access to shared resources safely and efficiently, Codexion relies on native POSIX threading primitives:

* **`pthread_mutex_t` (Mutual Exclusion):**
Mutexes are deployed extensively to protect critical sections and prevent race conditions on shared states:
* Each dongle possesses its own `dongle_mutex` protecting its availability state, holder status, and internal queue tickets.
* Coder arguments (`last_compilation` timestamps and completion flags `end`) are guarded by individual mutexes (`last_compil_mutex` and `end_mutex`).
* Simulation-wide parameters (`running` state) and log outputs (`print_mutex`) are strictly serialized to avoid race conditions and interleaved terminal text.


* **`pthread_cond_t` (Condition Variables):**
Pausing threads via condition variables (`pthread_cond_wait` / `pthread_cond_broadcast`) prevents CPU-intensive busy waiting (spinning) while a coder waits for a locked or cooling dongle. When a dongle is released, a broadcast signals waiting threads to re-evaluate availability.
* **Thread-Safe Communication & Race Prevention:**
Communication between coder threads and the independent monitor thread occurs entirely through shared data structures protected by mutex locks. For instance, when the monitor checks if a coder has burned out, it locks `end_mutex` and accesses `last_compilation` safely through a thread-safe helper `last_ct()`, preventing torn reads or asynchronous memory corruption.

---

## Blocking Cases Handled

* **Deadlock Prevention (Coffman's Conditions):**
Since every coder requires two adjacent dongles (left and right), circular wait deadlocks are neutralized using a strict **resource acquisition ordering based on parity**. Even-numbered coders lock their right dongle first, then their left dongle, while odd-numbered coders acquire them in reverse order (or vice versa), breaking the circular dependency chain.
* **Starvation Prevention & Fair Arbitration:**
When multiple coders request the same dongle, fair arbitration is enforced according to the selected scheduler:


* **FIFO (First-In, First-Out):** Access is granted strictly in the order requests arrived.


* **EDF (Earliest Deadline First):** Access prioritizes the coder with the closest burnout deadline (`last_compile_start + time_to_burnout`). Ticket-based queue structures manage these priorities deterministically.




* **Cooldown Handling:**
When a dongle is released, its `available_at` timestamp is updated to `current_time + dongle_cooldown`. Other threads sleep or wait conditionally until this cooldown duration completely elapses.


* **Precise Burnout Detection (<10 ms):**
A dedicated monitor thread loops continuously, polling coder timestamps. If a coder exceeds `time_to_burnout`, the monitor immediately flips the global simulation flag, broadcasts condition variables to unblock waiting threads, and prints the burnout log within the mandatory 10 ms window.


* **Log Serialization:**
All simulation events (`has taken a dongle`, `is compiling`, `is debugging`, `is refactoring`, `burned out`) pass through a central thread-safe printing function wrapped around `print_mutex`, ensuring messages never mix up or interleave on the same line.



---

## Logic, Challenges & Difficulties Faced

1. **Managing Granular Timings and Micro-Delays:**
Balancing `usleep` durations with real-time checks using `gettimeofday()` presented challenges regarding OS scheduling jitter. Fine-tuning the monitor's polling loop frequency (using 1 ms sleeps) ensured that burnouts were detected with extreme accuracy without overloading CPU cores.
2. **Avoiding Deadlocks under High-Concurrency Pressure:**
Ensuring that no edge cases caused a deadlock—especially when coders contended for overlapping resources under tight cooldowns—required rigorous validation of the lock/unlock sequences inside `get_dongles()` and `release_dongles()`.
3. **Norm Compliance and Code Splitting:**
Adhering strictly to the 42 Norm (max 25 lines per function, max 80 columns per line) while handling complex thread initialization loops, custom struct cleanups, and safe error-handling paths required careful decomposition of tasks into modular static helper functions.

---

## Resources

* **Concurrency & POSIX References:**
* *POSIX Threads Programming* (Lawrence Livermore National Laboratory) — Guide on pthreads, mutexes, and condition variables.
* *The Little Book of Semaphores* by Allen B. Downey — Concepts on synchronization patterns and concurrency hazards.


* **AI Usage:**
* *Gemini* was utilized to assist in structuring the documentation, formulating explanations for complex concurrency paradigms (such as deadlock prevention and scheduling policies), and refining the overall layout of this README.md file.