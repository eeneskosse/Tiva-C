#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "lcd.h"

void UART_Init(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}

char UART_ReadChar(void) {
    while (UARTCharsAvail(UART0_BASE) == false); // Veri gelene kadar bekle
    return UARTCharGet(UART0_BASE);
}

void UART_ReadString(char* buffer, int maxLength) {
    int i = 0;
    while (i < maxLength - 1) {
        char c = UART_ReadChar();
        if (c == '\n') break; // Satýr sonu karakteri
        buffer[i++] = c;
    }
    buffer[i] = '\0'; // Null-terminate
}

void ADC_Init(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
}

uint32_t ADC_Read(void) {
    uint32_t value;
    ADCProcessorTrigger(ADC0_BASE, 3);
    while (!ADCIntStatus(ADC0_BASE, 3, false));
    ADCIntClear(ADC0_BASE, 3);
    ADCSequenceDataGet(ADC0_BASE, 3, &value);
    return value;
}

int main(void) {
    char buffer[16];
    char timeBuffer[16];
    char voltageBuffer[16];
    uint32_t adcValue;
    float voltage;

    // Sistem ve UART baþlatma
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    Lcd_init();
    UART_Init();
    ADC_Init();

    while (1) {
        UART_ReadString(buffer, sizeof(buffer));
        if (buffer[0] == '#') {
            // Saat verisi
            snprintf(timeBuffer, sizeof(timeBuffer), "%s", buffer + 1);
            Lcd_Temizle();
            Lcd_Koy("Saat:");
            Lcd_Koy(timeBuffer);
        } else if (buffer[0] == '*') {
            // ADC'den gelen voltaj
            adcValue = ADC_Read();
            voltage = (adcValue * 3.3) / 4096.0;
            snprintf(voltageBuffer, sizeof(voltageBuffer), "ADC: %.2fV", voltage);
            Lcd_Temizle();
            Lcd_Koy(voltageBuffer);
        }
    }
}
