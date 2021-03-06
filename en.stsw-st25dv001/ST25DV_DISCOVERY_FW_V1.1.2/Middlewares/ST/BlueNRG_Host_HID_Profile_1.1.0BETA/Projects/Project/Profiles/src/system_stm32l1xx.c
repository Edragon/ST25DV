/**
  ******************************************************************************
  * @file    system_stm32l1xx.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11-February-2014
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer System Source File.
  *          This file contains the system clock configuration for STM32L1xx Ultra
  *          Low Power devices and is customized for use with stm32f152_nucleo Kit. 
  *          The STM32F152 is configured to run at 32 MHz, following the three  
  *          configuration below:
  *            - PLL_SOURCE_HSI   (default)     : HSI (~16MHz) used to clock the PLL, and
  *                                               the PLL is used as system clock source.
  *            - PLL_SOURCE_HSE                 : HSE (8MHz) used to clock the PLL, and 
  *                                               the PLL is used as system clock source.
  *            - PLL_SOURCE_HSE_BYPASS          : HSE bypassed with an external clock 
  *                                              (8MHz, coming from ST-Link) used to clock
  *                                              the PLL, and the PLL is used as system
  *                                              clock source.             
  * 1.  This file provides two functions and one global variable to be called from 
  *     user application:
  *      - SystemInit(): Setups the system clock (System clock source, PLL Multiplier
  *                      and Divider factors, AHB/APBx prescalers and Flash settings),                      
  *                      This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32l1xx_xx.s" file.
  *                        
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick 
  *                                  timer or configure other parameters.
  *                                     
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.   
  *      
  * 2. After each device reset the MSI (2.1 MHz Range) is used as system clock source.
  *    Then SystemInit() function is called, in "startup_stm32l1xx_xx.s" file, to
  *    configure the system clock before to branch to main program.    
  *    
  * 3. If the system clock source selected by user fails to startup, the SystemInit()
  *    function will do nothing and MSI still used as system clock source. User can 
  *    add some code to deal with this issue inside the SetSysClock() function.
  * 
  * 4. The default value of HSE crystal is set to 8MHz, refer to "HSE_VALUE" define
  *    in "stm32l1xx.h" file. When HSE is used as system clock source, directly or
  *    through PLL, and you are using different crystal you have to adapt the HSE
  *    value to your own configuration.
  * 
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

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32l1xx_system
  * @{
  */  
  
/** @addtogroup STM32L1xx_System_Private_Includes
  * @{
  */

#include "stm32l1xx.h"

/**
  * @}
  */

/** @addtogroup STM32L1xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32L1xx_System_Private_Defines
  * @{
  */
  
/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */ 
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x0 /*!< Vector Table base offset field. 
                                  This value must be a multiple of 0x200. */

/* Select the PLL clock source */

#ifndef SYSCLK_MSI
#define PLL_SOURCE_HSI        /* HSI (~16MHz) used to clock the PLL, and the PLL is used as system clock source*/
//#define PLL_SOURCE_HSE        /* HSE (8MHz) used to clock the PLL, and the PLL is used as system clock source */
//#define PLL_SOURCE_HSE_BYPASS   /* HSE bypassed with an external clock (8MHz, coming from ST-Link) used to clock
//                                   the PLL, and the PLL is used as system clock source */
#else
#define SYSCLK_FREQ_MSI_4MHz      4000000
#endif  /*  SYSCLK_MSI */



/**
  * @}
  */


/** @addtogroup STM32L1xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32L1xx_System_Private_Variables
  * @{
  */
uint32_t SystemCoreClock    = 32000000;
__I uint8_t PLLMulTable[9] = {3, 4, 6, 8, 12, 16, 24, 32, 48};
__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

/**
  * @}
  */

/** @addtogroup STM32L1xx_System_Private_FunctionPrototypes
  * @{
  */

static void SetSysClock(void);

#ifdef SYSCLK_MSI
static void SetSysClockToMSI(void);
#endif 

/**
  * @}
  */

