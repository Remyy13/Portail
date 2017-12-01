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

// D�clarations des fonctions
void init_GPIO_TIM3(uint16_t dutycycle) ;      // Initialisation du TIM3 pour faire sonner le Buzzer, le MiniMoteur et les servo moteurs
void init_PWM_BUZZ(void);       // Initialisation du PWM pour faire sonner le Buzzer sur le TIM3_CH2 sur PC7
void init_BUZZER(void);         // Initialisation pour faire sonner le Buzzer
void wait(uint32_t tmp);        // Fonction Attendre (pour cr�er les secondes)
void init_PWM_MINIMOT(void);    // Initialisation du PWM pour faire tourner le MiniMot sur le TIM3_CH1 sur PB4
void init_minimot(void);        // Initialisation pour faire tourner le MiniMot
void init_PWM_MOT1(void);       // Initialisation du PWM pour faire tourner le Servo Moteur 1 sur le TIM3_CH3 sur PC8
void init_MOT1(void);           // Initialisation pour faire tourner le Servo Moteur 1
void init_PWM_MOT2(void);       // Initialisation du PWM pour faire tourner le Servo Moteur 2 sur le TIM3_CH4 sur PC9
void init_MOT2(void);           // Initialisation pour faire tourner le Servo Moteur 2

void init_GPIO_ADC(void);
void init_ADC(void);

void main()
{ 
   
  // Initialisation Programme
 // init_GPIO_TIM3(dutycycle);
 // init_PWM_BUZZ(); 
 // init_BUZZER();
 // init_PWM_MINIMOT();
 // init_minimot();
  init_MOT1();
  init_PWM_MOT1();
  
   
  //   init_GPIO_TIM3(2000);  
 // init_PWM_MOT2();
//  init_MOT2();
  
   init_GPIO_TIM3(500);     
   TIM4->CR1|= TIM_CR1_CEN ;                        // On active le compteur 
   wait(5000000); 
   TIM4->CR1 &= ~ TIM_CR1_CEN ; 
   
    init_GPIO_TIM3(2000);     
   TIM4->CR1|= TIM_CR1_CEN ;                        // On active le compteur 
   wait(5000000); 
   TIM4->CR1 &= ~ TIM_CR1_CEN ; 
   
  
  // Partie Fonctionnelle
  while(1)
  {        
   init_GPIO_TIM3(500);     
   TIM4->CR1|= TIM_CR1_CEN ;                        // On active le compteur 
   wait(5000000); 
   TIM4->CR1 &= ~ TIM_CR1_CEN ; 
   
    init_GPIO_TIM3(2000);     
   TIM4->CR1|= TIM_CR1_CEN ;                        // On active le compteur 
   wait(5000000); 
   TIM4->CR1 &= ~ TIM_CR1_CEN ; 
   
   
  }                                   // fin du WHILE
}                                     // fin  du Main


void wait(uint32_t tmp)                                   // Fonction Attendre (pour cr�er les secondes)
{
   uint32_t i = 0;
   for(i = 0; i < tmp; i++);
}

// D�but de la configuration du Timer3

void init_GPIO_TIM3(uint16_t dutycycle)                                 // Initialisation du TIM3 pour faire sonner le Buzzer, le MiniMoteur et les servo moteurs
{
 // RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;                     // Activation de l'horloge sur le Timer3 (TIM3)
 // TIM4->CR1|= TIM_CR1_ARPE;                        // On active l'auto-reload
  TIM4->CR1 &= ~ TIM_CR1_CMS_0;      // On active le mode edge-aligned
  TIM4->CR1 |= TIM_CR1_CMS_1;
  TIM4->CR1 &= ~  TIM_CR1_DIR;                         // On active le mode compteur (upcounter)  
  TIM4->PSC = 8;     //on est a 100kH                                // On divise la fr�quence de la clk de la carte (avant = 439)
  TIM4->ARR = 10000;     //on compte 500 coup dhorloge               // On choisi la valeur � laquelle notre compteur doit aller (avant = 39) 4990
 // TIM4->CCR4= 100;     // regle le rapport cyclique 4950
  TIM4->CCR3= dutycycle;
// il faut �tre entre 550 et 2330 us
}

