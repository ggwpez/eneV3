#include <stdio.h>

#include "compiler.h"
#include "errors.hpp"

#define SKIP_COMPILATION 0
#define SKIP_LAUNCH 0

int main(void)
{
    const char* input = "../src/input.ene";
    const char* output = "output.asm";
    const char* obj = ".o";
    const char* exe = ".exec";

    compiler cmp;
    cmp.compile(input);

    std::wcout << "Writing output to file...";
    cmp.write_wstr(output);
    std::wcout << "Done." << std::endl;

    std::stringstream cmd, cmd_launch;
    cmd << "nasm -f elf" << __BITS__ << " " << output << " -o " << output << obj << " && " <<
           "gcc -m" << __BITS__ << " " << output << obj << " -o " << output << exe;

    std::wcout << "Assemling and linking IL...";

    if (SKIP_COMPILATION)
        return std::wcout << L"Skipped." << std::endl, 0;
    else if (system(cmd.str().c_str()))
        ERR(err_t::POST_PROCESSING_FAILED);

    std::wcout << L"Done." << std::endl <<
                  L"Launching " << output << exe << L"...";

    cmd_launch << "./" << output << exe;
    if (SKIP_LAUNCH)
        std::wcout << L"Skipped.";
    else
    {
        std::wcout << L": " << std::endl;
        system(cmd_launch.str().c_str());
    }

    std::wcout << std::endl << L"Done." << std::endl;
    return 0;
}

