#include "stm32l1xx_nucleo.h"
#include "modules/fonctions.h"


#define addr_w 0x9E
#define addr_r 0x9F



//Variables
uint16_t dutycycle; //Rapport cyclique 
uint8_t data_w[5]; //variable global
uint8_t data_r[5]; //variable global
char essai[100];
uint8_t etat1=0;//bp1
uint8_t etat2=0;//bp2
uint8_t etat3=0;//bp3
uint8_t etat4=0;//bp4
uint8_t etat5; // TIM2



void init_SPI(void)
{
  etat1=0;
  etat2=0;
  etat3=0;
  etat4=0;
   RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //On passe le bit un a 1 pour AHB pour les ports A
   RCC->APB2ENR |= RCC_APB2ENR_SPI1EN ; //Horloge sur le I2C 
  
   //Paramétrages des trois pattes du MAX (PA5 PA8 et PA7)
    GPIOA->MODER &= ~ GPIO_MODER_MODER5_0;  //PA5 en Alternate fonction
    GPIOA->MODER |= GPIO_MODER_MODER5_1;
    GPIOA->MODER &= ~ GPIO_MODER_MODER7_0; //PA7 en Alternate fonction
    GPIOA->MODER |= GPIO_MODER_MODER7_1;
    GPIOA->MODER &= ~ GPIO_MODER_MODER8_1; // PA8 en sortie
    GPIOA->MODER |= GPIO_MODER_MODER8_0;
   
    GPIOA->OTYPER &= ~ GPIO_OTYPER_OT_5;  //PA5
    GPIOA->OTYPER &= ~ GPIO_OTYPER_OT_7;  //PA7
    GPIOA->OTYPER &= ~ GPIO_OTYPER_OT_8;  //PA8
  
    
// registre de selection de la fonction multiplexe pour PA5 et PA7
// SPI = AFR5 (pour PA5) => 0101 : bit 20/24 -  §7.4.9 p44
  GPIOA->AFR[0] |= 5<<20;     

// SPI = AFR5 (pour PA7) => 0101 : bit 28/32 -  §7.4.9 p44
  GPIOA->AFR[0] |= 5<<28;     
     
    // p 783 BR
  SPI1->CR1 &= ~ SPI_CR1_BR_0 ; 
  SPI1->CR1 |= SPI_CR1_BR_1 ; 
  SPI1->CR1 &= ~ SPI_CR1_BR_2 ; 
  
  SPI1->CR1 |= SPI_CR1_CPHA ; 
  SPI1->CR1 |= SPI_CR1_CPOL ;
  
  SPI1->CR1 |= SPI_CR1_MSTR ;
  SPI1->CR1 &= ~ SPI_CR1_DFF ; // données de l'afficheur sur 8 bits
  SPI1->CR1 &= ~ SPI_CR1_LSBFIRST ; 
 
  // mode only transit
  SPI1->CR1 |= SPI_CR1_BIDIOE ;
  SPI1->CR1 |= SPI_CR1_BIDIMODE ;
  
  SPI1->CR2 |=  SPI_CR2_SSOE ;
 
  // activation 
  SPI1->CR1 |= SPI_CR1_SPE ; 
   
}
  
void SPI_Write( uint8_t add,uint8_t data)
{
    Set_CS();
    Reset_CS();
    SendData_SPI(add);
    SendData_SPI(data);
    while ((SPI1->SR & SPI_SR_BSY) ==  SPI_SR_BSY);
    Set_CS();
}

void SendData_SPI (uint8_t add)
{
  while ((SPI1->SR & SPI_SR_TXE)!= SPI_SR_TXE);
  SPI1->DR = add;
}

void Set_CS(void)  // Fonction pour lancer l'afficheur
{
GPIOA->ODR |= GPIO_ODR_ODR_8 ;
}

void Reset_CS(void)  //Fonction pour reset l'afficheur
{
GPIOA->ODR &= ~ GPIO_ODR_ODR_8 ;
}

