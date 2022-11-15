/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2021/03/09
 * Description        : ADC sampling examples, including temperature detection, single-channel detection, differential channel detection, TouchKey detection, interrupt sampling.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

#include "CH58x_common.h"

uint16_t abcBuff[40];

volatile uint8_t adclen;
volatile uint8_t DMA_end = 0;

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
    uint8_t      i;
    signed short RoughCalib_Value = 0; // ADC coarse offset value

    SetSysClock(CLK_SOURCE_PLL_60MHz);

    /* Configure serial debugging */
    DebugInit();
    PRINT("Start @ChipID=%02X\n", R8_CHIP_ID);
    /* Temperature sampling and output */
    PRINT("\n1.Temperature sampling...\n");
    ADC_InterTSSampInit();
    for(i = 0; i < 20; i++)
    {
        abcBuff[i] = ADC_ExcutSingleConver(); // 20 consecutive samples
    }
    for(i = 0; i < 20; i++)
    {
        PRINT("%d \n", adc_to_temperature_celsius(abcBuff[i]));
    }

    /* Single-channel sampling: select adc channel 0 for sampling, corresponding to the PA4 pin, with data calibration function */
    PRINT("\n2.Single channel sampling...\n");
    GPIOA_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_Floating);
    ADC_ExtSingleChSampInit(SampleFreq_3_2, ADC_PGA_0);

    RoughCalib_Value = ADC_DataCalib_Rough(); // Used to calculate the internal deviation of the ADC, recorded in the global variable RoughCalib_Value
    PRINT("RoughCalib_Value =%d \n", RoughCalib_Value);

    ADC_ChannelCfg(0);

    for(i = 0; i < 20; i++)
    {
        abcBuff[i] = ADC_ExcutSingleConver() + RoughCalib_Value; // 20 consecutive samples
    }
    for(i = 0; i < 20; i++)
    {
        PRINT("%d \n", abcBuff[i]); // Note: Due to the existence of the internal deviation of the ADC, when the sampling voltage is near the limit of the selected gain range, data overflow may occur
    }

    /* DMA single-channel sampling: select adc channel 0 for sampling, corresponding to the PA4 pin */
    GPIOA_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_Floating);
    ADC_ExtSingleChSampInit(SampleFreq_3_2, ADC_PGA_0);
    ADC_ChannelCfg(0);
    ADC_AutoConverCycle(192); // The sampling period is (256-192)*16 system clocks
    ADC_DMACfg(ENABLE, (uint16_t)(uint32_t)&abcBuff[0], (uint16_t)(uint32_t)&abcBuff[40], ADC_Mode_Single);
    PFIC_EnableIRQ(ADC_IRQn);
    ADC_StartDMA();
    while(!DMA_end);
    DMA_end = 0;
    PRINT("ADC DMA end \n");
    for(i = 0; i < 40; i++)
    {
        PRINT("%d \n", abcBuff[i]);
    }

    /* Differential channel sampling: select adc channel 0 for sampling, corresponding to PA4(AIN0), PA12(AIN2) */
    PRINT("\n3.Diff channel sampling...\n");
    GPIOA_ModeCfg(GPIO_Pin_4 | GPIO_Pin_12, GPIO_ModeIN_Floating);
    ADC_ExtDiffChSampInit(SampleFreq_3_2, ADC_PGA_0);
    ADC_ChannelCfg(0);
    for(i = 0; i < 20; i++)
    {
        abcBuff[i] = ADC_ExcutSingleConver(); // 20 consecutive samples
    }
    for(i = 0; i < 20; i++)
    {
        PRINT("%d \n", abcBuff[i]);
    }

    /* TouchKey sampling: select adc channel 2 for sampling, corresponding to PA12 */
    PRINT("\n4.TouchKey sampling...\n");
    GPIOA_ModeCfg(GPIO_Pin_12, GPIO_ModeIN_Floating);
    TouchKey_ChSampInit();
    ADC_ChannelCfg(2);

    for(i = 0; i < 20; i++)
    {
        abcBuff[i] = TouchKey_ExcutSingleConver(0x10, 0); // 20 consecutive samples
    }
    for(i = 0; i < 20; i++)
    {
        PRINT("%d \n", abcBuff[i]);
    }

    /* Single-channel sampling: interrupt mode, select adc channel 1 for sampling, corresponding to PA5 pin, without data calibration function */
    PRINT("\n5.Single channel sampling in interrupt mode...\n");
    GPIOA_ModeCfg(GPIO_Pin_5, GPIO_ModeIN_Floating);
    ADC_ExtSingleChSampInit(SampleFreq_3_2, ADC_PGA_0);
    ADC_ChannelCfg(1);
    adclen = 0;
    ADC_ClearITFlag();
    PFIC_EnableIRQ(ADC_IRQn);

    ADC_StartUp();
    while(adclen < 20);
    PFIC_DisableIRQ(ADC_IRQn);
    for(i = 0; i < 20; i++)
    {
        PRINT("%d \n", abcBuff[i]);
    }

    while(1);
}

/*********************************************************************
 * @fn      ADC_IRQHandler
 *
 * @brief   ADC interrupt function
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void ADC_IRQHandler(void) //adc interrupt service routine
{
    if(ADC_GetDMAStatus())
    {
        ADC_StopDMA();
        R16_ADC_DMA_BEG = ((uint32_t)abcBuff) & 0xffff;
        ADC_ClearDMAFlag();
        DMA_end = 1;
    }
    if(ADC_GetITStatus())
    {
        ADC_ClearITFlag();
        if(adclen < 20)
        {
            abcBuff[adclen] = ADC_ReadConverValue();
            ADC_StartUp(); // Function to clear the interrupt flag and start a new round of sampling
        }
        adclen++;
    }
}
