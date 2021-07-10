org 0000h
	
delay1 equ 02h
	
LED equ P3.4
orl TMOD, #01h

start:
mov th0,#96h
mov tl0, #6ch
mov r0, #delay1

cpl LED
here:
acall delay
djnz r0, here

sjmp start

delay:
setb tr0

again:
jnb tf0, again
clr tf0
clr tr0
ret

end