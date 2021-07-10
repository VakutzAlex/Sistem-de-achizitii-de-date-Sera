#include <reg51.h>
#include <stdio.h>
#include <math.h>
#define LCD P2 // pin for LCD
#define ADC P3 // pin for ADC
#define SHTAddr 0x80 //Humidity sensor address 

//bool data type
typedef enum { false, true } bool; //declaring bool type
bool key_pressed=false;
bool welcome = true;
bool mainScreen=true;

//LED Pins
sbit LED_UP = P1^4;
sbit LED_DOWN = P1^5;
sbit LED_OK = P1^6;
sbit LED_BACK = P1^7;

//Button Pins
sbit UP = P0^4;
sbit DOWN = P0^5;
sbit OK = P0^6;
sbit BACK = P0^7;
sbit SET = P0^3;

//Control pins
sbit RS = P1^0;
sbit E  = P1^1;
sbit EOC = P0^0;
sbit OE = P0^1;
sbit START = P0^2;


//SCL, SDA
sbit SCL=P1^2;
sbit SDA=P1^3;

//variables
int choice;
float num=0;
float humidity=0;
char result[5];

//Set variables
float setTargetTemp;
float setTargetHum;

//Variables for readHum()
unsigned char ack1, hum[2];

//Variables for readThermistor()
const float bal_res=5000;
const float max_adc=256;
const float beta=3965;
const float room_temp=298.15;
const float res_room_temp=5000;

//LCD functions 
void delay(int);
void LCD_init(void);
void cmd(char);
void dispString(char*);
void LCD_disp(char);

//Sensor functions
void readThermistor();
void readHum();
void readHum1();

//Setting target temp/hum
void setTemp();
void setHum();

//ADC functions
void read_ADC();

//I2C functions
void I2C_init(); //initialize I2C pins
void I2C_start(); //send start bit on I2C
void I2c_restart(); //send start bit on I2C
unsigned char I2C_writeByte(unsigned char); //send data on I2C
unsigned char I2C_readByte(); //read value from I2C
void I2C_sendACK(); //send ACK bit on i2c
void I2C_sendNACK(); //send NACK bit on i2c
void I2C_stop(); //send stop bit on I2C

//Menu functions
void startScreen(); //shows options
void optionSelect(); //selects an option
void mainScreenP(); //shows selected option
void goBack(); //return to previous screen
void logOut();

//Additional function
void dispInstr();
void LED_light();
void updateT();
void updateH();

// Main function
void main(void)
{
	//INPUT-OUPUT defining section
	 UP=1; //making input
	 DOWN=1; //making input
	 BACK=1; //making input
	 SET=1; //making input
	
	 LED_UP=0; //making output
	 LED_DOWN=0; //making output
	 LED_OK=0; //making output
	 LED_BACK=0; //making output
	
	 ADC=0xFF; // making input
	 LCD=0x00; //making output
	
	 EOC=1;
	 OE=0;
	 START=0;
	
	 //Initializing the LCD
	 LCD_init();

	 //Initializing the I2C communication
	 I2C_init();
	 
	 
   while(1)
   {
		 OK=1;//Establish OK as input
		 if(key_pressed==false)
		 {dispString("Press any key");
		 cmd(0xC0);
		 dispString("to continue...");}
		 while(key_pressed==false)
		 {
			 if(UP==0 || DOWN==0 || OK==0 || BACK==0 || SET==0)
			 {
				 key_pressed=true;
				 LED_OK=1;
				 delay(100);
				 cmd(0x01);
				 dispString("Starting up...");
				 delay(400);
				 LED_OK=0;
			 }
		 }
		 
		 if(key_pressed==true)
		 {
			 if(welcome==true)
			 {
				 cmd(0x01);
				 startScreen();
				 welcome=false;
			 }
			 if(welcome==false)
			 {
				 if(mainScreen==true)
				 {
					 while(OK==1)
					 {
						 startScreen();
						 optionSelect();
						 mainScreenP();
						 if(BACK==0)
						 {
							 OK=0;
							 logOut();
						 }
						 if(SET==0)
						 {
							 cmd(0x01);
							 dispString("Preparing for");
							 cmd(0xc0);
							 dispString("setup");
							 LED_light();
							 delay(400);
							 //setTemp();
							 setHum();
							 OK=0;
							 cmd(0x01);
						 }
					 }
					 mainScreen=false;
				 }
				 
				 if(mainScreen==false)
				 {
					 if(choice==1)
					 {
						 cmd(0x01);
						 readThermistor();
						 goBack();
					 }
					 else
						 if(choice==2)
						 {
							 readHum();
							 delay(500);
							 readHum1();
							 goBack();
						 }
						 else
						 {
							 mainScreen=true;
						 }
				 }
				 
			 }
		 }
	 }
}


