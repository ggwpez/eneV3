bits 32
global main

boolNot:        ;0 -> 0xffffffff.   1, 830, 555 -> 0. implicit normalization.
test eax, eax
jnz .notzero
mov eax, dword ~0
ret
.notzero:
mov eax, dword 0
ret
boolNormalize:  ;0 -> 0.            1, 830, 555 -> 0xffffffff.
test eax, eax
jnz .notzero
mov eax, dword 0
ret
.notzero:
mov eax, dword ~0
ret
