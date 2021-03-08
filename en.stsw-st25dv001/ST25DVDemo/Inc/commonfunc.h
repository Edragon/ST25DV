/**
  ******************************************************************************
  * @file    commonfunc.h 
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   Header for commonfunc.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
  *
  * Licensed under MMY-ST Liberty SW License Agreement V2, (the "License");
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMONFUNC_H
#define __COMMONFUNC_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include <stdbool.h>

/** @addtogroup ST25_Discovery_Demo
  * @{
  */

/** @defgroup ST25DV_Common_Functions ST25DV common functions
  * @{
  * Common functions for the ST25DV management.
  */
/* Exported types ------------------------------------------------------------*/
/**
 * @brief  ST25DV GPO interrupt status structure.
 * @details This structure is used to return the event(s) that raised the GPO interrupt.
 */
typedef struct
{
  uint8_t WriteInEEPROM;  /**< EEPROM has been written by the RF */
  uint8_t RfActivity;     /**< The tag is processing on RF side */
  uint8_t FieldOn;        /**< RF field becomes active */
  uint8_t FieldOff;       /**< RF field becomes inactive */
  uint8_t MsgInMailbox;   /**< Fast Transfer Mode: message from the RF received */
  uint8_t MailboxMsgRead; /**< Fast Transfer Mode: message read by the RF */
  uint8_t RfInterrupt;    /**< Interrupt generated by a dedicated RF command  */
  uint8_t Rfuser;         /**< GPO level controlled through the RF */
} IT_GPO_STATUS;

/* Exported macro ------------------------------------------------------------*/
#define ST25_RETRY_NB     ((uint8_t) 15)
#define ST25_RETRY_DELAY  ((uint8_t) 40)

/**
  * @brief Iterate ST25DV command depending on the command return status.
  * @param cmd A ST25DV function returning a NFCTAG_StatusTypeDef status.
  */
#define ST25_RETRY(cmd) do {                                                  \
                          int st25_retry = ST25_RETRY_NB;                     \
                          int32_t st25_status = NFCTAG_ERROR;    \
                          while(st25_status != NFCTAG_OK)                     \
                          {                                                   \
                            st25_status = cmd;                                \
                            if(st25_status != NFCTAG_OK)                      \
                              HAL_Delay(ST25_RETRY_DELAY);                    \
                            if(st25_retry-- <= 0)                             \
                            {                                                 \
                              st25_error(st25_status);                        \
                              st25_retry = ST25_RETRY_NB;                     \
                            }                                                 \
                          }                                                   \
                      } while(0)

/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
ST25DV_I2CSSO_STATUS PresentPasswd( const bool passwd );
int32_t InitITGPOMode( const uint16_t ITConfig );
void DeInitITGPOMode( void );
void ManageGPO( IT_GPO_STATUS * const gpo );
void st25_error(int32_t status);
/**
  * @}
  */ 

/**
  * @}
  */ 

#endif /* __COMMONFUNC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
