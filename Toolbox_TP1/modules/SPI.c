#include "stm32l1xx_nucleo.h"
#include "modules/fonctions.h"


/* 
********initialisation du module SPI1********

fonctions :     void GPIO_init_SPI1()
                void Set_CS()
                void Reset_CS()
                void SendData_SPI(uint8_t data)
                void SPI1_write(uint8_t add, uint8_t data)
*/

void init_SPI(void)
{
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