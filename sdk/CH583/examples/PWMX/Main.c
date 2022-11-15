/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description 		   : PWM4-11 function demonstration
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

#include "CH58x_common.h"

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

    /* Configure GPIO */
    GPIOA_ModeCfg(GPIO_Pin_12, GPIO_ModeOut_PP_5mA); // PA12 - PWM4
    GPIOA_ModeCfg(GPIO_Pin_13, GPIO_ModeOut_PP_5mA); // PA13 - PWM5
    GPIOB_ModeCfg(GPIO_Pin_0, GPIO_ModeOut_PP_5mA);  // PB0 - PWM6
    GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeOut_PP_5mA);  // PB4 - PWM7
    GPIOB_ModeCfg(GPIO_Pin_6, GPIO_ModeOut_PP_5mA);  // PB6 - PWM8
    GPIOB_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA);  // PB7 - PWM9
    GPIOB_ModeCfg(GPIO_Pin_14, GPIO_ModeOut_PP_5mA); // PB14 - PWM10
    //GPIOB_ModeCfg(GPIO_Pin_23, GPIO_ModeOut_PP_5mA); // PB23 - PWM11 This pin is multiplexed as an external reset pin, you need to turn off this function to enable the PWM function

    PWMX_CLKCfg(4);                                   // cycle = 4/Fsys
    PWMX_CycleCfg(PWMX_Cycle_64);                     // period = 64*cycle
    PWMX_ACTOUT(CH_PWM4, 64 / 4, Low_Level, ENABLE);  // 25% duty cycle
    PWMX_ACTOUT(CH_PWM5, 64 / 4, Low_Level, ENABLE);  // 25% duty cycle
    PWMX_ACTOUT(CH_PWM6, 64 / 4, Low_Level, ENABLE);  // 25% duty cycle
    PWMX_ACTOUT(CH_PWM7, 64 / 4, Low_Level, ENABLE);  // 25% duty cycle
    PWMX_ACTOUT(CH_PWM8, 64 / 4, Low_Level, ENABLE);  // 25% duty cycle
    PWMX_ACTOUT(CH_PWM9, 64 / 4, Low_Level, ENABLE);  // 25% duty cycle
    PWMX_ACTOUT(CH_PWM10, 64 / 4, Low_Level, ENABLE); // 25% duty cycle

    while(1);
}
