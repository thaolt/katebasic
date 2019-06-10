include 'c8asm.inc'

; test

start: 
cls
jmp 0x234
bjp 0x567
shr v1, v2
mov va, 0x88
mov va, vb
mov va, dtm
mov i, 0xabc
add vb, 2
add i, v9
drw v1, v2, 8
xdrw v5, v9
rnd va, 100

se va, vb
se vc, 0x66

sne v8, v9
sne vd, 0x99

regd vf
call print_sth

halt

print_sth:
xdrw vd, ve
ret

