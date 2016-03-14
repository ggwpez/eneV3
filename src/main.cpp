#include <stdio.h>

#include "compiler.h"
#include "errors.hpp"

#define SKIP_COMPILATION 1

int main(void)
{
    const char* output = "output.asm";
    const char* obj = ".o";
    const char* exe = ".exec";

    compiler cmp;
    cmp.compile("../src/input.ene");

    std::wcout << "Writing output to file...";
    cmp.write_wstr(output);
    std::wcout << "Done." << std::endl;

#if (SKIP_COMPILATION == 1)
    return 0;
#endif

    std::stringstream cmd;
    cmd << "nasm -felf32 " << output << " -o " << output << obj << " && " <<
           "gcc -m32 " << output << obj << " -o " << output << exe;

    std::wcout << "Assemling and linking IL...";

    if (system(cmd.str().c_str()))
        ERR(err_t::POST_PROCESSING_FAILED);

    std::wcout << L"Done." << std::endl <<
                  output << exe << " ready." << std::endl;

    return 0;
}

