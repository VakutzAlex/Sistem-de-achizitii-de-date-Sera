org 0000h

clr TR1
mov TMOD, #20h
mov TH1,#-6h
mov TL1, TH1
mov SCON, #40h
setb TR1

TRANS:
mov SBUF, #'A'
here: jnb TI, here
clr TI
sjmp TRANS

end