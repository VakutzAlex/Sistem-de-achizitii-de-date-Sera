org 0000h
	
LED equ P3.4
orl TMOD, #10h

start:
mov th1, #96h
mov tl1, #6ch

cpl LED
acall delay

sjmp start

delay:
setb tr1

again:
jnb tf1, again
clr tf1
clr tr1
ret

end