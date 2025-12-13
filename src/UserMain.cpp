//
// Created by os on 1/3/25.
//

#include "../lib/console.h"
#include "../h/MemAllocator.hpp"
#include "../h/riscv.hpp"

#include "../h/syscall_c.hpp"
#include "../h/print.hpp"
#include "../test/printing.hpp"
#include "../test/ConsumerProducer_C_API_test.hpp"
#include "../test/ThreadSleep_C_API_test.hpp"






void userMain()
{

    printStringLock("Krenuo user main\n");

    Threads_C_API_test();
    // producerConsumer_C_API();
    // testSleeping();
    return ;
}


