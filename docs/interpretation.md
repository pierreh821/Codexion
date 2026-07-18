# Arguments:
- **number_of_coders**: as name says but also number of usb dongles
- **time_to_burnout** (ms): max time before compilation, else coder will burnout
- **time_to_compile** (ms): time needed for compilation. during that time, the coder needs 2 usb dongles
- **time_to_debug** (ms)
- **time_to_refactor** (ms)
- **number_of_compiles_required**: program stops once all coders compiled this many times
- **dongle_cooldown** (ms): time needed for dongles to be available after use
- **scheduler**: must be fifo or edf only:
    - **fifo**: First In, First Out, dongle is given to the first coder that requested
    - **edf**: Earliest Deadline First with deadline, serves the coder with the earliest burn ou (`last_compile_start + time_to_burnout`)

# How it works
- One common compiler (Quantum compiler)
- As many USB dongles as coder
- At the start, there is a dongle between every coder
- Each coder has an id (starting from 1)
- Coders have defined sits: number `1` sits next to coder `number_of_coders`. Coder `N` sits between coder `N - 1` and coder `N + 1`
- Process order: **compile**, **debug**, and **refactor**
    - Coder can only do one task at a time
    - To compile, coder needs **2** dongles
- No communication between coders (cannot know if any other coder is about to burn out)

- If the coder does not compile in the alloted time, they burn out
- If any coder burns out, the program stops

# Log
Log syntax:
```
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```
With:
- `timestamp_in_ms` being the current timestamp
- `X` being the coder's ID

Example
```
0 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
201 1 is debugging
401 1 is refactoring
402 2 has taken a dongle
403 2 has taken a dongle
403 2 is compiling
603 2 is debugging
803 2 is refactoring
1204 3 burned out
```

### Warnings
- Messages must not be mixed
- Burnout logs must be displayed no more than 10ms following the actual burnout

### Timing consideration
To reduce hardware impact on performance measurements, consider using CPU usage
time instead of real-time clock when feasible. However, for this project, real-time
measurements using `gettimeofday()` are acceptable and recommended for simplicity.

# Technical points
- Each coder is represented by a **thread** (`pthread_create`)
- Dongles' state must be protected with a mutex (`pthread_mutex_t`), a condition variable can be used for queue management (`pthread_cond_t`)
- A separate thread `monitor` to detect burnout at the right time and stop the program
- Logging must be serialized to avoid mixing signals (mutex usage to protect output)
- Must use a priority queue (heap) for FIFO/EDF scheduling (no standard library priority queue)
- No memory leaks

# Authorized external functions
- `pthread_create`: create the thread
- `pthread_join`: wait for the thread to end
- `pthread_mutex_init`: https://stackoverflow.com/questions/14320041/pthread-mutex-initializer-vs-pthread-mutex-init-mutex-param
- `pthread_mutex_lock`: try to reserve the mutex. if unavailable, wait.
- `pthread_mutex_unlock`: release the mutex (so that other threads can use it)
- `pthread_mutex_destroy`: free mutex, makes the var uninitialized
- `pthread_cond_init`
- `pthread_cond_wait`
- `pthread_cond_timedwait`
- `pthread_cond_signal`
- `pthread_cond_broadcast`
- `pthread_cond_destroy`
- `gettimeofday`
- `usleep`
- `write`
- `malloc`
- `free`
- `printf`
- `fprintf`
- `strcmp`
- `strlen`
- `atoi`
- `memset`

# Not sure to understand
- What impact does the dongle side have on the way the coders take them?
- > The program must guarantee liveness: coders should not starve under edf scheduling, provided the parameters are feasible.

# Resources

- Volatile keyword - https://stackoverflow.com/questions/246127/why-is-volatile-needed-in-c
- Time - https://openclassrooms.com/forum/sujet/temps-d-execution-en-c?__cf_chl_f_tk=mhTkTVgvh6uidiOR7KHIexhCYaQ4mLJyPWjttVZWl30-1783305968-1.0.1.-WAZ6aJmOC4dddQaCXGg8faTOUB9IY62HcOoVagSg0Os

## Threads in C
- https://perso.ens-lyon.fr/francois.schwarzentruber/teaching/l3-prog/book/c_thread.html
- https://franckh.developpez.com/tutoriels/posix/pthreads/#LIX-A
- Mutex - https://stackoverflow.com/questions/34524/what-is-a-mutex
- Cond signals - https://stackoverflow.com/questions/52960662/trying-to-understand-pthread-cond-lock-and-pthread-cond-signal#comment92859253_52963925


# Structure

Could look like this at the end:

```
.
├── Makefile
├── .gitignore
├── docs/               # Tes sujets, pseudocodes, documentations
├── models/             # Strictement tes structures de données (.h)
│   ├── args.h
│   └── coder.h
├── includes/           # Tes prototypes de fonctions globaux
│   └── codexion.h
└── srcs/               # Tout ton code source (.c) dispatché par module
    ├── main.c
    ├── parser/         # Tout ce qui touche aux arguments et au parsing
    │   └── parser.c
    ├── engine/         # Le cœur de ton programme (gestion des threads, boucles)
    │   └── scheduler.c
    └── utils/          # Tes fonctions outils (error, affichage, libération...)
        ├── coder_utils.c
        └── error.c
```

# Tests

## testing if everything works when the function returns error (0, NULL, etc..)

- `clean_args`
- `ft_calloc`
- `ft_strdup`
- `init_table`
- `create_team`
- `alloc_team`
- `assign_cond`
- `assign_coders`
- `assign_dongles`
- `launch_threads`