/** @addtogroup STM32L1xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system.
  *         Initialize the Embedded Flash Interface, the PLL and update the 
  *         SystemCoreClock variable.
  * @param  None
  * @retval None
  */
void SystemInit (void)
{
  /*!< Set MSION bit */
  RCC->CR |= (uint32_t)0x00000100;

  /*!< Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], MCOSEL[2:0] and MCOPRE[2:0] bits */
  RCC->CFGR &= (uint32_t)0x88FFC00C;
  
  /*!< Reset HSION, HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xEEFEFFFE;

  /*!< Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /*!< Reset PLLSRC, PLLMUL[3:0] and PLLDIV[1:0] bits */
  RCC->CFGR &= (uint32_t)0xFF02FFFF;

  /*!< Disable all interrupts */
  RCC->CIR = 0x00000000;
    
  /* Configure the System clock frequency, AHB/APBx prescalers and Flash settings */
  SetSysClock();

#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM. */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH. */
#endif
}

/**
  * @brief  Update SystemCoreClock according to Clock Register Values
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *           
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.         
  *     
  * @note   - The system frequency computed by this function is not the real 
  *           frequency in the chip. It is calculated based on the predefined 
  *           constant and the selected clock source:
  *             
  *           - If SYSCLK source is MSI, SystemCoreClock will contain the MSI 
  *             value as defined by the MSI range.
  *                                   
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
  *                                              
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
  *                          
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**)
  *             or HSI_VALUE(*) multiplied/divided by the PLL factors.
  *         
  *         (*) HSI_VALUE is a constant defined in stm32l1xx.h file (default value
  *             16 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.   
  *    
  *         (**) HSE_VALUE is a constant defined in stm32l1xx.h file (default value
  *              8 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *                
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate (void)
{
  uint32_t tmp = 0, pllmul = 0, plldiv = 0, pllsource = 0, msirange = 0;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;
  
  switch (tmp)
  {
    case 0x00:  /* MSI used as system clock */
      msirange = (RCC->ICSCR & RCC_ICSCR_MSIRANGE) >> 13;
      SystemCoreClock = (32768 * (1 << (msirange + 1)));
      break;
    case 0x04:  /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x08:  /* HSE used as system clock */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x0C:  /* PLL used as system clock */
      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmul = RCC->CFGR & RCC_CFGR_PLLMUL;
      plldiv = RCC->CFGR & RCC_CFGR_PLLDIV;
      pllmul = PLLMulTable[(pllmul >> 18)];
      plldiv = (plldiv >> 22) + 1;
      
      pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;

      if (pllsource == 0x00)
      {
        /* HSI oscillator clock selected as PLL clock entry */
        SystemCoreClock = (((HSI_VALUE) * pllmul) / plldiv);
      }
      else
      {
        /* HSE selected as PLL clock entry */
        SystemCoreClock = (((HSE_VALUE) * pllmul) / plldiv);
      }
      break;
    default: /* MSI used as system clock */
      msirange = (RCC->ICSCR & RCC_ICSCR_MSIRANGE) >> 13;
      SystemCoreClock = (32768 * (1 << (msirange + 1)));
      break;
  }
  /* Compute HCLK clock frequency --------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK clock frequency */
  SystemCoreClock >>= tmp;
}

/**
  * @brief  Configures the System clock frequency, AHB/APBx prescalers and Flash 
  *         settings.
  * @note   This function should be called only once the RCC clock configuration  
  *         is reset to the default reset state (done in SystemInit() function).
  * @param  None
  * @retval None
  */
