/**
  ******************************************************************************
  * @file    st25dx_bluenrg_ble.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
  *
  * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/myliberty  
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "st25dx_discovery.h"
#include "st25dx_bluenrg_ble.h"
#include "gp_timer.h"
#include "debug.h"

extern volatile uint32_t ms_counter;

/** @addtogroup BSP
 *  @{
 */

/** @addtogroup ST25DX_DISCOVERY_BSP
 *  @{
 */
 
/** @defgroup ST25DX_BLUENRG_BLE ST25DX BlueNRG BLE
 *  @{
 */


#define HEADER_SIZE 5
#define MAX_BUFFER_SIZE 255
#define TIMEOUT_DURATION 15


SPI_HandleTypeDef SpiHandle;


/* Private function prototypes -----------------------------------------------*/
static void us150Delay(void);
void set_irq_as_output(void);
void set_irq_as_input(void);


/**
 * @brief  This function is a utility to print the log time
 *         in the format HH:MM:SS:MSS (DK GUI time format)
 * @param  None
 * @retval None
 */
void print_csv_time(void)
{
  PRINT_CSV("%02d:%02d:%02d.%03d", ms_counter/(60*60*1000)%24, ms_counter/(60*1000)%60,
              (ms_counter/1000)%60, ms_counter%1000);
}

/**
 * @brief  Writes data to a serial interface.
 * @param  data1   :  1st buffer
 * @param  data2   :  2nd buffer
 * @param  n_bytes1: number of bytes in 1st buffer
 * @param  n_bytes2: number of bytes in 2nd buffer
 * @retval None
 */
void Hal_Write_Serial(const void* data1, const void* data2, int32_t n_bytes1,
                      int32_t n_bytes2)
{
#ifdef OPTIMIZED_SPI /* used by the server (L0 and F4, not L4) for the throughput test */
  struct timer t;
  int ret;
  uint8_t data2_offset = 0;

  Timer_Set(&t, CLOCK_SECOND/10);

  Disable_SPI_IRQ();

  while(1)
  {
    ret = BlueNRG_SPI_Write((uint8_t *)data1,(uint8_t *)data2 + data2_offset, n_bytes1, n_bytes2);

    if (ret >= 0)
    {
      n_bytes1 = 0;
      n_bytes2 -= ret;
      data2_offset += ret;
      if (n_bytes2==0)
      {
        break;
      }
    }

    if (Timer_Expired(&t))
    {
      break;
    }
  }

  Enable_SPI_IRQ();

#else /* not OPTIMIZED_SPI */
  struct timer t;

  Timer_Set(&t, CLOCK_SECOND/10);

#if PRINT_CSV_FORMAT
  print_csv_time();
  for (int i=0; i<n_bytes1; i++) 
  {
    PRINT_CSV(" %02x", ((uint8_t *)data1)[i]);
  }
  for (int i=0; i<n_bytes2; i++) 
  {
    PRINT_CSV(" %02x", ((uint8_t *)data2)[i]);
  }
  PRINT_CSV("\n");
#endif

  while(1)
  {
    if (BlueNRG_SPI_Write((uint8_t *)data1,(uint8_t *)data2, n_bytes1, n_bytes2)==0)
    {
      break;
    }
    if (Timer_Expired(&t))
    {
      break;
    }
  }
#endif /* OPTIMIZED_SPI */
}

/**
 * @brief  Initializes the SPI communication with the BlueNRG
 *         Expansion Board.
 * @param  None
 * @retval None
 */
void BNRG_SPI_Init(void)
{
  BSP_SPI3_Init();

#ifdef OPTIMIZED_SPI /* used by the server (L0 and F4, not L4) for the throughput test */
  /* Added HAP to enable SPI since Optimized SPI Transmit, Receive and Transmit/Receive APIs are 
     used for BlueNRG, BlueNRG-MS SPI communication in order to get the best performance in terms of 
     BLE throughput */
  __HAL_SPI_ENABLE(&SpiHandle);
#endif
}

/**
 * @brief  Resets the BlueNRG.
 * @param  None
 * @retval None
 */
void BlueNRG_RST(void)
{
  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_RESET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_SET);
  HAL_Delay(5);
}

/**
  * @brief  BSP BlueNRG IRQ Handler
  * @retval None.
  */
void BSP_BlueNRG_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(BNRG_SPI_EXTI_PIN);
}