void init_MAX(void){
 //Commandes d'initialisation pour MAX
  SPI_Write(0x0C,0x00); //Eteindre MAX
  SPI_Write(0x0C,0x01); //Mode Normal
  SPI_Write(0x0A,0x0A); //Luminosité
  SPI_Write(0x09,0x0F); // code B decode activé   
  SPI_Write(0x0B,0x03); // Scan Limit
  SPI_Write(0x0F,0x00); //Display test
  
  //Initialisation des digits à 0
  SPI_Write(0x01,0x00);
  SPI_Write(0x02,0x00);
  SPI_Write(0x03,0x00);
  SPI_Write(0x04,0x00);
}






void init_TIM2(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // horloge sur APB1 car TIM2 sur APB1
  
  TIM2->SR &= ~ TIM_SR_UIF;          // init interrupt
  
  TIM2->CR1 |= TIM_CR1_CEN;          // activer le compteur  -  p11 Timer
  TIM2->CR1 &= ~ TIM_CR1_DIR;         // DIR : incrementaion 0 ou decrementation 1
  TIM2->CR1 &= ~ TIM_CR1_CMS;
  TIM2->CR1 |= TIM_CR1_ARPE;
  
  TIM2->PSC = 3199;            // 16Mz / PSC+1 pour avoir 1ms : 16000 
  TIM2->ARR = 2500;              // valeur max du compteur. Si DIR 0=> valeur max (1500ms dans ce cas). Si DIR 1=> mettre 0 
   
}


void enable_interrupt_ext(void)
{

  //timer 2
NVIC->ISER[0] |= NVIC_ISER_SETENA_28;    // bit 28 à 1 vecteur 28 pour TIM2 -  § 4.3.2 p13 Interrupt
TIM2->DIER |= TIM_DIER_UIE; 
 
  
// BP

 NVIC->ISER[1] |= (1<<8);       // bit 40 à 1 vecteur 40 pour PA11/12 : BP1/2  -  § 4.3.2 p13 Interrupt
 NVIC->ISER[0] |= NVIC_ISER_SETENA_23;      // bit 23 à 1 vecteur 23 pour PC5/6 : BP3/4
 
 RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;        // horloge sur APB2 car EXTI sur APB2
  
 // Masquage interrruption + config front montant/descendant
 EXTI->IMR |= EXTI_IMR_MR11;    // bit 11 à 1 car PA11 signifie interruption non masquable (active) -  § 10.3.1 p23 Interrupt 
 EXTI->FTSR |= EXTI_FTSR_TR11;         // bit 11 à 1 car PA11 signifie front descendant  -  § 10.3.4 p25 Interrupt
 
 EXTI->IMR |= EXTI_IMR_MR12;          // bit 12 à 1 car PA12   
 EXTI->FTSR |= EXTI_FTSR_TR12;
 
 EXTI->IMR |= EXTI_IMR_MR5;          // bit 5 à 1 car PC5
 EXTI->FTSR |= EXTI_FTSR_TR5;
 
 EXTI->IMR |= EXTI_IMR_MR6;          // bit 6 à 1 car PC6
 EXTI->FTSR |= EXTI_FTSR_TR6;
 
 // PA11 : 4 bits à 0000 sur bits 12-15 du EXTICR2  -  § 8.5.4  p27 Interrupt
 SYSCFG->EXTICR[2] &= ~(1<<12);
 SYSCFG->EXTICR[2] &= ~(1<<13);
 SYSCFG->EXTICR[2] &= ~(1<<14); 
 SYSCFG->EXTICR[2] &= ~(1<<15);  
 
 // PA12 : 4 bits à 0000 sur bits 0-3 du EXTICR3  -  § 8.5.4  p27 Interrupt
 SYSCFG->EXTICR[3] &= ~(1<<0);
 SYSCFG->EXTICR[3] &= ~(1<<1);
 SYSCFG->EXTICR[3] &= ~(1<<2); 
 SYSCFG->EXTICR[3] &= ~(1<<3);  
 
  // PA5 : 0010 sur bits 4-7 du EXTICR1  -  § 8.5.4  p27 Interrupt
 SYSCFG->EXTICR[1] &= ~(1<<4);
 SYSCFG->EXTICR[1] |= (1<<5);
 SYSCFG->EXTICR[1] &= ~(1<<6); 
 SYSCFG->EXTICR[1] &= ~(1<<7); 
 
 // PA6 : 0010 sur bits 8-11 du EXTICR1  -  § 8.5.4  p27 Interrupt
 SYSCFG->EXTICR[1] &= ~(1<<8);
 SYSCFG->EXTICR[1] |= (1<<9);
 SYSCFG->EXTICR[1] &= ~(1<<10); 
 SYSCFG->EXTICR[1] &= ~(1<<11); 
 
}
 


