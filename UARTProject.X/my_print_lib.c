/*
 * File:   my_print_lib.c
 * Author: ettor
 *
 * Created on 3 novembre 2022, 11.15
 */

#include "xc.h"
#include "my_print_lib.h"
#include <p30F4011.h>

void displayText(char text[])
{
    init_SPI();
    
    SPI1BUF = 0x80;
    
    for(int i = 0; text[i] != '\0'; i++)
    {
        
        if (i % 16 == 0 && i > 15)
        {
            
            if ((int)(i>>4) % 2 == 0)
            {
                while (SPI1STATbits.SPITBF == 1); // wait until not full
                SPI1BUF = 0x80;
            }

            if ((int)(i>>4) % 2 != 0)
            {
                while (SPI1STATbits.SPITBF == 1); // wait until not full
                SPI1BUF = 0xC0;
            }
        }
     
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = text[i];
    }
}

void init_SPI()
{
    SPI1CONbits.MSTEN = 1; // master mode 
    SPI1CONbits.MODE16 = 0; // 8 bit mode 
    SPI1CONbits.PPRE = 3; // primary prescaler 
    SPI1CONbits.SPRE = 6; // secondary prescaler 
    SPI1STATbits.SPIEN = 1; // enable SPI
}

void clearFirstRow()
{
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x80;
    for(int i = 0; i<16 ; i++)
    {
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = ' ';
    }
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x80;
}

void clearSecondRow()
{
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0xC0;
    for (int i = 0; i < 16; i++)
    {
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = ' ';
    }
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0xC0;
}

void update_second_line(int num)
{    
    while (SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = 0xCB;
    
    // fill the string for the second row
    char str[5];
    sprintf(str, "%d", num);
    
    for(int i = 0; str[i] != '\0'; i++)
    {
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = str[i];
    }
}

void refresh_second_line(void)
{
    while (SPI1STATbits.SPITBF == 1); // wait until not full
    SPI1BUF = 0xC0;
    
    char str[] = "Char Recv: 0";
    
    for(int i = 0; str[i] != '\0'; i++)
    {
        while (SPI1STATbits.SPITBF == 1); // wait until not full
        SPI1BUF = str[i];
    }
}