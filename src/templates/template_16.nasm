bits 16
global main

boolNot:        ;0 -> 0xffff.   1, 830, 555 -> 0. implicit normalization.
test ax, ax
jnz .notzero
mov ax, word ~0
ret
.notzero:
mov ax, word 0
ret
boolNormalize:  ;0 -> 0.            1, 830, 555 -> 0xffff.
test ax, ax
jnz .notzero
mov ax, word 0
ret
.notzero:
mov ax, word ~0
ret
