/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; Portions COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/**
  ******************************************************************************
  * <h2><center>&copy; Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    main.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Main program body      
  *          Modified to support the STM32F4DISCOVERY, STM32F4DIS-BB and
  *          STM32F4DIS-LCD modules. 
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Embest SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
  * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
  * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4x7_eth.h"
#include "netconf.h"
#include "main.h"
#include "lwip/tcp.h"
#include "tcp_echoclient.h"
#include "DS1307.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SYSTEMTICK_PERIOD_MS  10

/*--------------- LCD Messages ---------------*/
#define MESSAGE1   "     STM32F4x7      "
#define MESSAGE2   "  STM32F-4 Series   "
#define MESSAGE3   " TCP echoclient Demo"
#define MESSAGE4   "                    "

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;
uint32_t Button_TimerBack;
__IO uint32_t Button_Flag;

uint32_t time = 0;
uint8_t DEST_PORT;
uint8_t DEST_IP_ADDR0;
uint8_t DEST_IP_ADDR1;
uint8_t DEST_IP_ADDR2;
uint8_t DEST_IP_ADDR3;
uint8_t IDBOARD;
//char s[40], s1[40]; uint8_t pos,pos1 = 0; // Use for USART

char _read = 1;
uint8_t mode = NONE;
uint16_t number;
uint16_t quantity = 0;

char Conn_use = 0;

/* Private function prototypes -----------------------------------------------*/
void LCD_LED_BUTTON_Init(void);
uint8_t Button_State(void);
void Init_peripheral();
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured to 
       168 MHz, this is done through SystemInit() function which is called from
       startup file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */
  /* Variables */
	char *stime = malloc(sizeof(char) * 20);
	unsigned char *str = malloc(sizeof(char) * 12);
	uint8_t *strByte = malloc(sizeof(uint8_t) * 12);
	uint8_t i;
	uint8_t flag_connected = 0;
	void* voidStr;
	
	Init_peripheral();
	
  /*Initialize LCD and Leds */ 
  LCD_LED_BUTTON_Init();
  
  /* Configure ethernet (GPIOs, clocks, MAC, DMA) */ 
  ETH_BSP_Config();
    
  /* Initilaize the LwIP stack */
  LwIP_Init();
	
	DS1307Init();
	Init_peripheral();
	
