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
#include "chb.h"
#include "chb_spi.h"
#include "types.h"

/**************************************************************************/
/*!
    Initialize the SPI
*/
/**************************************************************************/
void chb_spi_init()
{
   // configure the SPI slave_select, spi clk, and mosi pins as output. the miso pin
    // is cleared since its an input.
   // SPI_DIRPORT |= (1<<SPI_SSPIN) | (1<<SPI_MOSI) | (1<<SPI_SCK);
   // SPI_OUTPORT |= (1<<SPI_SSPIN);

    // set to master mode
    // set the clock freq to fck/16
  //  SPI_CTRL |= (1<<SPI_MASTER_bp) | (1<<SPI_ENABLE_bp) | (1<<SPI_PRESCALER_gp);

    // set the slave select to idle
   // SPI_SEL_DISABLE();
   
    PORTC_DIR |= 0x01; //Reset pin
  PORTC_OUT |= 0x01; //Reset pin / maak 1, Werk
  
   PORTC_DIR |= 0x10; //Sleep pin
  PORTC_OUT |= 0x10; //Sleep pin / maak 1, Werk
  
    // configure the SPI slave_select, spi clk, and mosi pins as output. the miso pin
    // is cleared since its an input.
    SPI_DIRPORT |= (1<<SPI_SSPIN) | (1<<SPI_MOSI) | (1<<SPI_SCK);
    SPI_OUTPORT |= (1<<SPI_SSPIN);

    // set to master mode
    // set the clock freq to fck/16
    SPI_CTRL |= (1<<SPI_MASTER_bp) | (1<<SPI_ENABLE_bp) | (1<<SPI_PRESCALER_gp);

    // set the slave select to idle
    SPI_SEL_DISABLE();
   
   
}

/**************************************************************************/
/*!
    This function both reads and writes data. For write operations, include data
    to be written as argument. For read ops, use dummy data as arg. Returned
    data is read byte val.
*/
/**************************************************************************/
U8 chb_xfer_byte(U8 data)
{
     SPI_DATA = data;
    while (!(SPI_STATUS & (1<<SPI_SPIF)));
    return SPI_DATA;
}

U8 spi_read_byte(U8 addr)
{
    U8 data, rd_addr;

    /* Add the register read command to the register address. */
    rd_addr = addr | 0x80;

    SPI_SEL_ENABLE();

    /*Send Register address and read register content.*/
    data = chb_xfer_byte(rd_addr);
    data = chb_xfer_byte(data);

    SPI_SEL_DISABLE();

    return data;
}

void spi_write_byte(U8 addr, U8 val)
{
    U8 wr_addr, dummy;

    /* Add the Register Write command to the address. */
    wr_addr = addr | 0xC0;

    SPI_SEL_ENABLE();

    /*Send Register address and write register content.*/
    dummy = chb_xfer_byte(wr_addr);
    dummy = chb_xfer_byte(val);

    SPI_SEL_DISABLE();
}


//uint8_t SPI_MasterTransceiveByte(SPI_Master_t *spi, uint8_t TXdata)
//{
	/* Send pattern. */
//	spi->module->DATA = TXdata;

	/* Wait for transmission complete. */
//	while(!(spi->module->STATUS & SPI_IF_bm)) {

//	}
	/* Read received data. */
//	uint8_t result = spi->module->DATA;

//	return(result);
//}