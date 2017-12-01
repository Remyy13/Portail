/******************************************************************************/
/*                                                                            */
/*                      MAIN BASIQUE STM32L152RE                              */
/*                                                                            */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/*                       Briton, Adrien                                       */
/*                       Décembre 2017                                        */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/*                                                                            */
/*                                                                            */
/******************************************************************************/


#include <stdlib.h>
#include "stm32l1xx_nucleo.h"
#include "time.h"


void Delay(uint32_t Tempo)
{
    while (Tempo--);
}

void Init_Led()
{
  RCC->AHBENR |= (1<<1); //On passe le bit un a 1 pour AHB

   //Parametrage GPIOB en sortie
   
  GPIOB->MODER |= GPIO_MODER_MODER1_0;  //Led 1
  GPIOB->MODER &= ~GPIO_MODER_MODER1_1;
  GPIOB->MODER |= GPIO_MODER_MODER2_0;  //Led 2
  GPIOB->MODER &= ~GPIO_MODER_MODER2_1;
  GPIOB->MODER |= GPIO_MODER_MODER10_0;  /*Led 10*/
  GPIOB->MODER &= ~GPIO_MODER_MODER10_1;
  GPIOB->MODER |= GPIO_MODER_MODER11_0;  /*Led 11*/
  GPIOB->MODER &= ~GPIO_MODER_MODER11_1;
  GPIOB->MODER |= GPIO_MODER_MODER12_0;  /*Led 12*/
  GPIOB->MODER &= ~GPIO_MODER_MODER12_1;
  GPIOB->MODER |= GPIO_MODER_MODER13_0;  /*Led 13*/
  GPIOB->MODER &= ~GPIO_MODER_MODER13_1;
  GPIOB->MODER |= GPIO_MODER_MODER14_0;  /*Led 14*/
  GPIOB->MODER &= ~GPIO_MODER_MODER14_1;
  GPIOB->MODER |= GPIO_MODER_MODER15_0;  /*Led 15*/
  GPIOB->MODER &= ~GPIO_MODER_MODER15_1;
   
   //Parametrage GPIOB en push-pull mode
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_1;  //SET OTYPER1 à 0
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_2;
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_10;
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_11;
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_12;
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_13;
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_14;
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_15;
}
  
void clignotement_LED()
{
    int i = 0;
    
    GPIOB->ODR |= GPIO_ODR_ODR_1; //passage du port 1 du GPIOB à 1 (allumage)
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_2;
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_10;
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_11;
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_12;
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_13;
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_14;
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_15;
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_1; //passage du port 1 du GPIOB à 1 (allumage)
    Delay(50000);
    GPIOB->ODR &=~ GPIO_ODR_ODR_1; //passage du port 1 du GPIOB à 1 (allumage)
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_2;
    Delay(50000);
    GPIOB->ODR &=~ GPIO_ODR_ODR_2;
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_10;
    Delay(50000);
    GPIOB->ODR &=~ GPIO_ODR_ODR_10;
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_11;
    Delay(50000);
    GPIOB->ODR &=~ GPIO_ODR_ODR_11;
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_12;
    Delay(50000);
    GPIOB->ODR &=~ GPIO_ODR_ODR_12;
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_13;
    Delay(50000);
    GPIOB->ODR &=~ GPIO_ODR_ODR_13;
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_14;
    Delay(50000);
    GPIOB->ODR &=~ GPIO_ODR_ODR_14;
    Delay(50000);
    GPIOB->ODR |= GPIO_ODR_ODR_15;
    Delay(50000);
    GPIOB->ODR &=~ GPIO_ODR_ODR_15;
    Delay(50000);
    i++;
}