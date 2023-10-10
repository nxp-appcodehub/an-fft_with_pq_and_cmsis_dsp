/*
* Copyright 2023 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "app.h"

extern uint32_t   timerCounter;
extern float32_t  inputF32[APP_FFT_LEN_512*2];
extern float32_t outputF32[APP_FFT_LEN_512*2];

void App_CmsisDsp_CFFT_F32_Example(void)
{
    uint32_t i;

    PRINTF("%s\r\n", __func__);

    /* input. */
    for (i = 0u; i < APP_FFT_LEN_512; i++)
    {
        inputF32[2*i  ] = (1.0f + i%2); /* real part. */
        inputF32[2*i+1] = 0;                    /* complex part. */
    }

    TimerCount_Start();
    arm_cfft_f32(&arm_cfft_sR_f32_len512, inputF32, 0, 1);
    TimerCount_Stop(timerCounter);

    /* output. */
#if defined(APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS) && (APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS==1)
    PRINTF("Output :\r\n");
    for (i = 0u; i < APP_FFT_LEN_512; i++)
    {
        PRINTF("%4d: %f, %f\r\n", i, inputF32[2*i], inputF32[2*i+1]);
    }
#endif /* APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS */
    PRINTF("Cycles : %6d  | us : %d\r\n", timerCounter, timerCounter/TIMER_TICKERS_PER_US);
    PRINTF("\r\n");
}

/* EOF. */

