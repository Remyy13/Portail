/******************************************************************************/
/*                                                                            */
/*                      MAIN BASIQUE STM32L152RE                              */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/*                              Jonathan MANEIRO                              */
/*                                                                            */
/*         PWM + BUZZER + MiniMot + Servo Mot 1 + Servo Mot 2                 */
/*                                                                            */
/******************************************************************************/

#include "stm32l1xx_nucleo.h"

// Déclarations des fonctions
void init_GPIO_TIM4(uint16_t dutycycle) ;      // Initialisation du TIM3 pour faire sonner le Buzzer, le MiniMoteur et les servo moteurs
void init_PWM_BUZZ(void);       // Initialisation du PWM pour faire sonner le Buzzer sur le TIM3_CH2 sur PC7
void init_BUZZER(void);         // Initialisation pour faire sonner le Buzzer
void wait(uint32_t tmp);        // Fonction Attendre (pour créer les secondes)
void init_PWM_MINIMOT(void);    // Initialisation du PWM pour faire tourner le MiniMot sur le TIM3_CH1 sur PB4
void init_minimot(void);        // Initialisation pour faire tourner le MiniMot
void init_PWM_MOT1(void);       // Initialisation du PWM pour faire tourner le Servo Moteur 1 sur le TIM4_CH3 sur PB8
void init_MOT1(void);           // Initialisation pour faire tourner le Servo Moteur 1
void init_PWM_MOT2(void);       // Initialisation du PWM pour faire tourner le Servo Moteur 2 sur le TIM4_CH4 sur PB9
void init_MOT2(void);           // Initialisation pour faire tourner le Servo Moteur 2

void init_tim3(void);
void open_gate (void);



void main()
{ 
  // Initialisation Programme
 // init_GPIO_TIM4(dutycycle);
  init_PWM_BUZZ(); 
  init_BUZZER();
  init_PWM_MINIMOT();
  init_minimot();
  init_MOT1();
  init_PWM_MOT1();
 // init_PWM_MOT2();
 // init_MOT2();
  init_tim3();
  open_gate ();


 /*
   init_GPIO_TIM4(500);     
   TIM4->CR1|= TIM_CR1_CEN ;                        // On active le compteur 
   wait(4000000); 
   TIM4->CR1 &= ~ TIM_CR1_CEN ; 

   init_GPIO_TIM4(2000);     
   TIM4->CR1|= TIM_CR1_CEN ;                        // On active le compteur 
   wait(4000000); 
   TIM4->CR1 &= ~ TIM_CR1_CEN ; 
  */
  
  // Partie Fonctionnelle
  while(1)
  {        

  }                                   // fin du WHILE
}                                     // fin  du Main


void wait(uint32_t tmp)                                   // Fonction Attendre (pour créer les secondes)
{
   uint32_t i = 0;
   for(i = 0; i < tmp; i++);
}

// Début de la configuration du Timer3

void init_GPIO_TIM4(uint16_t dutycycle)                                 // Initialisation du TIM4 pour utiliser les servo moteurs
{
  TIM4->CR1 &= ~ TIM_CR1_CMS_0;                         // On active le mode edge-aligned
  TIM4->CR1 |= TIM_CR1_CMS_1;
  TIM4->CR1 &= ~  TIM_CR1_DIR;                          // On active le mode compteur (upcounter)  
  TIM4->PSC = 8;                                        // On divise la fréquence de la clk de la carte | On est à XX kHz
  TIM4->ARR = 10000;                                    // On choisi la valeur à laquelle notre compteur doit aller | On compte 10000 coup d'horloge
  TIM4->CCR3= dutycycle;                                // On choisie la valeur de comparaison. Le signal passe a 1 quand le compteur dépasse cette valeur
                                                        // il faut un dutycycle compris entre 550 et 2330 usd'après la documentation des servo moteurs
}
// IL FAUT RECONFIGURER LE TIM3 POUR LE BUZZER ET LE MINI MOT
// Début de la configuration du buzzer

void init_PWM_BUZZ(void)                                  // Initialisation du PWM pour faire sonner le Buzzer sur le TIM3_CH2 sur PC7
{
  TIM3->CCMR1 &= ~ (TIM_CCMR1_CC2S_0 | TIM_CCMR1_CC2S_0); // On définie la sortie channel 2 en output pour activer le PWM
  TIM3->CCMR1 &= ~  TIM_CCMR1_OC2M_0;                     // On choisie le mode PWM mode 1
  TIM3->CCMR1 |=   (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2); // On choisie le mode PWM mode 1  
  TIM3->CCR1   = 20;                                      // On choisie la valeur de comparaison. Le signal passe a 1 quand le compteur dépasse cette valeur
  TIM3->CCER  |= TIM_CCER_CC2E;                           // On active la comparaison sur le channel 2
  TIM3->CCR2   = 1;                                       // Dutycycle
}

