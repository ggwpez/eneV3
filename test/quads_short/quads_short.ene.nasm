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
section .text
extern printf
extern putchar
quads_short:
push ebp
mov ebp, esp
sub esp, 0
lea edx, [ebp +8]
push edx
pop  eax
mov  eax, dword [eax]
push eax
pop  eax
push eax
push eax
pop  eax
pop  ecx
mul ecx
push eax
pop  eax
jmp .end
quads_short.end:
add esp, 0
mov esp, ebp
pop ebp
ret 
main:
push ebp
mov ebp, esp
sub esp, 4
lea edx, [ebp -4]
push edx
push 0
push 1
pop  ecx
pop  eax
sub eax, ecx
push eax
pop  eax
pop  ecx
mov dword [ecx], eax
__while_1.start:
lea edx, [ebp -4]
push edx
pop  eax
mov  eax, dword [eax]
push eax
push 10
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
lea edx, [ebp -4]
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
pop  eax
test eax, -1
jnz __while_1.code
jmp __while_1.end
__while_1.code:
lea edx, [ebp -4]
push edx
pop  eax
mov  eax, dword [eax]
push eax
call quads_short
add esp, 4
push eax
lea edx, [ebp -4]
push edx
pop  eax
mov  eax, dword [eax]
push eax
push __str_1
call printf
add esp, 12
push 10
call putchar
add esp, 4
jmp __while_1.start
__while_1.end:
main.end:
add esp, 4
mov esp, ebp
pop ebp
ret 
section .data
__str_1: db "x = %d  x^2 = %d", 0