static void SetSysClock(void)
{
#ifdef SYSCLK_MSI
  SetSysClockToMSI();
#else

  __IO uint32_t StartUpCounter = 0, HSIStatus = 0, HSEStatus = 0;
  
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
  
#ifdef PLL_SOURCE_HSI  

    /* Enable HSI */
  RCC->CR |= ((uint32_t)RCC_CR_HSION);
   
   /* Wait till HSI is ready and if Time out is reached exit */
  do
  {
    HSIStatus = RCC->CR & RCC_CR_HSIRDY;
  } while((HSIStatus == 0) && (StartUpCounter != HSI_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSIRDY) != RESET)
  {
    HSIStatus = (uint32_t)0x01;
  }
  else
  {
    HSIStatus = (uint32_t)0x00;
  }
    
  if (HSIStatus == (uint32_t)0x01)
  {
    /*  PLL configuration: PLLCLK = (HSI * 6)/3 = 32 MHz */
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL |RCC_CFGR_PLLDIV));
  RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSI | RCC_CFGR_PLLMUL4 | RCC_CFGR_PLLDIV2 );
  }
  
      else
  {
    /* If HSI fails to start-up, the application will have wrong clock
    configuration. User can add here some code to deal with this error */
  }
  
#else /* PLL_SOURCE_HSE_BYPASS or PLL_SOURCE_HSE */  
  
  /* Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
#ifdef PLL_SOURCE_HSE_BYPASS
  RCC->CR |= ((uint32_t)RCC_CR_HSEBYP);
#endif   /* PLL_SOURCE_HSE_BYPASS */
  
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));
  
  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }
  
  if (HSEStatus == (uint32_t)0x01)
  {
    
    /*  PLL configuration PLLCLK = (HSE * 8)/2 = 32 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL |
                                        RCC_CFGR_PLLDIV));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMUL8 | RCC_CFGR_PLLDIV2);
    
  }
  
      else
  {
    /* If HSE fails to start-up, the application will have wrong clock
    configuration. User can add here some code to deal with this error */
  }

  
#endif /*PLL_SOURCE_HSI*/
  
    /* Enable 64-bit access */
  FLASH->ACR |= FLASH_ACR_ACC64;
  
  /* Enable Prefetch Buffer */
  FLASH->ACR |= FLASH_ACR_PRFTEN;
  
  /* Flash 1 wait state */
  FLASH->ACR |= FLASH_ACR_LATENCY;
  
  /* Power enable */
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  
  /* Select the Voltage Range 1 (1.8 V) */
  PWR->CR = PWR_CR_VOS_0;
  
  /* Wait Until the Voltage Regulator is ready */
  while((PWR->CSR & PWR_CSR_VOSF) != RESET)
  {
  }
  
  /* HCLK = SYSCLK /1*/
  RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
  
  /* PCLK2 = HCLK /1*/
  RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
  
  /* PCLK1 = HCLK /1*/
  RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
  
#ifdef SYSCLK_FREQ_HSI_32MHz /* new code */
    
  /*  PLL configuration */
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL |
                                        RCC_CFGR_PLLDIV));
  RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSI | RCC_CFGR_PLLMUL6 | RCC_CFGR_PLLDIV3);

  /* Enable PLL */
  RCC->CR |= RCC_CR_PLLON;

  /* Wait till PLL is ready */
  while((RCC->CR & RCC_CR_PLLRDY) == 0)
  {
  }
        
  /* Select PLL as system clock source */
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

  /* Wait till PLL is used as system clock source */
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
  {
  }    
    
#else /* original code */

  /* Enable PLL */
  RCC->CR |= RCC_CR_PLLON;
  
  /* Wait till PLL is ready */
  while((RCC->CR & RCC_CR_PLLRDY) == 0)
  {
  }
  
  /* Select PLL as system clock source */
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;
  
  /* Wait till PLL is used as system clock source */
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
  {
  }
#endif /* #ifdef SYSCLK_FREQ_HSI_32MHz */

#endif /* #ifdef SYSCLK_MSI */
}

#ifdef SYSCLK_MSI 

