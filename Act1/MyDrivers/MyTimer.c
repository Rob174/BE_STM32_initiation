// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des p�riph�riques.
 Rem : OBLIGATION d'utiliser les d�finitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilit� du code.

 Pour les masques, utiliser �galement les d�finitions propos�e
 Rappel : pour mettre � 1  , reg = reg | Mask (ou Mask est le repr�sente le ou les bits � positionner � 1)
				  pour mettre � 0  , reg = reg&~ Mask (ou Mask est le repr�sente le ou les bits � positionner � 0)
 
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
	
	// Identifier quel TIM est donn� en param�tre et activer celui voulu
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
	
	// Identifier quel TIM est donn� en param�tre et d�sactiver celui voulu
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
	* @brief  Active l'horloge et r�gle l'ARR et le PSC du timer vis�
  * @note   Fonction � lancer avant toute autre. Le timer n'est pas encore lanc� (voir MyTimerStart)
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
	* 				int Arr   : valeur � placer dans ARR
	*					int Psc   : valeur � placer dans PSC
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