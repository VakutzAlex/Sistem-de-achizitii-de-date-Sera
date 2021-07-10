#include <reg51.h>

unsigned char Rx();

void main(void)
{
	SCON = 0x50;
	TMOD = 0x20;
	TH1=-6;
	TR1=1;
	RI=0;
	
	while(1)
	{
		P2=Rx();
	}
}

unsigned char Rx()
{
	unsigned char rec;
	while(RI==0);
	rec=SBUF;
	RI=0;
	
	return rec;
}