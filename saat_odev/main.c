#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "hss.h"

void SetInitSettings();
void timerkesme();
void UART0_Init();
void UART0_Handler();

int sn = 0;
int dk = 0;
int saat = 0;

int main(void)
{
    // Sistem ayarlarýný baþlat
    SetInitSettings();

    // LCD baþlatma
    Lcd_init();

    // UART baþlatma
    UART0_Init();

    while (1)
    {
        // Ana döngüde kesme ile gelen veriler iþlenecek
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

void UART0_Init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTIntRegister(UART0_BASE, UART0_Handler);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    IntEnable(INT_UART0);
}

void UART0_Handler()
{
    char buffer[8];
    static int index = 0;
    uint32_t status = UARTIntStatus(UART0_BASE, true);

    UARTIntClear(UART0_BASE, status);

    while (UARTCharsAvail(UART0_BASE))
    {
        char c = UARTCharGetNonBlocking(UART0_BASE);
        if (c == '\n') // Satýr sonu kontrolü
        {
            buffer[index] = '\0';
            sscanf(buffer, "%02d:%02d:%02d", &saat, &dk, &sn);
            index = 0;
        }
        else if (index < sizeof(buffer) - 1)
        {
            buffer[index++] = c;
        }
    }
}

void lcd_git(uint8_t row, uint8_t col)
{
    unsigned char pos;
    if (row == 0)
        pos = 0x00 + col;
    else if (row == 1)
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

    // Zamaný LCD'ye yazdýr
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", saat, dk, sn);
    Lcd_Temizle();
    lcd_git(1, 8);
    Lcd_Koy(buffer);
}
