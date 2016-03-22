bits 32
global main

boolNot:        ;0 -> 0xffffffff.   1, 830, 555 -> 0. implicit normalization.
test eax, eax
jnz zero
xor eax, eax
not eax
ret
zero:
xor eax, eax
ret

boolNormalize:  ;0 -> 0.            1, 830, 555 -> 0xffffffff.
test eax, eax
jnz notzero
xor eax, eax
ret
notzero:
xor eax, eax
not eax
ret
section .bss
test_c: resb 4
section .text
extern printf
extern putchar
assert:
push ebp
mov ebp, esp
sub esp, 0
__if_1:

lea edx, [ebp +8]
push edx
pop  eax
test eax, -1
jz __if_1.else
lea edx, [test_c]
push edx
pop  eax
mov  eax, dword [eax]
push eax
push __str_1
call printf
add esp, 8
jmp __if_1.end
__if_1.else:
lea edx, [test_c]
push edx
pop  eax
mov  eax, dword [eax]
push eax
push __str_2
call printf
add esp, 8
__if_1.end:
lea edx, [test_c]
push edx
pop  eax
push eax
push eax
pop  eax
mov  eax, dword [eax]
push eax
push 1
pop  eax
pop  ecx
add eax, ecx
push eax
pop  eax
pop  ecx
mov dword [ecx], eax
assert.end:
add esp, 0
mov esp, ebp
pop ebp
ret 
compare_sml:
push ebp
mov ebp, esp
sub esp, 0
lea edx, [ebp +8]
push edx
pop  eax
mov  eax, dword [eax]
push eax
lea edx, [ebp +12]
push edx
pop  eax
mov  eax, dword [eax]
push eax
pop  eax
pop  ecx
cmp eax, ecx
jl __sml_1.fail
jmp __sml_1.ok
__sml_1.ok:
xor eax, eax
not eax
jmp __sml_1.end
__sml_1.fail:
xor eax, eax
jmp __sml_1.end
__sml_1.end:
push eax
call assert
add esp, 4
compare_sml.end:
add esp, 0
mov esp, ebp
pop ebp
ret 
compare_grt:
push ebp
mov ebp, esp
sub esp, 0
lea edx, [ebp +8]
push edx
pop  eax
mov  eax, dword [eax]
push eax
lea edx, [ebp +12]
push edx
pop  eax
mov  eax, dword [eax]
push eax
pop  eax
pop  ecx
cmp eax, ecx
jg __grt_1.fail
jmp __grt_1.ok
__grt_1.ok:
xor eax, eax
not eax
jmp __grt_1.end
__grt_1.fail:
xor eax, eax
jmp __grt_1.end
__grt_1.end:
push eax
call assert
add esp, 4
compare_grt.end:
add esp, 0
mov esp, ebp
pop ebp
ret 
compare_equ:
push ebp
mov ebp, esp
sub esp, 0
lea edx, [ebp +8]
push edx
pop  eax
mov  eax, dword [eax]
push eax
lea edx, [ebp +12]
push edx
pop  eax
mov  eax, dword [eax]
push eax
pop  eax
pop  ecx
xor eax, ecx
call boolNormalize
push eax
pop  eax
call boolNormalize
call boolNot
push eax
call assert
add esp, 4
compare_equ.end:
add esp, 0
mov esp, ebp
pop ebp
ret 
compare_neq:
push ebp
mov ebp, esp
sub esp, 0
lea edx, [ebp +8]
push edx
pop  eax
mov  eax, dword [eax]
push eax
lea edx, [ebp +12]
push edx
pop  eax
mov  eax, dword [eax]
push eax
pop  eax
pop  ecx
xor eax, ecx
call boolNormalize
push eax
pop  eax
call boolNormalize
call boolNot
push eax
pop  eax
call boolNormalize
call boolNot
push eax
call assert
add esp, 4
compare_neq.end:
add esp, 0
mov esp, ebp
pop ebp
ret 
main:
push ebp
mov ebp, esp
sub esp, 0
lea edx, [test_c]
push edx
push 0
pop  eax
pop  ecx
mov dword [ecx], eax
push 1
push 0
call compare_sml
add esp, 8
push eax
push 11
push 10
call compare_sml
add esp, 8
push eax
push 0
push 3
pop  ecx
pop  eax
sub eax, ecx
push eax
push 0
push 5
pop  ecx
pop  eax
sub eax, ecx
push eax
call compare_sml
add esp, 8
push eax
push 0
push 9
pop  ecx
pop  eax
sub eax, ecx
push eax
push 0
push 10
pop  ecx
pop  eax
sub eax, ecx
push eax
call compare_sml
add esp, 8
push eax
push 90
push 8
pop  ecx
pop  eax
sub eax, ecx
push eax
push 9
push 9
pop  eax
pop  ecx
mul ecx
push eax
call compare_sml
add esp, 8
push eax
push 7
push 4
pop  eax
pop  ecx
mul ecx
push eax
push 1
pop  eax
pop  ecx
add eax, ecx
push eax
push 28
call compare_sml
add esp, 8
push eax
push 10
call putchar
add esp, 4
push 0
push 10
call compare_grt
add esp, 8
push eax
push 3
push 5
call compare_grt
add esp, 8
push eax
push 0
push 15
pop  ecx
pop  eax
sub eax, ecx
push eax
push 0
push 10
pop  ecx
pop  eax
sub eax, ecx
push eax
call compare_grt
add esp, 8
push eax
push 99
push 200
call compare_grt
add esp, 8
push eax
push 10
push 10
pop  eax
pop  ecx
mul ecx
push eax
push 9
push 9
pop  eax
pop  ecx
mul ecx
push eax
push 20
pop  eax
pop  ecx
add eax, ecx
push eax
call compare_grt
add esp, 8
push eax
push 2
push 2
push 2
pop  eax
pop  ecx
mul ecx
push eax
pop  eax
pop  ecx
mul ecx
push eax
push 2
push 2
push 3
pop  eax
pop  ecx
mul ecx
push eax
pop  eax
pop  ecx
mul ecx
push eax
call compare_grt
add esp, 8
push eax
push 10
call putchar
add esp, 4
push 10
push 10
call compare_equ
add esp, 8
push eax
push 0
push 8
pop  ecx
pop  eax
sub eax, ecx
push eax
push 0
push 8
pop  ecx
pop  eax
sub eax, ecx
push eax
call compare_equ
add esp, 8
push eax
push 0
push 0
call compare_equ
add esp, 8
push eax
push 55
push 55
call compare_equ
add esp, 8
push eax
push 10
push 10
pop  eax
pop  ecx
mul ecx
push eax
push 1
pop  eax
pop  ecx
add eax, ecx
push eax
push 808
push 8
pop  ecx
pop  eax
xor edx, edx
div ecx
push eax
call compare_equ
add esp, 8
push eax
push 2800
push 28
pop  ecx
pop  eax
xor edx, edx
div ecx
push eax
push 77
push 23
pop  eax
pop  ecx
add eax, ecx
push eax
call compare_equ
add esp, 8
push eax
push 10
call putchar
add esp, 4
push 1
push 0
call compare_neq
add esp, 8
push eax
push 0
push 10
call compare_neq
add esp, 8
push eax
push 0
push 9
pop  ecx
pop  eax
sub eax, ecx
push eax
push 0
push 90
pop  ecx
pop  eax
sub eax, ecx
push eax
call compare_neq
add esp, 8
push eax
push 80
push 8
call compare_neq
add esp, 8
push eax
push 44
push 55
pop  eax
pop  ecx
mul ecx
push eax
push 22
push 33
pop  eax
pop  ecx
mul ecx
push eax
call compare_neq
add esp, 8
push eax
push 3000
push 6
pop  ecx
pop  eax
xor edx, edx
div ecx
push eax
push 100
push 6
pop  eax
pop  ecx
mul ecx
push eax
call compare_neq
add esp, 8
push eax
push 10
call putchar
add esp, 4
main.end:
add esp, 0
mov esp, ebp
pop ebp
ret 
section .data
__str_1: db "%d: OK  ", 0
__str_2: db "%d: ###### ERR ######", 0