/**
  * @brief  Selects MSI as System clock source and configure HCLK, PCLK2
  *         and PCLK1 prescalers.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
static void SetSysClockToMSI(void)
{
  __IO uint32_t StartUpCounter = 0, MSIStatus = 0;
  
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
  /* Enable MSI */    
  RCC->CR |= ((uint32_t)RCC_CR_MSION);
 
  /* Wait till MSI is ready and if Time out is reached exit */
  do
  {
    MSIStatus = RCC->CR & RCC_CR_MSIRDY;
    StartUpCounter++;  
  } while((MSIStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_MSIRDY) != RESET)
  {
    MSIStatus = (uint32_t)0x01;
  }
  else
  {
    MSIStatus = (uint32_t)0x00;
  }  
  
  if (MSIStatus == (uint32_t)0x01)
  { 
#ifdef SYSCLK_MSI
#ifdef SYSCLK_FREQ_MSI_4MHz
    /* Enable 64-bit access */
    FLASH->ACR |= FLASH_ACR_ACC64;
    
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTEN;    

    /* Flash 1 wait state */
    FLASH->ACR |= FLASH_ACR_LATENCY; 
#else
    /* Flash 0 wait state */
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    
    /* Disable Prefetch Buffer */
    FLASH->ACR &= ~FLASH_ACR_PRFTEN;    

    /* Disable 64-bit access */
    FLASH->ACR &= ~FLASH_ACR_ACC64;
#endif
#endif
    /* Enable the PWR APB1 Clock */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  
    /* Select the Voltage Range 3 (1.2V) */
    PWR->CR = PWR_CR_VOS;
  
    /* Wait Until the Voltage Regulator is ready */
    while((PWR->CSR & PWR_CSR_VOSF) != RESET)
    {
    }
    
    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /* PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;

#ifdef SYSCLK_MSI
  #ifdef SYSCLK_FREQ_MSI_64KHz
    /* Set MSI clock range */
    RCC->ICSCR &= (uint32_t)((uint32_t)~(RCC_ICSCR_MSIRANGE));
    RCC->ICSCR |= (uint32_t)RCC_ICSCR_MSIRANGE_0;   
  #elif defined SYSCLK_FREQ_MSI_128KHz
    /* Set MSI clock range */
    RCC->ICSCR &= (uint32_t)((uint32_t)~(RCC_ICSCR_MSIRANGE));
    RCC->ICSCR |= (uint32_t)RCC_ICSCR_MSIRANGE_1; 
  #elif defined SYSCLK_FREQ_MSI_256KHz
    /* Set MSI clock range */
    RCC->ICSCR &= (uint32_t)((uint32_t)~(RCC_ICSCR_MSIRANGE));    
    RCC->ICSCR |= (uint32_t)RCC_ICSCR_MSIRANGE_2;
  #elif defined SYSCLK_FREQ_MSI_512KHz
    /* Set MSI clock range */
    RCC->ICSCR &= (uint32_t)((uint32_t)~(RCC_ICSCR_MSIRANGE));
    RCC->ICSCR |= (uint32_t)RCC_ICSCR_MSIRANGE_3;
  #elif defined SYSCLK_FREQ_MSI_1MHz
    /* Set MSI clock range */
    RCC->ICSCR &= (uint32_t)((uint32_t)~(RCC_ICSCR_MSIRANGE));
    RCC->ICSCR |= (uint32_t)RCC_ICSCR_MSIRANGE_4;
  #elif defined SYSCLK_FREQ_MSI_2MHz
    /* Set MSI clock range */
    RCC->ICSCR &= (uint32_t)((uint32_t)~(RCC_ICSCR_MSIRANGE));    
    RCC->ICSCR |= (uint32_t)RCC_ICSCR_MSIRANGE_5;
  #elif defined SYSCLK_FREQ_MSI_4MHz     
    /* Set MSI clock range */
    RCC->ICSCR &= (uint32_t)((uint32_t)~(RCC_ICSCR_MSIRANGE));    
    RCC->ICSCR |= (uint32_t)RCC_ICSCR_MSIRANGE_6;
  #endif
#endif    
    
    /* Select MSI as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_MSI;    

    /* Wait till MSI is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x00)
    {
    }
  }
  else
  { 
    /* If MSI fails to start-up, the application will have wrong clock 
       configuration. User can add here some code to deal with this error */    
  }
}
#endif /* SYSCLK_MSI */


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
