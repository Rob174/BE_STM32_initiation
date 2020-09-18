// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des périphériques.
 Rem : OBLIGATION d'utiliser les définitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilité du code.

 Pour les masques, utiliser également les définitions proposée
 Rappel : pour mettre à 1  , reg = reg | Mask (ou Mask est le représente le ou les bits à positionner à 1)
				  pour mettre à 0  , reg = reg&~ Mask (ou Mask est le représente le ou les bits à positionner à 0)
 
*/ 
#include "stm32f103xb.h" 

void MyTimer_Start(TIM_TypeDef * Timer)
{
	/*
	Timer = TIM1, TIM2, TIM3 ou TIM4:
	
	TIM1EN: TIM1 timer clock enable
		Set and cleared by software.
		0: TIM1 clock disabled
		1: TIM1 clock enabled
	*/
	
	// Identifier quel TIM est donné en paramètre et activer celui voulu
	if(Timer == TIM1)
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	else if(Timer == TIM2)
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	else if(Timer == TIM3)
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	else if(Timer == TIM4)
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
}

void MyTimer_Stop(TIM_TypeDef * Timer)
{
	/*
	Timer = TIM1, TIM2, TIM3 ou TIM4:
	
	TIM1EN: TIM1 timer clock enable
		Set and cleared by software.
		0: TIM1 clock disabled
		1: TIM1 clock enabled
	*/
	
	// Identifier quel TIM est donné en paramètre et désactiver celui voulu
	if(Timer == TIM1)
		RCC->APB2ENR &= ~(RCC_APB2ENR_TIM1EN);
	else if(Timer == TIM2)
		RCC->APB1ENR &= ~(RCC_APB1ENR_TIM2EN);
	else if(Timer == TIM3)
		RCC->APB1ENR &= ~(RCC_APB1ENR_TIM3EN);
	else if(Timer == TIM4)
		RCC->APB1ENR &= ~(RCC_APB1ENR_TIM4EN);
}
/**
	* @brief  Active l'horloge et règle l'ARR et le PSC du timer visé
  * @note   Fonction à lancer avant toute autre. Le timer n'est pas encore lancé (voir MyTimerStart)
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
	* 				int Arr   : valeur à placer dans ARR
	*					int Psc   : valeur à placer dans PSC
  * @retval None
  */

void MyTimer_Conf(TIM_TypeDef * Timer,int Arr, int Psc) {
	//active l'horloge pour les timers 2,3,4,5,6,7,12,13,14
	if (Timer == TIM1 ) {
		// On active l'horloge du timer 1
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN ;
	}
	else if (Timer == TIM2 ) {
		// On active l'horloge des timers 2
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	else if (Timer == TIM3) {
		// On active l'horloge des timers 3
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	else if (Timer == TIM4) {
		// On active l'horloge des timers 4
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	Timer->CR1 &= ~TIM_CR1_OPM;
	// On met la valeur d'auto-reload dans le registre correspondant
	Timer->ARR = Arr;
	// Active la sortie du prescaler CK_CNT
	Timer->CR1 |= TIM_CR1_CEN;
	Timer->PSC = Psc;
}