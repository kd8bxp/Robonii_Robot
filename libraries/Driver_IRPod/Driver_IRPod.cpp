#include "Driver_IRPod.h"

//#include <avr/io.h>
//#include <compat/deprecated.h>

// ====== Defines =====================================================
// TWI uses 7-bit address 0x01 for robot (0b001)
// IRPOD 7bit address == 0x02 (0b010)
#define TWI_ADDR_ROBOT 0x01
#define TWI_ADDR_IRPOD 0x02

#define TWI_CMD_MASTER_WRITE 0x00
#define TWI_CMD_MASTER_READ  0x01
#define START_TWI_TX                twiTxIndex = 0; TWIE_MASTER_ADDR = ((TWI_ADDR_IRPOD << 1) | TWI_CMD_MASTER_WRITE); twiTxDataFinalFlag = 0;

// Command codes to IRPOD
#define IRPOD_CMD_READ_DEV_INFO  0x01
#define IRPOD_CMD_TXIR           0x02
#define IRPOD_CMD_RDIR_ONLY      0x03
// Toggle clock output with specified calibration byte
#define IRPOD_CMD_TESTMODE       0x04

#define IRPOD_CMD_BM_IR_RESET    0x20
#define IRPOD_CMD_BM_IR_TXWEAK   0x10
#define IRPOD_CMD_MASK           0x0F


#ifndef PRODUCTION_TESTING 
  #define TWI_RETRANSMIT_MAX 6
#else
  #define TWI_RETRANSMIT_MAX 0
#endif


Robonii_IRPod_Driver Robonii_IRPod=Robonii_IRPod_Driver();

volatile unsigned char ir_tx_level;


volatile unsigned char twiTxDataFinal[6];
volatile unsigned char twiTxDataFinalFlag=0;

volatile unsigned char twiTxData[6];
volatile unsigned char twiTxDataRetransmit[6];
volatile unsigned char twiRetransmitCmdCount;
volatile unsigned char twiTxIndex ;
volatile unsigned char twiRetransmitBufferValid;

volatile unsigned char twiRxData[10];
volatile unsigned char twiRxIndex = 0;

volatile unsigned char twiCurrentTransaction = 0;
volatile unsigned char twiRxLimit = 0;
volatile unsigned char twiTxLimit = 0;

unsigned char ledArray[6] = {0};
unsigned char bs_command_data;

unsigned char pod_led_data1;
unsigned char pod_led_data2;

unsigned char bs_cmd_loaded = 0;


//===================================================================
Robonii_IRPod_Driver::Robonii_IRPod_Driver()
{
ledArray[0] = 0;
ledArray[1] = 0;
ledArray[2] = 0;
ledArray[3] = 0;
ledArray[4] = 0;
}



// Data variables for most recent base station command.
volatile unsigned char data1;
volatile unsigned char data2;
volatile unsigned char data3;

