/**
* \file
* \version 1.0
* \author bazhen.levkovets
** \date 2018
*
*************************************************************************************
* \copyright	Bazhen Levkovets
* \copyright	Brovary, Kyiv region
* \copyright	Ukraine
*
*************************************************************************************
*
* \brief
*
*/

/*
**************************************************************************
*							INCLUDE FILES
**************************************************************************
*/
	#include "dosimeter_sbm19_sm.h"
/*
**************************************************************************
*							LOCAL DEFINES
**************************************************************************
*/

/*
**************************************************************************
*							LOCAL CONSTANTS
**************************************************************************
*/


/*
**************************************************************************
*						    LOCAL DATA TYPES
**************************************************************************
*/


/*
**************************************************************************
*							  LOCAL TABLES
**************************************************************************
*/

/*
**************************************************************************
*								 MACRO'S
**************************************************************************
*/

/*
**************************************************************************
*						    GLOBAL VARIABLES
**************************************************************************
*/

const unsigned long port_mask_UL[] = {
	1UL<<0,			/*  0 LED PC  0 */
	1UL<<1,			/*  1 LED PC  1 */
	1UL<<2,			/*  2 LED PC  2 */
	1UL<<3,			/*  3 LED PC  3 */
	1UL<<4,			/*  4 LED PC  4 */
	1UL<<5,			/*  5 LED PC  5 */
	1UL<<6,			/*  6 LED PC  6 */
	1UL<<7,			/*  7 LED PC  7 */
	1UL<<8,			/*  8 LED PC  8 */
	1UL<<9, 		/*  9 LED PC  9 */
	1UL<<0x0A,		/* 10 LED PC 10 */
	1UL<<0x0B,		/* 11 LED PC 11 */
	1UL<<0x0C,		/* 12 LED PC 12 */
	1UL<<0x0D,		/* 13 LED PC 13 */
	1UL<<0x0E,		/* 14 LED PC 14 */
	1UL<<0x0F		/* 15 LED PC 15 */
 };

	uint8_t 	tim3_flag_u8 			= 0;
	uint8_t		update_flag_u8 			= 0;
	uint8_t 	led_count_u8			= 0;
	uint8_t		electron_array_count_u8	= 0;
	uint32_t	electron_hard_count_u32	= 0;
	uint32_t 	one_electron_time_u32_arr[VALUE_ARRAY_CNT];
	#ifdef	DISPLAY_TM1637
		tm1637_struct 	h1_tm1637;
	#endif
/*
**************************************************************************
*                        LOCAL FUNCTION PROTOTYPES
**************************************************************************
*/
	void LED_Blink	(uint8_t _position_u8);
	void Buzzer_Beep(void);
	#ifdef DISPLAY_TM1637
		void Display_radiation_TM1637(tm1637_struct* htm1637, uint32_t _radiation_u32);
	#else
		void Display_radiation(uint32_t _radiation_u32);
	#endif

	void Port_A_Off (uint8_t _bit_u8);
	void Port_A_On  (uint8_t _bit_u8);
	void Port_B_Off (uint8_t _bit_u8);
	void Port_B_On  (uint8_t _bit_u8);
	void Port_C_Off (uint8_t _bit_u8);
	void Port_C_On  (uint8_t _bit_u8);

	void Print_d1_0 (void);
	void Print_d1_1 (void);
	void Print_d1_2 (void);
	void Print_d1_3 (void);
	void Print_d1_4 (void);
	void Print_d1_5 (void);
	void Print_d1_6 (void);
	void Print_d1_7 (void);
	void Print_d1_8 (void);
	void Print_d1_9 (void);
	void Print_d1_A (void);
	void Print_d1_B (void);
	void Print_d1_C (void);
	void Print_d1_D (void);
	void Print_d1_E (void);
	void Print_d1_F (void);
	void Print_d1_M (void);

	void Print_d2_0 (void);
	void Print_d2_1 (void);
	void Print_d2_2 (void);
	void Print_d2_3 (void);
	void Print_d2_4 (void);
	void Print_d2_5 (void);
	void Print_d2_6 (void);
	void Print_d2_7 (void);
	void Print_d2_8 (void);
	void Print_d2_9 (void);
	void Print_d2_F (void);

/*
**************************************************************************
*                           GLOBAL FUNCTIONS
**************************************************************************
*/

void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin) {	//	irqq pin
	if ( GPIO_Pin == SBM19_Pin	) {
		electron_array_count_u8++;
		if (electron_array_count_u8 >= VALUE_ARRAY_CNT) electron_array_count_u8 = 0;
		one_electron_time_u32_arr[electron_array_count_u8] = TIM4->CNT;
		TIM4->CNT = 0;
		electron_hard_count_u32++;
		update_flag_u8 = 1;
	}
} //**************************************************************************

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {	//	irqq tim
	if ( htim == &TIM_60_SEC ) {
		tim3_flag_u8 =1;
	}
} //**************************************************************************

