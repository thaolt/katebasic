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

r0? equ [:0x10:]
r1? equ [:0x11:]
r2? equ [:0x12:]
r3? equ [:0x13:]
r4? equ [:0x14:]
r5? equ [:0x15:]
r6? equ [:0x16:]
r7? equ [:0x17:]

i? = 0
sp? = 0
pc? = 0x200

dtm? = 0
stm? = 0

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

macro se? dst, src
	match [:x:], dst
		if (x < 0x10)
			match [:y:], src ;se vx, vy
				if y < 0x10
					dw ((0x5000 or (x shl 8)) or (y shl 4)) bswap 2
				else
					error "invalid register"
				end if
			else match nn, src ;se vx, nn
				if nn < 0xFF
					dw ((0x3000 or (x shl 8)) or nn) bswap 2
				else
					error "invalid instruction, out of range"
				end if
			end match
		else
			error "invalid instruction"
		end if
	else
		error "invalid instruction"
	end match
end macro

macro sne? dst, src
	match [:x:], dst
		if (x < 0x10)
			match [:y:], src  ;sne vx, vy
				if y < 0x10
					dw ((0x9000 or (x shl 8)) or (y shl 4)) bswap 2
				else
					error "invalid register"
				end if
			else match nn, src
				if nn < 0x100 ;sne vx, nn
					dw ((0x4000 or (x shl 8)) or nn) bswap 2
				else
					error "invalid instruction, out of range"
				end if
			end match
		else
			error "invalid instruction"
		end if
	else
		error "invalid instruction"
	end match
end macro


macro mov? dst, src
	match [:rd:], dst
		if rd < 0x10
			match [:rs:], src  ;mov vx, vy
				if rs < 0x10
					dw ((0x8000 or (rd shl 8)) or rs shl 4) bswap 2
				else
					error "invalid instruction"
				end if
			else match =dtm?, src ; mov vx, dtm
				dw (0xF007 or (rd shl 8)) bswap 2
				reg.dst = dtm
			else match nn, src ; mov vx, nn
				if nn < 0x100
					dw ((0x6000 or (rd shl 8)) or nn) bswap 2
					reg.dst = nn
				else
					error "invalid instruction"
				end if
			end match
		else
			error "invalid instruction"
		end if
	else match =i?, dst ; mov i, nnn
		if (src < 0x1000)
			dw (0xA000 or src) bswap 2
			i = src
		else
			error "invalid instruction"
		end if
	else match =dtm?, dst ; mov dtm, vx
		match [:rs:], src
			if rs < 0x10
				dw (0xF015 or (rs shl 8)) bswap 2
				dtm = reg.src
			else
				error "invalid instruction"
			end if
		else
			error "invalid instruction"
		end match
	else match =stm?, dst  ; mov stm, vx
		match [:rs:], src
			if rs < 0x10
				dw (0xF018 or (rs shl 8)) bswap 2
				stm = reg.src
			else
				error "invalid instruction"
			end if
		else
			error "invalid instruction"
		end match
	else 
		error "invalid instruction"
	end match
end macro ;mov


macro add? dst, src
	match [:rx:], dst
		if rx < 0x10 
			match [:ry:], src ;add vx, vy
				if ry < 0x10
					dw ((0x8004 or (rx shl 8)) or (ry shl 4)) bswap 2
				else
					error "invalid register"
				end if
			else ;add vx, nn
				if src < 0x100
					dw ((0x7000 or (rx shl 8)) or src) bswap 2
				else
					error "out of range"
				end if
			end match
		else
			error "invalid dest"
		end if
	else match =i?, dst ;add i, vx
		match [:rs:], src
			if rs < 0x10
				dw (0xF01E or (rs shl 8)) bswap 2
			else 
				error "invalid instruction"
			end if
		else
			error "invalid instruction"
		end match
	else
		error "invalid instruction"
	end match
end macro ;add

macro sub? rx, ry
	match [:x:], rx
		if (x < 0x10)
			match [:y:], ry
				if y < 0x10
					dw ((0x8005 or (x shl 8)) or (y shl 4)) bswap 2
				else
					error "invalid register"
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

macro rsub? rx, ry
	match [:x:], rx
		if (x < 0x10)
			match [:y:], ry
				if y < 0x10
					dw ((0x8007 or (x shl 8)) or (y shl 4)) bswap 2
				else
					error "invalid register"
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

macro call? nnn
	if nnn < 0xFFF
		db (0x2000 or nnn) bswap 2
	else
		error "invalid instruction"
	end if
end macro

macro or? rx, ry
	match [:x:], rx
		if (x < 0x10)
			match [:y:], ry
				if y < 0x10
					dw ((0x8001 or (x shl 8)) or (y shl 4)) bswap 2
				else
					error "invalid register"
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

macro and? rx, ry
	match [:x:], rx
		if (x < 0x10)
			match [:y:], ry
				if y < 0x10
					dw ((0x8002 or (x shl 8)) or (y shl 4)) bswap 2
				else
					error "invalid register"
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

macro xor? rx, ry
	match [:x:], rx
		if (x < 0x10)
			match [:y:], ry
				if y < 0x10
					dw ((0x8003 or (x shl 8)) or (y shl 4)) bswap 2
				else
					error "invalid register"
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

macro shr? rx, ry
	match [:x:], rx
		if (x < 0x10)
			match [:y:], ry
				if y < 0x10
					dw ((0x8006 or (x shl 8)) or (y shl 4)) bswap 2
				else
					error "invalid register"
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

macro shl? rx, ry
	match [:x:], rx
		if (x < 0x10)
			match [:y:], ry
				if y < 0x10
					dw ((0x800E or (x shl 8)) or (y shl 4)) bswap 2
				else
					error "invalid register"
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

macro skp? rx
	match [:x:], rx ; skp vx
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
	match [:x:], rx ; sknp vx
		if x < 0x10
			dw (0xE0A1 or (x shl 8)) bswap 2
		else
			error "invalid instruction"	
		end if
	else
		error "invalid instruction"
	end match
end macro

macro wkp? rx
	match [:x:], rx ; wkp vx
		if x < 0x10
			dw (0xF00A or (x shl 8)) bswap 2
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

macro scd? n
	if n < 0x10
		dw (0x00C0 or n) bswap 2
	else
		error "scd n: n must <= 0xF"
	end if
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



macro hires?
	dw 0x00FF bswap 2
end macro

macro lores?
	dw 0x00FE bswap 2
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

macro regl? rx
	match [:x:], rx
		if x < 0x10
			dw (0xF065 or (x shl 8)) bswap 2
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

macro flgl? rx
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

