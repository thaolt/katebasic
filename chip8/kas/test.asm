include 'c8.asm'
; Companion Cube Sample program. Disregard its advice.

; Assemble the program to a CHIP-8 binary like so:
;    $ ./c8asm -o GAMES/CUBE8.ch8 examples/cube.asm
; Once you've assembled it, you can run it in the interpreter like so:
;    $ ./chip8 -f FFB6F8 -b B2B2B2 GAMES/CUBE.ch8

; Define some more human-friendly names for the registers
; define v0 V0 ; Sprite X,Y position
;define v1 V1
;define v2 V2 ; Previous sprite X,Y position
;define v3 V3
;define v4 V4 ; Sprite direction
;define v5 V5
;define ve VE

; Clear the screen
CLS
	
; Load the variables' initial values
mov v0, 1
mov v4, 1
mov v1, 10
mov I, sprite1
DRW v0, v1, 8
mov ve, 1  

; The main loop
loop:
	; Store the current position 
mov v2, v0
mov v3, v1
	
	; If the X direction is 0, go to sub1...
SE v4, 0
JMP sub1
	
	; ...otherwise add 1 to the box' position 
ADD v0, 1

	; If you reached the right edge of the screen, change direction
	SNE v0, 56
mov v4, 1
jmp draw1
sub1: 
	; subtract 1 from the box' position
SUB v0, ve
	
	; If you reached the left edge of the screen, change direction
SNE v0, 0
mov v4, 0 

; Draw the box
draw1:  
	; Load the address of the sprite's graphics into register I
mov I, sprite1
	; Erase the sprite at the old position
DRW v2, v3, 8
	; Draw the sprite at the new position
DRW v0, v1, 8

	; Return to the start of the loop
	JMP  loop

; Binary data of the sprite.
; 1s represent pixels to be drawn, 0s are blank pixels.
sprite1:
db  01111110b, 10000001b, 10100101b, 10111101b, 10111101b, 10011001b, 10000001b, 01111110b

halt