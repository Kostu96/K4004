#include "assembler/source/assembler.hpp"

#include <iostream>
#include <fstream>

const char* helpMessage =
R"=(Kostu96 Intel 4004 and 4040 assembler.

Usage:
assembler [-h|--help]
or
assembler <command> [-device <device>] <input_file> <output_file>

Commands:
asm    - assembles <file> into <device> binary code.
disasm - disassembles <file>.

-device - can be either i4004 or i4040.
          Default value is i4040.
)=";

const char* errorMessage = "Insufficient number of arguments. Use -h or --help to see usage hints.\n";

int main(int argc, const char* argv[])
{
    int retVal = 0;
    Assembler assembler;
    std::string inputFile, outputFile;
    bool i4004ModeEnabled = false;
    bool assemble = true;
    std::vector<uint8_t> bytecode;
    std::vector<std::string> disassembly;

    if (argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
        std::cout << helpMessage;
    else if (argc > 3 && (strcmp(argv[1], "asm") == 0)) {
        if (argc > 5 && strcmp(argv[2], "-device") == 0) {
            i4004ModeEnabled = strcmp(argv[3], "i4004") == 0;
            inputFile = argv[4];
            outputFile = argv[5];
        }
        else {
            inputFile = argv[2];
            outputFile = argv[3];
        }
    }
    else if (argc > 3 && (strcmp(argv[1], "disasm") == 0)) {
        assemble = false;
        inputFile = argv[2];
        outputFile = argv[3];
    }
    else {
        retVal = -1;
        std::cout << errorMessage;
    }

    bool success = false;
    if (retVal == 0) {
        if (assemble) {
            success = assembler.assemble(inputFile.c_str(), bytecode, i4004ModeEnabled);
        }
        else {
            std::ifstream fin(inputFile, std::ios_base::binary);
            uint8_t byte;
            while (fin >> byte)
                bytecode.push_back(byte);

            success = assembler.disassemble(bytecode, disassembly);
        }
    }

    if (!success)
        retVal = -1;

    if (retVal == 0) {
        if (assemble) {
            std::ofstream fout(outputFile, std::ios_base::binary);
            for (auto byte : bytecode)
                fout << byte;
        }
        else {
            std::ofstream fout(outputFile);
            for (auto str : disassembly)
                fout << str << '\n';
        }
    }

    return retVal;
}
