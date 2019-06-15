include 'c8.asm'
org 0x200

; global a = 1
; global b = 2
; global c
; c = a + b
; draw( 0, 0, c )

jmp	_code

_global:
	.a: db	0x01
	.b: db	0x02
	.c: db	0

_code:
	; clear screen
	cls
	
	; EAdd (
	; v1 = a
	mov I, _global.a
	regl v0
	mov vE, v0

	; v1 = b
	mov I, _global.b
	regl v0
	mov vD, v0

	; v0 += v1
	add vE, vd
	mov v0, ve
	; ) EAdd
	
	; EAsg (
	; c = v0
	mov I, _global.c
	regd v0
	; ) EAsg

	; draw( 0, 0, c )
	mov I, _global.c
	regl v0

	hex v0

	mov v0, 0
	mov v1, 0
	drw v0, v1, 5

_halt:
	jmp	_halt

_stack:

