// A COMPLETER

/*
Service permettant de chornom�trer jusqu'� 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 � TIMER4).
Utilise la lib MyTimers.h /.c
*/



#include "stm32f1xx_ll_usart.h"
#include "Chrono.h"
#include "MyTimer.h"

// variable priv�e de type Time qui m�morise la dur�e mesur�e
static Time Chrono_Time; // rem : static rend la visibilit� de la variable Chrono_Time limit�e � ce fichier 

// variable priv�e qui m�morise pour le module le timer utilis� par le module
static TIM_TypeDef * Chrono_Timer=TIM1; // init par d�faut au cas o� l'utilisateur ne lance pas Chrono_Conf avant toute autre fct.

// d�claration callback appel� toute les 10ms
void Chrono_Task_10ms(void);

USART_TypeDef * USARTx_used;
LL_USART_InitTypeDef init_usart_struct = {19200, LL_USART_DATAWIDTH_8B,
							LL_USART_STOPBITS_1,LL_USART_PARITY_EVEN,
							LL_USART_DIRECTION_TX_RX,LL_USART_HWCONTROL_NONE};

/**
	* @brief  Configure le chronom�tre. 
  * @note   A lancer avant toute autre fonction.
	* @param  Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Chrono_Conf(TIM_TypeDef * Timer)
{
	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
	
	// Fixation du Timer
	Chrono_Timer=Timer;

	// R�glage Timer pour un d�bordement � 10ms
	// 1/72Mz * 1 * autoreload = 10e-3
	// void MyTimer_Conf(TIM_TypeDef * Timer,int Arr, int Psc)
	MyTimer_Conf(Chrono_Timer,10000 - 1 , 71);
	
	// R�glage interruption du Timer avec callback : Chrono_Task_10ms()
	MyTimer_IT_Conf(Chrono_Timer,Chrono_Task_10ms,0);
	
	// Validation IT
	MyTimer_IT_Enable(Chrono_Timer);
	
	
}


/**
	* @brief  D�marre le chronom�tre. 
  * @note   si la dur�e d�passe 59mn 59sec 99 Hund, elle est remise � z�ro et repart
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Start(void)
{
	MyTimer_Start(Chrono_Timer);
}


/**
	* @brief  Arr�te le chronom�tre. 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Stop(void)
{
	MyTimer_Stop(Chrono_Timer);
}


/**
	* @brief  Remet le chronom�tre � 0 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Reset(void)
{
  // Arr�t Chrono
	MyTimer_Stop(Chrono_Timer);

	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
}


/**
	* @brief  Renvoie l'adresse de la variable Time priv�e g�r�e dans le module Chrono.c
  * @note   
	* @param  Aucun
  * @retval adresse de la variable Time
  */
Time * Chrono_Read(void)
{
	return &Chrono_Time;
}


void config_USART(USART_TypeDef * USARTx) {
		//Allumer l'horloge
		if (USARTx == USART1)
			RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
		else if (USARTx == USART2)
			RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
		else if (USARTx == USART3)
			RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
		GPIOA->CRL |= GPIO_CRL_MODE2_1; // OUT max speed 10 MHz.
		GPIOA->CRL |= GPIO_CRL_CNF2_1; // Push-pull
		// Config clock
		LL_USART_Init(USARTx,&init_usart_struct);
		// Enable USART
		LL_USART_Enable(USARTx);
		USARTx_used = USARTx;
}


/**
	* @brief  incr�mente la variable priv�e Chron_Time modulo 60mn 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Task_10ms(void)
{
	char trame[11];
	trame[2] = 0x3a;
	trame[5] = 0x3a;
	trame[8] = 0x3a;
	trame[9] = 0x0d;
	trame[10] = '\0';
	Chrono_Time.Hund++;
	if (Chrono_Time.Hund==100)
	{
		Chrono_Time.Sec++;
		if (Chrono_Time.Sec < 10) {
			trame[3] = 0x30;
			trame[4] = Chrono_Time.Sec + 0x30;
		}
		else {
			trame[3] = Chrono_Time.Sec / 10 + 0x30;
			trame[4] = Chrono_Time.Sec % 10 + 0x30;
		}
		
		Chrono_Time.Hund=0;
	}
	if (Chrono_Time.Sec==60)
	{
		Chrono_Time.Min++;
		Chrono_Time.Sec=0;
		if (Chrono_Time.Min < 10) {
			trame[0] = 0x30;
			trame[1] = Chrono_Time.Min + 0x30;
		}
		else {
			trame[0] = Chrono_Time.Min / 10 + 0x30;
			trame[1] = Chrono_Time.Min % 10 + 0x30;
		}
	}
	if (Chrono_Time.Min==60)
	{
		Chrono_Time.Hund=0;
	}
	if (Chrono_Time.Hund < 10) {
		trame[6] = 0x30;
		trame[7] = Chrono_Time.Hund + 0x30;
	}
	else {
		trame[6] = Chrono_Time.Hund / 10 + 0x30;
		trame[7] = Chrono_Time.Hund % 10 + 0x30;
	}
	for (int i = 0; i <11;i++)
		LL_USART_TransmitData8(USARTx_used,trame[i]);
	
}


