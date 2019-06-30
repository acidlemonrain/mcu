#include <REG52.H>

#define uchar unsigned char
#define uint unsigned int

sbit Beep = P1 ^ 5;
sbit lcdEN = P2 ^ 7;
sbit lcdRS = P2 ^ 6;
sbit lcdRW = P2 ^ 5;
sbit light = P2 ^ 0;
int aa = 0;
uchar key;
uint second = 0;
uint minute = 12;
uint hour = 1;
uint day = 0;
uint month = 0;
uint year = 0;
//set mode
uint setMode = 10;
uint normalMode = 11;
uint insertMode = 12;
//keyboard pressed para
uint numberFlag = 0;
//lcd display mode
uint mode = 0;
uint displayMode = 0;
uint editModeHour = 1;
uint editModeMinute = 2;
uint editModeSecond = 3;
uchar code keyValue[] = "123456789*0#";

//alram
void alarm()
{
	if (second = 20)
	{
		Beep = !Beep;
	}
}

//delay function
void delayms(int z)
{
	int x, y;
	for (x = 0; x < z; x++)
	{
		for (y = 0; y < 110; y++)
		{
			;
		}
	}
}

//Lcd display ing...
void writeCom(uchar com) // write instruction
{
	lcdEN = 0;
	lcdRS = 0;
	P0 = com;
	delayms(5);
	lcdEN = 1;
	delayms(5);
	lcdEN = 0;
}
void writeData(uchar date) //write data
{
	lcdEN = 0;
	lcdRS = 1;
	P0 = date;
	delayms(5);
	lcdEN = 1;
	delayms(5);
	lcdEN = 0;
}
//init 1602
void init()
{
	Beep = 1;
	TMOD = 0x01;
	TH0 = (65536 - 2000) / 256;
	TL0 = (65536 - 2000) % 256;
	TR0 = 1;
	ET0 = 1;
	EA = 1;

	// dula=0;
	// wela=0;
	lcdEN = 0;
	lcdRW = 0;
	//	lcdRS = 0;
	writeCom(0x38);
	writeCom(0x0c);
	writeCom(0x06);
	writeCom(0x01);
	writeCom(0x80);
	delayms(5);
}
//displaying a number hundred
void writeSecond(uchar add, uchar dat)
{
	uchar shi, ge;

	shi = dat / 10;
	ge = dat % 10;
	writeCom(0xc0 + add);
	writeData(0x30 + shi);
	writeData(0x30 + ge);
} //displaying a number dozens
void writeFirst(uchar add, uchar dat)
{
	uchar shi, ge;

	shi = dat / 10;
	ge = dat % 10;
	writeCom(0x80 + add);
	writeData(0x30 + shi);
	writeData(0x30 + ge);
}
//display char
void writechar(uchar add, int m)
{

	writeCom(0x80 + add);
	writeData(m);
}
void writechar2(uchar add, int m)
{
	writeCom(0xc0 + add);
	writeData(m);
}
//updatetime
void updateTime()
{
	second++;
	if (second >= 60)
	{
		minute++;
		second = 0;
	}
	if (minute >= 60)
	{
		hour++;
		minute = 0;
	}
	if (hour >= 24)
	{
		hour = 0;
	}
	if (day >= 30)
	{
		month++;
		day = 0;
	}
	if (month >= 12)
	{
		year++;
		month = 0;
	}
}
void clear()
{
	writechar(0, 0x20);
	writechar(1, 0x20);
	writechar(2, 0x20);
	writechar(3, 0x20);
	writechar(4, 0x20);
	writechar(5, 0x20);
	writechar(6, 0x20);
	writechar(7, 0x20);
	writechar(8, 0x20);
	writechar(9, 0x20);
	writechar(10, 0x20);
	writechar(11, 0x20);
	writechar(12, 0x20);
	writechar2(0, 0x20);
	writechar2(1, 0x20);
	writechar2(2, 0x20);
	writechar2(3, 0x20);
	writechar2(4, 0x20);
	writechar2(5, 0x20);
	writechar2(6, 0x20);
	writechar2(7, 0x20);
	writechar2(8, 0x20);
	writechar2(9, 0x20);
	writechar2(10, 0x20);
	writechar2(11, 0x20);
	writechar2(12, 0x20);
}
//updatelcd
void updateLcd()
{

	if (mode == editModeHour)
	{
		writechar(0, 0x48);
		writechar(1, 0x4F);
		writechar(2, 0x55);
		writechar(3, 0x52);
		writechar(4, 0x3A);
		writeFirst(6, hour);
	}
	if (mode == editModeMinute)
	{
		writechar(0, 0x4D);
		writechar(1, 0x49);
		writechar(2, 0x4E);
		writechar(3, 0x55);
		writechar(4, 0x54);
		writechar(5, 0x45);
		writechar(6, 0x3A);
		writeFirst(8, minute);
	}

	if (mode == displayMode)
	{

		writeFirst(0, 20);
		writeFirst(2, year);
		writechar(4, 0x2F);
		writeFirst(5, month);
		writechar(7, 0x2F);
		writeFirst(8, day);

		writeSecond(0, hour);

		writechar2(2, 0x2D);
		writeSecond(3, minute);
		writechar2(5, 0x2D);
		writeSecond(6, second);
	}
}

