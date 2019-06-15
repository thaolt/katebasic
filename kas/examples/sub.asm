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
	.stkpg:		db 0
	.stkptr:	db 0

_start:
	; clear screen
	cls
	call sub_sum	

_halt:
	jmp	_halt

sub_sum:
	; pusha
	mov I, _stack ;not enough, have to find current stack size and position
	regd vF 

	; a = 4
	mov v0, 0
	mov v1, 0x4
	call stack_set_var

	; b = 5
	mov v0, 1
	mov v1, 0x5
	call stack_set_var

	; c = 0
	mov v0, 2
	mov v1, 0x0
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

	; popa
	mov I, _stack
	regl vF 
ret

; v0 var index
jmp_stack_var:
	mov I, _stack
	add v0, 16 ; pass through regs store
	add I, v0
ret

; v0 is var position
; return value to v0
stack_load_var:
	call jmp_stack_var
	regl v0
ret

; v0 is var position
; v1 is for set value
stack_set_var:
	call jmp_stack_var
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


