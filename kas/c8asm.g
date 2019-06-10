namespace c8
v0 = 0
v1 = 1
v2 = 2
v3 = 3
v4 = 4
v5 = 5
v6 = 6
v7 = 7
v8 = 8
v9 = 9
va = 0xa
vb = 0xb
vc = 0xc
vd = 0xd
ve = 0xe
vf = 0xf
i = 0
sp = 0
td = 0 ; timer delay
ts = 0 ; timer sound
r0 = 0
r1 = 0
r2 = 0
r3 = 0
r4 = 0
r5 = 0
r6 = 0
r7 = 0

macro cls
    dw 0x00E0
end macro

macro ret
    dw 0x00EE
end macro

macro jmp nnn
	if nnn < 0xFFF
		dw (0x1000 or nnn)
	end if
end macro

macro bjp nnn
	if nnn < 0xFFF
		dw (0xB000 or nnn)
	end if
end macro

macro mov? dst, src
	
end macro

macro shr dst, src
	db (0x80 or dst)
	db (src shl 4) or 6
end macro

; test

shr v1, v2
shr v1, v2
shr v1, v2
shr v1, v2

end namespace