/**
 * @brief  Reports if the BlueNRG has data for the host micro.
 * @param  None
 * @retval 1 if data are present, 0 otherwise
 */
// FIXME: find a better way to handle this return value (bool type? TRUE and FALSE)
uint8_t BlueNRG_DataPresent(void)
{
  if (HAL_GPIO_ReadPin(BNRG_SPI_EXTI_PORT, BNRG_SPI_EXTI_PIN) == GPIO_PIN_SET)
  {
    return 1;
  }
  else
  {
    return 0;
  }
} /* end BlueNRG_DataPresent() */

/**
 * @brief  Activate internal bootloader using pin.
 * @param  None
 * @retval None
 */
void BlueNRG_HW_Bootloader(void)
{
  Disable_SPI_IRQ();
  set_irq_as_output();
  BlueNRG_RST();
  set_irq_as_input();
  Enable_SPI_IRQ();
}

/**
 * @brief  Reads from BlueNRG SPI buffer and store data into local buffer.
 * @param  buffer   : Buffer where data from SPI are stored
 * @param  buff_size: Buffer size
 * @retval int32_t  : Number of read bytes
 */
int32_t BlueNRG_SPI_Read_All(uint8_t *buffer, uint8_t buff_size)
{
#ifdef OPTIMIZED_SPI /* used by the server (L0 and F4, not L4) for the throughput test */
  uint16_t byte_count;
  uint8_t len = 0;

  const uint8_t header_master[5] = {0x0b, 0x00, 0x00, 0x00, 0x00};
  uint8_t header_slave[5];

  __disable_irq();

  BNRG_SPI_CS_LOW();

  /* Read the header */
  HAL_SPI_TransmitReceive_Opt(header_master, header_slave, HEADER_SIZE);

  if (header_slave[0] == 0x02) 
  {
    // device is ready

    byte_count = (header_slave[4]<<8)|header_slave[3];

    if (byte_count > 0) 
    {

      // avoid to read more data that size of the buffer
      if (byte_count > buff_size)
      {
        byte_count = buff_size;
      }

      HAL_SPI_Receive_Opt(buffer, byte_count);

      len = byte_count;
    }
  }

  // Release CS line.
  BNRG_SPI_CS_HIGH();

  __enable_irq();

#if PRINT_CSV_FORMAT
  if (len > 0) 
  {
    print_csv_time();
    for (int i=0; i<len; i++) 
    {
      PRINT_CSV(" %02x", buffer[i]);
    }
    PRINT_CSV("\n");
  }
#endif

  return len;

#else /* not OPTIMIZED_SPI */
  uint16_t byte_count;
  uint8_t len = 0;
  uint8_t char_ff = 0xff;
  volatile uint8_t read_char;

  uint8_t header_master[HEADER_SIZE] = {0x0b, 0x00, 0x00, 0x00, 0x00};
  uint8_t header_slave[HEADER_SIZE];

  /* CS reset */
  BNRG_SPI_CS_LOW();

  /* Read the header */
  BSP_SPI3_SendRecv(header_master, header_slave, HEADER_SIZE);

  if (header_slave[0] == 0x02) {
    /* device is ready */
    byte_count = (header_slave[4]<<8)|header_slave[3];

    if (byte_count > 0)
    {

      /* avoid to read more data that size of the buffer */
      if (byte_count > buff_size)
      {
        byte_count = buff_size;
      }

      for (len = 0; len < byte_count; len++)
      {
        BSP_SPI3_SendRecv(&char_ff, (uint8_t*)&read_char, 1);
        buffer[len] = read_char;
      }

    }
  }
  /* Release CS line */
  BNRG_SPI_CS_HIGH();
  
  // Add a small delay to give time to the BlueNRG to set the IRQ pin low
  // to avoid a useless SPI read at the end of the transaction
  for (volatile int i = 0; i < 2; i++)
  {
    __NOP();
  }

#if PRINT_CSV_FORMAT
  if (len > 0) {
    print_csv_time();
    for (int i=0; i<len; i++)
    {
      PRINT_CSV(" %02x", buffer[i]);
    }
    PRINT_CSV("\n");
  }
#endif /* OPTIMIZED_SPI */

  return len;
#endif
}

/**
 * @brief  Writes data from local buffer to SPI.
 * @param  data1    : First data buffer to be written
 * @param  data2    : Second data buffer to be written
 * @param  Nb_bytes1: Size of first data buffer to be written
 * @param  Nb_bytes2: Size of second data buffer to be written
 * @retval Number of read bytes
 */
