#include "Vtop.h"
#include "verilated.h"

#include <cstdlib>
#include <iostream>

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
    dut.reset_n = 0;
    tick(dut);

    dut.reset_n = 1;

    char byte{};
    while (!context.gotFinish() && std::cin.get(byte)) {
        dut.code = static_cast<unsigned char>(byte);
        tick(dut);

        if (dut.unlocked) {
            std::abort();
        }
    }
    dut.final();
    return 0;
}
