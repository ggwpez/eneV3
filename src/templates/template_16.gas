.global main
.intel_syntax noprefix

.text
boolNot:        #0 -> 0xffffffff.   1, 830, 555 -> 0. implicit normalization.
test  ax,  ax
jnz .zero
xor  ax,  ax
not  ax
ret
.zero:
xor  ax,  ax
ret

boolNormalize:  #0 -> 0.            1, 830, 555 -> 0xffffffff.
test  ax,  ax
jnz .notzero
xor  ax,  ax
ret
.notzero:
xor  ax,  ax
not  ax
ret

get_eip:
    mov eax, [esp]
    ret
