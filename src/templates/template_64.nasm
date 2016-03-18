bits 64
global main

boolNot:        ;0 -> 0xffffffffffffffff.   1, 830, 555 -> 0. implicit normalization.
test rax, rax
jnz .notzero
mov rax, qword ~0
ret
.notzero:
mov rax, qword 0
ret
boolNormalize:  ;0 -> 0.            1, 830, 555 -> 0xffffffffffffffff.
test rax, rax
jnz .notzero
mov rax, qword 0
ret
.notzero:
mov rax, qword ~0
ret