void Dozimeter_Init(void) {
	DWT_Delay_Init();
	DebugSoftVersion(SOFT_VERSION);
	DBG1("\t Debug: UART1 / 62500\r\n");
	for (int i=0; i<VALUE_ARRAY_CNT; i++) {
	  one_electron_time_u32_arr[i] = 60000 / START_RADIATION_VALUE;
	}

	#ifdef	DISPLAY_TM1637
		DBG1("\t Display: TM1637\r\n");
		DBG1("\t     LED: 4 pcs\r\n");
		h1_tm1637.clk_pin  = TM1637_CLK_Pin;
		h1_tm1637.clk_port = TM1637_CLK_GPIO_Port;
		h1_tm1637.dio_pin  = TM1637_DIO_Pin;
		h1_tm1637.dio_port = TM1637_DIO_GPIO_Port;
		h1_tm1637.digit_qnt= 4;
		TM1637_Init(&h1_tm1637);
		TM1637_Set_Brightness(&h1_tm1637, bright_15percent);
		TM1637_Display_Decimal(&h1_tm1637, SOFT_VERSION , no_double_dot, symbol_dec);
		HAL_Delay(1000);
	#else
		DBG1("\t Display: simple 2 digits\r\n");
		DBG1("\t     LED: 3 pcs\r\n");
	#endif

	HAL_TIM_Base_Start_IT( &TIM_60_SEC  );
	HAL_TIM_Base_Start   ( &TIM_BETWEEN );
	tim3_flag_u8 = 0 ;
	DBG1("\t End Init.\r\n\r\n");
} //************************************************************************

void Dozimeter_Main(void) {
	if (tim3_flag_u8 == 1) {
		DBG1(" TIM3=60Sec hard-cnt= %d imp\r\n\r\n", (int)electron_hard_count_u32);
		electron_hard_count_u32 = 0;
		tim3_flag_u8 = 0;
	}
	if (update_flag_u8 > 0) {
		Buzzer_Beep();
		uint32_t summa_of_all_array_u32 = 0;
		for (int i=0; i<VALUE_ARRAY_CNT; i++) {
			summa_of_all_array_u32 = summa_of_all_array_u32 + one_electron_time_u32_arr[i];
		}
		uint32_t qnt_electrons_per_60sec_u32 = ( 60000 * VALUE_ARRAY_CNT ) / summa_of_all_array_u32;
		DBG1("hard-cnt60sec:%02d\t cnt100:%02d\t one-electron-time: %04d\t 100-time-suma:%d\t calc-per60sec: %03d\r\n",
				(int) electron_hard_count_u32 							,
				(int) electron_array_count_u8							,
				(int) one_electron_time_u32_arr[electron_array_count_u8],
				(int) summa_of_all_array_u32							,
				(int) qnt_electrons_per_60sec_u32						) ;
		#ifdef DISPLAY_TM1637
			Display_radiation_TM1637(&h1_tm1637, qnt_electrons_per_60sec_u32);
		#else
			Display_radiation(qnt_electrons_per_60sec_u32);
		#endif
		LED_Blink(electron_array_count_u8);
		update_flag_u8 = 0;
	}
} //************************************************************************

/*
**************************************************************************
*                           LOCAL FUNCTIONS
**************************************************************************
*/

void LED_Blink(uint8_t _position_u8) {
	#ifdef LED_4_PCS
		_position_u8 = _position_u8 % 6;
		HAL_GPIO_WritePin(LED_1_GPIO_Port,	LED_1_Pin, RESET);
		HAL_GPIO_WritePin(LED_2_GPIO_Port,	LED_2_Pin, RESET);
		HAL_GPIO_WritePin(LED_3_GPIO_Port,	LED_3_Pin, RESET);
		HAL_GPIO_WritePin(LED_4_GPIO_Port,	LED_4_Pin, RESET);
		switch (_position_u8) {
			case 0: HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, SET); break;
			case 1: HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, SET); break;
			case 2: HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, SET); break;
			case 3: HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, SET); break;
			case 4: HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, SET); break;
			case 5: HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, SET); break;
			default: break;
		}
	#else
		_position_u8 = _position_u8 % 4;
		HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, SET);
		HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, SET);
		HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, SET);
		switch (_position_u8) {
			case 0: HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, RESET); break;
			case 1: HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, RESET); break;
			case 2: HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, RESET); break;
			case 3: HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, RESET); break;
			default: break;
		}
	#endif
} //************************************************************************

void Display_radiation_TM1637(tm1637_struct* htm1637, uint32_t _radiation_u32)	{
	#ifdef DISPLAY_TM1637
		TM1637_Display_Decimal(htm1637, _radiation_u32, no_double_dot, symbol_dec);
	#endif
} //************************************************************************

void Buzzer_Beep (void) {
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, SET);
	DWT_Delay_us(100);
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, RESET);
} //************************************************************************

