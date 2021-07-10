#include <reg51.h>
void Tx(unsigned char x);

void main()
{
	SCON=0x40;
	TR1=0;
	TMOD=0x20;
	TH1=-6;
	TL1=TH1;
	TR1=1;
	TI=0;
	while(1)
	{
		Tx('A');
	}
}

void Tx(unsigned char x)
{
	SBUF = x;
	while(TI==0);
	TI=0;
}