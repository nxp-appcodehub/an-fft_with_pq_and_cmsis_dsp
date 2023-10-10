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

void App_PowerQuad_RFFT_Q15_Example(void)
{
    uint16_t i;

    PRINTF("%s\r\n", __func__);

    /* input. */
    for (i = 0u; i < APP_FFT_LEN_512; i++)
    {

#if defined(APP_CFG_POWERQUAD_ENABLE_HW_PRESCALER) && (APP_CFG_POWERQUAD_ENABLE_HW_PRESCALER==1)
        inputQ15[i  ] = (1 + i%2); /* only real part. */
#else
        inputQ15[i  ] = APP_FFT_LEN_512 * (1 + i%2); /* only real part. */
#endif /* APP_CFG_POWERQUAD_ENABLE_HW_PRESCALER */
    }
    memset(outputQ15, 0, sizeof(outputQ15)); /* clear output. */


    /* computing by PowerQuad hardware. */
    {
        pq_config_t pq_cfg;

        PQ_Init(POWERQUAD); /* initialize the PowerQuad hardware. */

        pq_cfg.inputAFormat = kPQ_16Bit; /* for q15_t. */
#if defined(APP_CFG_POWERQUAD_ENABLE_HW_PRESCALER) && (APP_CFG_POWERQUAD_ENABLE_HW_PRESCALER==1)
        pq_cfg.inputAPrescale = 9; /* 2 ^9 for 512 len of input. */
#else
        pq_cfg.inputAPrescale = 0;
#endif /* APP_CFG_POWERQUAD_ENABLE_HW_PRESCALER */
        pq_cfg.inputBFormat = kPQ_16Bit; /* for q15_t. */
        pq_cfg.inputBPrescale = 0;
        pq_cfg.tmpFormat = kPQ_16Bit; /* for q15_t. */
        pq_cfg.tmpPrescale = 0;
        pq_cfg.outputFormat = kPQ_16Bit; /* for q15_t. */
        pq_cfg.outputPrescale = 0;
        pq_cfg.tmpBase = (uint32_t *)0xE0000000; /* private ram. */
        pq_cfg.machineFormat = kPQ_32Bit; /* even q15_t, they are used as 32-bit internally. */
        PQ_SetConfig(POWERQUAD, &pq_cfg);

        TimerCount_Start();
        PQ_TransformRFFT(POWERQUAD, APP_FFT_LEN_512, inputQ15, outputQ15);
        PQ_WaitDone(POWERQUAD);
        TimerCount_Stop(timerCounter);
    }

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

