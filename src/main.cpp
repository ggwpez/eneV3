#include <stdio.h>

#include "compiler.h"

int main(void)
{
    const char* output = "output.asm";
    const char* obj = ".o";
    const char* exe = ".exec";

    compiler cmp;
    cmp.compile("../ene/input.ene");

    std::wcout << "Writing output to file...";
    cmp.write_wstr(output);
    std::wcout << "Done." << std::endl;

    std::stringstream cmd;
    cmd << "nasm -felf32 " << output << " -o " << output << obj << " && " <<
           "gcc -m32 " << output << obj << " -o " << output << exe;

    std::wcout << "Assemling and linking IL...";

    system(cmd.str().c_str());

    std::wcout << L"Done." << std::endl <<
                  output << exe << " ready." << std::endl;

    return 0;
}

