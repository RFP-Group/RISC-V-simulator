#include "interpreter/executor.h"

namespace simulator::interpreter {

void Executor::GetTrace(const simulator::Instruction *cur_instr)
{
    FILE *dumpFile = fopen("instr_trace.trace", "a");
    if (!dumpFile) {
        std::cerr << "Error during file opening to emit instructions traces\n";
        return;
    }

    fprintf(dumpFile, "PC:%li \n", getPC());

    fprintf(dumpFile, "rd: %hhu, ", cur_instr->rd);
    fprintf(dumpFile, "rs1: %hhu, ", cur_instr->rs1);
    fprintf(dumpFile, "rs2: %hhu, ", cur_instr->rs2);
    fprintf(dumpFile, "rs3: %hhu, ", cur_instr->rs3);
    fprintf(dumpFile, "imm: %x, ", cur_instr->imm & 0xFFF);
    fprintf(dumpFile, "rm: %hhu\n", cur_instr->rm);

    auto &gprf = getGPRfile();

    fprintf(dumpFile, "rd: %lu, ", gprf.read(cur_instr->rd));
    fprintf(dumpFile, "rs1: %lu, ", gprf.read(cur_instr->rs1));
    fprintf(dumpFile, "rs2: %lu, ", gprf.read(cur_instr->rs2));
    fprintf(dumpFile, "rs3: %lu, ", gprf.read(cur_instr->rs3));
    fprintf(dumpFile, "rm: %lu\n", gprf.read(cur_instr->rm));

    fprintf(dumpFile, "#################################\n");

    fclose(dumpFile);
}

}  // namespace simulator::interpreter