// D�but de la configuration du buzzer

void init_PWM_BUZZ(void)                                  // Initialisation du PWM pour faire sonner le Buzzer sur le TIM3_CH2 sur PC7
{
  TIM3->CCMR1 &= ~ (TIM_CCMR1_CC2S_0 | TIM_CCMR1_CC2S_0); // On d�finie la sortie channel 2 en output pour activer le PWM
  TIM3->CCMR1 &= ~  TIM_CCMR1_OC2M_0;                     // On choisie le mode PWM mode 1
  TIM3->CCMR1 |=   (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2); // On choisie le mode PWM mode 1  
  TIM3->CCR1   = 20;                                      // On choisie la valeur de comparaison. Le signal passe a 1 quand le compteur d�passe cette valeur
  TIM3->CCER  |= TIM_CCER_CC2E;                           // On active la comparaison sur le channel 2
  TIM3->CCR2   = 1;                                       // Dutycycle
}

void init_BUZZER(void)                                    // Initialisation pour faire sonner le Buzzer
{
  RCC->AHBENR   |=   RCC_AHBENR_GPIOCEN;                  // Activation de l'horloge sur le Port C (GPIOC) car le buzzer est sur PC7
  GPIOC->MODER  |=   GPIO_MODER_MODER7_1;                 // On configure le MODER7 (car le buzzer est sur PC7) en alternate function pour acc�der au TIM3 sur AF2
  GPIOC->MODER  &= ~ GPIO_MODER_MODER7_0;                 // On configure le MODER7 (car le buzzer est sur PC7) en alternate function pour acc�der au TIM3 sur AF2
  GPIOC->OTYPER &= ~ GPIO_OTYPER_OT_7;                    // On configure en mode push-pull
  GPIOC->AFR[0] |= (1<<29);                               // Registre de s�l�ction de la fonction multiplex�e
                                                          // AFR[0] le crochet 0 = AFRL LOW car on est sur PC7 (AFRL7). PC7 donc sur les bits 28 � 31.
                                                          // On est sur TIM3 donc ==> AF2 ==>0010 ==> donc on met le bit 29 � 1 car par defaut les autres sont � 0
}

// D�but de la configuration du mini moteur

void init_PWM_MINIMOT(void)                               // Initialisation du PWM pour faire tourner le MiniMot sur le TIM3_CH1 sur PB4
{
  TIM3->CCMR1 &= ~(TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC1S_1);    // on definie la sortie channel 1 en output 
  TIM3->CCMR1 &= ~TIM_CCMR1_OC1M_0;                         // on choisie mode PWM mode 1
  TIM3->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);     // on choisie mode PWM mode 1
  TIM3->CCR1 = 45;                                          // On choisie la valeur de comparaison. Le signal passe a 1 quand le compteur d�passe cette valeur
  TIM3->CCER |= TIM_CCER_CC1E ;                             // On active la comparaison sur le channel 1 
    //20
}

void init_minimot(void)                                   // Initialisation pour faire tourner le MiniMot
{
  RCC->AHBENR  |=   RCC_AHBENR_GPIOBEN;                  // Activation de l'horloge sur le Port B (GPIOB) car le minimot est sur PB4               
  GPIOB->MODER |=   GPIO_MODER_MODER4_1;                 // On configure le MODER4 (car le minimot est sur PB4) en alternate function pour acc�der au TIM3 sur AF2
  GPIOB->MODER &= ~ GPIO_MODER_MODER4_0;                 // On configure le MODER4 (car le minimot est sur PB4) en alternate function pour acc�der au TIM3 sur AF2              
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_4;                   // On configure en mode push-pull
  GPIOB->AFR[0] |= (1<<17);                              // Registre de s�l�ction de la fonction multiplex�e
                                                         // AFR[0] le crochet 0 = AFRL LOW car on est sur PB4 (AFRL4). PB4 donc sur les bits 16 � 19.
                                                         // On est sur TIM3 ==> AF2 ==>0010 ==> donc on met le bit 17 � 1 car par defaut on est a 0
}

// D�but de la configuration du servo moteur 1