int32_t BlueNRG_SPI_Write(uint8_t* data1,
                          uint8_t* data2, uint8_t Nb_bytes1, uint8_t Nb_bytes2)
{
#ifdef OPTIMIZED_SPI /* used by the server (L0 and F4, not L4) for the throughput test */
  int16_t result = 0;
  uint16_t tx_bytes;
  uint8_t rx_bytes;

  const uint8_t header_master[5] = {0x0a, 0x00, 0x00, 0x00, 0x00};
  uint8_t header_slave[5]  = {0x00};

  BNRG_SPI_CS_LOW();

  HAL_SPI_TransmitReceive_Opt(header_master, header_slave, HEADER_SIZE);

  if (header_slave[0] != 0x02)
  {
    result = -1;
    goto failed; // BlueNRG not awake.
  }

  rx_bytes = header_slave[1];

  if (rx_bytes < Nb_bytes1)
  {
    result = -2;
    goto failed; // BlueNRG .
  }

  HAL_SPI_Transmit_Opt(data1, Nb_bytes1);

  rx_bytes -= Nb_bytes1;

  if (Nb_bytes2 > rx_bytes)
  {
    tx_bytes = rx_bytes;
  }
  else
  {
    tx_bytes = Nb_bytes2;
  }

  HAL_SPI_Transmit_Opt(data2, tx_bytes);

  result = tx_bytes;

failed:

  // Release CS line
  BNRG_SPI_CS_HIGH();

  return result;

#else /* not OPTIMIZED_SPI */
  int32_t result = 0;
  int32_t spi_fix_enabled = 0;

#ifdef ENABLE_SPI_FIX
  spi_fix_enabled = 1;
#endif //ENABLE_SPI_FIX

  unsigned char header_master[HEADER_SIZE] = {0x0a, 0x00, 0x00, 0x00, 0x00};
  unsigned char header_slave[HEADER_SIZE]  = {0xaa, 0x00, 0x00, 0x00, 0x00};

  unsigned char read_char_buf[MAX_BUFFER_SIZE];

  Disable_SPI_IRQ();

  /*
   If the SPI_FIX is enabled the IRQ is set in Output mode, then it is pulled
   high and, after a delay of at least 112us, the CS line is asserted and the
   header transmit/receive operations are started.
   After these transmit/receive operations the IRQ is reset in input mode.
 */
  if (spi_fix_enabled)
  {
    set_irq_as_output();

    /* Assert CS line after at least 112us */
    us150Delay();
  }

  /* CS reset */
  BNRG_SPI_CS_LOW();

  /* Exchange header */
  BSP_SPI3_SendRecv(header_master, header_slave, HEADER_SIZE);

  if (spi_fix_enabled)
  {
    set_irq_as_input();
  }

  if (header_slave[0] == 0x02)
  {
    /* SPI is ready */
    if (header_slave[1] >= (Nb_bytes1+Nb_bytes2))
    {

      /*  Buffer is big enough */
      if (Nb_bytes1 > 0)
      {
        BSP_SPI3_SendRecv(data1, read_char_buf, Nb_bytes1);
      }
      if (Nb_bytes2 > 0)
      {
        BSP_SPI3_SendRecv(data2, read_char_buf, Nb_bytes2);
      }

    }
    else
    {
      /* Buffer is too small */
      result = -2;
    }
  }
  else
  {
    /* SPI is not ready */
    result = -1;
  }

  /* Release CS line */
  BNRG_SPI_CS_HIGH();

  Enable_SPI_IRQ();

  return result;

#endif	/* OPTIMIZED_SPI */

}

/**
 * @brief  Set in Output mode the IRQ.
 * @param  None
 * @retval None
 */
void set_irq_as_output(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Pull IRQ high */
  GPIO_InitStructure.Pin = BNRG_SPI_IRQ_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = BNRG_SPI_IRQ_SPEED;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStructure);
  HAL_GPIO_WritePin(BNRG_SPI_IRQ_PORT, BNRG_SPI_IRQ_PIN, GPIO_PIN_SET);
}

/**
 * @brief  Set the IRQ in input mode.
 * @param  None
 * @retval None
 */
