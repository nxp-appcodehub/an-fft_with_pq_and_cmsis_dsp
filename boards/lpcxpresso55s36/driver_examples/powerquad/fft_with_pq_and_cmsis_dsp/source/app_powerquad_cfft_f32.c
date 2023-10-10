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
extern q31_t      inputQ31[APP_FFT_LEN_512*2];
extern q31_t     outputQ31[APP_FFT_LEN_512*2];

void App_PowerQuad_CFFT_F32_Example(void)
{
    uint32_t i;

    PRINTF("%s\r\n", __func__);

    /* input. */
    for (i = 0u; i < APP_FFT_LEN_512; i++)
    {
#if defined(APP_CFG_POWERQUAD_ENABLE_HW_PRESCALER) && (APP_CFG_POWERQUAD_ENABLE_HW_PRESCALER==1)
        inputF32[2*i  ] = (1.0f + i%2); /* real part. */
#else
        inputF32[2*i  ] = APP_FFT_LEN_512 * (1.0f + i%2); /* real part. */
#endif /* APP_CFG_POWERQUAD_ENABLE_HW_PRESCALER */
        inputF32[2*i+1] = 0; /* complex part. */
    }
    memset(inputQ31 , 0, sizeof(inputQ31 )); /* clear input. */
    memset(outputQ31, 0, sizeof(outputQ31)); /* clear output. */
    memset(outputF32, 0, sizeof(outputF32)); /* clear output. */

    /* initialize the PowerQuad hardware. */
    PQ_Init(POWERQUAD);

    TimerCount_Start();

    /* convert the floating numbers into q31 numbers. */
    {
        pq_config_t pq_cfg;

        pq_cfg.inputAFormat = kPQ_Float;
        pq_cfg.inputAPrescale = 0;
        // pq_cfg.inputBFormat = kPQ_32Bit; /* no use. */
        // pq_cfg.inputBPrescale = 0;
        //pq_cfg.tmpFormat = kPQ_32Bit;
        //pq_cfg.tmpPrescale = 0;
        pq_cfg.outputFormat = kPQ_32Bit;
        pq_cfg.outputPrescale = 0;
        pq_cfg.tmpBase = (uint32_t *)0xE0000000; /* private ram. */
        pq_cfg.machineFormat = kPQ_Float;
        PQ_SetConfig(POWERQUAD, &pq_cfg);

        /* total 1024 items for 512-point CFFT. */
        PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, inputF32    , inputQ31    ); /* 256 items. */
        PQ_WaitDone(POWERQUAD);
        PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, inputF32+256, inputQ31+256); /* 256 items. */
        PQ_WaitDone(POWERQUAD);
        PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, inputF32+512, inputQ31+512); /* 256 items. */
        PQ_WaitDone(POWERQUAD);
        PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, inputF32+768, inputQ31+768); /* 256 items. */
        PQ_WaitDone(POWERQUAD);
#if 0
        PRINTF("PQ_MatrixScale float to q31:\r\n");
        for (i = 0u; i < APP_FFT_LEN_512 * 2u; i++)
        {
            PRINTF("[%4d]: %6f, %6d\r\n", i, inputF32[i], inputQ31[i]);
        }
        PRINTF("\r\n");
