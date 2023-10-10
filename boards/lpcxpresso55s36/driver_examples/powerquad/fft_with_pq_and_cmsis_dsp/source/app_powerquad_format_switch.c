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

/* input */
void App_PowerQuad_float_to_q31_Example(void)
{
    uint32_t i;
    pq_config_t pq_cfg;

    PRINTF("%s\r\n", __func__);

    /* input. */
    for (i = 0u; i < APP_FFT_LEN_512; i++)
    {
        inputF32[i*2  ] = (1.0f + i%2); /* real part. */
        inputF32[i*2+1] = 0.0f;         /* imaginary part. */

        inputQ31[i*2  ] = 0; /* clear output. */
        inputQ31[i*2+1] = 0;
    }

    /* convert the data. */
    PQ_Init(POWERQUAD);

    pq_cfg.inputAFormat = kPQ_32Bit; /* input. */
    pq_cfg.inputAPrescale = 0;
    pq_cfg.outputFormat = kPQ_Float; /* output */
    pq_cfg.outputPrescale = 0;
    pq_cfg.machineFormat = kPQ_Float;
    PQ_SetConfig(POWERQUAD, &pq_cfg);

    TimerCount_Start();

    /* total 1024 items for 512-point CFFT. */
    PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, inputF32    , inputQ31    ); /* 256 items. */
    PQ_WaitDone(POWERQUAD);
    PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, inputF32+256, inputQ31+256); /* 256 items. */
    PQ_WaitDone(POWERQUAD);
    PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, inputF32+512, inputQ31+512); /* 256 items. */
    PQ_WaitDone(POWERQUAD);
    PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, inputF32+768, inputQ31+768); /* 256 items. */
    PQ_WaitDone(POWERQUAD);

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

/* output */
void App_PowerQuad_q31_to_float_Example(void)
{
    uint32_t i;
    pq_config_t pq_cfg;

    PRINTF("%s\r\n", __func__);

    /* input. */
    for (i = 0u; i < APP_FFT_LEN_512; i++)
    {
        outputQ31[2*i  ] = (1 + i%2); /* real part. */
        outputQ31[2*i+1] = 0;         /* imaginary part. */

        outputF32[2*i  ] = 0.0f; /* clear output. */
        outputF32[2*i+1] = 0.0f;
    }

    /* convert the data. */
    PQ_Init(POWERQUAD);

    pq_cfg.inputAFormat = kPQ_32Bit;
    pq_cfg.inputAPrescale = 0;
    pq_cfg.outputFormat = kPQ_Float;
    pq_cfg.outputPrescale = 0;
    pq_cfg.machineFormat = kPQ_Float;
    PQ_SetConfig(POWERQUAD, &pq_cfg);

    TimerCount_Start();

    PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, outputQ31    , outputF32    ); /* 256 items. */
    PQ_WaitDone(POWERQUAD);
    PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, outputQ31+256, outputF32+256); /* 256 items. */
    PQ_WaitDone(POWERQUAD);
    PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, outputQ31+512, outputF32+512); /* 256 items. */
    PQ_WaitDone(POWERQUAD);
    PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, outputQ31+768, outputF32+768); /* 256 items. */
    PQ_WaitDone(POWERQUAD);

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

