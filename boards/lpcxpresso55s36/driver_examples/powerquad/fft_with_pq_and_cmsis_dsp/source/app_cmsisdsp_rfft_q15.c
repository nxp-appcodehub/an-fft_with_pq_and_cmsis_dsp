/*
* Copyright 2023 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#include "app.h"

extern uint32_t   timerCounter;
extern q15_t      inputQ15[APP_FFT_LEN_512*2];
extern q15_t     outputQ15[APP_FFT_LEN_512*2];
extern const arm_rfft_instance_q15 arm_rfft_sR_q15_len512;
void App_CmsisDsp_RFFT_Q15_Example(void)
{
    uint32_t i;

    PRINTF("%s\r\n", __func__);

    /* input. */
    for (i = 0u; i < APP_FFT_LEN_512; i++)
    {
        inputQ15[i] = APP_FFT_LEN_512 * (1 + i%2); /* only real part. */
    }

    TimerCount_Start();
    arm_rfft_q15(&arm_rfft_sR_q15_len512, inputQ15, outputQ15);
    TimerCount_Stop(timerCounter);

    /* output. */
#if defined(APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS) && (APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS==1)
    PRINTF("Output :\r\n");
    for (i = 0u; i < APP_FFT_LEN_512; i++)
    {
        PRINTF("%4d: %d, %d\r\n", i, outputQ15[2*i], outputQ15[2*i+1]);
    }
#endif /* APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS */
    PRINTF("Cycles : %6d  | us : %d\r\n", timerCounter, timerCounter/TIMER_TICKERS_PER_US);
    PRINTF("\r\n");
}

/* EOF. */

