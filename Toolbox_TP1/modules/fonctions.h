/******************************************************************************/
/*                                                                            */
/*                      MAIN BASIQUE STM32L152RE                              */
/*                                                                            */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/*                        PROJET STM32                                        */
/*                       Décembre  2017                                       */
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

/* **************protype***********²*** */
void test(void);
void uart_rx();
void uart_tx(char);
void uart_tx2(void);
void GPIO_init_USART2(void);
void uart2_config(void);

void init_gpio_bouton_poussoir(void);
void enable_interrupt_ext(void);
void Init_Led(void);
void led_eteinte (void);
void led(void);
void clignotement_LED(void);

void Delay(uint32_t Tempo);

// SPI

void init_SPI(void);
void init_I2C(void);
void init_TIM2(void);
void enable_interrupt_ext(void);
void I2C_Write(uint8_t add, uint8_t nb_bytes,uint8_t *data );
void I2C_Read(uint8_t add, uint8_t nb_bytes,uint8_t *data );
void SPI_Write(uint8_t add,uint8_t data);
void SendData_SPI(uint8_t add);
void init_MAX(void);
void Set_CS (void);
void Reset_CS (void);
void Delay (uint32_t Tempo);

uint8_t convertion_char_LCD(uint8_t CHAR);
void SPI_Write_chaine_carac(uint8_t* chaine);
