include 'c8.asm'
org 0x200

;
; sub sum
; 	a = 1
;   b = 2
;   let c
;  	c = a + b
;   draw( 0, 0, c )
; endsub
;
; cls()
; gosub sum
;


jmp	_start

_global:
	.dummy		db 0
	.stk_idx	db 0 ; last stack index
	.reg_dump	db 16 dup 0

_start:
	hires
	; clear screen
	cls

	mov v0, 0xB

	call _sub.sum

	hex v0

	mov v1, 6
	mov v2, 0
	drw v1, v2, 5

	mov v0, 0xA
	hex v0
	mov v1, 12
	drw v1, v2, 5

	mov v0, 0xC
	hex v0
	mov v1, 18
	drw v1, v2, 5

	mov v0, 0xA
	hex v0
	mov v1, 24
	drw v1, v2, 5

_halt:
	jmp	_halt

_sub:
	.sum:
		pusha
		; TODO: set stack size at beginning

		; a = 4
		mov v1, 0x4
		mov v0, 0
		call stack_set_var

		; b = 5
		mov v1, 0x5
		mov v0, 1
		call stack_set_var

		; c = 0
		mov v1, 0x0
		mov v0, 2
		call stack_set_var

		; vE = a
		mov v0, 0
		call stack_load_var
		mov vE, v0

		; vD = b
		mov v0, 1
		call stack_load_var
		mov vD, v0

		; vE += vD
		add vE, vD
		mov v0, vE
		; ) EAdd
		
		; EAsg (
		; c = v0
		mov v1, v0
		mov v0, 2
		call stack_set_var
		; ) EAsg

		; draw( 0, 0, c )
		mov v0, 2
		call stack_load_var

		hex v0

		mov v0, 0
		mov v1, 0
		drw v0, v1, 5
		; ) draw

		popa
	ret

macro pusha?
	call i_to_current_stack
	regd vF
end macro

macro popa?
	call i_to_current_stack
	regl vF
end macro

i_to_current_stack:
	flgd v7

	mov I, _global.stk_idx
	regl v0
	mov v1, v0

	mov v2, 1
	mov v3, 16
	
	mov I, _stack

	.loop: 
		sne v1, 0
		jmp .endloop
		add I, v3
		regl v0
		add I, v0
		sub v1, v2
		jmp .loop
	.endloop:

	flgl v7
ret

; v0 var index
i_stack_var:
	call i_to_current_stack
	add v0, 17 ; 16 regs + 1 byte stack size
	add I, v0
ret

; v0 is var position
; return value to v0
stack_load_var:
	call i_stack_var
	regl v0
ret

; v0 is var position
; v1 is for set value
stack_set_var:
	call i_stack_var
	mov v0, v1
	regd v0
ret

_stack:
; simulate run time
;	.sum: ; not fixed, generated at run time, calculated at compile time (a func/sub stack size < 256)
;		db 16 dup 0
;		; compiler has to keep track of local vars indexes
;		db 0 ; a
;		db 0 ; b
;		db 0 ; c


