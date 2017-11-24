#include "stm32l1xx_nucleo.h"



/* 
********initialisation du module USART2********

fonctions :     void GPIO_init_USART2()
                void uart_tx()
                void uart_rx()
                void uart_config(void)
*/




GPIO_init_USART2(){ 
  
    RCC   -> AHBENR |= RCC_AHBENR_GPIOAEN; //envoyé l'horloge sur Registre Port A
    GPIOA -> MODER &= ~GPIO_MODER_MODER2_0; //Bit 0 du port A à etat 1
    GPIOA -> MODER |= GPIO_MODER_MODER2_1; // mettre à zero le bit 0 du port A à etat 0
    
    GPIOA -> MODER &= ~GPIO_MODER_MODER3_0; //Bit 0 du port A à etat 1
    GPIOA -> MODER |= GPIO_MODER_MODER3_1; // mettre à zero le bit 0 du port A à etat 0
    
    GPIOA -> OTYPER |=  GPIO_OTYPER_OT_3;
    GPIOA -> OTYPER |=  GPIO_OTYPER_OT_2; // configration open_drain (sortie) du bit 1 port A
    
    GPIOA->PUPDR |=(1<<4); 
    GPIOA->PUPDR &=~(1<<5);
    
    GPIOA->PUPDR |= (1<<6); 
    GPIOA->PUPDR &=~ (1<<7);

    GPIOA->AFR[0] &= ~(1<<11);
    GPIOA->AFR[0] |= (1<<10);
    GPIOA->AFR[0] |= (1<<9);
    GPIOA->AFR[0] |= (1<<8);
    
    GPIOA->AFR[0] &= ~(1<<15);
    GPIOA->AFR[0] |= (1<<14);
    GPIOA->AFR[0] |= (1<<13);
    GPIOA->AFR[0] |= (1<<12);
    

}



void uart_tx() {
  
    USART2 ->CR1 |= USART_CR1_TE;  
    USART2 -> DR = 'A';
    
    while( (USART2 ->SR & USART_SR_TC)==0);
    USART2 ->SR &= ~USART_SR_TC ; 
  
}


void uart_rx() {

     uint8_t val;
     USART2 ->CR1 |= USART_CR1_RE;
     
     while ((USART2 ->SR & USART_SR_RXNE)==0);
     
     
  
}
void uart2_config(void)
{
  
   RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
   USART2 -> CR1 |= USART_CR1_UE; // Activation du uart2
   USART2 ->CR1 &= ~USART_CR1_M; // longueur du mot sur 8bits
   USART2 ->CR2 &= ~((USART_CR2_STOP_0) | (USART_CR2_STOP_1)); // definition de nombre de bit de stop -> 1 bit
   USART2 ->CR3 &= ~USART_CR3_DMAR; // activation du DMA en receveur
   USART2 ->BRR = 0x683;
   
   USART2 ->CR3 &= ~USART_CR3_DMAT; // activation du DMA en transmetteur
   USART2 ->CR1 &=  ~USART_CR1_OVER8 ; 
   
  
}



/*void main()
{ 
 
     GPIO_init_USART2();
     uart2_config();
     
    while(1)
    {

      uart_tx();
                  
    } 
}*/