#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/fpu.h"

int main(void) {
    // Sistem saatini 50 MHz'e ayarla
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Floating Point Unit (FPU) etkinle�tir
    FPUEnable();
    FPULazyStackingEnable();

    // GPIOF portunu etkinle�tir (LED i�in)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // PF1 (k�rm�z� LED) ��k�� olarak ayarla
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

    // Floating point i�lemleri
    float a = 3.14159;
    float b = 2.71828;
    float c = a * b; // �arpma i�lemi
    float d = c / a; // B�lme i�lemi

    // Sonuca ba�l� olarak LED'i yak/s�nd�r
    if (d > 2.5) {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // LED yak
    } else {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); // LED s�nd�r
    }

    while (1) {
        // LED durumu sabit tut
    }
}