//	EEPROM_writeByte(8,1);
//	time = 0;
//	TIM_SetCounter(TIM4, 0);
//	while (time < 50) time = TIM_GetCounter(TIM4);
//	
//	EEPROM_writeByte(9,192);
//	//Delay(10);
//	time = 0;
//	TIM_SetCounter(TIM4, 0);
//	while (time < 50) time = TIM_GetCounter(TIM4);
//	EEPROM_writeByte(10,168);
//	Delay(10);
//	EEPROM_writeByte(11,1);
//	Delay(10);
//	EEPROM_writeByte(12,4);
//	Delay(10);
//	EEPROM_writeByte(13,35);
//	Delay(10);
	
	// Set Quantity
	/*
	EEPROM_writeByte(14,0);
	time = 0;
	TIM_SetCounter(TIM4, 0);
	while (time < 50) time = TIM_GetCounter(TIM4);
	//Delay(1);
	EEPROM_writeByte(15,0);
	time = 0;
	TIM_SetCounter(TIM4, 0);
	//Delay(1);
	while (time < 50) time = TIM_GetCounter(TIM4);
	*/
	
	/*
	EEPROM_writeByte(16,138);
	Delay(10);
	EEPROM_writeByte(17,59);
	Delay(10);
	EEPROM_writeByte(18,65);
	Delay(10);
	EEPROM_writeByte(19,77);
	Delay(10);
	k++;
	EEPROM_writeByte(20,1);
	Delay(10);
	k++;
	EEPROM_writeByte(21,254);
	Delay(10);
	k++;
	EEPROM_writeByte(22,0);
	Delay(10);
	k++;
	EEPROM_writeByte(23,0);
	Delay(10);
	k++;
	EEPROM_writeByte(24,23);
	Delay(10);
	k++;
	EEPROM_writeByte(25,59);
	Delay(10);
	*/
	GPIO_ToggleBits(GPIOD, LED_RED);
	
	/*Get ID Of Board*/
	IDBOARD = EEPROM_readByte(8);
	
	/* Get Destination IP Address & Port */
	// IP Address 4 bytes
	DEST_IP_ADDR0 = EEPROM_readByte(9);
	DEST_IP_ADDR1 = EEPROM_readByte(10);
	DEST_IP_ADDR2 = EEPROM_readByte(11);
	DEST_IP_ADDR3 = EEPROM_readByte(12);
	// Port 1 byte
	DEST_PORT = EEPROM_readByte(13);
	//DEST_PORT = 35;
	
	/* Get Quantity 8 high bits(0x0014) and 8 high bits (0x0015)*/
	quantity = (EEPROM_readByte(14) << 8) | (EEPROM_readByte(15));
	//DS1307SetTime();
	
  /* Infinite loop */
	TIM_SetCounter(TIM4, 0);
	flag_connected = 0;
  while (1)
  {  
    /* check if any packet received */
    if (ETH_CheckFrameReceived()) { 
      /* process received ethernet packet */
      LwIP_Pkt_Handle();
    }
    /* handle periodic timers for LwIP */
    LwIP_Periodic_Handle(LocalTime);
		//tcp_echoclient_connect();
		time = TIM_GetCounter(TIM4);
		if ((time >= 65000) && (flag_connected == 0))
		{
			tcp_echoclient_connect();
			flag_connected = 1;
			GPIO_ToggleBits(GPIOD, LED_BLUE);
		}
    if (Button_State()) {
      /*connect to tcp server */ 
			tcp_echoclient_connect(); //Must put in here
			flag_connected = 1;
			GPIO_ToggleBits(GPIOD, LED_BLUE);
			//tcp_write(get_tcp_pcb(), "aaaa", 4, 1);
    }
		
		
		strcpy(str,get_data()); // copy data to str
		str[get_strlen()] = '\0';
		// Get AT Mode
		if (get_strlen() > 0)
		{
			voidStr = GetVoidStr();
			strByte = GetDataByteType();
			mode = str[1];
			clear_data(); // Clear Read data
			set_strlen(0); // Set strlen = 0
		}
		
		if (str[0] == IDBOARD)
		{
			if (mode == NONE)
			{
				
			}
			/* Check RFID, Time, Door */
			else if (mode == IDCHECK)
			{
				mode = CheckOpenDoor(voidStr);
			}
			else if (mode == DS1307)
			{
				GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
				stime= DS1307GetTimeString();
				tcp_write(get_tcp_pcb(), stime, 20, 1);
				mode = NONE;
			}
			else if (mode == OPEN)
			{
				if (str[6] == 1)
					OpenDoor(0);
				else if (str[6] == 2)
					OpenDoor(1);
				else if (str[6] == 3)
					OpenDoor(2);
				else if (str[6] == 4)
					OpenDoor(3);
				
				tcp_write(get_tcp_pcb(), GetOutPutText(), 12, 1);
				
				Delay(5000);
				CloseDoor(0);
				CloseDoor(1);
				CloseDoor(2);
				CloseDoor(3);
				mode = NONE;
			}
			else if (mode == IDADD)
			{
				AddNewUser(voidStr);
				str[1] = STOREOK;
				tcp_write(get_tcp_pcb(), str, 12, 1);
				mode = NONE;
			}
			else if (mode == CLEARALL)
			{
				SetQuantity(0);
			}
			else if (mode == GETINFO)
			{
				str[2] = DEST_IP_ADDR0;
				str[3] = DEST_IP_ADDR1;
				str[4] = DEST_IP_ADDR2;
				str[5] = DEST_IP_ADDR3;
				str[6] = DEST_PORT;
				tcp_write(get_tcp_pcb(), str, 12, 1);
			}
			else if (mode == SETINFO)
			{
				EEPROM_writeByte(9,str[2]);
				Delay(10);
				EEPROM_writeByte(10,str[3]);
				Delay(10);
				EEPROM_writeByte(11,str[4]);
				Delay(10);
				EEPROM_writeByte(12,str[5]);
				Delay(10);
				EEPROM_writeByte(13,str[6]);
				Delay(10);
				EEPROM_writeByte(8,str[7]);
				Delay(10);
			}
		}
  }   
}