//LCD functions
void cmd(char t)
{
	LCD = t;
	RS=0;
	E=1;
	delay(5);
	E=0;
}

void LCD_init()
{
	cmd(0x38); //data init
	cmd(0x0C); //LCD display on and cursor off
	cmd(0x01); //clear LCD dispaly
	cmd(0x80); //positioning cursor at the first line
}

void delay(int n)
{
	int i,j;
	for(i=0;i<n;i++)
		for(j=0;j<100;j++)
			{}
}

void dispString(char *p)
{
	while(*p)
	{
		LCD_disp(*p++);
	}
}

void LCD_disp(char x)
{
	LCD=x;
	RS=1;
	E=1;
	delay(5);
	E=0;
}


//ADC functions
void read_ADC()
{
	num=0;
	START=1;
	delay(5);
	START=0;
	while(EOC==1);
	while(EOC==0);
	OE=1;
	num=ADC;
	delay(5);
	OE=0;
}


//Menu functions
void startScreen()
{
	cmd(0x80);
	dispString("1.Temperature");
	cmd(0xC0);
	dispString("2.Humidity");
}

void optionSelect()
{
	if(UP==0)
		choice=1;
	else
		if(DOWN==0)
			choice=2;
		else
			choice=choice;
}

void mainScreenP()
{
	if(choice==1)
	{
		cmd(0x80);
		dispString(">.Temperature");
		cmd(0xC0);
		dispString("2.Humidity");
	}
	else
	 if(choice==2)
	 {
		 cmd(0x80);
		 dispString("1.Temperature");
		 cmd(0xC0);
		 dispString(">.Humidity");
	 }
}

void goBack()
{
	while(BACK==1);
	cmd(0x01);
	mainScreen=true;
	key_pressed=true;
}

void logOut()
{
	cmd(0x01);
	cmd(0x80);
	dispString("System powering");
	cmd(0xC0);
	dispString("down...");
	key_pressed=false;
	choice=0;
	mainScreen=true;
	welcome=true;
	delay(500);
	cmd(0x01);
}


//I2C functions
void I2C_init()
{
	SDA=1;
	SCL=1;
}
void I2C_start()
{
	SDA=0;
	SCL=0;
}
void I2C_restart()
{
	SDA=1;
	SCL=1;
	SDA=0;
	SCL=0;
}
void I2C_stop()
{
	SDA=0;
	SCL=0;
	SDA=1;
	SCL=1;
}
void I2C_sendACK()
{
	SDA=0;
	SCL=1;
	SCL=0;
	SDA=1;
}
void I2C_sendNACK()
{
	SDA=1;
	SCL=1;
	SCL=0;
	SDA=1;
}
unsigned char I2C_writeByte(unsigned char myData)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		if((myData & 0x80)==0)
			SDA=0;
		else
			SDA=1;
		SCL=1;
		SCL=0;
		myData=myData<<1;
	}
	SDA=1;
	SCL=1;
	SCL=0;
	return SDA;
}
unsigned char I2C_readByte()
{
	unsigned char i, myData=0;
	for(i=0;i<8;i++)
	{
		SCL=1;
		if(SDA)
			myData=myData | 1;
		if(i<7)
			myData=myData<<1;
		SCL=0;
	}
	return myData;
}



