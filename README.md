# katebasic

Originally, I called it c8basic, but wanted KateBASIC, sounds much better

# Install depedencies

```
sudo apt install bnfc flex bison
```

# Building KateBASIC

```
make
```

# Kate Assembly language

## Example

Draw user key input with blinking cursor

```asm
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
```

# KateBasic Programming Language 

## Types

Currently there is only one type: `byte`

## Comment

Line comment `//`

Block comment `/*` `*/`

## Declare variables

```
global flag = 1
global binnum as byte = %10010011
global max_height as byte = $10 // decimal 16

let a
let b as byte
let c as byte = 10
let d as byte[2] = { 50, 30 } // array declare and init
```

## Sub and Functions

```
sub dosomething
  putc(0, 0, 5, $A) // draw number 0xA at top left of the display
endsub

function sum(a, b)
  let c = a + b
return c
```

## Branching

```
if a > b then
  putc(10, 10, 1)
else
  putc(10, 10, 0)
endif
```

## Looping

```
for i = 0 to 5
  putc( 0, i * 5, i )
endfor

for i = 0 to 10 step 2
  // do something
endfor
```

For loop has not step down yet (negative step)

## Labels

```
if a > $100 then
  goto exit
endif

// do other thing

exit:
  halt
```
