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

    // Floating Point Unit (FPU) etkinleþtir
    FPUEnable();
    FPULazyStackingEnable();

    // GPIOF portunu etkinleþtir (LED için)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // PF1 (kýrmýzý LED) çýkýþ olarak ayarla
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

    // Floating point iþlemleri
    float a = 3.14159;
    float b = 2.71828;
    float c = a * b; // Çarpma iþlemi
    float d = c / a; // Bölme iþlemi

    // Sonuca baðlý olarak LED'i yak/söndür
    if (d > 2.5) {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // LED yak
    } else {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); // LED söndür
    }

    while (1) {
        // LED durumu sabit tut
    }
}
