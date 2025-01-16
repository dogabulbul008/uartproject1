//headerlar
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"

#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/watchdog.h"
#include "driverlib/adc.h"
#include "driverlib/can.h"
#include "driverlib/eeprom.h"
#include "driverlib/timer.h"
#include "driverlib/pwm.h"
#include "inc/hw_hibernate.h"
#include "driverlib/hibernate.h"
#include "driverlib/i2c.h"
#include "inc/hw_i2c.h"
#include "driverlib/ssi.h"

//global degiskenler


char uartBuffer[50];          //UART üzerinden alinan verileri depolamak için kullanilan bir karakter dizisi. 50 byte
uint8_t uartCnt;             //UART'tan alinan verilerin sayisini tutan bir sayaç. 0-255 arasinda bir tamsayi tutar. 50 bunun icinde
uint8_t tmrCnt;

typedef struct LED
{
    uint8_t Cnt;
    bool Flag;           // bool degiskeni true(1) ya da false(0) oldugunu ifade eder.
}LED;

LED red;                 //led yapisinin icindeki degiskenler
LED blue;
LED green;

void timerInterrupt(void)        // timer kesmesi
{

     TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
     tmrCnt++;

     if(red.Flag)             //eger kirmiziledin bayrak 1 ise
     {
         red.Cnt++;
     }
     else if(blue.Flag)
     {
         blue.Cnt++;
     }
     else if(green.Flag)
     {
         green.Cnt++;
     }
     else
     {
         red.Cnt = 0;
         blue.Cnt = 0;
         green.Cnt = 0;
     }

}

void uartinterrupt()
{
    uint32_t ui32Status;
    uint32_t rcvData;

    ui32Status = UARTIntStatus(UART1_BASE, true); //get interrupt status , kesme durumunu kontrol eder
    UARTIntClear(UART1_BASE, ui32Status); //clear the asserted interrupts   //kesme bayragi sifirlanir
    rcvData=UARTCharGetNonBlocking(UART1_BASE);

    uartBuffer[uartCnt++]=rcvData;              //gelen veriyi uartBuffer dizisine ekler. veri geldikce sayac artar

    //1. byte'ý kontrol etmek icin
    if (uartBuffer[0] == '-'  )                //0. byte da '-' olmasi kosuluyla
    {
        if(uartBuffer[2] == '*')               //2. byte da * olmasi kosuluyla
        {
            if (uartBuffer[1] == 'A' )         // 1. byte A ise kirmizi ledi yak
            {
//                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
//                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0);
                red.Flag=true;            //eger A verisi gönderilirse kirmiziled bayragini 1 yap.

            }
            else if (uartBuffer[1] == 'B')          //1. byte B ise mavi ledi yak
            {
//                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
//                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_3, 0);
                blue.Flag=true;

            }
            else if (uartBuffer[1] == 'C')          //1. byte C ise yesil redi yak
            {
//                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);
//                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2, 0);
                green.Flag=true;

            }
            memset(uartBuffer, 0, sizeof(uartBuffer));     //uartBuffer 0 landi, sizeof operatörü uartbuffer in byte cinsinden boyutunu hesaplar.
            uartCnt=0;
        }
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);   // kosullar saglanmiyorsa tüm ledleri aöndür.
        memset(uartBuffer, 0, sizeof(uartBuffer));
        uartCnt=0;
    }


//    if (uartBuffer[0] == '-' && uartBuffer[1] == 'A' && uartBuffer[2] == '*' )
//    {
//            // Kýrmýzý LED'i yak
//            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
//            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0); // Diðer LED'leri söndür
//
//            memset(uartBuffer, 0, sizeof(uartBuffer));     //uartBuffer 0 landi
//            uartCnt=0;
//    }
//    else if (uartBuffer[0] == '-' && uartBuffer[1] == 'B' && uartBuffer[2] == '*')
//    {
//            // Mavi LED'i yak
//            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
//            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_3, 0); // Diðer LED'leri söndür
//
//            memset(uartBuffer, 0, sizeof(uartBuffer));
//            uartCnt=0;
//    }
//    else if (uartBuffer[0] == '-' && uartBuffer[1] == 'C' && uartBuffer[2] == '*')
//    {
//            // Yeþil LED'i yak
//            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);
//            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2, 0); // Diðer LED'leri söndür
//
//            memset(uartBuffer, 0, sizeof(uartBuffer));
//            uartCnt=0;
//    }
//
//    if (uartCnt >= sizeof(uartBuffer))
//    {
//            uartCnt = 0; // Buffer taþmasýný önlemek için sayacý sýfýrla
//    }

}

