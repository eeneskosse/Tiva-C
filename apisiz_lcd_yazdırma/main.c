#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "apisiz.h"


int main(void) {

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    Lcd_init();
        Lcd_Koy("aaasssg");
        SysCtlDelay(3e7);
        Lcd_Temizle();
        Lcd_Koy("testo taylan");
    while(1)
    {

    }
}