void init_BUZZER(void)                                    // Initialisation pour faire sonner le Buzzer
{
  RCC->AHBENR   |=   RCC_AHBENR_GPIOCEN;                  // Activation de l'horloge sur le Port C (GPIOC) car le buzzer est sur PC7
  GPIOC->MODER  |=   GPIO_MODER_MODER7_1;                 // On configure le MODER7 (car le buzzer est sur PC7) en alternate function pour accéder au TIM3 sur AF2
  GPIOC->MODER  &= ~ GPIO_MODER_MODER7_0;                 // On configure le MODER7 (car le buzzer est sur PC7) en alternate function pour accéder au TIM3 sur AF2
  GPIOC->OTYPER &= ~ GPIO_OTYPER_OT_7;                    // On configure en mode push-pull
  GPIOC->AFR[0] |= (1<<29);                               // Registre de séléction de la fonction multiplexée
                                                          // AFR[0] le crochet 0 = AFRL LOW car on est sur PC7 (AFRL7). PC7 donc sur les bits 28 à 31.
                                                          // On est sur TIM3 donc ==> AF2 ==>0010 ==> donc on met le bit 29 à 1 car par defaut les autres sont à 0
}

// Début de la configuration du mini moteur

void init_PWM_MINIMOT(void)                               // Initialisation du PWM pour faire tourner le MiniMot sur le TIM3_CH1 sur PB4
{
  TIM3->CCMR1 &= ~(TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC1S_1);    // on definie la sortie channel 1 en output 
  TIM3->CCMR1 &= ~TIM_CCMR1_OC1M_0;                         // on choisie mode PWM mode 1
  TIM3->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);     // on choisie mode PWM mode 1
  TIM3->CCR1 = 45;                                          // On choisie la valeur de comparaison. Le signal passe a 1 quand le compteur dépasse cette valeur
  TIM3->CCER |= TIM_CCER_CC1E ;                             // On active la comparaison sur le channel 1 
    //20
}

void init_minimot(void)                                   // Initialisation pour faire tourner le MiniMot
{
  RCC->AHBENR  |=   RCC_AHBENR_GPIOBEN;                  // Activation de l'horloge sur le Port B (GPIOB) car le minimot est sur PB4               
  GPIOB->MODER |=   GPIO_MODER_MODER4_1;                 // On configure le MODER4 (car le minimot est sur PB4) en alternate function pour accéder au TIM3 sur AF2
  GPIOB->MODER &= ~ GPIO_MODER_MODER4_0;                 // On configure le MODER4 (car le minimot est sur PB4) en alternate function pour accéder au TIM3 sur AF2              
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_4;                   // On configure en mode push-pull
  GPIOB->AFR[0] |= (1<<17);                              // Registre de séléction de la fonction multiplexée
                                                         // AFR[0] le crochet 0 = AFRL LOW car on est sur PB4 (AFRL4). PB4 donc sur les bits 16 à 19.
                                                         // On est sur TIM3 ==> AF2 ==>0010 ==> donc on met le bit 17 à 1 car par defaut on est a 0
}

// Début de la configuration du servo moteur 1

void init_PWM_MOT1(void)                                  // Initialisation du PWM pour faire tourner le Servo Moteur 1 sur le TIM3_CH3 sur PC8
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;                       // Activation de l'horloge sur le Timer4 (TIM4)
  TIM4->CCMR2 &= ~(TIM_CCMR2_CC3S_0 | TIM_CCMR2_CC3S_1);    // on definie la sortie channel 3 en output
  TIM4->CCMR2 &= ~TIM_CCMR2_OC3M_0;                         // on choisie mode PWM mode 1
  TIM4->CCMR2 |= ( TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2);    // on choisie mode PWM mode 1
  TIM4->CCER |= TIM_CCER_CC3E ;                             // On active la comparaison sur le channel 3 
}

