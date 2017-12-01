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

/* **************protype************** */
void test(void);
void uart_rx();
void uart_tx(char);
void uart_tx2(void);
void GPIO_init_USART2(void);
void uart2_config(void);

void init_gpio_bouton_poussoir(void);
void enable_interrupt_ext(void);
void init_led(void);
void led_eteinte (void);
void led(void);