int main(void)
{
        SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

        //Timer uygulamasý
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
        TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);          //timer0 ý alir ve periyodik sekilde yapilandirir.
        TimerLoadSet(TIMER0_BASE, TIMER_A,SysCtlClockGet()-1 );   //-1 olmasi 1 sn gecmesi demek
        TimerIntRegister(TIMER0_BASE, TIMER_A,timerInterrupt );
        IntEnable(INT_TIMER0A);
        TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        TimerEnable(TIMER0_BASE, TIMER_A);



        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);   //portf üzerindeki pinleri kullanabiliriz     //cevre birimine hayat verir.


        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);   //PortF de pin1(kirmiziled) ve pin2(maviled) ve pin3(yesilled) aktif
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,2);

        //UART uygulamasi

        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);       //UART1 aktif
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);       //GPIOB aktif

        GPIOPinConfigure(GPIO_PB0_U1RX);                   //uart icin gerekli pini yapilandirir. PB0 alici RX(receive)
        GPIOPinConfigure(GPIO_PB1_U1TX);                   //uart icin gerekli pini yapilandirir. PB1 verici TX(transmit)
        //RX ve TX donanýmda PB0 ve PB1 e çapraz baglanir yani PB0 la TX, PB1 le RX.

        GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        UARTDisable( UART1_BASE );   //UART4'ü devre disi birakir.Bu,yapilandirma islemlerine baslamadan önce UART'ýn güvenli bir sekilde kapatilmasini saglar.
        UARTClockSourceSet( UART1_BASE, UART_CLOCK_PIOSC );                  //saat kaynagini ayarlar.
        //UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
        UARTConfigSetExpClk( UART1_BASE, 16000000, 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE );
        //baud rate =115200 , Wlen = bit uzunlugu , Stop biti , Parite(hata kontrolü) biti
        UARTIntRegister( UART1_BASE, uartinterrupt );                       //kesme islevini kaydeder
        UARTIntEnable( UART1_BASE, UART_INT_RX | UART_INT_RT );             //alim kesmelerini aktiflestirir.
        UARTEnable( UART1_BASE);                                            //uart aktiflestirir


        while(1)
        {
//            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,2);    //kirmizi led yanar
//            SysCtlDelay(13333333);
//            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);    //kirmizi led söner
//            SysCtlDelay(13333333);
//
//            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,4);    //mavi led yanar
//            SysCtlDelay(13333333);
//            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);    //mavi led söner
//            SysCtlDelay(13333333);

//            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2,6);  //iki ledi de ayni anda yakar
//            SysCtlDelay(13333333);
//            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2,0);  //iki ledi de ayni anda söndürür
//            SysCtlDelay(13333333);

//              GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2,2);     //kirmizi ledi yakti, mavi led söndü
//              SysCtlDelay(13333333);

//              GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2,4);     //mavi ledi yakti , kirmizi ledi söndürdü
//              SysCtlDelay(13333333);

//              GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3,4);      // pin3(mavi) yanar , digerleri söner
//              SysCtlDelay(13333333);
//
//            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3,15);      // 3ledi de ayni anda yakar
//            SysCtlDelay(13333333);
//            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3,0);       //3 ledi de ayni anda söndürür.


            if(red.Flag && red.Cnt>=3)         //eger kirmiziled bayragi 1 ve Counter 3 e esit olursa kirmizi ledi yak
            {
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,2);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2 | GPIO_PIN_3,0);
                red.Flag=0;                        //bayragi sifirla
                red.Cnt=0;                         //counter i sifirla
            }
            else if(blue.Flag && blue.Cnt>=6)  //eger maviled bayragi 1 ve Counter 6 e esit olursa mavi ledi yak
            {
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,4);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_3,0);
                blue.Flag=0;
                blue.Cnt=0;
            }
            else if(green.Flag && green.Cnt>=9)
            {
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,8);
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2,0);
                green.Flag=0;
                green.Cnt=0;
            }



        }

}








