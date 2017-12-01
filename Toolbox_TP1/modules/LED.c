#include "stm32l1xx_nucleo.h"
#include "fonctions.h"

void init_led()
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
   
   //Parametrage GPIOB en push-pull mode
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_1;  //SET OTYPER1 à 0
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_2;
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_10;
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_11;
}


void led ()
{
        GPIOB->ODR |= (GPIO_ODR_ODR_1);     /* state=1 means the led is turned on */
        GPIOB->ODR |= (GPIO_ODR_ODR_2);
        wait(1000000);
         GPIOB->ODR &= ~ (GPIO_ODR_ODR_1);
        GPIOB->ODR &= ~ (GPIO_ODR_ODR_2);
}
void led_eteinte ()
{
        GPIOB->ODR &= ~ (GPIO_ODR_ODR_1);
        GPIOB->ODR &= ~ (GPIO_ODR_ODR_1);
}