;namespace c8

org 0x200

v0? equ [:0x00:]
v1? equ [:0x01:]
v2? equ [:0x02:]
v3? equ [:0x03:]
v4? equ [:0x04:]
v5? equ [:0x05:]
v6? equ [:0x06:]
v7? equ [:0x07:]
v8? equ [:0x08:]
v9? equ [:0x09:]
va? equ [:0x0a:]
vb? equ [:0x0b:]
vc? equ [:0x0c:]
vd? equ [:0x0d:]
ve? equ [:0x0e:]
vf? equ [:0x0f:]

i? equ [:0x10:]
sp? equ [:0x11:]

dtm? equ [:0x12:]
stm? equ [:0x13:]

r0? equ [:0x20:]
r1? equ [:0x21:]
r2? equ [:0x22:]
r3? equ [:0x23:]
r4? equ [:0x24:]
r5? equ [:0x25:]
r6? equ [:0x26:]
r7? equ [:0x27:]

macro cls?
    db 0x00E0 bswap 2
end macro

macro ret?
    dw 0x00EE bswap 2
end macro

macro jmp? nnn
	if nnn < 0xFFF
		dw (0x1000 or nnn) bswap 2
	else
		error "invalid instruction"
	end if
end macro

macro bjp? nnn
	if nnn < 0xFFF
		dw (0xB000 or nnn) bswap 2
	else
		error "invalid instruction"
	end if
end macro

macro mov? dst, src
	match [:rd:], dst
		if rd < 0x10
			match [:rs:], src
				if rs < 0x10 ; mov vx, vy
					dw ((0x8000 or (rd shl 8)) or rs shl 4) bswap 2
				else if rs = 0x12 ; mov vx, dtm
					dw (0xF007 or (rd shl 8)) bswap 2
				else
					error "invalid instruction"
				end if
			else match nn, src ; mov vx, nn
				dw ((0x6000 or (rd shl 8)) or nn) bswap 2
			end match
		else if rd = 0x12 ; mov dtm, vx
			dw (0xF015 or (rd shl 8)) bswap 2
		else if rd = 0x13 ; mov stm, vx
			dw (0xF018 or (rd shl 8)) bswap 2
		else if rd = 0x10 ; mov i, nnn
			dw (0xA000 or src) bswap 2
		else
			error "invalid instruction"
		end if
	else 
		error "invalid instruction"
	end match
end macro

macro add? dst, src
	match [:rx:], dst
		if rx < 0x10 ;add vx, nn
			dw ((0x7000 or (rx shl 8)) or src) bswap 2
		else if rx = 0x10; add i, vx
			match [:rs:], src
				if rs < 0x10
					dw (0xF01E or (rs shl 8)) bswap 2
				else 
					error "invalid instruction"
				end if
			else
				error "invalid instruction"
			end match
		end if
	else
		error "invalid instruction"
	end match
end macro

macro skp? rx
	match [:x:], rx
		if x < 0x10
			dw (0xE09E or (x shl 8)) bswap 2
		else
			error "invalid instruction"	
		end if
	else
		error "invalid instruction"
	end match
end macro

macro sknp? rx
	match [:x:], rx
		if x < 0x10
			dw (0xE0A1 or (x shl 8)) bswap 2
		else
			error "invalid instruction"	
		end if
	else
		error "invalid instruction"
	end match
end macro

macro sknp? rx
	match [:x:], rx
		if x < 0x10
			dw (0xF00A or (x shl 8)) bswap 2
		else
			error "invalid instruction"	
		end if
	else
		error "invalid instruction"
	end match
end macro

macro drw? rx, ry, n
	match [:x:], rx
		if x < 0x10
			match [:y:], ry
				if y < 0x10
					if (n < 0x10)
						dw (((0xD000 or (x shl 8)) or y shl 4) or n) bswap 2
					else
						error "invalid instruction"			
					end if
				else
					error "invalid instruction"	
				end if
			else
				error "invalid instruction"	
			end match
		else
			error "invalid instruction"	
		end if
	else
		error "invalid instruction"
	end match
end macro

macro xdrw? rx, ry
	match [:x:], rx
		if x < 0x10
			match [:y:], ry
				if y < 0x10
					dw ((0xD000 or (x shl 8)) or y shl 4) bswap 2
				else
					error "invalid instruction"	
				end if
			else
				error "invalid instruction"	
			end match
		else
			error "invalid instruction"	
		end if
	else
		error "invalid instruction"
	end match
end macro

macro shr? dst, src
	match [:r:], dst
		db (0x80 or r)
	end match
	match [:r:], src
		db (r shl 4) or 6
	end match
end macro

macro shl? dst, src
	
end macro

macro hires?
	dw 0x00FF bswap 2
end macro

macro lores?
	dw 0x00FE bswap 2
end macro

macro scr?
	dw 0x00FB bswap 2
end macro

macro scl?
	dw 0x00FC bswap 2
end macro

macro exit?
	dw 0x00FD bswap 2
end macro

macro scd? n
	if n < 0x10
		dw (0x00C0 or n) bswap 2
	else
		error "scd n: n must <= 0xF"
	end if
end macro

macro bcd? rx
	match [:x:], rx
		if x < 0x10
			dw (0xF033 or (x shl 8)) bswap 2
		else
			error "invalid instruction"
		end if
	else
		error "invalid instruction"
	end match
end macro

macro hex? rx
	match [:x:], rx
		if x < 0x10
			dw (0xF029 or (x shl 8)) bswap 2
		else
			error "invalid instruction"
		end if
	else
		error "invalid instruction"
	end match
end macro

macro ehx? rx
	match [:x:], rx
		if x < 0x10
			dw (0xF030 or (x shl 8)) bswap 2
		else
			error "invalid instruction"
		end if
	else
		error "invalid instruction"
	end match
end macro

macro regd? rx
	match [:x:], rx
		if x < 0x10
			dw (0xF055 or (x shl 8)) bswap 2
		else
			error "invalid instruction"
		end if
	else
		error "invalid instruction"
	end match
end macro

macro regr? rx
	match [:x:], rx
		if x < 0x10
			dw (0xF065 (x shl 8)) bswap 2
		else
			error "invalid instruction"
		end if
	else
		error "invalid instruction"
	end match
end macro

macro flgd? rx
	match [:x:], rx
		if x < 0x8
			dw (0xF075 or (x shl 8)) bswap 2
		else
			error "invalid instruction"
		end if
	else
		error "invalid instruction"
	end match
end macro

macro flgr? rx
	match [:x:], rx
		if x < 0x8
			dw (0xF085 or (x shl 8)) bswap 2
		else
			error "invalid instruction"
		end if
	else
		error "invalid instruction"
	end match
end macro

macro rnd? rx, nn
	match [:x:], rx
		if x < 0x10
			if nn < 0x100
				dw ((0xC000 or (x shl 8)) or nn) bswap 2
			else
				error "invalid instruction"
			end if
		else
			error "invalid instruction"
		end if
	else
		error "invalid instruction"
	end match
end macro
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
regd vf
jmp start

;end namespace