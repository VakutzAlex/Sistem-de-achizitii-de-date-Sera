org 0000h

mov TMOD, #20h
mov TH1, #-3
mov SCON, #40h
setb TR1

start:
mov dptr, #table
again:
clr a
mov a, @a+dptr

jz start
mov SBUF, a
here:
jnb TI, here
clr TR1
inc dptr
sjmp again
org 100h
table:
db "YES ", 00h	
	
end