void init_MOT1(void)                                      // Initialisation pour faire tourner le Servo Moteur 1
{ 
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;                   // Activation de l'horloge sur le Port B (GPIOB) car le servo mot 1 est sur PB8              
  GPIOB->ODR &=~ GPIO_ODR_ODR_8;
  GPIOB->MODER |= GPIO_MODER_MODER8_1;                 // On configure le MODER8 (car le minimot est sur PB8) en alternate function pour accéder au TIM4 sur AF2
  GPIOB->MODER &=~ GPIO_MODER_MODER8_0;                // On configure le MODER8 (car le minimot est sur PB8) en alternate function pour accéder au TIM4 sur AF2              
  GPIOB->OTYPER &=~ GPIO_OTYPER_OT_8;                  // On configure en mode push-pull
  GPIOB->AFR[1] |= (2<<0);                             // Registre de séléction de la fonction multiplexée
                                                       // AFR[1] le crochet 1 = AFRL HIGH car on est sur PB8 (AFRH8). PB8 donc sur les bits 0 à 3.
                                                       // On est sur TIM4 ==> AF2 ==>0010 ==> donc on met le bit 1 à 1 car par defaut on est a 0
}

// Début de la configuration du servo moteur 2
/*
void init_PWM_MOT2(void)                                  // Initialisation du PWM pour faire tourner le Servo Moteur 2 sur le TIM4_CH4 sur PB9
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;                       // Activation de l'horloge sur le Timer4 (TIM4)
  TIM4->CCMR2 &= ~(TIM_CCMR2_CC4S_0 | TIM_CCMR2_CC4S_1);    // on definie la sortie channel 4 en output
  TIM4->CCMR2 &= ~TIM_CCMR2_OC4M_0;                         // on choisie mode PWM mode 1
  TIM4->CCMR2 |= (TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2);     // on choisie mode PWM mode 1
  TIM4->CCER |= TIM_CCER_CC4E ;                             // On active la comparaison sur le channel 4 
}

void init_MOT2(void)                                      // Initialisation pour faire tourner le Servo Moteur 2
{
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;                  // Activation de l'horloge sur le Port B (GPIOB) car le servo moteur est sur PB9 
  GPIOB->ODR &=~ GPIO_ODR_ODR_9;
  GPIOB->MODER |= GPIO_MODER_MODER9_1;               // On configure le MODER9 (car le minimot est sur PB9) en alternate function pour accéder au TIM4 sur AF2
  GPIOB->MODER &=~ GPIO_MODER_MODER9_0;              // On configure le MODER9 (car le minimot est sur PB9) en alternate function pour accéder au TIM4 sur AF2              
  GPIOB->OTYPER &=~ GPIO_OTYPER_OT_9;                // On configure en mode push-pull
  GPIOB->AFR[1] |= (2<<4);                           // Registre de séléction de la fonction multiplexée
                                                     // AFR[1] le crochet 1 = AFRL HIGH car on est sur PB9 (AFRH9). PB9 donc sur les bits 4 à 7.
                                                     // On est sur TIM4 ==> AF2 ==>0010 ==> donc on met le bit 5 à 1 car par defaut on est a 0
}
*/



void init_tim3(void)
{

  TIM3->CR1 |= (1<<0);              // On active le compteur DIR
        RCC->APB1ENR |= (1<<1);         // Activation de l'horloge sur le Timer3; Cours 1 Slide 27 (bit 1 a 1 pout TIM3)
      TIM3->PSC = 1599;             // Valeur du prescaler 1
                                      // PSC +1=1599+1=1600 soit F=10kHz donc T=100µs (F=16MHz/1600 = 10kHz)
      TIM3->ARR = 39;                 // On regle la valeur de l'auto reload (valeur compté max)(on met 39 pour compter jusqu'à 40)
                                      //40*100µs = 4ms de période donc ARR = 40-1 = 39
      
      TIM3->CR1 |= (1<<7);              // On active l'auto reload Cours 3 Slide 11
                                        // on active  l'ARPE (l'auto reload)
      TIM3->CR1 &= ~((1<<6) | (1<<5));  // On active le mode edge aligned Cours 3 Slide 11
                                        // D'ou on commence ??
      TIM3->CR1 &= ~(1<<4);             // On active le mode conteur (ou decompteur) Cours 3 Slide 11
                                        // soit on compte ,soit on decompte s
      
      
}

void open_gate (void)
{
  init_GPIO_TIM4(500);     
   TIM4->CR1|= TIM_CR1_CEN ;                        // On active le compteur 
   wait(4000000); 
   TIM4->CR1 &= ~ TIM_CR1_CEN ; 
  
}