void Robonii_IRPod_Driver:: BaseStationSetCommand(unsigned char bs_target_status, bsCommand_e bs_cmd, unsigned var1, unsigned var2, unsigned var3, unsigned var4, unsigned var5)
{
	// Local integer data types needed to extract the undefined parameters
  // from the function prototype.
  unsigned int temp1 = 0;
  unsigned int temp2 = 0;

  // A type suitable for holding information needed by macros va_start, 
  // va_end and va_arg.
	//va_list args;
	// Invokes a makro that intializes the object declared by va_list.
	//va_start(args, bs_cmd);

  switch(bs_cmd)
	{
		case BS_CMD_INIT:

      // The init command is currently passed with 2 additional arguments:
      // 1 - A byte that indicates the new base station id after init.
      // 2 - A byte that indicates the new team status of the base station.
      temp1 = var1;
      temp2 = var2;

      data1 = 0; data2 = 0; data3 = 0;
      //Insert the new status for the target BS.
      data1 = (unsigned char) (temp1 & 0x0003) << 4;
      data1 |= (unsigned char) temp2 & 0x000F;

//TIAN: TODO-Ranging
      // Instruct ir driver to use this new id for future ranging requests.
    //  BaseStationSetRangeStatus(temp1); TIAN: What is this for????

      IrPodSendCmd(IRPOD_CMD_INIT_BS, (unsigned char) bs_target_status - 10, data1, data2, data3);

      break;
		case BS_CMD_LEDS:

      // The basic LED command will be sent with 3 additional arguments:
		  // 1 - A byte indicating which LED's are on or off. (bsLed_e)
			// 2 - A byte indicating which LED's should blink. (bsLed_e)
      // 3 - A byte indicating which tri-cols will be on/off in next state.
      // 4 - A byte indicating which tri-cols will be flashing in next state.

			// Get first two additional parameters.
      temp1 = var1;
      temp2 = var2;

      data1 = 0; data2 = 0; data3 = 0;
      
      // Indicate which LED's should be on.
      if(temp1 & 0x0001) data1 |= 0x30;
      if(temp1 & 0x0002) data1 |= 0x0C;
      if(temp1 & 0x0004) data1 |= 0x03;
      if(temp1 & 0x0008) data2 |= 0x30;
      if(temp1 & 0x0010) data2 |= 0x0C;
      // Clear bit for flashing leds. (Only slow for now.)
      if(temp2 & 0x0001) data1 &= ~0x20;
      if(temp2 & 0x0002) data1 &= ~0x08;
      if(temp2 & 0x0004) data1 &= ~0x02;
      if(temp2 & 0x0008) data2 &= ~0x20;
      if(temp2 & 0x0010) data2 &= ~0x08;

      // Get paramters 3 and 4.
      temp1 = var3;
      temp2 = var4;

      // Set LED's as on.
      if(temp1 & 0x0008) data2 |= 0x03;
      if(temp1 & 0x0001) data3 |= 0x30;
      // Clear bit for flashing leds. (Only slow for now.)
      if(temp2 & 0x0008) data2 &= ~0x02;
      if(temp2 & 0x0001) data3 &= ~0x20;

      // Get parameter 5.
      // Get the time duration till the end state.
      // Single color reset bit also included.
      temp1 = var5;
      data3 |= temp1 & 0x0F;

      IrPodSendCmd(IRPOD_CMD_SET_BS_LEDS, (unsigned char) bs_target_status - 10, data1, data2, data3);

			break;

		default:
			break;
	}

  //va_end(args);

  //IrPodSendCmd(bs_cmd, (unsigned char) bs_target_status - 10, data1, data2, data3);
}


void Robonii_IRPod_Driver::InitTWI(void)
{


 // Reset IRPOD
  // PORTA PIN1 = IRPOD reset, drive low, then release high

  // Configure IRPOD reset line as input with pullup
  PORTA.PIN1CTRL = PORT_SRLEN_bm| PORT_OPC_PULLUP_gc;

  PORTA.DIRSET = PIN1_bm;
  PORTA.OUTCLR = PIN1_bm;


  PORTE.PIN1CTRL = PORT_SRLEN_bm| PORT_OPC_PULLUP_gc;
  PORTE.PIN0CTRL = PORT_SRLEN_bm| PORT_OPC_PULLUP_gc;


 // sbi(PORTF.DIR,1);
  //sbi(PORTF.OUT,1);
  //cbi(PORTF.OUT,1);
   
  // Set baud rate, value = Fsys / (2 * Baud Rate)
  // 150 -> 100 kHz. //for 31MHZ
  // 33 -> 400 kHz  //for 31MHZ
  //TWIE_MASTER_BAUD = 33; //for 31MHZ
  TWIE_MASTER_BAUD = 35;  //TIAN  //35 for 32MHz
  
  // Enable twi master. This also enables the twi bus state machine.
  // Set interrupt level for TWIE master to high.
  // Enable master read and write interrupt.
  TWIE_MASTER_CTRLA = (TWI_MASTER_ENABLE_bm | TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm) | (TWI_MASTER_INTLVL_HI_gc);

  // Enable smart mode to auto transmit ack after data register is read.
  //TWIE_MASTER_CTRLB = 0x00;        
  //TWIE_MASTER_CTRLB |= (TWI_MASTER_SMEN_bm | TWI_MASTER_TIMEOUT_gm);
  //TWIE_MASTER_CTRLB |= TWI_MASTER_TIMEOUT_gm;
  
  // Force the bus state to idle condition to allow for a START condition 
  TWIE_MASTER_STATUS |= TWI_MASTER_BUSSTATE_IDLE_gc;
  
  // Enable the twi slave.
  // Set interrupt level for TWIE slave to high.
  // Enable the data interrupt to fire after data has been read from the DATA register.
  //TWIE_SLAVE_CTRLA = 0x00;
  //TWIE_SLAVE_CTRLA = (TWI_SLAVE_ENABLE_bm | TWI_SLAVE_DIEN_bm | TWI_SLAVE_PIEN_bm | TWI_SLAVE_APIEN_bm) | (TWI_SLAVE_INTLVL_HI_gc);
  
  // Set slave address
  TWIE_SLAVE_ADDR = TWI_ADDR_ROBOT << TWI_SLAVE_ADDRMASK_gp;


  // Release reset 
  PORTA.DIRCLR = PIN1_bm;

 // irpod_status = IRPOD_STATUS_OK;  
 twiTxDataFinalFlag = 0;
}


