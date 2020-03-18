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

const unsigned long port_mask[] = {
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

/*
**************************************************************************
*                        LOCAL FUNCTION PROTOTYPES
**************************************************************************
*/

	void Port_A_Off (uint32_t num);
	void Port_A_On (uint32_t num);
	void Port_B_Off (uint32_t num);
	void Port_B_On (uint32_t num);
	void Port_C_Off (uint32_t num);
	void Port_C_On (uint32_t num);

	void Letter_1_0 (void);
	void Letter_1_1 (void);
	void Letter_1_2 (void);
	void Letter_1_3 (void);
	void Letter_1_4 (void);
	void Letter_1_5 (void);
	void Letter_1_6 (void);
	void Letter_1_7 (void);
	void Letter_1_8 (void);
	void Letter_1_9 (void);
	void Letter_1_A (void);
	void Letter_1_B (void);
	void Letter_1_C (void);
	void Letter_1_D (void);
	void Letter_1_E (void);
	void Letter_1_F (void);
	void Letter_1_M (void);

	void Letter_2_0 (void);
	void Letter_2_1 (void);
	void Letter_2_2 (void);
	void Letter_2_3 (void);
	void Letter_2_4 (void);
	void Letter_2_5 (void);
	void Letter_2_6 (void);
	void Letter_2_7 (void);
	void Letter_2_8 (void);
	void Letter_2_9 (void);
	void Letter_2_F (void);

/*
**************************************************************************
*                           GLOBAL FUNCTIONS
**************************************************************************
*/

void Indikator(uint32_t dozator)	{
	uint8_t doza_10 = dozator/10;
	uint8_t doza_01 = dozator-((doza_10)*10);

	if (doza_10==0) Letter_1_0();
	if (doza_10==1) Letter_1_1();
	if (doza_10==2) Letter_1_2();
	if (doza_10==3) Letter_1_3();
	if (doza_10==4) Letter_1_4();
	if (doza_10==5) Letter_1_5();
	if (doza_10==6) Letter_1_6();
	if (doza_10==7) Letter_1_7();
	if (doza_10==8) Letter_1_8();
	if (doza_10==9) Letter_1_9();
	if (doza_10==10) Letter_1_A();
	if (doza_10==11) Letter_1_B();
	if (doza_10==12) Letter_1_C();
	if (doza_10==13) Letter_1_D();
	if (doza_10==14) Letter_1_E();
	if (doza_10==15) Letter_1_F();
	if (doza_10 >15) Letter_1_M();

	if (doza_01==0) Letter_2_0();
	if (doza_01==1) Letter_2_1();
	if (doza_01==2) Letter_2_2();
	if (doza_01==3) Letter_2_3();
	if (doza_01==4) Letter_2_4();
	if (doza_01==5) Letter_2_5();
	if (doza_01==6) Letter_2_6();
	if (doza_01==7) Letter_2_7();
	if (doza_01==8) Letter_2_8();
	if (doza_01==9) Letter_2_9();
	if (doza_01 >9) Letter_2_F();
}
//************************************************************************


//************************************************************************


//************************************************************************

/*
**************************************************************************
*                           LOCAL FUNCTIONS
**************************************************************************
*/

/*----------------------------------------------------------------------------
  Switch on
 *----------------------------------------------------------------------------*/
void Port_A_Off (uint32_t num)	{
	GPIOA->BRR = port_mask[num];                 /* Turn On  LED          */
}/*----------------------------------------------------------------------------*/

void Port_A_On (uint32_t num)	{
	GPIOA->BSRR  = port_mask[num];                 /* Turn Off LED      */
} /*----------------------------------------------------------------------------*/

void Port_B_Off (uint32_t num)	{
	GPIOB->BRR = port_mask[num];                 /* Turn On  LED          */
} /*----------------------------------------------------------------------------*/

void Port_B_On (uint32_t num)	{
	GPIOB->BSRR  = port_mask[num];                 /* Turn Off LED      */
} /*----------------------------------------------------------------------------*/

void Port_C_Off (uint32_t num)	{
	GPIOC->BRR = port_mask[num];                 /* Turn On  LED          */
} /*----------------------------------------------------------------------------*/

void Port_C_On (uint32_t num)	{
	GPIOC->BSRR  = port_mask[num];                 /* Turn Off LED      */
} /*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------
  Letter
 *----------------------------------------------------------------------------*/
void Letter_1_0 (void)	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_Off (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_1 (void)	{
	Port_A_Off (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_Off (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_2 (void)	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_Off ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_3 (void)	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_4 (void)	{
	Port_A_Off (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_5 (void)	{
	Port_A_On  (12);	//	a
	Port_A_Off  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_Off  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_6 (void)	{
	Port_A_On  (12);	//	a
	Port_A_Off  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_7 (void)	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_Off  (15);	//	d
	Port_B_Off  (14);	//	e
	Port_B_Off  (13);	//	f
	Port_B_Off  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_8 (void)	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_9 (void)	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_A (void)	{
	Port_A_On  (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_B (void)	{
	Port_A_Off (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_C (void)	{
	Port_A_On (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_Off ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_Off  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_D (void)	{
	Port_A_Off (12);	//	a
	Port_A_On  (11);	//	b
	Port_A_On  ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_E (void)	{
	Port_A_On  (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_Off ( 8);	//	c
	Port_B_On  (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_F (void)	{
	Port_A_On  (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_Off ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_On  (14);	//	e
	Port_B_On  (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_1_M (void)	{
	Port_A_Off (12);	//	a
	Port_A_Off (11);	//	b
	Port_A_Off ( 8);	//	c
	Port_B_Off (15);	//	d
	Port_B_Off (14);	//	e
	Port_B_Off (13);	//	f
	Port_B_On  (12);	//	g
} /*----------------------------------------------------------------------------*/

void Letter_2_0 (void)
	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_Off ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Letter_2_1 (void)
	{
	Port_C_Off (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_Off ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_Off ( 2);	// f
	Port_A_Off ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Letter_2_2 (void)
	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_Off ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_Off ( 2);	// f
	Port_A_On  ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Letter_2_3 (void)
	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_Off ( 2);	// f
	Port_A_On  ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Letter_2_4 (void)
	{
	Port_C_Off  (13);	// a
	Port_B_On   ( 9);	// b
	Port_B_On   ( 8);	// c
	Port_B_Off  ( 5);	// d
	Port_A_Off  ( 1);	// e
	Port_A_On   ( 2);	// f
	Port_A_On   ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Letter_2_5 (void)
	{
	Port_C_On  (13);	// a
	Port_B_Off ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Letter_2_6 (void)	{
	Port_C_On  (13);	// a
	Port_B_Off ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Letter_2_7 (void)	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_Off ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_Off ( 2);	// f
	Port_A_Off ( 3);	// g
	}

void Letter_2_8 (void)	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Letter_2_9 (void)	{
	Port_C_On  (13);	// a
	Port_B_On  ( 9);	// b
	Port_B_On  ( 8);	// c
	Port_B_On  ( 5);	// d
	Port_A_Off ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
} /*----------------------------------------------------------------------------*/

void Letter_2_F (void)	{
	Port_C_On  (13);	// a
	Port_B_Off ( 9);	// b
	Port_B_Off ( 8);	// c
	Port_B_Off ( 5);	// d
	Port_A_On  ( 1);	// e
	Port_A_On  ( 2);	// f
	Port_A_On  ( 3);	// g
} /*----------------------------------------------------------------------------*/
