org 0000h
	
;LCD pins
RS equ P1.0
E equ P1.1

;Button pins
UP equ P0.4
DOWN equ P0.5
OK equ P0.6
BACK equ P0.7
	
;LED pins
D_UP equ P1.4
D_DOWN equ P1.5
D_OK equ P1.6
D_BACK equ P1.7
	
;I2C pins
SDA equ P1.2
SCL equ P1.3
	
;ADC pins
EOC equ P0.0
OE equ P0.1
START equ P0.2
	
;Params
;ball_res equ #5000h
;max_adc equ #256h
;beta equ #3965h
;room_temp equ #298.15h
;res_room_temp equ #5000h	
;LCD_init()
mov a,#38h ;LCD-8bit mode
acall LCD_cmd
acall delay
mov a,#0ch ;display on, cursor off
acall LCD_cmd
acall delay
mov a,#01h ;clear LCD
acall LCD_cmd
acall delay
mov a,#80h ;cursor at line 1
acall LCD_cmd
acall delay

;INPUT-OUTPUT initialization
mov P2,#00h //lcd
mov P3,#0FFh //adc

setb UP ;UP=1
setb DOWN ;DOWN=1
setb OK ;OK=1
setb BACK ;BACK=1

clr D_UP ;D_UP=0
clr D_DOWN ;D_DOWN=0
clr D_OK ;D_OK=0
clr D_BACK ;D_BACK=0

setb EOC ;EOC=1
setb OE ;OE=1
setb START ;START=1

mov r0,#6;counter for dptr
mov dptr,#mesaj1
here1:
mov a,#00h
movc a,@a+dptr
mov p2,a
acall LCD_disp
acall delay
inc dptr
djnz r0,here1

;LCD_cmd()
LCD_cmd:
mov P2,a
setb RS ;RS=1
setb E; E=1
acall delay
clr e ;E=0
ret

;LCD_disp()
LCD_disp:
mov P2,a
setb RS ;RS=1
setb E ;E=1
acall delay
clr E
ret

;delay()
delay:
clr tr0
orl TMOD,#01
mov TH0, #0eeh
mov TL0, #00h
setb TR0
again:
jnb TF0, again
clr TF0
clr TR0
ret

;I2C_init()
I2c_init:
setb SDA ;SDA=1
setb SCL ;SCL=1
ret

rstart:
clr SCL ;SCL=0
setb SDA ;SDA=1
setb SCL ;SCL=1
clr SDA ;SDA=0
ret

I2C_start:
setb SCL ;SCL=1
clr SDA ;SDA=0
CLR SCL ;SCL=0
ret

I2C_stop:
clr SCL ;SCL=0
clr SDA ;SDA=1
setb SCL ;SCL=1
setb SDA ;SDA=1
ret

I2C_sendByte:
mov r7,#08h
back1:
clr SCL
rlc a
mov SDA,c
setb SCL
djnz r7, back1
clr SCL
setb SDA
setb SCL
mov c, SDA
clr SCL
ret

I2C_ack:
clr SDA
setb SCL
clr SCL
setb SDA
ret

I2C_nack:
setb SDA
setb SCL
clr SCL
setb SCL

I2C_readByte:
mov r7,#08h
back2:
clr SCL
setb SCL
mov c, SDA
rlc a
djnz r7, back2
clr SCL
setb SDA
ret

;message
mesaj1:
db 'Temperature';


end