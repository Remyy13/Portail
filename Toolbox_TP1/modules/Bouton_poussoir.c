#include "stm32l1xx_nucleo.h"
#include "fonctions.h"



void init_gpio_bouton_poussoir(void)
{
      /* peripherical activation AHB for the GPIO ports A, B et C */
      RCC->AHBENR |= RCC_AHBENR_GPIOAEN;        /* force b1 at 1 activating the GPIOB on AHB data bus */
      
       /* set the port GPIOA as an input respectively for BP1, BP2*/
      GPIOA->MODER &= ~((GPIO_MODER_MODER11_0) | (GPIO_MODER_MODER11_1));     /* Set MODER11 to 00 input for BP1 */   
      GPIOA->MODER &= ~((GPIO_MODER_MODER12_0) | (GPIO_MODER_MODER12_1));     /* Set MODER11 to 00 input for BP1 */    
      GPIOA->PUPDR &= ~((GPIO_PUPDR_PUPDR11_0) | (GPIO_PUPDR_PUPDR11_1));     /* set PUDR of BP1 to 00 floating in this case */    
      GPIOA->PUPDR &= ~((GPIO_PUPDR_PUPDR12_0) | (GPIO_PUPDR_PUPDR12_1));     /* set PUDR of BP1 to 00 floating in this case */
}
/*
void enable_interrupt_ext(void)
{
  NVIC->ISER[1] |= NVIC_ISER_SETENA_8;              // PA11 et PA12 Bit 8 vecteur d'interruption 40 cours 2 slide 40 
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;               // active l'horloge sur le bus APB2 pour le peripherique EXTI
  EXTI->IMR |= EXTI_IMR_MR11;                 // masquage Cours 2 slide 23 (on veut que l'interruption soit non masquable == visible(non masqué))
  EXTI->FTSR |= EXTI_FTSR_TR11;                // front descendant on met 1 dans le bit 11 car PA11 (RTSR pour front montant)
  EXTI->IMR |= EXTI_IMR_MR12;                 // masquage Cours 2 slide 23
  EXTI->FTSR |= EXTI_FTSR_TR12;                // front descendant on met 1 dans le bit 12 car PA12 donc TR12
  SYSCFG->EXTICR[2] &= ~ SYSCFG_EXTICR2_EXTI7_PA;        // 8.5.4 Cours 2 Slide 27
                                                        // on est sur PA11 donc registre EXTICR[2]
                                                        //BTN1

  SYSCFG->EXTICR[3] &= ~ SYSCFG_EXTICR3_EXTI8_PA;         // 8.5.4 Cours 2 Slide 27
                                                          // on est sur PA12 donc registre EXTICR[3] donc 0000 car on est sur A                                                     //BTN2
}*/
