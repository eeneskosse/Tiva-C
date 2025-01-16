#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/hibernate.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

int main(void) {
    // Sistem saatini 50 MHz'e ayarla
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // GPIOF portunu etkinleþtir (LED ve buton için)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // PF1 (kýrmýzý LED) çýkýþ olarak ayarla
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

    // PF4 (buton) giriþ olarak ayarla ve pull-up direnç aktif et
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Hibernation modülünü etkinleþtir
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
    HibernateEnableExpClk(SysCtlClockGet());

    // Hibernation uyanma koþulunu PF4 (buton) ile yapýlandýr
    HibernateGPIORetentionEnable();
    HibernateWakeSet(HIBERNATE_WAKE_PIN);

    // LED'i bir kez yak ve bekle
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    SysCtlDelay(SysCtlClockGet() / 3); // 1 saniye bekle
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);

    // Uyku moduna geç
    HibernateRequest();

    while (1) {
        // Eðer uyandýysanýz, LED'i tekrar yakýn
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    }
}
