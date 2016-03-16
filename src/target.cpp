#include "target.h"

target_t::target_t(size_t bits,  schar* int_t,
         const std::array<std::array<const schar*, 4>, 4>& regs,
         const std::array<const schar*, 4>& s_strs)
    : bits(bits), int_t(int_t), regs(regs), s_strs(s_strs)
{

}

target_t target64 = target_t(64, L"i64",
                           {{{ L"rax", L"rbx", L"rcx", L"rdx" },
                             { L"eax", L"ebx", L"ecx", L"edx" },
                             { L" ax", L" bx", L" cx", L" dx" },
                             { L" al", L" bl", L" cl", L" dl" }}},
                            {{ L"qword", L"dword", L"word", L"byte" }});

target_t target32 = target_t(32, L"i32",
                           {{{ L"eax", L"ebx", L"ecx", L"edx" },
                             { L"eax", L"ebx", L"ecx", L"edx" },
                             { L" ax", L" bx", L" cx", L" dx" },
                             { L" al", L" bl", L" cl", L" dl" }}},
                             {{ L"dword", L"dword", L"word", L"byte" }});

target_t target16 = target_t(16, L"i16",
                           {{{ L" ax", L" bx", L" cx", L" dx" },
                             { L" ax", L" bx", L" cx", L" dx" },
                             { L" ax", L" bx", L" cx", L" dx" },
                             { L" al", L" bl", L" cl", L" dl" }}},
                            {{ L" word", L" word", L"word", L"byte" }});

target_t* target = nullptr;
