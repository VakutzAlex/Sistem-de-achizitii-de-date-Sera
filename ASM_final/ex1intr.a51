org 0000h
ljmp main
org 000Bh
cpl P2.1
reti
org 30h
main:
mov P1, #0FFh
mov IE, #10000010b
mov TMOD, #02h
mov TH0,#-92
setb TR0
back:
mov a,P0
mov P1,a
sjmp back
end