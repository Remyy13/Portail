/******************************************************************************/
/*                                                                            */
/*                      MAIN BASIQUE STM32L152RE                              */
/*                                                                            */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/*                       ELodie PAULY                                         */
/*                        Février 2016                                       */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/*                                                                            */
/*                                                                            */
/******************************************************************************/



#include "stm32l1xx_nucleo.h"

uint8_t etat;
uint8_t etat1;
uint8_t etat2;
uint8_t etat3;
void enable_interrupt_ext(void);
void init_led(void);
void init_BP(void);
void wait(uint32_t tmp);


void main()
{
  init_led();                   // On lance le programme d'initialisation des leds
  enable_interrupt_ext();       // On active les interruptions externe
  init_BP();                    // On lance le programme d'initialisation des boutons poussoirs

  /* int index;*/

  
  GPIOB->ODR &= ~((1<<1) | (1<<2));     // Led eteinte sous la mise sous tension Cours 1 Slide 43 On active le registre en OUTPUT

  etat =0;
  etat1 =0;


  while(1)
  {
    /* Program Scrutation */
    if((GPIOA->IDR & (1<<11)) == 0)      /* when the BP is pushed (pull down) the state variable is checked */
    {
      wait(100000);
      if(etat==0) /* state=0 means the led is turned off */
      {
        GPIOB->ODR &= ~(1<<1);
        GPIOB->ODR &= ~(1<<2);
        etat = 1;
      }
      else
      {
        GPIOB->ODR |= (1<<1);     /* state=1 means the led is turned on */
        GPIOB->ODR |= (1<<2);
        etat = 0;
      }
    }
    
    if((GPIOA->IDR & (1<<12)) == 0)
    {
      if(etat1==0) /* state=0 means the led is turned off */
      {
        GPIOB->ODR &= ~(1<<10);
        GPIOB->ODR &= ~(1<<11);
        etat1 = 1;
      }
      else
      {
        GPIOB->ODR |= (1<<10);     /* state=1 means the led is turned on */
        GPIOB->ODR |= (1<<11);
        etat1 = 0;
      }
    }    
  }
}


void init_BP(void)
{
{
      /* peripherical activation AHB for the GPIO ports A, B et C */
      RCC->AHBENR |= RCC_AHBENR_GPIOAEN;        /* force b1 at 1 activating the GPIOB on AHB data bus */
  
       /* set the port GPIOA as an input respectively for BP1, BP2*/
      GPIOA->MODER &= ~((GPIO_MODER_MODER11_0) | (GPIO_MODER_MODER11_1));     /* Set MODER11 to 00 input for BP1 */
     
      GPIOA->MODER &= ~((GPIO_MODER_MODER12_0) | (GPIO_MODER_MODER12_1));     /* Set MODER11 to 00 input for BP1 */
      
      GPIOA->PUPDR &= ~((GPIO_PUPDR_PUPDR11_0) | (GPIO_PUPDR_PUPDR11_1));     /* set PUDR of BP1 to 00 floating in this case */
     
      GPIOA->PUPDR &= ~((GPIO_PUPDR_PUPDR12_0) | (GPIO_PUPDR_PUPDR12_1));     /* set PUDR of BP1 to 00 floating in this case */
}
}

void init_led(void)
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

void wait(uint32_t tmp)
{
  uint32_t i;
  for (i =0; i<tmp; i++);
}

void enable_interrupt_ext(void)
{
  NVIC->ISER[1] |= (1<<8);              // PA11 et PA12 Bit 8 vecteur d'interruption 40 cours 2 slide 40 
  NVIC->ISER[0] |= (1<<23);             // PC5 et PC6  vecteur d'interruption 23 Cours 2 slide 13
  
  RCC->APB2ENR |= (1<<0);               // active l'horloge sur le bus APB2 pour le peripherique EXTI
  
  EXTI->IMR |= (1<<11);                 // masquage Cours 2 slide 23 (on veut que l'interruption soit non masquable == visible(non masqué))
  EXTI->FTSR |= (1<<11);                // front descendant on met 1 dans le bit 11 car PA11 (RTSR pour front montant)
  
  EXTI->IMR |= (1<<12);                 // masquage Cours 2 slide 23
  EXTI->FTSR |= (1<<  12);                // front descendant on met 1 dans le bit 12 car PA12 donc TR12
  
  EXTI->IMR |= (1<<6);                 // masquage Cours 2 slide 23
  EXTI->FTSR |= (1<<6);                // front descendant on met 1 dans le bit 6 car PC6
  
  EXTI->IMR |= (1<<5);                 // masquage Cours 2 slide 23
  EXTI->FTSR |= (1<<5);                // front descendant on met 1 dans le bit 5 car PC5
  
  SYSCFG->EXTICR[2] &= ~(1<<12);        // 8.5.4 Cours 2 Slide 27
  SYSCFG->EXTICR[2] &= ~(1<<13);        // on est sur PA11 donc registre EXTICR[2]
  SYSCFG->EXTICR[2] &= ~(1<<14);        //BTN1
  SYSCFG->EXTICR[2] &= ~(1<<15);
  
  
  SYSCFG->EXTICR[3] &= ~(1<<0);         // 8.5.4 Cours 2 Slide 27
  SYSCFG->EXTICR[3] &= ~(1<<1);         // on est sur PA12 donc registre EXTICR[3] donc 0000 car on est sur A
  SYSCFG->EXTICR[3] &= ~(1<<2);         //BTN2
  SYSCFG->EXTICR[3] &= ~(1<<3);
  
  SYSCFG->EXTICR[1] &= ~(1<<8);         // 8.5.4 Cours 2 Slide 27
  SYSCFG->EXTICR[1] |=  (1<<9);         // on est sur PC6 donc registre EXTICR[1] donc 0010 car on est sur C
  SYSCFG->EXTICR[1] &= ~(1<<10);        //BTN3
  SYSCFG->EXTICR[1] &= ~(1<<11);
  
  SYSCFG->EXTICR[1] &= ~(1<<4);         // 8.5.4 Cours 2 Slide 27
  SYSCFG->EXTICR[1] |= (1<<5);         // on est sur PC5 donc registre EXTICR[1] donc 0010 car on est sur C
  SYSCFG->EXTICR[1] &= ~(1<<6);         //BTN4
  SYSCFG->EXTICR[1] &= ~(1<<7);                 //0010 = 7654
}


  