uint8_t convertion_char_LCD(uint8_t CHAR)
{
  switch(CHAR)
  {
    case ' ': return 0x00;
    case '0':case 0: return 0x7E;
    case '1':case 1: return 0x30;
    case '2':case 2: return 0x6D;
    case '3':case 3: return 0x79;
    case '4':case 4: return 0x33;
    case '5':case 5: return 0x5B;
    case '6':case 6: return 0x5F;
    case '7':case 7: return 0x70;
    case '8':case 8: return 0x7F;
    case '9':case 9: return 0x7B;
    case 'A':case 'a': return 0x77;//A
    case 'B':case 'b': return 0x7F;//B
    case 'C':case 'c': return 0x4E;//C
    case 'D':case 'd': return 0x7E;//D
    case 'E':case 'e': return 0x4F;//E
    case 'F':case 'f': return 0x47;//F
    case 'G':case 'g': return 0x5E;//G
    case 'H':case 'h': return 0x37;//H 
    case 'I':case 'i': return 0x06;//I
    case 'J':case 'j': return 0x3C;//J
    case 'L':case 'l': return 0x0E;//L
    case 'N':case 'n': return 0x76;//N
    case 'O':case 'o': return 0x7E;//O
    case 'P':case 'p': return 0x67;//P
    case 'Q':case 'q': return 0x73;//Q
    case 'R':case 'r': return 0x66;//Q
    case 'S':case 's': return 0x5B;//S
    case 'T':case 't': return 0x0F;//T
    case 'U':case 'u': return 0x3E;//U                 
  }
  return 0x00;
}

void SPI_Write_chaine_carac(uint8_t* chaine)
{
  uint8_t i=0, longChaine=0;
  uint8_t etat5Prec=etat5;
  
  if(chaine[0]=='\0')
    return;
 
 
  SPI_Write(1, convertion_char_LCD(' '));
  SPI_Write(2, convertion_char_LCD(' '));
  SPI_Write(3, convertion_char_LCD(' '));
  SPI_Write(4, convertion_char_LCD(' '));
  
  do
  {
    longChaine++;
  }while(chaine[longChaine]!='\0');
 
  
  while(etat5==etat5Prec);
  etat5Prec=~etat5;
      
    do
      {
        SPI_Write(1, convertion_char_LCD(chaine[i]));
        if(longChaine>1)
          SPI_Write(2, convertion_char_LCD(chaine[i+1]));
        else
          SPI_Write(2, convertion_char_LCD(' '));
        
        if(longChaine>2)
          SPI_Write(3, convertion_char_LCD(chaine[i+2]));
        else
          SPI_Write(3, convertion_char_LCD(' '));
        
        if(longChaine>3)
          SPI_Write(4, convertion_char_LCD(chaine[i+3]));
        else
          SPI_Write(4, convertion_char_LCD(' '));
        
      
        
        if(etat5==etat5Prec)
        {
          etat5Prec=~etat5;
          i++;
          SPI_Write(1, convertion_char_LCD(' '));
          SPI_Write(2, convertion_char_LCD(' '));
          SPI_Write(3, convertion_char_LCD(' '));
          SPI_Write(4, convertion_char_LCD(' '));
        }
        
     
        
      }while(i+3<longChaine);
}


