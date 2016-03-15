#ifndef TARGET_H
#define TARGET_H

#define __BITS__ 32
#define __BYTES__ 4

#if (__BITS__ == 32)
    #define __ALL_HIGH L"0xffffffff"
    #define __INT L"i32"
    #define eax L"eax"
    #define ebx L"ebx"
    #define ecx L"ecx"
    #define edx L"edx"
#elif (__BITS__ == 64)
    #define __ALL_HIGH L"0xffffffffffffffff"
    #define eax L"rax"
    #define ebx L"rbx"
    #define ecx L"rcx"
    #define edx L"rdx"
#else
    #error "__BITS__ supports only 32 and 64"
#endif

#endif // TARGET_H