//timer function
void timer0int(void) interrupt 1
{
	unsigned int timercp; //�������

	timercp++;
	TH0 = (65536 - 2000) / 256; //TH0,TL0װ�붨ʱ2mS�ĳ�ֵ
	TL0 = (65536 - 2000) % 256;
	if (timercp == 300)
	{

		light = !light;
		updateTime();
		timercp = 0;
		updateLcd();
	}
}

void delay(uint i)
{
	uint j;
	for (; i > 0; i--)
		for (j = 0; j < 333; j++)
			;
}

//keysScan api
uchar keyScan()
{
	uchar temp, key;
	P1 = 0x7F;
	temp = P1;
	temp = temp & 0x0F;
	if (temp != 0x0F)
	{
		delay(1);
		temp = P1;
		temp = temp & 0x0F;
		if (temp != 0x0F)
		{
			temp = P1;
			//			BeepInit(100);
			switch (temp)
			{
			case 0x77:
				key = 7;
				break;
			case 0x7B:
				key = 8;
				break;
			case 0x7D:
				key = 9;
				break;
			case 0x7E:
				key = 13;
			}
			while (temp != 0x0F)
			{
				temp = P1;
				temp = temp & 0x0F;
			}
			return keyValue[key - 1];
		}
	}
	P1 = 0xBF;
	temp = P1;
	temp = temp & 0x0E;
	if (temp != 0x0E)
	{
		delay(1);
		temp = P1;
		temp = temp & 0x0E;
		if (temp != 0x0E)
		{
			temp = P1;
			//			BeepInit(100);
			switch (temp)
			{
			case 0xB7:
				key = 4;
				break;
			case 0xBB:
				key = 5;
				break;
			case 0xBD:
				key = 6;
				break;
			}
			while (temp != 0x0E)
			{
				temp = P1;
				temp = temp & 0x0E;
			}
			return keyValue[key - 1];
		}
	}
	P1 = 0xDF; //�̨���yDD
	temp = P1;
	temp = temp & 0x0E; //�̨���yDD?����D��D
	if (temp != 0x0E)
	{
		delay(1);
		temp = P1;
		temp = temp & 0x0E;
		if (temp != 0x0E)
		{
			temp = P1;
			//			BeepInit(100);
			switch (temp)
			{
			case 0xD7:
				key = 1;
				break;
			case 0xDB:
				key = 2;
				break;
			case 0xDD:
				key = 3;
				break;
			}
			while (temp != 0x0E)
			{
				temp = P1;
				temp = temp & 0x0E;
			}
			return keyValue[key - 1];
		}
	}
	P1 = 0xEF; //�̨�??DD
	temp = P1;
	temp = temp & 0x0E; //�̨�??DD?����D��D
	if (temp != 0x0E)
	{
		delay(1);
		temp = P1;
		temp = temp & 0x0E;
		if (temp != 0x0E)
		{
			temp = P1;
			//			BeepInit(100);
			switch (temp)
			{
			case 0xE7:
				key = 10;
				break;
			case 0xEB:
				key = 11;
				break;
			case 0xED:
				key = 12;
				break;
			}
			while (temp != 0x0E)
			{
				temp = P1;
				temp = temp & 0x0E;
			}
			return keyValue[key - 1];
		}
	}
}

void set(int x)
{

	if (mode == editModeHour)
	{
		if (numberFlag == 1)
		{
			hour = x + hour * 10;
		}
		else
		{
			hour = x;
		}
	}

	if (mode == editModeMinute)
	{
		if (numberFlag == 1)
		{
			minute = x + minute * 10;
		}
		else
		{
			minute = x;
		}
	}
	numberFlag = 1;
}
void handleKey()
{

	if (setMode == insertMode)
	{
		//insert mode
		if (key == '0')
		{
		}
		if (key == '1')
		{
			set(1);
		}
		if (key == '2')
		{
			set(2);
		}
		if (key == '3')
		{
			set(3);
		}
		if (key == '4')
		{
			set(4);
		}
		if (key == '5')
		{
			set(5);
		}
		if (key == '6')
		{
			set(6);
		}
		if (key == '7')
		{
			set(7);
		}
		if (key == '8')
		{
			set(8);
		}
		if (key == '9')
		{
			set(9);
		}
		if (key == '#')
		{
			//quit insert mode
			numberFlag = 0;
			setMode = normalMode;
			mode = displayMode;
		}
	}
	//normol mode
	else
	{
		if (key == '1')
		{
			setMode = insertMode;
			mode = editModeHour;
			clear();
		}
		if (key == '2')
		{
			setMode = insertMode;
			mode = editModeMinute;
			clear();
		}
		if (key == '#')
		{
			//quit normal mode
			setMode = normalMode;
			mode = displayMode;
			clear();
		}
	}
}

//main
void main()
{
	init();
	updateLcd();
	while (1)
	{
		key = keyScan();
		handleKey();
	}
}