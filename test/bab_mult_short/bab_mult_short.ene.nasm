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

get_eip:
    mov eax, [esp]
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
add esp, 0
jmp __if_1.end
__if_1.else:
lea edx, [test_c]
push edx
pop  eax
mov  eax, dword [eax]
push eax
push __str_2
call printf
add esp, 0
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
bab_mult_short:
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
add eax, ecx
push eax
pop  eax
push eax
push eax
pop  eax
pop  ecx
mul ecx
push eax
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
pop  ecx
pop  eax
sub eax, ecx
push eax
pop  eax
push eax
push eax
pop  eax
pop  ecx
mul ecx
push eax
pop  ecx
pop  eax
sub eax, ecx
push eax
push 4
pop  ecx
pop  eax
xor edx, edx
div ecx
push eax
pop  eax
jmp .end
bab_mult_short.end:
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
push 18
push 20
call bab_mult_short
add esp, 8
push eax
push 360
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
push 10
push 10
call bab_mult_short
add esp, 8
push eax
push 100
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
push 15
push 5
call bab_mult_short
add esp, 8
push eax
push 75
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
push 45
push 89
call bab_mult_short
add esp, 8
push eax
push 4005
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
push 20
push 0
call bab_mult_short
add esp, 8
push eax
push 0
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
push 0
push 87
call bab_mult_short
add esp, 8
push eax
push 0
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
push 10
call putchar
add esp, 4
main.end:
add esp, 0
mov esp, ebp
pop ebp
ret 
section .data
__str_1: db "%02d: OK  ", 0
__str_2: db "%02d: ###### ERR ######", 0
