/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Templates/stm32l1xx_it.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    16-May-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_it.h"
#include "modules/fonctions.h"
extern uint8_t etat;
extern uint8_t etat1;
extern uint8_t etat2;
extern uint8_t etat3;
extern uint8_t etat4;
extern uint8_t etat5,CPT_sec,CPT_mn,CPT_heure;
char Message_to_TX;

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
    
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32L1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l1xx_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

  
  
  
void EXTI15_10_IRQHandler ()    //(dans fichier startup)
{
if(EXTI->PR &(1<<11))
  {
  EXTI->PR |= (1<<11);  // bit11 � 1 pour la prochaine interruption  pour remettre � 0 -  p26 Interrupt
  etat1 = ~etat1;
  
  }
  if(EXTI->PR &(1<<12))
  {
  EXTI->PR |= (1<<12);  // bit12 � 1 pour la prochaine interruption  pour remettre � 0 -  p26 Interrupt
  etat2 = ~etat2;
  }
  if(EXTI->PR & (EXTI_PR_PR11))
    {
      Message_to_TX='O';
      uart_tx(Message_to_TX);
      EXTI->PR |= (EXTI_PR_PR11); // cours 2 slide 26 (est ce que j'ai recu une demande d'interruption //  on le met ? 1 pour faire un reset
      etat=~etat;//p26
    }
}

void EXTI9_5_IRQHandler ()    //(dans fichier startup)
{
if(EXTI->PR &(1<<6))
  {
  EXTI->PR |= (1<<6);  // bit6 � 1 pour la prochaine interruption  pour remettre � 0 -  p26 Interrupt
  etat4 = ~etat4;
  }
  if(EXTI->PR &(1<<5))
  {
  EXTI->PR |= (1<<5);  // bit6 � 1 pour la prochaine interruption  pour remettre � 0 -  p26 Interrupt
  etat3 = ~etat3;
  }
}

void TIM2_IRQHandler ()
{

TIM2->SR &= ~(1<<0);
etat5=~etat5;  
}



/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