#endif
    }

    //GETCHAR(); PRINTF("computing...\r\n");

    /* computing by PowerQuad hardware. */
    {
        pq_config_t pq_cfg;

        pq_cfg.inputAFormat = kPQ_32Bit;
#if defined(APP_CFG_POWERQUAD_ENABLE_HW_PRESCALER) && (APP_CFG_POWERQUAD_ENABLE_HW_PRESCALER==1)
        pq_cfg.inputAPrescale = 9; /* 2 ^9 for 512 len of input. */
#else
        pq_cfg.inputAPrescale = 0;
#endif /* APP_CFG_POWERQUAD_ENABLE_HW_PRESCALER */
        //pq_cfg.inputBFormat = kPQ_32Bit;
        //pq_cfg.inputBPrescale = 0;
        pq_cfg.tmpFormat = kPQ_32Bit;
        pq_cfg.tmpPrescale = 0;
        pq_cfg.outputFormat = kPQ_32Bit;
        pq_cfg.outputPrescale = 0;
        pq_cfg.tmpBase = (uint32_t *)0xE0000000; /* private ram. */
        pq_cfg.machineFormat = kPQ_32Bit;
        PQ_SetConfig(POWERQUAD, &pq_cfg);

        PQ_TransformCFFT(POWERQUAD, APP_FFT_LEN_512, inputQ31, outputQ31);
        PQ_WaitDone(POWERQUAD);

        //PRINTF("FFT Output :\r\n");
        //for (i = 0u; i < APP_FFT_LEN_512; i++)
        //{
        //    PRINTF("[%4d]: %6d, %6d\r\n", i, outputQ31[2*i], outputQ31[2*i+1]);
        //}
        //PRINTF("\r\n");

    }

    //PRINTF("CFFT Done.\r\n");
    //GETCHAR(); PRINTF("convert q31 to floating...\r\n");

    /* convert the q31 numbers into floating numbers. */
    {
        pq_config_t pq_cfg;

        pq_cfg.inputAFormat = kPQ_32Bit;
        pq_cfg.inputAPrescale = 0;
        // pq_cfg.inputBFormat = kPQ_32Bit; /* no use. */
        // pq_cfg.inputBPrescale = 0;
        pq_cfg.tmpFormat = kPQ_Float;
        pq_cfg.tmpPrescale = 0;
        pq_cfg.outputFormat = kPQ_Float;
        pq_cfg.outputPrescale = 0;
        pq_cfg.tmpBase = (uint32_t *)0xE0000000; /* private ram. */
        pq_cfg.machineFormat = kPQ_Float;
        PQ_SetConfig(POWERQUAD, &pq_cfg);

        PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, outputQ31    , outputF32    ); /* 256 items. */
        PQ_WaitDone(POWERQUAD);
        PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, outputQ31+256, outputF32+256); /* 256 items. */
        PQ_WaitDone(POWERQUAD);
        PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, outputQ31+512, outputF32+512); /* 256 items. */
        PQ_WaitDone(POWERQUAD);
        PQ_MatrixScale(POWERQUAD, (16u << 8u) | 16u, 1.0f, outputQ31+768, outputF32+768); /* 256 items. */
        PQ_WaitDone(POWERQUAD);

        //PRINTF("PQ_MatrixScale q31 to float:\r\n");
        //for (i = 0u; i < APP_FFT_LEN_512; i++)
        //{
        //    PRINTF("[%4d]: %6f, %6f\r\n", i, outputF32[2*i], outputF32[2*i+1]);
        //}
        //PRINTF("\r\n");
    }

    TimerCount_Stop(timerCounter);

    //GETCHAR();
    //// ARM conversion
    //PRINTF("arm_q31_to_float:\r\n");
    //arm_q31_to_float(outputQ31, outputF32, APP_FFT_LEN_512 * 2);
    //for (i = 0u; i < APP_FFT_LEN_512; i++)
    //{
    //    PRINTF("[%4d]: %6f, %6f\r\n", i, outputF32[2*i], outputF32[2*i+1]);
    //}

    /* output. */
#if defined(APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS) && (APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS==1)
    PRINTF("Output :\r\n");
    for (i = 0u; i < APP_FFT_LEN_512; i++)
    {
        //PRINTF("%4d: %d, %d\r\n", i, outputQ31[2*i], outputQ31[2*i+1]);
        PRINTF("%4d: %f, %f\r\n", i, outputF32[2*i], outputF32[2*i+1]);
    }
#endif /* APP_CFG_ENABLE_SHOW_OUTPUT_NUMBERS */
    PRINTF("Cycles : %6d  | us : %d\r\n", timerCounter, timerCounter/TIMER_TICKERS_PER_US);
    PRINTF("\r\n");
}


/* EOF. */

