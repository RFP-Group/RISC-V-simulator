#include <iostream>
#include "hart.h"
#include "mmu.hpp"

#include <CLI/CLI.hpp>
#include <CLI/App.hpp>

namespace simulator {

static core::Hart::Mode getMode(const std::string &mode_str)
{
    core::Hart::Mode mode = core::Hart::Mode::NONE;
    if (mode_str == "simple") {
        mode = core::Hart::Mode::SIMPLE;
    } else if (mode_str == "bb") {
        mode = core::Hart::Mode::BB;
    }
    return mode;
}

int Main(int argc, const char **argv)
{
    CLI::App app("RISC-V simulator");

    std::string input_file {};
    auto *input_arg = app.add_option("--in", input_file, "Input file");
    input_arg->required();

    std::string mode {};
    auto *mode_arg = app.add_option("--mode", mode, "Execution mode [simple by default] [use lower case]");
    mode_arg->required();

    bool need_to_measure {};
    auto *need_to_measure_arg =
        app.add_option("--measure", need_to_measure, "Pass some true value if need to measure time");
    need_to_measure_arg->default_val(false);

    bool is_cosim {};
    auto *is_cosim_arg =
        app.add_option("--cosimulation", is_cosim, "Pass some true value if need to get trace of instructions");
    is_cosim_arg->default_val(false);

    CLI11_PARSE(app, argc, argv);

    mem::MMU *mmu = mem::MMU::CreateMMU();
    uintptr_t entry_point = mmu->StoreElfFile(input_file);
    core::Hart hart(mmu, entry_point, is_cosim);
    hart.RunImpl(getMode(mode), need_to_measure);
    return 0;
}
}  // namespace simulator

int main(int argc, const char **argv)
{
    return simulator::Main(argc, argv);
}