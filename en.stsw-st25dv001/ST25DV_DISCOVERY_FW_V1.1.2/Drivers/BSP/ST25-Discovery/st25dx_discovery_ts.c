/**
  ******************************************************************************
  * @file    st25dx_discovery_ts.c
  * @author  MMY Application Team
  * @version $Revision$
  * @date    $Date$
  * @brief   This file provides a set of functions needed to manage Touch
  *          screen available with STMPE811 IO Expander device mounted on
  *          ST25DX-Discovery Kit.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "st25dx_discovery_ts.h"
#include "st25dx_discovery_bus.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup ST25DX_DISCOVERY_BSP
  * @{
  */ 

/** @defgroup ST25DX_DISCOVERY_TS
  * @{
  */

static TS_DrvTypeDef     *TsDrv;
static uint16_t          TsXBoundary, TsYBoundary;

/**
  * @brief  Initializes and configures the touch screen functionalities and
  *         configures all necessary hardware resources (GPIOs, clocks..).
  * @param  XSize: The maximum X size of the TS area on LCD
  * @param  YSize: The maximum Y size of the TS area on LCD
  * @retval TS_OK: if all initializations are OK. Other value if error.
  */
uint8_t BSP_TS_Init(uint16_t XSize, uint16_t YSize)
{
  uint8_t ret = TS_ERROR;

  /* Initialize x and y positions boundaries */
  TsXBoundary = XSize;
  TsYBoundary = YSize;

  /* Read ID and verify if the IO expander is ready */
  if (stmpe811_ts_drv.ReadID(BUS_TS_I2C_ADDRESS) == STMPE811_ID)
  {
    /* Initialize the TS driver structure */
    TsDrv = &stmpe811_ts_drv;

    ret = TS_OK;
  }

  if (ret == TS_OK)
  {
    /* Initialize the LL TS Driver */
    TsDrv->Init(BUS_TS_I2C_ADDRESS);
    TsDrv->Start(BUS_TS_I2C_ADDRESS);
  }

  return ret;
}
/**
  * @brief  IOE Low Level Initialization.
  */
void BSP_TS_IO_Init(void)
{
  BSP_I2C1_Init();
}

/**
  * @brief  IOE Low Level Interrupt configuration.
  */
void BSP_TS_IO_ITConfig(void)
{
  GPIO_InitTypeDef gpio_init;

  /* Enable the GPIO EXTI Clock */
  BSP_TS_INT_CLK_ENABLE();

  gpio_init.Pin   = BSP_TS_INT_PIN;
  gpio_init.Pull  = GPIO_PULLUP;
  gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
  gpio_init.Mode  = GPIO_MODE_IT_FALLING;
  HAL_GPIO_Init(BSP_TS_INT_GPIO_PORT, &gpio_init);
}

/**
  * @brief  IOE Writes single data operation.
  * @param  Addr: I2C Address
  * @param  Reg: Reg Address 
  * @param  Value: Data to be written
  */
void BSP_TS_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
  BSP_I2C1_WriteReg(Addr, Reg, &Value, 1);
}

/**
  * @brief  IOE Reads single data.
  * @param  Addr: I2C Address
  * @param  Reg: Reg Address 
  * @retval The read data
  */
uint8_t BSP_TS_IO_Read(uint8_t Addr, uint8_t Reg)
{
  uint8_t data = 0;

  BSP_I2C1_ReadReg(Addr, Reg, &data, 1);

  return data;
}

/**
  * @brief  IOE Writes multiple data.
  * @param  Addr: I2C Address
  * @param  Reg: Reg Address 
  * @param  pBuffer: pointer to data buffer
  * @param  Length: length of the data
  */
void BSP_TS_IO_WriteMultiple(uint8_t Addr, uint8_t Reg, uint8_t *pBuffer, uint16_t Length)
{
  BSP_I2C1_WriteReg(Addr, Reg, pBuffer, Length);
}

/**
  * @brief  IOE Reads multiple data.
  * @param  Addr: I2C Address
  * @param  Reg: Reg Address 
  * @param  pBuffer: pointer to data buffer
  * @param  Length: length of the data
  * @retval 0 if no problems to read multiple data
  */
uint16_t BSP_TS_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *pBuffer, uint16_t Length)
{
  return BSP_I2C1_ReadReg(Addr, Reg, pBuffer, Length);
}

/**
  * @brief  IOE Delay.
  * @param  Delay in ms
  */
void BSP_TS_IO_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

/**
  * @brief  Configures and enables the touch screen interrupts.
  * @retval TS_OK: if ITconfig is OK. Other value if error.
  */
uint8_t BSP_TS_ITConfig(void)
{
  /* Enable the TS ITs */
  TsDrv->EnableIT(BUS_TS_I2C_ADDRESS);

  return TS_OK;
}

/**
  * @brief  Gets the TS IT status.
  * @retval Interrupt status.
  */
uint8_t BSP_TS_ITGetStatus(void)
{
  /* Return the TS IT status */
  return (TsDrv->GetITStatus(BUS_TS_I2C_ADDRESS));
}

/**
  * @brief  Returns status and positions of the touch screen.
  * @param  TsState: Pointer to touch screen current state structure
  */
void BSP_TS_GetState(TS_StateTypeDef* TsState)
{
  static uint32_t _x = 0, _y = 0;
  uint16_t xDiff, yDiff , x , y, xr, yr;

  TsState->TouchDetected = TsDrv->DetectTouch(BUS_TS_I2C_ADDRESS);

  if (TsState->TouchDetected)
  {
    TsDrv->GetXY(BUS_TS_I2C_ADDRESS, &x, &y);

    /* Y value first correction */
    y -= 360;

    /* Y value second correction */
    yr = y / 11;

    /* Return y position value */
    if (yr <= 0)
    {
      yr = 0;
    }
    else if (yr > TsYBoundary)
    {
      yr = TsYBoundary - 1;
    }
    else
    {}
    y = yr;

    /* X value first correction */
    if (x <= 3000)
    {
      x = 3870 - x;
    }
    else
    {
      x = 3800 - x;
    }

    /* X value second correction */
    xr = x / 15;

    /* Return X position value */
    if (xr <= 0)
    {
      xr = 0;
    }
    else if (xr > TsXBoundary)
    {
      xr = TsXBoundary - 1;
    }
    else 
    {}

    x = xr;
    xDiff = x > _x? (x - _x): (_x - x);
    yDiff = y > _y? (y - _y): (_y - y);

    if (xDiff + yDiff > 5)
    {
      _x = x;
      _y = TsYBoundary - y;
    }

    /* Update the X position */
    TsState->X = _y;

    /* Update the Y position */
    TsState->Y = _x;
  }
}

/**
  * @brief  Clears all touch screen interrupts.
  */  
void BSP_TS_ITClear(void)
{
  /* Clear TS IT pending bits */
  TsDrv->ClearIT(BUS_TS_I2C_ADDRESS);
}


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
