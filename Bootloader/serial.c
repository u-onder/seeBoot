/*! \file serial.c
    \brief   Serial channel handler
	
	\details Serial channel dependent implementations.
	\details We use no parity, 8 bit, single stop bit USART. Baudrate can be configured from
             the config file.
	\details This is an interrupt driven implementation of USART. So upon exit, all interrupts
             must be disabled, which can be done via close handler
	\details __serialLLx functions are low level functions. If you want to use another serial
			 channel, you should change these functions

	\date      2017
	\author    u-onder
	\copyright MIT License
*/

#include "config.h"

#include "serial.h"

#include "m4core.h"
#include "gpio.h"

//local call only functions
int_t __serialLLOpen(void);
int_t __serialLLClose(void);
void  __serialLLWaitWriteEnd(void);

//circular fill and empty buffer
typedef struct {
  char_t buffer[256];
  uint8_t head;
  uint8_t rear;
}serialBuffer_t;

//circular buffers for Receive and Transmit
serialBuffer_t serialRXBuffer;
serialBuffer_t serialTXBuffer;

//initialize serial channel
int_t serialInit (void)
{
  serialRXBuffer.head = 0;
  serialRXBuffer.rear = 0;
  
  serialTXBuffer.head = 0;
  serialTXBuffer.rear = 0;

  return SUCCESS;
}

//open initialized serial channel
int_t serialOpen (void)
{
  int_t ret;
  
  ret = __serialLLOpen();
      
  return ret;
}

//close initialized serial channel
int_t serialClose(void)
{
  return __serialLLClose();
}

//read all characters from receive buffer
int_t serialRead (char_t* byte)
{
  if (serialRXBuffer.head != serialRXBuffer.rear)
  {
    //WARNING: critical section
    *byte = serialRXBuffer.buffer[serialRXBuffer.rear];
    serialRXBuffer.rear++;
    return SUCCESS;
  }
  return ERROR;
}

//write null terminated string to transmit buffer. All write operations are handled in ISRs
//(hardware dependent)
int_t serialWriteStr(char_t* str)
{
  while (*str)
  {
    while (serialTXBuffer.head + 1 == serialTXBuffer.rear);
    
    //WARNING: critical section
    serialTXBuffer.buffer[serialTXBuffer.head] = *str;
    serialTXBuffer.head++;
    //ENABLE: TXIE
    *((i32_t*)(USART1_BASE + USART_CR1_OFFSET)) |= 0x00000080;
    
    str++;
  }
  return SUCCESS;
}

//1. Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this
//   for each data to be transmitted in case of single buffer.
//2. After writing the last data into the USART_DR register, wait until TC=1. This indicates
//   that the transmission of the last frame is complete. This is required for instance when
//   the USART is disabled or enters the Halt mode to avoid corrupting the last transmission.
//(hardware dependent)
int_t serialWriteBin(char_t* str, int_t ln)
{
  while (ln)
  {
    while (serialTXBuffer.head + 1 == serialTXBuffer.rear);
    
    //WARNING: critical section
    serialTXBuffer.buffer[serialTXBuffer.head] = *str;
    serialTXBuffer.head++;
    //ENABLE: TXIE
    *((i32_t*)(USART1_BASE + USART_CR1_OFFSET)) |= 0x00000080;
    
    str++;
    ln--;
  }
  return SUCCESS;
}

//wait till the end of transmission
int_t serialFlush(void)
{
  __serialLLWaitWriteEnd();
  return SUCCESS;
}

