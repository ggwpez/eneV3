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
sub esp, 8
lea edx, [ebp -4]
push edx
push 22
pop  eax
pop  ecx
mov dword [ecx], eax
lea edx, [ebp -8]
push edx
push 18
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
call bab_mult_short
add esp, 8
push eax
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
add esp, 16
main.end:
add esp, 8
mov esp, ebp
pop ebp
ret 
section .data
__str_1: db "a = %d, b = %d, a*b = %d", 0