void Display_radiation(uint32_t _radiation_u32)	{
#ifndef DISPLAY_TM1637
	_radiation_u32 = _radiation_u32 / HELP_DIVISION ;
	uint8_t digit_1_u8 = _radiation_u32 / 10 ;
	uint8_t digit_2_u8 = _radiation_u32 % 10 ;

	if (digit_1_u8== 0) Print_d1_0();
	if (digit_1_u8== 1) Print_d1_1();
	if (digit_1_u8== 2) Print_d1_2();
	if (digit_1_u8== 3) Print_d1_3();
	if (digit_1_u8== 4) Print_d1_4();
	if (digit_1_u8== 5) Print_d1_5();
	if (digit_1_u8== 6) Print_d1_6();
	if (digit_1_u8== 7) Print_d1_7();
	if (digit_1_u8== 8) Print_d1_8();
	if (digit_1_u8== 9) Print_d1_9();
	if (digit_1_u8==10) Print_d1_A();
	if (digit_1_u8==11) Print_d1_B();
	if (digit_1_u8==12) Print_d1_C();
	if (digit_1_u8==13) Print_d1_D();
	if (digit_1_u8==14) Print_d1_E();
	if (digit_1_u8==15) Print_d1_F();
	if (digit_1_u8 >15) Print_d1_M();

	if (digit_2_u8==0) Print_d2_0();
	if (digit_2_u8==1) Print_d2_1();
	if (digit_2_u8==2) Print_d2_2();
	if (digit_2_u8==3) Print_d2_3();
	if (digit_2_u8==4) Print_d2_4();
	if (digit_2_u8==5) Print_d2_5();
	if (digit_2_u8==6) Print_d2_6();
	if (digit_2_u8==7) Print_d2_7();
	if (digit_2_u8==8) Print_d2_8();
	if (digit_2_u8==9) Print_d2_9();
	if (digit_2_u8 >9) Print_d2_F();
#endif
} //************************************************************************

/*----------------------------------------------------------------------------
  Switch on
 *----------------------------------------------------------------------------*/
void Port_A_Off (uint8_t _bit_u8)	{	GPIOA->BRR  = port_mask_UL[_bit_u8];	}
void Port_A_On  (uint8_t _bit_u8)	{	GPIOA->BSRR = port_mask_UL[_bit_u8];	}

void Port_B_Off (uint8_t _bit_u8)	{	GPIOB->BRR  = port_mask_UL[_bit_u8];	}
void Port_B_On  (uint8_t _bit_u8)	{	GPIOB->BSRR = port_mask_UL[_bit_u8];	}

void Port_C_Off (uint8_t _bit_u8)	{	GPIOC->BRR  = port_mask_UL[_bit_u8];	}
void Port_C_On  (uint8_t _bit_u8)	{	GPIOC->BSRR = port_mask_UL[_bit_u8];	}

/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
void Print_d1_0 (void)	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_Off (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_1 (void)	{
	Port_A_Off (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_Off (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_2 (void)	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_Off ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_3 (void)	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_4 (void)	{
	Port_A_Off (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_5 (void)	{
	Port_A_On  (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_6 (void)	{
	Port_A_On  (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_7 (void)	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_Off (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_8 (void)	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_9 (void)	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_A (void)	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_B (void)	{
	Port_A_Off (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_C (void)	{
	Port_A_On  (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_Off ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_Off (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_D (void)	{
	Port_A_Off (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_E (void)	{
	Port_A_On  (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_Off ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_F (void)	{
	Port_A_On  (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_Off ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d1_M (void)	{
	Port_A_Off (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_Off ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Print_d2_0 (void)
	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_Off ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Print_d2_1 (void)
	{
	Port_C_Off (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_Off ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_Off ( 2);	// f
	Port_A_Off ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Print_d2_2 (void)
	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_Off ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_Off ( 2);	// f
	Port_A_On  ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Print_d2_3 (void)
	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_Off ( 2);	// f
	Port_A_On  ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Print_d2_4 (void)
	{
	Port_C_Off  (13);	// a
	Port_B_On   ( 9);	// b
	Port_B_On   ( 8);	// c
	Port_B_Off  ( 5);	// d
	Port_A_Off  ( 1);	// e
	Port_A_On   ( 2);	// f
	Port_A_On   ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Print_d2_5 (void)
	{
	Port_C_On  (13);	// a
	Port_B_Off ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Print_d2_6 (void)	{
	Port_C_On  (13);	// a
	Port_B_Off ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Print_d2_7 (void)	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_Off ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_Off ( 2);	// f
	Port_A_Off ( 3);	// g
	}

void Print_d2_8 (void)	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Print_d2_9 (void)	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Print_d2_F (void)	{
	Port_C_On  (13);	// a
	Port_B_Off ( 9);	// b
	Port_B_Off ( 8);	// c
	Port_B_Off ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
} /*----------------------------------------------------------------------------*/

//************************************************************************
//************************************************************************
