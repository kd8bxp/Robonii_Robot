/*******************************************************************
    Copyright (C) 2009 FreakLabs
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.

    Originally written by Christopher Wang aka Akiba.
    Please post support questions to the FreakLabs forum.

*******************************************************************/
/*!
    \file 
    \ingroup


*/
/**************************************************************************/

#ifndef CHB_SPI_H
#define CHB_SPI_H

#include <avr/io.h>
#include "chibiUsrCfg.h"

#define CHB_SPI_ENABLE()    SPI_SEL_ENABLE()
#define CHB_SPI_DISABLE()   SPI_SEL_DISABLE()


#include "types.h"
#include <avr/io.h>

#define SPI_OUTPORT     PORTC_OUT
#define SPI_DIRPORT     PORTC_DIR

#define SPI_SSPIN       4
#define SPI_SCK         7              
#define SPI_MOSI        5              
#define SPI_MISO        6              

#define SPI_CTRL        SPIC_CTRL
#define SPI_STATUS      SPIC_STATUS
#define SPI_DATA        SPIC_DATA

#define SPI_SPIF        SPI_IF_bp

#define SPI_SEL_ENABLE()    do {SPI_OUTPORT &= ~(1<<SPI_SSPIN);} while (0)
#define SPI_SEL_DISABLE()   do {SPI_OUTPORT |= (1<<SPI_SSPIN);} while (0)


#if defined(__AVR_ATmega1284P__)
    #define CHB_SPI_PORT    PORTB_OUT
    #define CHB_SPI_DDIR    DDRB
    #define CHB_SCK         7                 // PB.5 - Output: SPI Serial Clock (SCLK)
    #define CHB_MOSI        5                 // PB.3 - Output: SPI Master out - slave in (MOSI)
    #define CHB_MISO        6                 // PB.4 - Input:  SPI Master in - slave out (MISO)
    #define CHB_SPI_SELN    4                 // PB.2 - Input: The dedicated SPI CS pin needs to have internal pullup enabled if an input
#else
    /* Note: The SPI chip select pin is defined in chibiUsrCfg.h */
    #define CHB_SPI_PORT    PORTC_OUT
    #define CHB_SPI_DDIR    PORTC_DIR
    #define CHB_SCK         7                 // PB.5 - Output: SPI Serial Clock (SCLK)
    #define CHB_MOSI        5                 // PB.3 - Output: SPI Master out - slave in (MOSI)
    #define CHB_MISO        6                 // PB.4 - Input:  SPI Master in - slave out (MISO)
    #define CHB_SPI_SELN    4                 // PB.2 - Input: The dedicated SPI CS pin needs to have internal pullup enabled if an input
#endif
void chb_spi_init();
U8 chb_xfer_byte(U8 data);

#endif
