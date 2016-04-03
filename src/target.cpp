#include "target.h"

target_t::target_t(size_t bits,  schar* int_t, schar* string_t,
         std::array<std::array<const schar*, 4>, 4> const& regs,
         std::array<const schar*, 4> const& s_strs,
         std::array<const value_type, (int)value_type::size> const& types,
         as assembler)
    : bits(bits), bytes(bits >> 3), int_t(int_t), string_t(string_t), regs(regs), s_strs(s_strs), types(types), assembler(assembler)
{

}

target_t target_nasm_64 = target_t(64, L"i64", L"u8°",
                           {{{ L"rax", L"rbx", L"rcx", L"rdx" },
                             { L"eax", L"ebx", L"ecx", L"edx" },
                             { L" ax", L" bx", L" cx", L" dx" },
                             { L" al", L" bl", L" cl", L" dl" }}},
                            {{ L"qword", L"dword", L"word", L"byte" }},
                            {{ value_type::I8, value_type::U8,
                               value_type::I16, value_type::U16,
                               value_type::I32, value_type::U32,
                               value_type::I64, value_type::U64,
                               value_type::F32, value_type::F64 }},
                               as::NASM );

target_t target_nasm_32 = target_t(32, L"i32", L"u8°",
                           {{{ L"eax", L"ebx", L"ecx", L"edx" },
                             { L"eax", L"ebx", L"ecx", L"edx" },
                             { L" ax", L" bx", L" cx", L" dx" },
                             { L" al", L" bl", L" cl", L" dl" }}},
                            {{ L"dword", L"dword", L"word", L"byte" }},
                            {{ value_type::I8, value_type::U8,
                               value_type::I16, value_type::U16,
                               value_type::I32, value_type::U32,
                               value_type::size, value_type::size,
                               value_type::F32, value_type::F64 }},
                               as::NASM );

target_t target_nasm_16 = target_t(16, L"i16", L"u8°",
                           {{{ L" ax", L" bx", L" cx", L" dx" },
                             { L" ax", L" bx", L" cx", L" dx" },
                             { L" ax", L" bx", L" cx", L" dx" },
                             { L" al", L" bl", L" cl", L" dl" }}},
                            {{ L" word", L" word", L"word", L"byte" }},
                            {{ value_type::I8, value_type::U8,
                               value_type::I16, value_type::U16,
                               value_type::size, value_type::size,
                               value_type::size, value_type::size,
                               value_type::F32, value_type::F64 }},
                               as::NASM );

target_t target_gas_64 = target_t(64,  L"i64", L"u8°",
                           {{{ L"rax", L"rbx", L"rcx", L"rdx" },
                             { L"eax", L"ebx", L"ecx", L"edx" },
                             { L" ax", L" bx", L" cx", L" dx" },
                             { L" al", L" bl", L" cl", L" dl" }}},
                            {{ L"qword", L"dword", L"word", L"byte" }},
                            {{ value_type::I8, value_type::U8,
                               value_type::I16, value_type::U16,
                               value_type::I32, value_type::U32,
                               value_type::I64, value_type::U64,
                               value_type::F32, value_type::F64 }},
                               as::GAS );

target_t target_gas_32 = target_t(32,  L"i32", L"u8°",
                           {{{ L"eax", L"ebx", L"ecx", L"edx" },
                             { L"eax", L"ebx", L"ecx", L"edx" },
                             { L" ax", L" bx", L" cx", L" dx" },
                             { L" al", L" bl", L" cl", L" dl" }}},
                            {{ L"dword", L"dword", L"word", L"byte" }},
                            {{ value_type::I8, value_type::U8,
                               value_type::I16, value_type::U16,
                               value_type::I32, value_type::U32,
                               value_type::size, value_type::size,
                               value_type::F32, value_type::F64 }},
                               as::GAS );

target_t target_gas_16 = target_t(16,  L"i16", L"u8°",
                           {{{ L" ax", L" bx", L" cx", L" dx" },
                             { L" ax", L" bx", L" cx", L" dx" },
                             { L" ax", L" bx", L" cx", L" dx" },
                             { L" al", L" bl", L" cl", L" dl" }}},
                            {{ L" word", L" word", L"word", L"byte" }},
                            {{ value_type::I8, value_type::U8,
                               value_type::I16, value_type::U16,
                               value_type::size, value_type::size,
                               value_type::size, value_type::size,
                               value_type::F32, value_type::F64 }},
                               as::GAS );

target_t* targets[] = { &target_nasm_16, &target_nasm_32, &target_nasm_64, &target_gas_16, &target_gas_32, &target_gas_64 };
target_t* target = nullptr;