SIGNAL(TWIE_TWIM_vect)
{
 
  
  if(TWIE_MASTER_STATUS & TWI_MASTER_WIF_bm)
  {
  
    if ((TWIE_MASTER_STATUS & TWI_MASTER_RXACK_bm) != 0)
    {
      // Received NAK, should never happen, abort with stop condition
      TWIE_MASTER_CTRLC = TWI_MASTER_CMD_STOP_gc;
      //irpod_status = IRPOD_STATUS_ERROR;
    
      return;
    }

    // Interrupt generated from a master write operation.
    if((TWIE_MASTER_STATUS & TWI_MASTER_BUSERR_bm) || (TWIE_MASTER_STATUS & TWI_MASTER_ARBLOST_bm))
    {
      // Bus state error or arbitration lost - master could not take control of the bus.
      //TWIE_MASTER_CTRLC |= (TWI_MASTER_CMD0_bm | TWI_MASTER_CMD1_bm);
      TWIE_MASTER_STATUS |= 0x0C;
    
    }
     
    //else
    //{
      // Slave addressed replied with ack, can continue data transmission.
      // TODO: Make buffer size globally defined value.
      if(twiTxIndex < twiTxLimit)
      {
        //TODO: Tian Send Data Here....--------------------------------------------------------------
        TWIE_MASTER_DATA = twiTxData[twiTxIndex];
        twiTxIndex++;   
      }
      else
      {      
        // Twi transmission complete, issue repeated start, read       
        TWIE_MASTER_ADDR = (TWI_ADDR_IRPOD << 1) | TWI_CMD_MASTER_READ;
      }
  }
  else if(TWIE_MASTER_STATUS & TWI_MASTER_RIF_bm)
  {
     //while(1);
    if (twiRxIndex < twiRxLimit)
    {
      //TODO: TIAN Read data here---------------------------------------------------------------------------------
      twiRxData[twiRxIndex] = TWIE_MASTER_DATA;
      twiRxIndex++;     
    }
    if (twiRxIndex < twiRxLimit)
    {
      // ACK byte
      TWIE_MASTER_CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;     
    }
    else
    {
     
      // Read all the data, indicate NACK & stop condition
      TWIE_MASTER_CTRLC = TWI_MASTER_CMD_STOP_gc | TWI_MASTER_ACKACT_bm;
      //irpod_status = IRPOD_STATUS_OK;

      // Process received data
     // ProcessTwiBuffer();
     
     twiTxDataFinal[0]=twiRxData[0];
     twiTxDataFinal[1]=twiRxData[1];
     twiTxDataFinal[2]=twiRxData[2];
     twiTxDataFinal[3]=twiRxData[3];
     twiTxDataFinal[4]=twiRxData[4];
     twiTxDataFinal[5]=twiRxData[5];
    twiTxDataFinalFlag = twiRxIndex;
          
    }
  }  
}

void Robonii_IRPod_Driver::InitTwiHeader(unsigned char irpodcmd)
{
 twiTxData[0] = pod_led_data1 | (irpodcmd << 2);
  twiTxData[1] = pod_led_data2;
  
  twiTxLimit = 2;

  // Reset read index
  twiRxData[0] = twiTxData[0];
  twiRxIndex = 1;
  //twiRxLimit = 4;
  twiRxLimit = 7;
}

