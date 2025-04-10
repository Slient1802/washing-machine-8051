#include "Main.H"
#include "Port.H"
#include "Washer.H"

unsigned char trang_thai = 0;
unsigned char count = 0;

void function1(void);
void function2(void);
void os(void);
////////////////////	
void UsartConfiguration()
{
	  SCON = 0x50;             // Che do UART 8-bit, cho phép nhan
    TMOD |= 0x20;            // Timer 1 ? che do 8-bit t? d?ng nap lai // 32
    TH1 = 0xFD;              // Toc do baud 9600 cho 11.0592 MHz // 253
    TR1 = 1;                 // Khoi dong Timer 1
    EA = 1;  					
}
///////////////////
void change_state(){
    count++;
    switch (count) {
        case 20:
            trang_thai = 1;  
            break;
        case 30:
            trang_thai = 2;  
            count = 0;       
            break;
        default:
            break;
    }
    os();
}
///////////////////
void os(){
    switch (trang_thai) {
        case 1:
            function1();  trang_thai = 3;
            break;
        case 2:
            function2();  trang_thai = 3;
            break;
				case 3:
            break;
    }
}
////////////////////////

void txdata(char c) {
    SBUF = c;
    while (!TI);    // Ch? truyen xong
    TI = 0;         // Xóa c? truyen
} 

void txString(unsigned char *str){
	while (*str != '\0')
	{
		txdata(*str);
		++str;
	}
}
////////////////////
void txInt(int num){
	unsigned char i = 0, c[10];			// So nguyen nho hon 10 chu so
	int temp;
	temp = num; 
	if (temp != 0){
		if (temp < 0){
			txdata('-');
			temp = - temp; 
		}
		while(temp){
			c[i++] = temp%10; 
			temp /= 10;
		}	 
	while(i) txdata(c[--i] + '0');
	}
	else txdata('0');
}
////////////////
void function1(){
	txString("		;	Ham 1: ");	
	txInt(count);
}
void function2(){
	txString("		;	Ham 2: ");
	txInt(count);
}
/////////////////