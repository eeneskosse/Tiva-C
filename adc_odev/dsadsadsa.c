#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "lcd.h"

void Lcd_init() {
    SysCtlPeripheralEnable(lcd_port_aktif);
    GPIOPinTypeGPIOOutput(lcdnin_portu , 0xFF);
    SysCtlDelay(50000);
    GPIOPinWrite(lcdnin_portu , RS,  0x00);

    // Ba�lang�� komutlar�
    int i;
    for (i = 0; i < 3; i++) {
        GPIOPinWrite(lcdnin_portu , D4 | D5 | D6 | D7, 0x30);
        GPIOPinWrite(lcdnin_portu , E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(lcdnin_portu , E, 0x00);
        SysCtlDelay(50000);
    }

    GPIOPinWrite(lcdnin_portu , D4 | D5 | D6 | D7, 0x20);
    GPIOPinWrite(lcdnin_portu , E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(lcdnin_portu , E, 0x00);
    SysCtlDelay(50000);

    Lcd_Temizle();
    Lcd_Komut(0x28, 0); // Fonksiyon set
    Lcd_Komut(0x06, 0); // Kurs�r� sa�a kayd�r
    Lcd_Komut(0x0F, 0); // Kurs�r� a�
    Lcd_Temizle();
    Lcd_Komut(0x80, 0); // Kurs�r konumu: �lk sat�r, ilk s�tun
}

void Lcd_Temizle(void) {
    Lcd_Komut(0x01, 0); // Ekran� temizle
    SysCtlDelay(2000);  // Daha uzun bir gecikme
}

void Lcd_Koy(char* str) {
    while (*str) {
        Lcd_Komut(*str++, 1);
    }
}

void Lcd_Komut(unsigned char c, int type) {
    GPIOPinWrite(lcdnin_portu , D4 | D5 | D6 | D7, (c & 0xf0));
    GPIOPinWrite(lcdnin_portu , RS, type == 1 ? 0x01 : 0x00);
    GPIOPinWrite(lcdnin_portu , E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(lcdnin_portu , E, 0x00);
    SysCtlDelay(50000);

    GPIOPinWrite(lcdnin_portu , D4 | D5 | D6 | D7, (c & 0x0f) << 4);
    GPIOPinWrite(lcdnin_portu , RS, type == 1 ? 0x01 : 0x00);
    GPIOPinWrite(lcdnin_portu , E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(lcdnin_portu , E, 0x00);
    SysCtlDelay(50000);
}
