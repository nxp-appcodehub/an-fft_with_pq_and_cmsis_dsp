/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/
uint32_t   timerCounter;

float32_t  inputF32[APP_FFT_LEN_512*2];
float32_t outputF32[APP_FFT_LEN_512*2];
q31_t      inputQ31[APP_FFT_LEN_512*2];
q31_t     outputQ31[APP_FFT_LEN_512*2];
q15_t      inputQ15[APP_FFT_LEN_512*2];
q15_t     outputQ15[APP_FFT_LEN_512*2];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern void App_CmsisDsp_CFFT_F32_Example(void);
extern void App_CmsisDsp_CFFT_Q31_Example(void);
extern void App_CmsisDsp_CFFT_Q15_Example(void);
extern void App_CmsisDsp_RFFT_Fast_F32_Example(void);
extern void App_CmsisDsp_RFFT_Q31_Example(void);
extern void App_CmsisDsp_RFFT_Q15_Example(void);

extern void App_PowerQuad_CFFT_Q31_Example(void);
extern void App_PowerQuad_RFFT_Q31_Example(void);
extern void App_PowerQuad_CFFT_Q15_Example(void);
extern void App_PowerQuad_RFFT_Q15_Example(void);
extern void App_PowerQuad_CFFT_F32_Example(void);
extern void App_PowerQuad_RFFT_F32_Example(void);

extern void App_CmsisDsp_q31_to_float_Example(void);
extern void App_CmsisDsp_float_to_q31_Example(void);

extern void App_PowerQuad_q31_to_float_Example(void);
extern void App_PowerQuad_float_to_q31_Example(void);

void test_pq_matrix_scale(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("\r\nARM CMSIS-DSP vs PowerQuad FFT.\r\n\r\n");

    while (1)
    {
        /* CMSIS-DSP FFT. */
        GETCHAR(); App_CmsisDsp_CFFT_F32_Example();
        GETCHAR(); App_CmsisDsp_CFFT_Q31_Example();
        GETCHAR(); App_CmsisDsp_CFFT_Q15_Example();
        GETCHAR(); App_CmsisDsp_RFFT_Fast_F32_Example();
        GETCHAR(); App_CmsisDsp_RFFT_Q31_Example();
        GETCHAR(); App_CmsisDsp_RFFT_Q15_Example();

        /* PowerQuad FFT. */
        GETCHAR(); App_PowerQuad_CFFT_Q31_Example();
        GETCHAR(); App_PowerQuad_RFFT_Q31_Example();
        GETCHAR(); App_PowerQuad_CFFT_Q15_Example();
        GETCHAR(); App_PowerQuad_RFFT_Q15_Example();
        GETCHAR(); App_PowerQuad_CFFT_F32_Example();
        GETCHAR(); App_PowerQuad_RFFT_F32_Example(); // issued.
        //GETCHAR(); test_pq_matrix_scale();


        /* Number format conversion. */
        GETCHAR(); App_CmsisDsp_float_to_q31_Example();
        GETCHAR(); App_CmsisDsp_q31_to_float_Example();
        GETCHAR(); App_PowerQuad_float_to_q31_Example();
        GETCHAR(); App_PowerQuad_q31_to_float_Example();
    }
}

void test_pq_matrix_scale(void)
{
    uint32_t i;
    pq_config_t pq_cfg;

    PRINTF("%s\r\n", __func__);

    for (i = 0u; i < 36u; i++)
    {
        inputF32[i] = 1.0f * i; //8.0f * i + 7.0f;
    }

    //inputF32[0 ] = 1.0f;
#if 0
    inputF32[0 ] = 1.0f;
    inputF32[1]  = 2.0f;
    inputF32[2]  = 4.0f;

    inputF32[7 ] = 1.0f;
    inputF32[15] = 2.0f;
    inputF32[23] = 4.0f;
    inputF32[31] = 3.0f;
#endif
    memset(inputQ31 , 0, sizeof(inputQ31 ));

    PQ_Init(POWERQUAD);

#if 1
    //PQ_GetDefaultConfig(&pq_cfg);
    pq_cfg.inputAFormat = kPQ_Float;
    pq_cfg.inputAPrescale = 0;
    pq_cfg.outputFormat = kPQ_32Bit; /* output. */
    pq_cfg.outputPrescale = 0;
    pq_cfg.tmpBase = (uint32_t *)0xE0000000; /* private ram. */
    pq_cfg.machineFormat = kPQ_Float;
    PQ_SetConfig(POWERQUAD, &pq_cfg);
#endif

#if 0
//#define PQ_SET_MAT_FIX32_WORKAROUND_SCALE_CONFIG
    POWERQUAD->OUTFORMAT = ((uint32_t)(0) << 8U) | ((uint32_t)kPQ_Float << 4U) | kPQ_Float;
    POWERQUAD->INAFORMAT = ((uint32_t)(0) << 8U) | ((uint32_t)kPQ_32Bit << 4U) | kPQ_Float;
    POWERQUAD->INBFORMAT = ((uint32_t)(0) << 8U) | ((uint32_t)kPQ_32Bit << 4U) | kPQ_Float;
    POWERQUAD->TMPFORMAT = ((uint32_t)(0) << 8U) | ((uint32_t)kPQ_Float << 4U) | kPQ_Float;
    POWERQUAD->TMPBASE = 0xE0000000;
#endif

    PQ_MatrixScale(POWERQUAD, (6u << 8u) | 6u, 1.0f, inputF32    , inputQ31    ); /* 36 items. */
    //PQ_MatrixScale(POWERQUAD, (5u << 8u) | 5u, 1.0f, inputF32    , inputQ31    ); /* 25 items. */
    //PQ_MatrixScale(POWERQUAD, (16u << 8u) | 1u, 1.0f, inputF32    , inputQ31    ); /* 256 items. */
    //PQ_MatrixScale(POWERQUAD, (1u << 8u) | 16u, 1.0f, inputF32    , inputQ31    ); /* 16 items. */
    //PQ_MatrixScale(POWERQUAD, (16u << 8u) | 1u, 2.0f / 2147483648.0f, inputF32    , inputQ31    ); /* 256 items. */

    PQ_WaitDone(POWERQUAD);

    for (i = 0u; i < 36u; i++)
    {
        PRINTF("[%4d]: %f, %d\r\n", i, inputF32[i], inputQ31[i]);
    }
    PRINTF("DONE..\r\n\r\n");
}

/*
test_pq_matrix_scale
[   0]: 0, 0
[1072693248]: 1.000000, 1
[1073741824]: 2.000000, 2
[1074266112]: 3.000000, 3
[1074790400]: 4.000000, 4
[1075052544]: 5.000000, 5
[1075314688]: 6.000000, 6
[1075576832]: 7.000000, 1088421888 [issue]
[1075838976]: 8.000000, 8
[1075970048]: 9.000000, 9
[1076101120]: 10.000000, 10
[1076232192]: 11.000000, 11
[1076363264]: 12.000000, 12
[1076494336]: 13.000000, 13
[1076625408]: 14.000000, 14
[1076756480]: 15.000000, 1097859072 [issue]
DONE..
*/

/*
* https://www.h-schmidt.net/FloatConverter/IEEE754.html
*/

/* EOF. */