void Robonii_IRPod_Driver::IrpodReqVersion(void)
{
  // Clear globals for IRPOD version
 // spes_port_device_id = 0;
//  spes_port_device_vers = 0;

  InitTwiHeader(IRPOD_CMD_READ_DEV_INFO);
  twiRxLimit = 3;
  twiTxLimit = 2;
//ir_rprintf3("\n\rIR REQ VER");
  START_TWI_TX;
}



// IRPOD Led settings stored in globals pod_led_data1 & pod_led_data2
//
// LED1   bit7..0: unused bits (7:2) | RED (1:0)
// LED2, 2 bits per LED, 00=off, 01=slow flash, 10=fast flash, 11=on
//        bit7..0: YELLOW (7:6) | LEFTMUZZLE (5:4) | RIGHTMUZZLE (3:2) | GREEN (1:0)
void Robonii_IRPod_Driver::IrPodSetLeds(uint8_t yellowLed,uint8_t leftMuzzleLed,uint8_t rightMuzzleLed,uint8_t greenLed,uint8_t redLed)
{
  pod_led_data1 = 0;
  pod_led_data2 = 0;
  
  pod_led_data1 = (redLed & 0x03);
  pod_led_data2 |= (yellowLed << 6);
  pod_led_data2 |= (leftMuzzleLed << 4);
  pod_led_data2 |= (rightMuzzleLed << 2);
  pod_led_data2 |= greenLed;
 }
 
 
 void Robonii_IRPod_Driver::IrPodSendCmd(irPodCmd_e cmd, unsigned char cmd_target, unsigned char data1, unsigned char data2, unsigned char data3)
{

  // Clear command result variable at this point.
  //prev_cmd_result = 0;
  
  //ir_rprintf3("IRPOD CMD %x  TARGET %x ",cmd, cmd_target);

  // Init variables.
  bs_command_data = 0;
    
  // Check if bs command is received.
  if(cmd == IRPOD_CMD_SET_BS_LEDS)
  {
    // Set command flag.
    bs_cmd_loaded = 1;
    
    // Insert target device id.
    bs_command_data = cmd_target << 6;
    // Insert command id. Set base station leds. == 0x02.   
    bs_command_data |= (0x02 << 2);
  }
  else if(cmd == IRPOD_CMD_INIT_BS)
  {
    // Set command flag.
    bs_cmd_loaded = 1;
    
    // Insert target device id.
    bs_command_data = cmd_target << 6;
    // Insert command id. Range request == 0x00.   
    bs_command_data |= (0x00 << 2);

    ir_tx_level = IRPOD_CMD_BM_IR_TXWEAK;
  }
  
  // Assign data buffer in ir protocol format.
  twiTxData[3] = data1;
  twiTxData[4] = data2;
  twiTxData[5] = data3;
  
  //TIAN Add
    InitTwiHeader(IRPOD_CMD_TXIR);
  twiRxLimit = 3;
  twiTxLimit = 6;
//ir_rprintf3("\n\rIR REQ VER");
  START_TWI_TX;
  
  
}


//First byte contains first byte sent to IR pod for iodentification.
unsigned char Robonii_IRPod_Driver::ReadData(unsigned char *chars)
{
unsigned char dum = twiTxDataFinalFlag;
  if (twiTxDataFinalFlag == 0)
  {
  return  0;
  }else
   {
     char i = 0;
	 
	 for (i=0;i<twiTxDataFinalFlag;i++)
	 {
	 chars[i]  = twiTxDataFinal[i];
	 }
   }  
   
   twiTxDataFinalFlag =0;
  return dum;
}

void Robonii_IRPod_Driver::SetLED(irpod_leds_e led, irpod_leds_state_e state)
{
  ledArray[led] = state;
  IrPodSetLeds(ledArray[0],ledArray[1],ledArray[2],ledArray[3],ledArray[4]);
  IrpodReqVersion();
}
/*
irpod_leds_state_e Robonii_IRPod_Driver::GetLEDstate(irpod_leds_e led)
{
return irpod_leds_state_e(ledArray[char(led)]);
}
*/
