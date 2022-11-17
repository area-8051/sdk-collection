/********************************** (C) COPYRIGHT  *******************************
* File Name          : falsh.h
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/12/16
* Description        :
*******************************************************************************/
#ifndef __FLASH_H
#define __FLASH_H

#include "ch32v10x_it.h"
#include "stdio.h"

u8 CH32_IAP_Verity(u32 adr, u32* buf);
void CH32_IAP_Program(u32 adr, u32* buf);
void CH32_IAP_ERASE(u32 Start_adr, u32 End_adr);

#endif