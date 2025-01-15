#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/sysctl.c"
#include "driverlib/gpio.h"
#include "apisiz.h"

//rs=pin 0 da yani 0   e pin1 de yani 2 d4 pin4 de 16 d5 32 d6 64 d7 128
void Lcd_init() {

    HWREGBITW(SYSCTL_RCGCBASE + ((lcd_port_aktif & 0xff00)>>8),lcd_port_aktif & 0xff)=1;
    GPIO_PORTB_DIR_R = 0xff;
    GPIO_PORTB_DEN_R |= 0xff;
    SysCtlDelay(50000);
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (RS << 2)));
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (RS << 2))) = 0;  //HWREG(0x40005000)=0

    // Baþlangýç komutlarý
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (240 << 2))) = 0x30;
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) = 0x02; //HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) = 0x02;
    SysCtlDelay(10);
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) = 0; //HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) = 0;
    SysCtlDelay(50000);

    HWREG(lcdnin_portu  + (GPIO_O_DATA + (240 << 2))) = 0x30;
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) = 0x02; //HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) = 0x02;
    SysCtlDelay(10);
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) = 0; //HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) = 0;
    SysCtlDelay(50000);

    HWREG(lcdnin_portu  + (GPIO_O_DATA + (240 << 2))) = 0x30;
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) = 0x02;
    SysCtlDelay(10);
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) = 0;
    SysCtlDelay(50000);

    HWREG(lcdnin_portu  + (GPIO_O_DATA + (240 << 2))) = 0x20;
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) = 0x02;
    SysCtlDelay(10);
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) = 0;
    SysCtlDelay(50000);

    Lcd_Temizle();
    Lcd_Komut(0x28, 0); // Fonksiyon set
    Lcd_Komut(0x06, 0); // Kursörü saða kaydýr
    Lcd_Komut(0x0F, 0); // Kursörü aç
    Lcd_Temizle();
    Lcd_Komut(0x80, 0); // Kursör konumu: Ýlk satýr, ilk sütun
}

void Lcd_Temizle(void) {
    Lcd_Komut(0x01, 0); // Ekraný temizle
    SysCtlDelay(2000);  // Daha uzun bir gecikme
}

void Lcd_Koy(char* str) {
    while (*str) {
        Lcd_Komut(*str++, 1);
    }
}

// Komut ise 0, karakter ise 1
void Lcd_Komut(unsigned char c, int type) {
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (240 << 2))) =c & 0xf0;
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (RS << 2))) =type == 1 ? 0x01 : 0x00;
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) = 0x02;
    SysCtlDelay(10);
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) = 0;
    SysCtlDelay(50000);

    HWREG(lcdnin_portu  + (GPIO_O_DATA + (240 << 2))) =(c & 0x0f) << 4;
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (RS << 2))) =type == 1 ? 0x01 : 0x00;
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) =2;
    SysCtlDelay(10);
    HWREG(lcdnin_portu  + (GPIO_O_DATA + (E << 2))) =0;
    SysCtlDelay(50000);
}