void set_irq_as_input(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* IRQ input */
  GPIO_InitStructure.Pin = BNRG_SPI_IRQ_PIN;
  GPIO_InitStructure.Mode = BNRG_SPI_IRQ_MODE;
  GPIO_InitStructure.Pull = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed = BNRG_SPI_IRQ_SPEED;
  GPIO_InitStructure.Alternate = BNRG_SPI_IRQ_ALTERNATE;
  HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pull = BNRG_SPI_IRQ_PULL;
  HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStructure);
}

/**
 * @brief  Utility function for delay
 * @param  None
 * @retval None
 * NOTE: TODO: implement with clock-independent function.
 */
static void us150Delay(void)
{
#if SYSCLK_FREQ == 4000000
  for (volatile int i = 0; i < 35; i++)__NOP();
#elif SYSCLK_FREQ == 32000000
  for (volatile int i = 0; i < 420; i++)__NOP();
#elif SYSCLK_FREQ == 80000000
  for (volatile int i = 0; i < 1072; i++)__NOP();
#elif SYSCLK_FREQ == 84000000
  for (volatile int i = 0; i < 1125; i++)__NOP();
#else
#error Implement delay function.
#endif
}

/**
 * @brief  Enable SPI IRQ.
 * @param  None
 * @retval None
 */
void Enable_SPI_IRQ(void)
{
  HAL_NVIC_EnableIRQ(BNRG_SPI_EXTI_IRQn);
}

/**
 * @brief  Disable SPI IRQ.
 * @param  None
 * @retval None
 */
void Disable_SPI_IRQ(void)
{ 
  HAL_NVIC_DisableIRQ(BNRG_SPI_EXTI_IRQn);
}

/**
 * @brief  Clear Pending SPI IRQ.
 * @param  None
 * @retval None
 */
void Clear_SPI_IRQ(void)
{
  HAL_NVIC_ClearPendingIRQ(BNRG_SPI_EXTI_IRQn);
}

/**
 * @brief  Clear EXTI (External Interrupt) line for SPI IRQ.
 * @param  None
 * @retval None
 */
void Clear_SPI_EXTI_Flag(void)
{  
  __HAL_GPIO_EXTI_CLEAR_IT(BNRG_SPI_EXTI_PIN);
}

#ifdef OPTIMIZED_SPI
/* used by the server (L0 and F4, not L4) for the throughput test */
static void SPI_I2S_SendData(SPI_HandleTypeDef *hspi, uint8_t data)
{
  hspi->Instance->DR = data;
}

static  uint8_t SPI_I2S_ReceiveData(SPI_HandleTypeDef *hspi)
{
  return hspi->Instance->DR;
}

/**
  * @brief  Transmit and Receive an amount of data in blocking mode 
  * @param  pTxData: pointer to transmission data buffer
  * @param  pRxData: pointer to reception data buffer to be
  * @param  Size: amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_TransmitReceive_Opt(const uint8_t *pTxData, uint8_t *pRxData, uint8_t Size)
{
  uint8_t i;

  for (i = 0; i < Size; i++)
  {
    SPI_I2S_SendData(&SpiHandle, *pTxData++);
    while(__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE) == RESET);
    while(__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE) == RESET);
    *pRxData++ = SPI_I2S_ReceiveData(&SpiHandle);
  }

  return HAL_OK;
}

/**
  * @brief  Transmit an amount of data in blocking mode (optimized version)
  * @param  pData: pointer to data buffer
  * @param  Size: amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Transmit_Opt(const uint8_t *pTxData, uint8_t Size)
{
  uint8_t i;

  for (i = 0; i < Size; i++)
  {
    SPI_I2S_SendData(&SpiHandle, *pTxData++);
    while(__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE) == RESET);
    while(__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(&SpiHandle);
  }

  return HAL_OK;
}

/**
  * @brief  Receive an amount of data in blocking mode (optimized version)
  * @param  pData: pointer to data buffer
  * @param  Size: amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Receive_Opt(uint8_t *pRxData, uint8_t Size)
{
  uint8_t i;

  for (i = 0; i < Size; i++)
  {
    SPI_I2S_SendData(&SpiHandle, 0xFF);
    while(__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE) == RESET);
    while(__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE) == RESET);
    *pRxData++ = SPI_I2S_ReceiveData(&SpiHandle);
  }

  return HAL_OK;
}
#endif /* OPTIMIZED_SPI */

/**
 * @}
 */

/**
 * @}
 */
   
/**
 * @}
 */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