//1. Enable peripheral clock using the follwoing functions
//2. Enable the GPIO clocks using 
//3. Peripheral's alternate function: 
//   - Select the type, pull-up/pull-down and output speed
//   - Configure the desired pin in alternate function
//   - Connect the pin to the desired peripherals' Alternate Function (AF)
//4. Enable the USART by writing the UE bit in USART_CR1 register to 1.
//5. Program the M bit in USART_CR1 to define the word length.
//6. Program the number of stop bits in USART_CR2.
//7. Select the desired baud rate using the USART_BRR register.
//8. Set the TE bit in USART_CR1 to send an idle frame as first transmission.
//9. Set the RE bit USART_CR1. This enables the receiver which begins searching for a start bit.
int_t __serialLLOpen(void)
{
  uint_t dummy;
  uint_t clock;
  uint_t idivider, fdivider;
  
  //-------------------------------ENABLE GPIO CLOCK----------------------------
  //AHB1ENR: RCC AHB1 peripheral clock register, Address offset: 0x30
  *((i32_t*)(RCC_BASE + 0x00000030)) |= (ui32_t)0x00000001;
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  //---------------------SET GPIO PINS TO ALTERNATE FUNCTIONS-------------------
  //MODER: GPIO port mode register, Address offset: 0x00
  *((i32_t*)(GPIOA_R_BASE + GPIO_MODER_OFFSET))   &= (ui32_t)(0xFFC3FFFF); 
  *((i32_t*)(GPIOA_R_BASE + GPIO_MODER_OFFSET))   |= (ui32_t)(0x00280000);
  //OTYPER: GPIO port output type register, Address offset: 0x04
  *((i32_t*)(GPIOA_R_BASE + GPIO_OTYPER_OFFSET))  &= (ui32_t)(0xFFFFF9FF);
  //OSPEEDR: GPIO port output speed register, Address offset: 0x08
  *((i32_t*)(GPIOA_R_BASE + GPIO_OSPEEDR_OFFSET)) &= (ui32_t)(0xFFC3FFFF);
  *((i32_t*)(GPIOA_R_BASE + GPIO_OSPEEDR_OFFSET)) |= (ui32_t)(0x00280000);
  //PUPDR: GPIO port pull-up/pull-down register, Address offset: 0x0C
  *((i32_t*)(GPIOA_R_BASE + GPIO_PUPDR_OFFSET))   &= (ui32_t)(0xFFC3FFFF);
  *((i32_t*)(GPIOA_R_BASE + GPIO_PUPDR_OFFSET))   |= (ui32_t)(0x00140050);
  
  //AFRL: GPIO alternate function low register, Address offset: 0x20
  *((i32_t*)(GPIOA_R_BASE + GPIO_AFRH_OFFSET)) &= (ui32_t)(0xFFFFFF0F);
  *((i32_t*)(GPIOA_R_BASE + GPIO_AFRH_OFFSET)) |= (ui32_t)(0x00000770);
  
  //-------------------------ENABLE USART PERIPHERAL CLOCK----------------------
  //APB2ENR: RCC APB2 peripheral clock enable register, Address offset: 0x44 
  *((i32_t*)(RCC_BASE + 0x00000044)) |= (ui32_t)0x00000010;
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  //---------------------------- USART CR2 Configuration -----------------------
  dummy = *((i32_t*)(USART1_BASE + USART_CR2_OFFSET));
  dummy &= 0xCFFF;      //1 stop bit...
  *((i32_t*)(USART1_BASE + USART_CR2_OFFSET)) = (ui16_t)dummy;

  //---------------------------- USART CR1 Configuration -----------------------
  dummy = *((i32_t*)(USART1_BASE + USART_CR1_OFFSET));
  dummy &= 0xEFFF;                        //8 bit
  dummy &= 0xF9FF;                        //no parity
  dummy &= 0xDE03;                        //disable all interrupts
  dummy |= 0x2000 | 0x00A0 | 0x000C;      //enable uart rx and tx and its interrupts
  *((i32_t*)(USART1_BASE + USART_CR1_OFFSET)) = (ui16_t)dummy;

  //---------------------------- USART CR3 Configuration -----------------------
  dummy = *((i32_t*)(USART1_BASE + USART_CR3_OFFSET));
  dummy &= 0x0000;                        //no flow control
  *((i32_t*)(USART1_BASE + USART_CR3_OFFSET)) = (ui16_t)dummy;

  //---------------------------- USART BRR Configuration -----------------------
  clock = 84000000;
  /* Determine the integer part for over sampling value 8 and 16*/
  if ((*((i32_t*)(USART1_BASE + USART_CR1_OFFSET)) & 0x8000) != 0)
    idivider = ((25 * clock) / (2 * SERIAL_BAUDRATE));    
  else
    idivider = ((25 * clock) / (4 * SERIAL_BAUDRATE));    
  
  dummy = (idivider / 100) << 4;

  /* Determine the fractional part */
  fdivider = idivider - (100 * (dummy >> 4));

  /* Implement the fractional part in the register for over sampling value 9 and 16*/
  if ((*((i32_t*)(USART1_BASE + USART_CR1_OFFSET)) & 0x8000) != 0)
    dummy |= ((((fdivider * 8) + 50) / 100)) & ((ui08_t)0x07);
  else
    dummy |= ((((fdivider * 16) + 50) / 100)) & ((ui08_t)0x0F);

  *((i32_t*)(USART1_BASE + USART_BRR_OFFSET)) = (ui16_t)dummy;     

  //Enable usart interrupt
  ((i32_t*)(NVIC_R_BASE + NVIC_IPR_OFFSET))[9] &= 0xFFFF00FF;
  ((i32_t*)(NVIC_R_BASE + NVIC_IPR_OFFSET))[9] |= 0x00000000;
  
  ((i32_t*)(NVIC_R_BASE + NVIC_ISER_OFFSET))[1] = 0x00000020;
  
  return SUCCESS;
}

//disable serial channel
int_t __serialLLClose(void)
{
  //APB1RSTR: RCC APB1 peripheral reset register, Address offset: 0x20
  *((i32_t*)(RCC_BASE + 0x00000020)) &= ~((ui32_t)0x00020000);
  
  //Disable usart interrupt
  ((i32_t*)(NVIC_R_BASE + NVIC_ICER_OFFSET))[1] = 0x00000040;
  
  return SUCCESS;
}

//wait till the end of transmission
void __serialLLWaitWriteEnd(void)
{
  //SR: USART Status register, Address offset: 0x00
  while(((*((i32_t*)(USART1_BASE + USART_SR_OFFSET))) & 0x0040) == 0);
}

//ISR Handler for USART1
void USART1_IRQHandler (void)
{
  i32_t stat = *((i32_t*)(USART1_BASE + USART_SR_OFFSET));
  i32_t cr   = *((i32_t*)(USART1_BASE + USART_CR1_OFFSET));
  
  if ((stat & 0x00000080) && (cr & 0x00000080))      //transmitter empty
  {
    if (serialTXBuffer.head != serialTXBuffer.rear)
    {
      *((i32_t*)(USART1_BASE + USART_DR_OFFSET)) = serialTXBuffer.buffer[serialTXBuffer.rear];
      serialTXBuffer.rear++;
    }
    else
    {
      *((i32_t*)(USART1_BASE + USART_CR1_OFFSET)) &= ~0x00000080;
    }
  }
  if (stat & 0x00000020)
  {
    serialRXBuffer.buffer[serialRXBuffer.head] = *((i32_t*)(USART1_BASE + USART_DR_OFFSET));
    serialRXBuffer.head++;
    if (serialRXBuffer.head == serialRXBuffer.rear)
      serialRXBuffer.rear++;
  }
}
