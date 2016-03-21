#include <stdio.h>
#include <getopt.h>
#include <vector>

#include "compiler.h"
#include "errors.hpp"

cmp_args parse_args(int argc, char** argv)
{
    char arg = 0;
    size_t bits = -1;
    std::vector<std::string> inputs;
    std::string output;
    as assembler = as::size;
    bool no_warn = false, only_compile = false;

    while ((arg = getopt(argc, argv, "i:o:b:a:wc")) != -1)
    {
        switch (arg)
        {
            case 'c':
                only_compile |= 1;
                break;
            case 'w':
                no_warn |= 1;
                break;
            case 'a':
                if (!strcmp("nasm", optarg))
                    assembler = as::NASM;
                else if (!strcmp("gas", optarg))
                    assembler = as::GAS;
                else
                    ERR(err_t::IO_CMD_ARG_AS_UNKNOWN, optarg);
                break;
            case 'b':
                bits = atoi(optarg);
                break;
            case 'o':
                output = std::string(optarg);
                break;
            case 'i':
                optind--;
                for (; optind < argc && argv[optind][0] != '-'; optind++)
                    inputs.push_back(std::string(argv[optind]));
                break;
            default:
                ERR(err_t::IO_CMD_ARG_UNKNOWN, optarg);
                break;
        }
    }

    return cmp_args(bits, inputs, output, assembler, no_warn, only_compile);
}

int main(int argc, char** argv)
{
    std::wcout << L"Compiling... ";

    cmp_args args = parse_args(argc, argv);
    compiler cmp = compiler(args);
    cmp.compile();

    std::wcout << L"Done.";

    return 0;
}

