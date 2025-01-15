#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "head.h"

void SetInitSettings();
void timerkesme();

int sn = 0;
int dk = 0;
int saat = 0;

int main(void)
{
    // Sistem ayarlarýný baþlat
    SetInitSettings();

    // LCD baþlatma
    Lcd_init();

    while (1)
    {
        // Ana döngü boþ, iþlemler timer kesmesi içinde yapýlýr
    }
}

void SetInitSettings()
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); // 40 MHz saat frekansý
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); // GPIO F pinleri output

    // Timer ayarlarý
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()); // 1 saniye
    TimerIntRegister(TIMER0_BASE, TIMER_A, timerkesme); // Kesme fonksiyonu ata
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER0A);
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);
}

void lcd_git(uint8_t row, uint8_t col)
{
    unsigned char pos;
    if(row==0)
   pos = 0x00 + col;
    else if(row==1)
        pos = 0xc0 + col;
    Lcd_Komut(pos, 0);
}


void timerkesme()
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    sn++;
    if (sn == 60)
    {
        sn = 0;
        dk++;
        if (dk == 60)
        {
            dk = 0;
            saat++;
            if (saat == 24)
            {
                saat = 0;
            }
        }
    }

    // Kronometre deðerlerini LCD'ye yazdýr
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", saat, dk, sn);
    Lcd_Temizle();
    lcd_git(1, 8);
    Lcd_Koy(buffer);
}
