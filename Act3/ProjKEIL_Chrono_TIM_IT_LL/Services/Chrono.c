// A COMPLETER

/*
Service permettant de chornom�trer jusqu'� 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 � TIMER4).
Utilise la lib MyTimers.h /.c
*/



#include "Chrono.h"
#include "MyTimer.h"
#include "stm32f1xx_ll_gpio.h"
#define ETAT_STOP 0
#define ETAT_START 1
#define ETAT_RESET 2

// variable priv�e de type Time qui m�morise la dur�e mesur�e
static Time Chrono_Time; // rem : static rend la visibilit� de la variable Chrono_Time limit�e � ce fichier 

// variable priv�e qui m�morise pour le module le timer utilis� par le module
static TIM_TypeDef * Chrono_Timer=TIM1; // init par d�faut au cas o� l'utilisateur ne lance pas Chrono_Conf avant toute autre fct.
static int etat = ETAT_STOP;

/**
	* @brief  incr�mente la variable priv�e Chron_Time modulo 60mn 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Task_10ms(void)
{ 
	Chrono_Time.Hund++;
	if (Chrono_Time.Hund==100)
	{
		Chrono_Time.Sec++;
		Chrono_Time.Hund=0;
		LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_10);
	}
	if (Chrono_Time.Sec==60)
	{
		Chrono_Time.Min++;
		Chrono_Time.Sec=0;
	}
	if (Chrono_Time.Min==60)
	{
		Chrono_Time.Hund=0;
	}
}
void Chrono_Conf_io(void) {
	//bouton USER : START / STOP
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_13,LL_GPIO_MODE_FLOATING);
	//bouton Exterieur : RESET
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_8,LL_GPIO_MODE_FLOATING);
	// LED
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_10,LL_GPIO_MODE_OUTPUT_10MHz);
	LL_GPIO_SetPinOutputType(GPIOC,LL_GPIO_PIN_10,LL_GPIO_OUTPUT_PUSHPULL);
}
void Chrono_Background(void) {
	if(LL_GPIO_IsPinLocked(GPIOC,LL_GPIO_PIN_13)) {
		if (etat == ETAT_STOP) {
			etat = ETAT_START;
			Chrono_Start();
		}
		else if (etat == ETAT_START) {
			etat = ETAT_STOP;
			Chrono_Stop();
		}
		else {
			etat = ETAT_STOP;
			Chrono_Reset();
		}
	}
}

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
	MyTimer_Conf(Chrono_Timer,999, 719);
	
	// R�glage interruption du Timer avec callback : Chrono_Task_10ms()
	MyTimer_IT_Conf(Chrono_Timer, Chrono_Task_10ms,3);
	
	// Validation IT
	MyTimer_IT_Enable(Chrono_Timer);
	
	//Configuration des 3 ios (2BP + LED)
	Chrono_Conf_io();
	//T�che de fond � ex�cuter dans le while 
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

