include 'c8.asm'
org 0x200


jmp	_start

_global:
	.cursor:	
		db	11111000b

_start:
	cls

	mov I, _global.cursor

	mov v1, 0 ; x
	mov v2, 6 ; y
	mov v3, 30 ; delay period 1/2 sec.
	mov v5, 0 ; x = 0 for draw character
	mov v6, 0 ; cursor need redraw?

	.loop:
		mov v4, 0 ; key scanner

		mov v0, dtm

		sne v0, 0
		jmp .draw_cursor

		.for:
		sknp v4
		jmp .keypress
		add v4, 1
		se v4, 16
		jmp .for

		.draw_cursor:
		sne v6, 0
		sne v0, 0
		drw v1, v2, 1
		se v6, 0
		mov v6, 0
		sne v0, 0
		mov dtm, v3
		jmp .loop

		.keypress:
		se vF, 1
		drw v1, v2, 1
		hex v4
		drw v1, v5, 5
		add v1, 5
		mov I, _global.cursor
		mov v6, 1
		jmp .draw_cursor



_halt:
	jmp	_halt





_stack:
; simulate run time
;	.sum: ; not fixed, generated at run time, calculated at compile time (a func/sub stack size < 256)
;		db 16 dup 0
;		; compiler has to keep track of local vars indexes
;		db 0 ; a
;		db 0 ; b
;		db 0 ; c


