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

_start:
	; clear screen
	cls
	call sub_sum	

_halt:
	jmp	_halt

sub_sum:
	mov I, _stack ;not enough, have to find current stack size and position
	regd vf ; pusha

	; pass through V regs dump
	mov v0, 16
	add I, v0

	; a = 1
	mov v0, 0x05
	regd v0

	; b = 2
	mov v0, 1
	add I, v0
	mov v0, 0x05
	regd v0

	; let c
	mov v0, 1
	add I, v0
	mov v0, 0x00
	regd v0

	; vE = a
	mov I, _stack
	mov v0, 16
	add I, v0
	regl v0
	mov vE, v0

	; vD = b
	mov I, _stack
	mov v0, 17
	add I, v0
	regl v0
	mov vD, v0

	; vE += vD
	add vE, vD
	mov v0, vE
	; ) EAdd
	
	; EAsg (
	; c = v0
	mov I, _stack
	mov v0, 18
	add I, v0
	mov v0, vE
	regd v0
	; ) EAsg

	; draw( 0, 0, c )
	mov I, _stack
	mov v0, 18
	add I, v0
	regl v0

	hex v0

	mov v0, 0
	mov v1, 0
	drw v0, v1, 5

	mov I, _stack
	regl vf ; popa
	ret

_stack:
	; simulate run time
;	.sum: ; not fixed, generated at run time, calculated at compile time (a func/sub stack size < 256)
;		db 16 dup 0
;		; compiler has to keep track of local vars indexes
;		db 0 ; a
;		db 0 ; b
;		db 0 ; c