void Init_peripheral()
{
	GPIO_InitTypeDef GPIO,GPIO_InitStruct;
	USART_InitTypeDef USART;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIMER;
	SPI_InitTypeDef SPI_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,	ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	/*---- GPIO Init ----*/

	GPIO.GPIO_Mode = GPIO_Mode_OUT;
	GPIO.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO);

	GPIO.GPIO_Mode = GPIO_Mode_OUT;
	GPIO.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO);
	
	/*---- Relay GPIO Init ----*/
	
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
	
	/*---- PB6-TX, PB7-RX, PD5-TX, PD6-RX ----*/
	GPIO.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO.GPIO_Mode = GPIO_Mode_AF;
	GPIO.GPIO_OType = GPIO_OType_PP;
	GPIO.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO);
	
	GPIO.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO.GPIO_Mode = GPIO_Mode_AF;
	GPIO.GPIO_OType = GPIO_OType_PP;
	GPIO.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO);
	
	/*GPIO.GPIO_Pin = GPIO_Pin_10;
	GPIO.GPIO_Mode = GPIO_Mode_IN;
	GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO);*/
	
	/*---- Timer Init ----*/
	// Timer 4 -----> Max 50Mhz
	
	TIMER.TIM_ClockDivision = TIM_CKD_DIV1;
	TIMER.TIM_Prescaler = 5000;
	TIMER.TIM_CounterMode = TIM_CounterMode_Up;
	TIMER.TIM_Period = 65535;
	TIMER.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIMER);
	TIM_Cmd(TIM4, ENABLE);
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_SetCounter(TIM4, 0);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	/*---- USART Init ----*/

	USART.USART_BaudRate = 9600;
	USART.USART_StopBits = USART_StopBits_1;
	USART.USART_Parity = USART_Parity_No;
	USART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt 
	USART_Cmd(USART2, ENABLE);
	
	USART.USART_BaudRate = 115200;
	USART.USART_StopBits = USART_StopBits_1;
	USART.USART_Parity = USART_Parity_No;
	USART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt 
	USART_Cmd(USART1, ENABLE);
	
	/*---- SPI Init ----*/
	/* configure pins used by SPI1
	 * PC10 = SCK
	 * PC11 = MISO
	 * PC12 = MOSI
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	// connect SPI1 pins to SPI alternate function
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
	
	// Chip Select Pin
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_15);
	
	
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex mode, seperate MOSI and MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;        // clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      // data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // SPI frequency is APB2 frequency / 4
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
	SPI_Init(SPI3, &SPI_InitStruct); 
	
	SPI_Cmd(SPI3, ENABLE); // enable SPI1
	
	/*----- NVIC USART2 interrupt -----*/
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/*----- NVIC USART1 interrupt -----*/
	if (Conn_use == 1){
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	}
	
	/*----- NVIC Timer interrupt -----*/
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

uint8_t GetPort(void) {return DEST_PORT;}
uint8_t GetIPADDR0(void) {return DEST_IP_ADDR0;}
uint8_t GetIPADDR1(void) {return DEST_IP_ADDR1;}
uint8_t GetIPADDR2(void) {return DEST_IP_ADDR2;}
uint8_t GetIPADDR3(void) {return DEST_IP_ADDR3;}
uint16_t GetQuantity(void) 
{
	uint16_t a = (EEPROM_readByte(14) << 8) | (EEPROM_readByte(15));
	return a;
}
void SetQuantity(uint16_t number) 
{
	quantity = number;
	EEPROM_writeByte(14, (uint8_t) number >> 8);
	time = 0;
	TIM_SetCounter(TIM4, 0);
	while (time < 50) time = TIM_GetCounter(TIM4);
	//Delay(1);
	
	EEPROM_writeByte(15, (uint8_t) number & 0x00FF);
	time = 0;
	TIM_SetCounter(TIM4, 0);
	while (time < 50) time = TIM_GetCounter(TIM4);
	//Delay(1);
}

void USART1_IRQHandler(void)
{
	// check if the USART1 receive interrupt flag was set
	if( USART_GetITStatus(USART1, USART_IT_RXNE) )
		{
			
		}
}

void USART2_IRQHandler(void)
{
	// check if the USART1 receive interrupt flag was set
	if( USART_GetITStatus(USART2, USART_IT_RXNE) )
		{
			GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
			
		}
}

void TIM4_IRQHandler()
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
			//led_toggle();
			time++;
      TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
				//time_now ++;
        //led_toggle();
    }
}
/**
  * @brief  Button state.
  * @param  None.
  * @retval 1: button is pressed.
  *         0: button is unpressed.
  */
uint8_t Button_State(void)
{
  uint8_t state = GPIO_ReadInputDataBit(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN);
  
  switch(Button_Flag) {
    case 0: 
      if (state) {
        Button_Flag = 1;
        Button_TimerBack = LocalTime;
      }
      state = 0;
      break;
    case 1:
      if ((state) && ((LocalTime - Button_TimerBack) >= 40)) {
        Button_Flag = 2;        
      } else {
        state = 0;
      }
      break;
    default:
      if (state == 0) {
        Button_Flag = 0;  
      }
      state = 0;
      break;
  }
  return state;
}
/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = LocalTime + nCount;  

  /* wait until the desired delay finish */  
  while (timingdelay > LocalTime);
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
}

/**
  * @brief  Initializes the STM324xG-EVAL's LCD, LEDs and push-buttons resources.
  * @param  None
  * @retval None
  */
void LCD_LED_BUTTON_Init(void)
{
#ifdef USE_LCD
  /* Initialize the STM324xG-EVAL's LCD */
  STM32f4_Discovery_LCD_Init();
#endif
 
#ifdef USE_LCD
  /* Clear the LCD */
  LCD_Clear(Black);

  /* Set the LCD Back Color */
  LCD_SetBackColor(Black);

  /* Set the LCD Text Color */
  LCD_SetTextColor(White);

  /* Display message on the LCD*/
  LCD_DisplayStringLine(Line0, (uint8_t*)MESSAGE1);
  LCD_DisplayStringLine(Line1, (uint8_t*)MESSAGE2);
  LCD_DisplayStringLine(Line2, (uint8_t*)MESSAGE3);
  LCD_DisplayStringLine(Line3, (uint8_t*)MESSAGE4);  
#endif

  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
