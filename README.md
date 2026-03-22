# Operating Systems 1 — kernel project

Course project for **Operating Systems 1** (*Operativni sistemi 1*) at the University of Belgrade. The assignment is to implement a small **bare-metal kernel** for **RISC-V** (`rv64ima`, `lp64`) that runs under **QEMU** on the `virt` machine, without a general-purpose OS underneath.

## Scope

The kernel provides a minimal runtime for user-level code: **trap handling** (supervisor trap entry in assembly), **thread scheduling** with timer-based **preemption** (TCB, dispatcher, context switch in `contextSwitch.S`), and a **system call layer** exposed as:

- **C API** (`syscall_c.hpp`): heap (`mem_alloc` / `mem_free` and related), threads (`thread_create`, `thread_dispatch`, `thread_exit`, …), semaphores (`sem_open`, `sem_wait`, `sem_signal`, …), **timed sleep** (`time_sleep`), and **console** I/O (`getc`, `putc`).
- **C++ API** (`syscall_cpp.hpp`): RAII-style wrappers around the same facilities.

Supporting pieces include a **memory allocator**, **kernel console** (`KConsole`), **scheduler** and **queues**, **RISC-V** control/status helpers, and **tests** under `test/` (threads, producer–consumer, sleep, system mode, C/C++ API variants). The build produces a single linked **`kernel`** image (`Makefile`, `kernel.ld`) for QEMU; optional **GDB** debugging is wired via the `qemu-gdb` target and `.gdbinit.tmpl-riscv`.

## Build and run

Requires a **RISC-V bare-metal** toolchain (`riscv64-unknown-elf-*` or `riscv64-linux-gnu-*`, auto-detected by the Makefile) and **qemu-system-riscv64**.

- **Build:** `make all` — produces `kernel` and `kernel.asm`.
- **Run in QEMU:** `make qemu`

## Development notes

- Before the trap / interrupt path is fully installed, avoid relying on **system calls** in code that runs too early during bring-up.
- **`Thread` / `Semaphore` (C++ API)** and dynamic allocation: do not construct these or call **`new`** in **global scope**; global constructors run before the kernel is ready and can conflict with teardown ordering.
- Early **`putc`**-style output may use paths that do not depend on the full console pipeline; once the console subsystem is initialized, use the documented APIs consistently.
