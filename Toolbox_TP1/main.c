/******************************************************************************/
/*                                                                            */
/*                      MAIN BASIQUE STM32L152RE                              */
/*                                                                            */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/*                        PROJET STM32                                        */
/*                       D�cembre  2017                                       */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/*                                                                            */
/*                                                                            */
/******************************************************************************/



#include "stm32l1xx_nucleo.h"
#include "modules/fonctions.h"


// On commence par initialiser les diff�rents composants de la carte :
//      - On configure les GPIOs (Donc les GPIOA, GPIOB, GPIOC)
//      - On configure les servo-moteurs (Donc PWM_SERVO3 et PWM_SERVO4)
//              * On configure le Timer3 (Donc sur les canaux CH3 et CH4)
//              * On configure le PWM_MOT1
//              * On configure le PWM_MOT2
//      - On configure les afficheurs
//              * On configure les LEDs (Donc les port PBs)
//              * On configure l'afficheur 7 segments
//                      - On configure la Liaison SPI (Donc la SPI1)
//                      - On configure l'initialisation des afficheurs (Initialement tous �teints)
//              * On configure le Buzzer
//                      - On configure le Timer3 (Donc sur le canal CH2)
//                      - On configre le PWM_BUZZ
//      - On configure les Boutons Poussoirs (Donc sur les port PAs et PCs)
//      - On configure le Capteur de mouvement
//              * On configure la liaison I2C (Donc sur l'I2C1)
//      - On configure les Potentiom�tres
//              * On configure le convertisseur Analogique/Num�rique (Donc ADC1)
//      - On configure la communication avec le PC
//              * On configure la liaison UART (Donc sur USART2)
//      - On configure l'Etat Initial (Le portail est ferm�)


//Variables

//uint8_t etat;


 uint8_t etat;
  uint8_t etat_interuption;

void wait(uint32_t tmp)
{
  uint32_t i;
  for (i =0; i<tmp; i++);
}

void main()
{


//GPIO_init_USART2();
     //uart2_config();
  
     
  //enable_interrupt_ext();       // On active les interruptions externe
  //init_gpio_bouton_poussoir();                    // On lance le programme d'initialisation des boutons poussoirs

GPIO_init_USART2();
uart2_config();   
Init_Led();                   // On lance le programme d'initialisation des leds
enable_interrupt_ext();       // On active les interruptions externe
init_gpio_bouton_poussoir();                    // On lance le programme d'initialisation des boutons poussoirs


  /* int index;*/
 
GPIOB->ODR &= ~ ((GPIO_ODR_ODR_1) | (GPIO_ODR_ODR_2));     // Led eteinte sous la mise sous tension Cours 1 Slide 43 On active le registre en OUTPUT

etat =0;
etat_interuption =0;
init_SPI();
init_MAX();
init_TIM2();
enable_interrupt_ext();
  

  
init_SPI();
init_MAX();
init_TIM2();
enable_interrupt_ext();
  
SPI_Write(0x09,0x00); // code B decode desactiv�

  while(1)
  {
 
    
    if((GPIOA->IDR & (GPIO_IDR_IDR_11)) == 0)      /* when the BP is pushed (pull down) the state variable is checked */
    {
    
    }
    uart_rx();
     // uart_rx();
       /* Program Scrutation */

    // Je lis mon capteur 
        // S'il y a quelqu'un => je dois ouvrir
                //  Si la donn�e capteur > valeur seuil  et que le portail est ferm� alors j'actionne mon moteur pour ouvrir et mes leds doivent clignoter et le buzzer doit sonner et l'afficheur doit afficher OPENING
                    // Si le portail atteint sa position ouverte on arrete les moteurs et on d�marre la temporisation
                        // Si fin tempo et on ne d�tecte pas d'obstacle on actionne le moteur pour fermer  et mes leds doivent clignoter et le buzzer doit sonner et l'afficheur doit afficher CLOSING             
                            // Si le portail atteint sa position ferm�e on arr�te les moteurs
                            // Si le portail n'a pas encore atteint sa position ferm�e et que l'on d�tecte un obstacle on r�ouvre la porte et mes leds clignotent
                                // Si pendant la r�ouverture on d�tecte un obstacle on arr�te les moteurs et on d�marre la temporisation
                                    // Si fin tempo et pas d'obstacle on ferme et mes leds clignotent
                                    // Si fin tempo et on d�tecte un obstacle on relance la tempo
                        // Si fin tempo et on detecte un obstacle on relance la tempo
                    // Si le portail n'a pas encore atteint sa position ouverte et que l'on d�tecte un obstacle on referme et mes leds clignotent
                                // Si pendant la refermeture on d�tecte un obstacle on arr�te les moteurs et on d�marre la temporisation
                                    // Si fin tempo et pas d'obstacle on r�ouvre et mes leds clignotent
                                    // Si fin tempo et on d�tecte un obstacle on relance la tempo
        

        // S'il y a personne je fais rien
                // Si la donn�e capteur < valeur on fait rien
        
// EN DEHORS DU WHILE CAR C'EST UNE INTERRUPTION        
    // J'appuie sur l'interphone
            // J'attends le retour de Labview et on lance une tempo (commande d'ouverture on lance le cycle)
    
            // Si a la fin de la tempo et que je n'ai pas de retour Labview je sors de l'interruption
    
    
    
    
  }
  

}
