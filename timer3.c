#include <reg51.h>
#define LED P3^4

void T0Delay();

int i;

void main()
{
	while(1)
	{
		for(i=0;i<2;i++)
			{
				LED=~LED;
			  T0Delay();
			}
	}
}

void T0Delay()
{
	TMOD = TMOD | 0x01;
	TL0=0x96;
	TH0=0x6C;
	TR0=1;
	while(TF0=0);
	TR0=0;
	TF0=0;
}