void init_PWM_MOT1(void)                                  // Initialisation du PWM pour faire tourner le Servo Moteur 1 sur le TIM3_CH3 sur PC8
{
   RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;  
  TIM4->CCMR2 &= ~(TIM_CCMR2_CC3S_0 | TIM_CCMR2_CC3S_1);    // on definie la sortie channel 3 en output
 TIM4->CCMR2 &= ~TIM_CCMR2_OC3M_0;                         // on choisie mode PWM mode 1
  TIM4->CCMR2 |= ( TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2);     // on choisie mode PWM mode 1
 // TIM3->CCR1 = 100;                                       // On choisie la valeur de comparaison. Le signal passe a 1 quand le compteur d�passe cette valeur
  TIM4->CCER |= TIM_CCER_CC3E ;                             // On active la comparaison sur le channel 3 
}

void init_MOT1(void)                                      // Initialisation pour faire tourner le Servo Moteur 1
{ 
  RCC->AHBENR  |=   RCC_AHBENR_GPIOBEN;                  // Activation de l'horloge sur le Port C (GPIOC) car le servo mot est sur PC8              
  GPIOB->ODR  &= ~ GPIO_ODR_ODR_8;
  GPIOB->MODER |=   GPIO_MODER_MODER8_1;                 // On configure le MODER8 (car le minimot est sur PC8) en alternate function pour acc�der au TIM3 sur AF2
  GPIOB->MODER &= ~ GPIO_MODER_MODER8_0;                 // On configure le MODER8 (car le minimot est sur PC8) en alternate function pour acc�der au TIM3 sur AF2              
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_8;                   // On configure en mode push-pull
  
  GPIOB->AFR[1] |= (2<<0);                               // Registre de s�l�ction de la fonction multiplex�e
                                                         // AFR[1] le crochet 1 = AFRL HIGH car on est sur PC8 (AFRH8). PC8 donc sur les bits 0 � 3.
                                                         // On est sur TIM3 ==> AF2 ==>0010 ==> donc on met le bit 1 � 1 car par defaut on est a 0
}

// D�but de la configuration du servo moteur 2
/*
void init_PWM_MOT2(void)                                  // Initialisation du PWM pour faire tourner le Servo Moteur 2 sur le TIM3_CH4 sur PC9
{
  TIM3->CCMR1 &= ~(TIM_CCMR2_CC4S_0 | TIM_CCMR2_CC4S_1);    // on definie la sortie channel 4 en output
  TIM3->CCMR1 &= ~TIM_CCMR1_OC1M_0;                         // on choisie mode PWM mode 1
  TIM3->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);     // on choisie mode PWM mode 1
  TIM3->CCR1 = 20;                                          // On choisie la valeur de comparaison. Le signal passe a 1 quand le compteur d�passe cette valeur
  TIM3->CCER |= TIM_CCER_CC4E ;                             // On active la comparaison sur le channel 4 
}

void init_MOT2(void)                                      // Initialisation pour faire tourner le Servo Moteur 2
{
  RCC->AHBENR  |=   RCC_AHBENR_GPIOCEN;                  // Activation de l'horloge sur le Port C (GPIOC) car le servo moteur est sur PC9              
  GPIOB->MODER |=   GPIO_MODER_MODER9_1;                 // On configure le MODER9 (car le minimot est sur PC9) en alternate function pour acc�der au TIM3 sur AF2
  GPIOB->MODER &= ~ GPIO_MODER_MODER9_0;                 // On configure le MODER9 (car le minimot est sur PC9) en alternate function pour acc�der au TIM3 sur AF2              
  GPIOB->OTYPER &= ~ GPIO_OTYPER_OT_9;                   // On configure en mode push-pull
  GPIOB->AFR[1] |= (1<<5);                              // Registre de s�l�ction de la fonction multiplex�e
                                                         // AFR[1] le crochet 1 = AFRL HIGH car on est sur PC9 (AFRH9). PC9 donc sur les bits 4 � 7.
                                                         // On est sur TIM3 ==> AF2 ==>0010 ==> donc on met le bit 5 � 1 car par defaut on est a 0
}*/

