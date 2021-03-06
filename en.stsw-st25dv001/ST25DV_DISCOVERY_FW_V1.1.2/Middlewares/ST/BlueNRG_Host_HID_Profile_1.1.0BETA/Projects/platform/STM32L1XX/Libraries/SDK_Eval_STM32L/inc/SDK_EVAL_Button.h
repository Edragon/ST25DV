/**
 * @file    SDK_EVAL_Button.h
 * @author  AMS - AAS Division
 * @version V1.0.0
 * @date    Febrary 3, 2014
 * @brief   This file contains definitions for Software Development Kit eval board push-buttons.
 * @details
 *
 * This module exports functions used to configure and manage the SDK motherboard
 * push-buttons.
 *
 * <b>Example:</b>
 * @code
 *
 *   SdkEvalPushButtonInit(BUTTON_KEY, BUTTON_MODE_EXTI);
 *
 * @endcode
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
 *
 * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_BUTTON_H
#define __SDK_EVAL_BUTTON_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

#ifdef __cplusplus
 extern "C" {
#endif


/** @addtogroup SDK_EVAL_STM32L
 * @{
 */

/** @addtogroup SDK_EVAL_Button         SDK EVAL Button
 * @brief Management of Software Development Kit eval board push-buttons.
 * @details See the file <i>@ref SDK_EVAL_Button.h</i> for more details.
 * @{
 */

/** @addtogroup SDK_EVAL_Button_Exported_Types          SDK EVAL Button Exported Types
 * @{
 */

/**
 * @brief  Buttons for SDK EVAL enumeration
 */
typedef enum
{
  BUTTON_1 = 0, /*!< BUTTON_1 */
  BUTTON_2 = 1, /*!< BUTTON_2 */
  BUTTON_3 = 2, /*!< BUTTON_3 */
  BUTTON_4 = 3, /*!< BUTTON_4 */
  BUTTON_5 = 4, /*!< BUTTON_5 */
  BUTTON_6 = 5, /*!< BUTTON_6 */
  BUTTON_7 = 6  /*!< BUTTON_7 */

} SdkEvalButton;

/**
 * @brief  Button Mode for SDK EVAL enumeration
 */
typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1

} SdkEvalButtonMode;

/**
 * @brief  Joystick for SDK EVAL enumeration
 */
typedef enum
{
  JOY_NONE = 0,
  JOY_SEL = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5

} SdkEvalJoyState;

/**
 * @}
 */


/** @defgroup SDK_EVAL_Button_Exported_Constants                SDK EVAL Button Exported Constants
 * @{
 */

/**
 * @brief  Number of buttons of the SDL EVAL board
 */
#define BUTTONn                          7

/**
 * @brief  BlueNRG USB Dongle SW2 button (BUTTON_KEY)
 */
#define	KEY_BUTTON_VD1_PIN	                GPIO_Pin_1
#define	KEY_BUTTON_VD1_GPIO_PORT	        GPIOA
#define	KEY_BUTTON_VD1_GPIO_CLK	                RCC_AHBPeriph_GPIOA
#define	KEY_BUTTON_VD1_EXTI_LINE	        EXTI_Line1
#define	KEY_BUTTON_VD1_EXTI_PORT_SOURCE	        EXTI_PortSourceGPIOA
#define	KEY_BUTTON_VD1_EXTI_PIN_SOURCE	        EXTI_PinSource1
#define	KEY_BUTTON_VD1_EXTI_IRQn	        EXTI1_IRQn
#define	KEY_BUTTON_VD1_EXTI_IRQ_HANDLER	        EXTI1_IRQHandler
#define BUTTON_KEY			        BUTTON_1

#define SDK_DONGLE_BUTTON_SW2                   BUTTON_1
   
/**
 * @brief BlueNRG Eval Board (STEVAL-IDB002V1) Joystick Right push-button
 */
#define RIGHT_BUTTON_PIN                 GPIO_Pin_1
#define RIGHT_BUTTON_GPIO_PORT           GPIOB
#define RIGHT_BUTTON_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define RIGHT_BUTTON_EXTI_LINE           EXTI_Line1
#define RIGHT_BUTTON_EXTI_PORT_SOURCE    EXTI_PortSourceGPIOB
#define RIGHT_BUTTON_EXTI_PIN_SOURCE     EXTI_PinSource1
#define RIGHT_BUTTON_EXTI_IRQn           EXTI1_IRQn
#define RIGHT_BUTTON_EXTI_IRQ_HANDLER    EXTI1_IRQHandler
#define BUTTON_RIGHT			 BUTTON_2


/**
 * @brief BlueNRG Eval Board (STEVAL-IDB002V1) Joystick Left push-button
 */
#define LEFT_BUTTON_V3_PIN                  GPIO_Pin_8
#define LEFT_BUTTON_V3_GPIO_PORT            GPIOA
#define LEFT_BUTTON_V3_GPIO_CLK             RCC_AHBPeriph_GPIOA
#define LEFT_BUTTON_V3_EXTI_LINE            EXTI_Line8
#define LEFT_BUTTON_V3_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOA
#define LEFT_BUTTON_V3_EXTI_PIN_SOURCE      EXTI_PinSource8
#define LEFT_BUTTON_V3_EXTI_IRQn            EXTI9_5_IRQn
#define LEFT_BUTTON_V3_EXTI_IRQ_HANDLER     EXTI9_5_IRQHandler

