#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "inc/tm4c123gh6pm.h"
#include "lcd.h"

// Fonksiyon Prototipleri
void UART0_Init(void);
void ADC0_Init(void);
void UART0_WriteChar(char c);
void UART0_WriteString(const char* str);
char UART0_ReadChar(void);
void delayMs(int n);

float ADC_to_Voltage(uint32_t adcValue);

int main(void) {
    uint32_t adcValue;
    float voltage;
    char lcdBuffer[20];  // LCD i�in yaz� buffer

    // Donan�m ba�latma
    UART0_Init();
    ADC0_Init();
    Lcd_init();

    while (1) {
        // ADC D�n���m Ba�lat
        ADC0_PSSI_R = 0x0008;
        while ((ADC0_RIS_R & 0x08) == 0);

        // ADC de�eri ve voltaj hesaplama
        adcValue = ADC0_SSFIFO3_R & 0xFFF;
        voltage = ADC_to_Voltage(adcValue);

        // LCD'ye yazd�rma


        int voltInt = (int)voltage;                    // Voltaj�n tam say� k�sm�
        int voltFrac = (int)((voltage - voltInt) * 1000);  // Voltaj�n ondal�k k�sm� (3 basamak)

        // Voltaj� string'e d�n��t�rme (manuel)
        lcdBuffer[0] = 'V';
        lcdBuffer[1] = ':';
        lcdBuffer[2] = ' ';
        lcdBuffer[3] = (voltInt / 10) + '0';         // Tam say� k�sm�n�n onlar basama��
        lcdBuffer[4] = (voltInt % 10) + '0';         // Tam say� k�sm�n�n birler basama��
        lcdBuffer[5] = '.';
        lcdBuffer[6] = (voltFrac / 100) + '0';       // Ondal�k k�sm�n y�zler basama��
        lcdBuffer[7] = ((voltFrac / 10) % 10) + '0'; // Ondal�k k�sm�n onlar basama��
        lcdBuffer[8] = (voltFrac % 10) + '0';        // Ondal�k k�sm�n birler basama��
        lcdBuffer[9] = ' ';
        lcdBuffer[10] = 'V';
        lcdBuffer[11] = '\0';                       // Null-terminator

        // LCD'ye yazd�rma
        Lcd_Temizle();
        Lcd_Koy(lcdBuffer);

        lcdBuffer[11] = '\n';
        // UART �zerinden g�nderme (SharpDevelop i�in)
        UART0_WriteString(lcdBuffer);

        // Bayra�� temizle ve gecikme ekle
        ADC0_ISC_R = 0x0008;
        delayMs(500);
    }
}

// ADC De�erini Voltaja D�n��t�rme
float ADC_to_Voltage(uint32_t adcValue) {
    return (adcValue * 3.3) / 4095.0;
}

// UART0 Ayarlar�
void UART0_Init(void) {
    SYSCTL_RCGCUART_R |= 0x01;
    SYSCTL_RCGCGPIO_R |= 0x01;

    UART0_CTL_R &= ~0x01;
    UART0_IBRD_R = 104;
    UART0_FBRD_R = 11;
    UART0_LCRH_R = 0x70;
    UART0_CTL_R |= 0x301;

    GPIO_PORTA_AFSEL_R |= 0x03;
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) | 0x00000011;
    GPIO_PORTA_DEN_R |= 0x03;
    GPIO_PORTA_AMSEL_R &= ~0x03;
}

// ADC0 Ayarlar�
void ADC0_Init(void) {
    SYSCTL_RCGCADC_R |= 0x01;
    volatile int delay = SYSCTL_RCGCADC_R;

    SYSCTL_RCGCGPIO_R |= 0x10;
    delay = SYSCTL_RCGCGPIO_R;

    GPIO_PORTE_AFSEL_R |= 0x08;
    GPIO_PORTE_DEN_R &= ~0x08;
    GPIO_PORTE_AMSEL_R |= 0x08;

    ADC0_ACTSS_R &= ~0x08;
    ADC0_EMUX_R = 0xF000;
    ADC0_SSMUX3_R = 0;
    ADC0_SSCTL3_R = 0x06;
    ADC0_ACTSS_R |= 0x08;
}

// UART0 Karakter G�nderme
void UART0_WriteChar(char c) {
    while ((UART0_FR_R & 0x20) != 0);
    UART0_DR_R = c;
}

// UART0 String G�nderme
void UART0_WriteString(const char* str) {
    while (*str) {
        UART0_WriteChar(*str);
        str++;
    }
}

// Gecikme Fonksiyonu
void delayMs(int n) {
    volatile int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 3180; j++);
}
