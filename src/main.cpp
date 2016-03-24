#include <stdio.h>
#include <getopt.h>
#include <vector>

#include "compiler.h"
#include "errors/errors.hpp"

void print_version()
{
    std::wcout << L"Version: " << L"3.1 build on " << __DATE__ << L" " << __TIME__ << std::endl;
}

void print_help()
{
    std::wcout << L"Use this program to compile ene-source." << std::endl <<
                  L"usage: ene [options] -i input_file..." << std::endl <<
                  L"  options:  (first is default)" << std::endl <<
                  L"    -a (nasm | gas)         Set the assembler" << std::endl <<
                  L"    -b (32 | 64 | 16)       Set register width" << std::endl <<
                  L"    -c                      Compile only, no assembling nor linking" << std::endl <<
                  L"    -h                      fixpoint" << std::endl <<
                  L"    -o output_file          Set the output name, default is first input file with .out" << std::endl <<
                  L"    -p                      Handle warnings as errors" << std::endl <<
                  L"    -t template_directory   Will then use other templates, def. is ../src/templates/" << std::endl <<
                  L"    -v                      Print version" << std::endl <<
                  L"    -w                      Turn off warnings" << std::endl;
}

int parse_args(int argc, char** argv, cmp_args& ret)
{
    char arg = 0;
    size_t bits = 32;
    std::vector<std::string> inputs;
    std::string output, template_path = std::string("../src/templates/");
    as assembler = as::NASM;
    bool no_warn = false, only_compile = false, pedantic_err = false;

    while ((arg = getopt(argc, argv, "t:i:o:b:a:wcphv")) != -1)
    {
        switch (arg)
        {
            case 'h':
                print_help();
                return -1;
            case 'p':
                pedantic_err = true;
                break;
            case 't':
                template_path = std::string(optarg);
                break;
            case 'c':
                only_compile |= 1;
                break;
            case 'w':
                no_warn = 1;
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
            case 'v':
                print_version();
                return -1;
            default:
                ERR(err_t::IO_CMD_ARG_AS_UNKNOWN, optarg);
                return -1;
        }
    }

    if (!inputs.size())
    {
        print_help();
        return -1;
    }

    if (!output.size())
        ERR(err_t::IO_CMD_ARG_NO_OUTPUT);

    ret = cmp_args(bits, inputs, output, template_path, assembler, no_warn, pedantic_err, only_compile);
    return 0;
}

int main(int argc, char** argv)
{
    cmp_args args;
    if (parse_args(argc, argv, args))
        return -1;

    compiler cmp = compiler(args);
    return cmp.compile();
}