#define BUTTON_LEFT			    BUTTON_3
	

/**
 * @brief BlueNRG Eval Board (STEVAL-IDB002V1) Joystick Up push-button
 */
#define	UP_BUTTON_V3_PIN	        GPIO_Pin_10
#define	UP_BUTTON_V3_GPIO_PORT	        GPIOA
#define	UP_BUTTON_V3_GPIO_CLK	        RCC_AHBPeriph_GPIOA
#define	UP_BUTTON_V3_EXTI_LINE	        EXTI_Line10
#define	UP_BUTTON_V3_EXTI_PORT_SOURCE	EXTI_PortSourceGPIOA
#define	UP_BUTTON_V3_EXTI_PIN_SOURCE	EXTI_PinSource10
#define	UP_BUTTON_V3_EXTI_IRQn	        EXTI15_10_IRQn
#define	UP_BUTTON_V3_EXTI_IRQ_HANDLER	EXTI15_10_IRQHandler

#define BUTTON_UP			BUTTON_4

/**
 * @brief BlueNRG Eval Board (STEVAL-IDB002V1) Joystick Down push-button
 */
#define DOWN_BUTTON_PIN                  GPIO_Pin_0
#define DOWN_BUTTON_GPIO_PORT            GPIOB
#define DOWN_BUTTON_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define DOWN_BUTTON_EXTI_LINE            EXTI_Line0
#define DOWN_BUTTON_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOB
#define DOWN_BUTTON_EXTI_PIN_SOURCE      EXTI_PinSource0
#define DOWN_BUTTON_EXTI_IRQn            EXTI0_IRQn
#define DOWN_BUTTON_EXTI_IRQ_HANDLER     EXTI0_IRQHandler

#define BUTTON_DOWN			 BUTTON_5


/**
 * @brief BlueNRG Eval Board (STEVAL-IDB002V1) Joystick Sel (Center) push-button
 */
#define SEL_BUTTON_V3_PIN                   GPIO_Pin_9
#define SEL_BUTTON_V3_GPIO_PORT             GPIOA
#define SEL_BUTTON_V3_GPIO_CLK              RCC_AHBPeriph_GPIOA
#define SEL_BUTTON_V3_EXTI_LINE             EXTI_Line9
#define SEL_BUTTON_V3_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOA
#define SEL_BUTTON_V3_EXTI_PIN_SOURCE       EXTI_PinSource9
#define SEL_BUTTON_V3_EXTI_IRQn             EXTI9_5_IRQn
#define SEL_BUTTON_V3_EXTI_IRQ_HANDLER      EXTI9_5_IRQHandler

#define BUTTON_SEL			    BUTTON_6

/**
 * @brief BlueNRG Eval Board (STEVAL-IDB002V1) SCM_PS push-button 
 */
#define SCM_PS_BUTTON_PIN                GPIO_Pin_6
#define SCM_PS_BUTTON_GPIO_PORT          GPIOC
#define SCM_PS_BUTTON_GPIO_CLK           RCC_AHBPeriph_GPIOC
#define SCM_PS_BUTTON_EXTI_LINE          EXTI_Line6
#define SCM_PS_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOC
#define SCM_PS_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource6
#define SCM_PS_BUTTON_EXTI_IRQn          EXTI9_5_IRQn
#define SCM_BUTTON_EXTI_IRQ_HANDLER      EXTI9_5_IRQHandler

#define BUTTON_SCM_PS			 BUTTON_7


/**
 * @brief USB Dongle SW1 button
 */
#define	SCM_PS_BUTTON_VD1_PIN	                GPIO_Pin_2
#define	SCM_PS_BUTTON_VD1_GPIO_PORT	        GPIOB
#define	SCM_PS_BUTTON_VD1_GPIO_CLK	        RCC_AHBPeriph_GPIOB
#define	SCM_PS_BUTTON_VD1_EXTI_LINE	        EXTI_Line2
#define	SCM_PS_BUTTON_VD1_EXTI_PORT_SOURCE	EXTI_PortSourceGPIOB
#define	SCM_PS_BUTTON_VD1_EXTI_PIN_SOURCE	EXTI_PinSource2
#define	SCM_PS_BUTTON_VD1_EXTI_IRQn	        EXTI2_IRQn
#define	SCM_PS_BUTTON_VD1_EXTI_IRQ_HANDLER	EXTI2_IRQHandler

#define SDK_DONGLE_BUTTON_SW1                   BUTTON_7

#define BUTTON_IRQ_PREEMPTION_PRIORITY	 15
#define BUTTON_IRQ_SUB_PRIORITY	          0

/**
 * @}
 */


/** @defgroup SDK_EVAL_Button_Exported_Macros           SDK EVAL Button Exported Macros
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Button_Exported_Functions        SDK EVAL Button Exported Functions
 * @{
 */

void SdkEvalPushButtonInit(SdkEvalButton xButton, SdkEvalButtonMode xButtonMode);
FlagStatus SdkEvalPushButtonGetState(SdkEvalButton xButton);

/**
 * @}
 */


/**
 * @}
 */


/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/
