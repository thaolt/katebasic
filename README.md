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

# Language syntax

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
