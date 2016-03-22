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
quads_long:
push ebp
mov ebp, esp
sub esp, 4
lea edx, [ebp -4]
push edx
lea edx, [ebp +8]
push edx
pop  eax
mov  eax, dword [eax]
push eax
lea edx, [ebp +8]
push edx
pop  eax
mov  eax, dword [eax]
push eax
pop  eax
pop  ecx
mul ecx
push eax
pop  eax
pop  ecx
mov dword [ecx], eax
lea edx, [ebp -4]
push edx
pop  eax
mov  eax, dword [eax]
push eax
pop  eax
jmp .end
quads_long.end:
add esp, 4
mov esp, ebp
pop ebp
ret 
main:
push ebp
mov ebp, esp
sub esp, 8
lea edx, [ebp -4]
push edx
push 0
pop  eax
pop  ecx
mov dword [ecx], eax
__while_1.start:
lea edx, [ebp -4]
push edx
pop  eax
mov  eax, dword [eax]
push eax
push 11
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
pop  eax
test eax, -1
jnz __while_1.code
jmp __while_1.end
__while_1.code:
lea edx, [ebp -8]
push edx
lea edx, [ebp -4]
push edx
pop  eax
mov  eax, dword [eax]
push eax
call quads_long
add esp, 4
push eax
pop  eax
pop  ecx
mov dword [ecx], eax
lea edx, [ebp -8]
push edx
pop  eax
mov  eax, dword [eax]
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
lea edx, [ebp -4]
push edx
lea edx, [ebp -4]
push edx
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
jmp __while_1.start
__while_1.end:
main.end:
add esp, 8
mov esp, ebp
pop ebp
ret 
section .data
__str_1: db "x = %d  x^2 = %d", 0
