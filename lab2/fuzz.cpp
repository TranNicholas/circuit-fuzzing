#include "Vtop.h"
#include "verilated.h"

#include <cstdlib>
#include <iostream>
#include <limits>

/* this lets the source compile without afl-clang-fast/lto */
#ifndef __AFL_FUZZ_TESTCASE_LEN

ssize_t       fuzz_len;
unsigned char fuzz_buf[1024000];

  #define __AFL_FUZZ_TESTCASE_LEN fuzz_len
  #define __AFL_FUZZ_TESTCASE_BUF fuzz_buf
  #define __AFL_FUZZ_INIT() void sync(void);
  #define __AFL_LOOP(x) \
    ((fuzz_len = read(0, fuzz_buf, sizeof(fuzz_buf))) > 0 ? 1 : 0)
  #define __AFL_INIT() sync()

#endif

__AFL_FUZZ_INIT();

/* Main entry point. */

/* To ensure checks are not optimized out it is recommended to disable
   code optimization for the fuzzer harness main() */
#pragma clang optimize off
#pragma GCC optimize("O0")

namespace {
    void tick(Vtop& dut)
    {
        dut.clk = 0;
        dut.eval();

        dut.clk = 1;
        dut.eval();
    }
}

int main(int argc, char** argv)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    VerilatedContext context;
    context.commandArgs(argc, argv);

    Vtop dut{&context};

    dut.clk = 0;
    dut.code = 0;

    ssize_t        len;                        /* how much input did we read? */
    unsigned char *buf;                        /* test case buffer pointer    */

    /* The number passed to __AFL_LOOP() controls the maximum number of
        iterations before the loop exits and the program is allowed to
        terminate normally. This limits the impact of accidental memory leaks
        and similar hiccups. */

    __AFL_INIT();
    buf = __AFL_FUZZ_TESTCASE_BUF;  // this must be assigned before __AFL_LOOP!

    while (__AFL_LOOP(std::numeric_limits<unsigned int>::max())) {  // increase if you have good stability
        len = __AFL_FUZZ_TESTCASE_LEN;  // do not use the macro directly in a call!
        if (len < 3) continue;

        dut.reset_n = 0;
        tick(dut);

        dut.reset_n = 1;
        for (ssize_t i = 0; i < len; i++) {
            dut.code = buf[i];
            tick(dut);

            if (dut.unlocked) {
                std::abort();
            }
        }
    }
    dut.final();
    return 0;
}
