org 0000h
	
;LCD pins
RS equ P1.0
E equ P1.1

;Button pins
UP equ P0.4
DOWN equ P0.5
OK equ P0.6
BACK equ P0.7
SET_BTN equ P0.3
	
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

;INPUT-OUTPUT initialization
mov P2,#00h //LCD
mov P3,#0FFh //ADC

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

;LCD initialization
mov a,#38h ;set LCD 8 bit mode
acall LCD_cmd
acall delay
mov a,0Ch ;display on, cursor off
acall LCD_cmd
acall delay
mov a,#01H ;clear dispaly
acall LCD_cmd
acall delay
mov a,#80h ;first line, first position
acall LCD_cmd
acall delay

;Initial message display
;Line1
mov r0,#13;counter for dptr
mov dptr,#mesaj_init1
here_init1:
mov a,#00h
movc a,@a+dptr
mov P2,a
acall LCD_disp
acall delay
inc dptr
djnz r0,here_init1
;Line2
mov r0,#14;counter for dptr
mov dptr,#mesaj_init2
mov a,#0C0h
acall LCD_cmd
acall delay
here_init2:
mov a,#00h
movc a,@a+dptr
mov P2,a
acall LCD_disp
acall delay
inc dptr
djnz r0,here_init2


check_btn:


sjmp check_btn

;LCD command routine
LCD_cmd:
mov P2,a
clr RS ;RS=0, instruction register
setb E ;E=1, start data read/write
clr E
ret

;LCD display routine
LCD_disp:
mov P2,a
setb RS ;RS=1, data register
setb E
acall delay
clr E
ret

;delay function
delay:
clr tr0
orl tmod,#01
mov th0,#0eeh
mov tl0,#00h
setb tr0
again:
jnb tf0,again
clr tf0
clr tr0
ret

;messages section
mesaj_init1:
db 'Press any key', 0
mesaj_init2:
db 'to continue...', 0
	
end