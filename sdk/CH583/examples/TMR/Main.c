/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description        : Timer example
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

#include "CH58x_common.h"

__attribute__((aligned(4))) uint32_t CapBuf[100];
__attribute__((aligned(4))) uint32_t PwmBuf[100];

volatile uint8_t capFlag = 0;

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
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
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
    uint8_t i;

    SetSysClock(CLK_SOURCE_PLL_60MHz);

    /* Configure serial debugging */
    DebugInit();
    PRINT("Start @ChipID=%02X\n", R8_CHIP_ID);

#if 1 /* Timer 0, set a 100ms timer for IO port flashing, PB15-LED */

    GPIOB_SetBits(GPIO_Pin_15);
    GPIOB_ModeCfg(GPIO_Pin_15, GPIO_ModeOut_PP_5mA);

    TMR0_TimerInit(FREQ_SYS / 10);         // Set the timing time to 100ms
    TMR0_ITCfg(ENABLE, TMR0_3_IT_CYC_END); // Enable interrupts
    PFIC_EnableIRQ(TMR0_IRQn);
#endif

#if 1 /* Timer 3, PWM output */

    GPIOB_ResetBits(GPIO_Pin_22); // Configure PWM port PB22
    GPIOB_ModeCfg(GPIO_Pin_22, GPIO_ModeOut_PP_5mA);

    TMR3_PWMInit(High_Level, PWM_Times_1);
    TMR3_PWMCycleCfg(6000); // period 100us
    TMR3_Disable();
    TMR3_PWMActDataWidth(3000); // The duty cycle is 50%, to modify the duty cycle, the timer must be temporarily closed
    TMR3_Enable();

#endif

#if 1                                      /* Timer 1, CAP capture, */
    PWR_UnitModCfg(DISABLE, UNIT_SYS_LSE); // Note that this pin is the LSE crystal pin, and other functions can only be used if it is turned off.
    GPIOA_ResetBits(GPIO_Pin_10);          // Configure PWM port PA10
    GPIOA_ModeCfg(GPIO_Pin_10, GPIO_ModeIN_PU);

    TMR1_CapInit(Edge_To_Edge);
    TMR1_CAPTimeoutCfg(0xFFFFFFFF); // Set capture timeout
    TMR1_DMACfg(ENABLE, (uint16_t)(uint32_t)&CapBuf[0], (uint16_t)(uint32_t)&CapBuf[100], Mode_Single);
    TMR1_ITCfg(ENABLE, TMR1_2_IT_DMA_END); // Enable DMA completion interrupt
    PFIC_EnableIRQ(TMR1_IRQn);

    while(capFlag == 0);
    capFlag = 0;
    for(i = 0; i < 100; i++)
    {
        PRINT("%08ld ", CapBuf[i] & 0x1ffffff); // 26bit, the highest bit indicates high level or low level
    }
    PRINT("\n");

#endif

#if 1 /* Timer 2, Counter, */
    GPIOB_ModeCfg(GPIO_Pin_11, GPIO_ModeIN_PD);
    GPIOPinRemap(ENABLE, RB_PIN_TMR2);

    TMR2_EXTSingleCounterInit(FallEdge_To_FallEdge);
    TMR2_CountOverflowCfg(1000); // Set the count upper limit to 1000

    /* Turn on the count overflow interrupt, count down 1000 cycles to enter the interrupt */
    TMR2_ClearITFlag(TMR0_3_IT_CYC_END);
    PFIC_EnableIRQ(TMR2_IRQn);
    TMR2_ITCfg(ENABLE, TMR0_3_IT_CYC_END);

    do
    {
        /* The current count value is printed once in 1s. If the input pulse frequency is high, the count may overflow soon, and it needs to be modified according to the actual situation. */
        mDelaymS(1000);
        PRINT("=%ld \n", TMR2_GetCurrentCount());
    } while(1);

#endif

#if 1 /* Timer 2, DMA PWM.*/
    GPIOB_ModeCfg(GPIO_Pin_11, GPIO_ModeOut_PP_5mA);
    GPIOPinRemap(ENABLE, RB_PIN_TMR2);

    PRINT("TMR2 DMA PWM\n");
    TMR2_PWMCycleCfg(120000); // period 2000us
    for(i=0; i<50; i++)
    {
      PwmBuf[i]=2400*i;
    }
    for(i=50; i<100; i++)
    {
      PwmBuf[i]=2400*(100-i);
    }
    TMR2_DMACfg(ENABLE, (uint16_t)(uint32_t)&PwmBuf[0], (uint16_t)(uint32_t)&PwmBuf[100], Mode_LOOP);
    TMR2_PWMInit(Low_Level, PWM_Times_16);
    /* Start the count overflow interrupt, count down 1000 cycles to enter the interrupt */
    TMR2_ClearITFlag(TMR1_2_IT_DMA_END);
    PFIC_EnableIRQ(TMR2_IRQn);
    TMR2_ITCfg(ENABLE, TMR1_2_IT_DMA_END);

#endif

    while(1);
}

/*********************************************************************
 * @fn      TMR0_IRQHandler
 *
 * @brief   TMR0 interrupt function
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void TMR0_IRQHandler(void) // TMR0 timed interrupt
{
    if(TMR0_GetITFlag(TMR0_3_IT_CYC_END))
    {
        TMR0_ClearITFlag(TMR0_3_IT_CYC_END); // clear interrupt flag
        GPIOB_InverseBits(GPIO_Pin_15);
    }
}

/*********************************************************************
 * @fn      TMR1_IRQHandler
 *
 * @brief   TMR1 interrupt function
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void TMR1_IRQHandler(void) // TMR1 timed interrupt
{
    if(TMR1_GetITFlag(TMR1_2_IT_DMA_END))
    {
        TMR1_ITCfg(DISABLE, TMR1_2_IT_DMA_END); // Use the single-shot DMA function + interrupt, pay attention to turn off the interrupt enable after completion, otherwise the interrupt will be reported all the time.
        TMR1_ClearITFlag(TMR1_2_IT_DMA_END);    // clear interrupt flag
        capFlag = 1;
    }
}

/*********************************************************************
 * @fn      TMR2_IRQHandler
 *
 * @brief   TMR2 interrupt function
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void TMR2_IRQHandler(void)
{
    if(TMR2_GetITFlag(TMR0_3_IT_CYC_END))
    {
        TMR2_ClearITFlag(TMR0_3_IT_CYC_END);
        /* When the counter is full, the hardware automatically clears it and restarts counting */
        /* Users can add the required processing by themselves */
    }
    if(TMR2_GetITFlag(TMR1_2_IT_DMA_END))
    {
        TMR2_ClearITFlag(TMR1_2_IT_DMA_END);
        PRINT("DMA end\n");
        /* DMA ends */
        /* Users can add the required processing by themselves */
    }
}
