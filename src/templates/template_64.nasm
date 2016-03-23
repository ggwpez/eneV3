bits 64
global main

boolNot:        ;0 -> 0xffffffff.   1, 830, 555 -> 0. implicit normalization.
test rax, rax
jnz zero
xor rax, rax
not rax
ret
zero:
xor rax, rax
ret

boolNormalize:  ;0 -> 0.            1, 830, 555 -> 0xffffffff.
test rax, rax
jnz notzero
xor rax, rax
ret
notzero:
xor rax, rax
not rax
ret


get_eip:
    mov rax, [esp]
    ret

