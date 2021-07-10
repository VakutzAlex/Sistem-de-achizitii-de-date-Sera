org 0000h
	
mov TMOD, #20h
mov TH1, #0FAh
mov SCON, #50h
setb TR1
receptie:
clr RI
here: jnb RI, here
mov P2, SBUF
sjmp receptie

end