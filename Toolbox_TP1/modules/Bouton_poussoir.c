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