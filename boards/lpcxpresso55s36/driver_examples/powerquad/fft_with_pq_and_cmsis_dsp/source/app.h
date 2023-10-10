/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _APP_H_
#define _APP_H_

#include <stdio.h>

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_common.h"
#include "fsl_clock.h"
//#include "fsl_spi.h"
#include "fsl_gpio.h"
#include "fsl_powerquad.h"

//#include "lcd.h"
//#include "lcd_fonts.h"

#include "arm_math.h"
#include "arm_const_structs.h" /* for fft. */

/* for sdcard. */
//#include "fsl_sd.h"
//#include "ff.h"
//#include "diskio.h"
//#include "fsl_sd_disk.h"

//#include "bmp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* TimerCount是基于单周期计数的精细计时
 * TickTimer是基于中断和定时器变量的粗略计时
 * 二者不能同时使用,因为二者工作时配置systick的超时时间不同.
 */
/* Systick Start */
#define TimerCount_Start()	do {\
							SysTick->LOAD  =  0xFFFFFF  ;	/* set reload register */\
							SysTick->VAL  =  0  ;			/* Clear Counter */		 \
							SysTick->CTRL  =  0x5 ;			/* Enable Counting*/	 \
							} while(0)

/* Systick Stop and retrieve CPU Clocks count */
#define TimerCount_Stop(Value) 	do {\
								SysTick->CTRL  =0;	/* Disable Counting */				 \
								Value = SysTick->VAL;/* Load the SysTick Counter Value */\
								Value = 0xFFFFFF - Value;/* Capture Counts in CPU Cycles*/\
								} while(0)

#define TIMER_TICKERS_PER_US   150

#define APP_FFT_LEN_512  512

/* enable to show the computing result values. */
#define APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS  0 //1

/* enable hardware prescale.
 * when disabled, the same software way is used.
 */
#define APP_CFG_POWERQUAD_ENABLE_HW_PRESCALER  1

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void BOARD_InitHardware(void);

#endif /* _APP_H_ */
