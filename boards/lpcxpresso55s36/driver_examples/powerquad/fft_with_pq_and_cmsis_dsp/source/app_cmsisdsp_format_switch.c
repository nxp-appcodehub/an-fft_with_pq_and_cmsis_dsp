/*
* Copyright 2023 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#include "app.h"

extern uint32_t   timerCounter;

extern float      inputF32[APP_FFT_LEN_512*2];
extern float     outputF32[APP_FFT_LEN_512*2];
extern q31_t      inputQ31[APP_FFT_LEN_512*2];
extern q31_t     outputQ31[APP_FFT_LEN_512*2];


/* input. */
void App_CmsisDsp_float_to_q31_Example(void)
{
    uint32_t i;

    PRINTF("%s\r\n", __func__);

    /* input. */
    for (i = 0u; i < APP_FFT_LEN_512; i++)
    {
        inputF32[2*i  ] = (1.0f + i%2)/ 10.0f; /* real part. */
        inputF32[2*i+1] = 0.0f;         /* imaginary part. */

        inputQ31[2*i  ] = 0;
        inputQ31[2*i+1] = 0;
    }

    TimerCount_Start();

    arm_float_to_q31(inputF32, inputQ31, APP_FFT_LEN_512*2);

    TimerCount_Stop(timerCounter);

    /* output. */
#if defined(APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS) && (APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS==1)
    PRINTF("Output :\r\n");
    for (i = 0u; i < APP_FFT_LEN_512; i++)
    {
        PRINTF("%4d: 0x%x, 0x%x\r\n", i, inputQ31[2*i], inputQ31[2*i+1]);
    }
#endif /* APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS */
    PRINTF("Cycles : %6d  | us : %d\r\n", timerCounter, timerCounter/TIMER_TICKERS_PER_US);
    PRINTF("\r\n");
}

void App_CmsisDsp_q31_to_float_Example(void)
{
    uint32_t i;

    PRINTF("%s\r\n", __func__);

    /* input. */
    for (i = 0u; i < APP_FFT_LEN_512; i++)
    {
        inputF32[2*i  ] = (1.0f + i%2)/ 10.0f; /* real part. */
        inputF32[2*i+1] = 0.0f;         /* imaginary part. */

        outputF32[i*2  ] = 0.0f;
        outputF32[i*2+1] = 0.0f;
    }

    arm_float_to_q31(inputF32, outputQ31, APP_FFT_LEN_512*2);

    TimerCount_Start();

    arm_q31_to_float(outputQ31, outputF32, APP_FFT_LEN_512*2);

    TimerCount_Stop(timerCounter);

    /* output. */
#if defined(APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS) && (APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS==1)
    PRINTF("Output :\r\n");
    for (i = 0u; i < APP_FFT_LEN_512; i++)
    {
        PRINTF("%4d: %f, %f\r\n", i, outputF32[2*i], outputF32[2*i+1]);
    }
#endif /* APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS */
    PRINTF("Cycles : %6d  | us : %d\r\n", timerCounter, timerCounter/TIMER_TICKERS_PER_US);
    PRINTF("\r\n");
}

/* EOF. */