//Sensor functions
void readThermistor()
{
	float rTh=0;
	float tKel=0;
	float tCel=0;
	read_ADC(); //puts the ADC value in num
	rTh=bal_res*((max_adc/num)-1);
	tKel=(beta*room_temp)/(beta+(room_temp*log(rTh/res_room_temp)));
	tCel=tKel-273.15;
	sprintf(result,"%.2f",tCel);
	cmd(0x01);
	dispString("Temperature: ");
	dispString(result);
	LCD_disp(0xDF); //ascii value of degree(°) symbol
	LCD_disp(0x43); //ascii value for C
	cmd(0xC0);
	dispString("Target: ");
	sprintf(result,"%.2f",setTargetTemp);
	dispString(result);
	LCD_disp(0xDF);
	LCD_disp(0x43);
}
void readHum()
{
		//write to slave device
		I2C_start();
		ack1=I2C_writeByte(SHTAddr);
		ack1=I2C_writeByte(0xE5);
		//ack1=I2C_writeByte();
		I2C_stop();
							 
		//read from slave device
		I2C_start();
		I2C_writeByte(SHTAddr|1); //slave address with read byte
		hum[0]=I2C_readByte();//read MSB
		I2C_sendACK();
		hum[1]=I2C_readByte();//read LSB
		I2C_sendNACK();
		I2C_stop();
	
		//hum conversion
	  humidity=(((hum[0]*256.0+hum[1])*125.0)/65536.0)-6;
}
void readHum1()
{
		//write to slave device
		I2C_start();
		ack1=I2C_writeByte(SHTAddr);
		ack1=I2C_writeByte(0xE5);
		//ack1=I2C_writeByte();
		I2C_stop();
							 
		//read from slave device
		I2C_start();
		I2C_writeByte(SHTAddr|1); //slave address with read byte
		hum[0]=I2C_readByte();//read MSB
		I2C_sendACK();
		hum[1]=I2C_readByte();//read LSB
		I2C_sendNACK();
		I2C_stop();
	
		//hum conversion
	  humidity=(((hum[0]*256.0+hum[1])*125.0)/65536.0)-6;

		cmd(0x01);
		dispString("Humidity: ");
		sprintf(result,"%.2f",humidity);
		dispString(result);
		dispString("%RH");
		cmd(0xC0);
		dispString("Target: ");
		sprintf(result,"%.2f",setTargetHum);
		dispString(result);
		dispString("%RH");
}



//Setting target temp/hum
void setTemp()
{
	dispInstr();
	cmd(0x01); //clear display
	sprintf(result,"%.2f",setTargetTemp);
	dispString("Target temperature:");
	cmd(0xC0);
	dispString(result);
	while(OK!=0)
	{
		if(UP==0)
		{
			setTargetTemp+=10;
			while(UP==0); //button debounce
			updateT();
		}
		if(DOWN==0)
		{
			setTargetTemp+=5;
			while(DOWN==0); //button debounce
			updateT();
		}
		if(BACK==0)
		{
			setTargetTemp+=1;
			while(BACK==0); //button debounce
			updateT();
		}
		if(SET==0)
		{
			setTargetTemp+=0.1;
			while(SET==0); //button debounce
			updateT();
		}
	}
	//ckeck for humidity to be within specified parameters
	if(setTargetTemp<10)
		setTargetTemp=10;
	if(setTargetTemp>45)
		setTargetTemp=45;
	
	delay(2000);
	return;
}
void setHum()
{
	dispInstr();
	cmd(0x01); //clear display
	sprintf(result,"%.2f",setTargetHum);
	dispString("Target humidity:");
	cmd(0xC0);
	dispString(result);
	while(OK!=0)
	{
		if(UP==0)
		{
			setTargetHum+=10;
			while(UP==0); //button debounce
			updateH();
		}
		if(DOWN==0)
		{
			setTargetHum+=1;
			while(DOWN==0); //button debounce
			updateH();
		}
		if(BACK==0)
		{
			setTargetHum-=10;
			while(BACK==0); //button debounce
			updateH();
		}
		if(SET==0)
		{
			setTargetHum-=1;
			while(SET==0); //button debounce
			updateH();
		}
	}
	//ckeck for temperature to be within specified parameters
	if(setTargetHum<0)
		setTargetHum=0;
	if(setTargetHum>100)
		setTargetHum=100;
	
	delay(2000);
	return;
}

//Additional function
void dispInstr()
{
	cmd(0x01); //clear display
	dispString("UP=+10  DOWN=+1");
	cmd(0xC0);
	dispString("BACK=-10 SET=-1");
	delay(2000);
}
void LED_light()
{
	LED_UP=1;
	delay(150);
	LED_UP=0;
	LED_DOWN=1;
	delay(150);
	LED_DOWN=0;
	LED_OK=1;
	delay(150);
	LED_OK=0;
	LED_BACK=1;
	delay(150);
	LED_BACK=0;
}
void updateT()
{
	cmd(0xC0);
	sprintf(result,"%.2f",setTargetTemp);
	dispString(result);
}
void updateH()
{
	cmd(0xC0);
	sprintf(result,"%.2f",setTargetHum);
	dispString(result);
}