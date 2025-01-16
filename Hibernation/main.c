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

    // GPIOF portunu etkinle�tir (LED ve buton i�in)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // PF1 (k�rm�z� LED) ��k�� olarak ayarla
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

    // PF4 (buton) giri� olarak ayarla ve pull-up diren� aktif et
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Hibernation mod�l�n� etkinle�tir
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
    HibernateEnableExpClk(SysCtlClockGet());

    // Hibernation uyanma ko�ulunu PF4 (buton) ile yap�land�r
    HibernateGPIORetentionEnable();
    HibernateWakeSet(HIBERNATE_WAKE_PIN);

    // LED'i bir kez yak ve bekle
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    SysCtlDelay(SysCtlClockGet() / 3); // 1 saniye bekle
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);

    // Uyku moduna ge�
    HibernateRequest();

    while (1) {
        // E�er uyand�ysan�z, LED'i tekrar yak�n
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    }
}
