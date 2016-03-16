#include <stdio.h>

#include "compiler.h"
#include "errors.hpp"

#define SKIP_COMPILATION 0
#define SKIP_LAUNCH 1

int main(void)
{
    char* input = "input.ene";

    std::wcout << L"Compiling " << input;

    compiler cmp = compiler(32);
    cmp.compile(input, "output");

    std::wcout << L"Done.";

    return 0;
}

