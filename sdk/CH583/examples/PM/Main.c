/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description        : System sleep mode and wake-up demo: GPIOA_5 is used as a wake-up source, with 4 sleep levels in total
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/*
 * Note: When switching to the HSE clock source, the waiting time for stabilization is related to the selected external crystal parameters. It is best to read the crystal and its load capacitance parameters provided by the manufacturer when choosing a new crystal. By configuring the R8_XT32M_TUNE register, different load capacitances and bias currents can be configured to adjust the crystal stabilization time.
 */

#include "CH58x_common.h"

/*********************************************************************
 * @fn      DebugInit
 *
 * @brief   Debug initialization
 *
 * @return  none
 */
void DebugInit(void)
{
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
}

/*********************************************************************
 * @fn      main
 *
 * @brief   main function
 *
 * @return  none
 */
int main()
{
    SetSysClock(CLK_SOURCE_PLL_60MHz);

    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);

    /* Configure serial debugging */
    DebugInit();
    PRINT("Start @ChipID=%02x\n", R8_CHIP_ID);
    DelayMs(200);

#if 1
    /* Configure wakeup source as GPIO - PA5 */
    GPIOA_ModeCfg(GPIO_Pin_5, GPIO_ModeIN_PU);
    GPIOA_ITModeCfg(GPIO_Pin_5, GPIO_ITMode_FallEdge); // Wake-up on falling edge
    PFIC_EnableIRQ(GPIO_A_IRQn);
    PWR_PeriphWakeUpCfg(ENABLE, RB_SLP_GPIO_WAKE, Long_Delay);
#endif

#if 1
    PRINT("IDLE mode sleep \n");
    DelayMs(1);
    LowPower_Idle();
    PRINT("wake.. \n");
    DelayMs(500);
#endif

#if 1
    PRINT("Halt mode sleep \n");
    DelayMs(2);
    LowPower_Halt();
    HSECFG_Current(HSE_RCur_100); // Reduced to rated current (HSE bias current boosted in low power function)
    DelayMs(2);
    PRINT("wake.. \n");
    DelayMs(500);
#endif

#if 1
    PRINT("sleep mode sleep \n");
    DelayMs(2);
    // Note that when the main frequency is 80M, the sleep wakeup interrupt cannot call the code in the flash.
    LowPower_Sleep(RB_PWR_RAM30K | RB_PWR_RAM2K); // Only keep 30+2K SRAM power supply
    HSECFG_Current(HSE_RCur_100);                 // Reduced to rated current (HSE bias current boosted in low power function)
    DelayMs(5);
    PRINT("wake.. \n");
    DelayMs(500);
#endif

#if 1
    PRINT("shut down mode sleep \n");
    DelayMs(2);
    LowPower_Shutdown(0); // All power off, reset after wake up
    /*
     * This mode will perform reset after waking up, so the following code will not run, pay attention to ensure that the system sleeps and then wakes up to wake up reset, otherwise it may become IDLE level wake-up
     */
    HSECFG_Current(HSE_RCur_100); // Reduced to rated current (HSE bias current boosted in low power function)
    PRINT("wake.. \n");
    DelayMs(500);
#endif

    while(1)
        ;
}

/*********************************************************************
 * @fn      GPIOA_IRQHandler
 *
 * @brief   GPIOA interrupt function
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void GPIOA_IRQHandler(void)
{
    GPIOA_ClearITFlagBit(GPIO_Pin_6 | GPIO_Pin_5);
}
