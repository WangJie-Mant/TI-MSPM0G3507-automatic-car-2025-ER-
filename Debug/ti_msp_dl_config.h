/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)



#define CPUCLK_FREQ                                                     80000000



/* Defines for PWM_MOTOR */
#define PWM_MOTOR_INST                                                     TIMG0
#define PWM_MOTOR_INST_IRQHandler                               TIMG0_IRQHandler
#define PWM_MOTOR_INST_INT_IRQN                                 (TIMG0_INT_IRQn)
#define PWM_MOTOR_INST_CLK_FREQ                                          4000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_MOTOR_C0_PORT                                             GPIOA
#define GPIO_PWM_MOTOR_C0_PIN                                     DL_GPIO_PIN_12
#define GPIO_PWM_MOTOR_C0_IOMUX                                  (IOMUX_PINCM34)
#define GPIO_PWM_MOTOR_C0_IOMUX_FUNC                 IOMUX_PINCM34_PF_TIMG0_CCP0
#define GPIO_PWM_MOTOR_C0_IDX                                DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_MOTOR_C1_PORT                                             GPIOA
#define GPIO_PWM_MOTOR_C1_PIN                                     DL_GPIO_PIN_13
#define GPIO_PWM_MOTOR_C1_IOMUX                                  (IOMUX_PINCM35)
#define GPIO_PWM_MOTOR_C1_IOMUX_FUNC                 IOMUX_PINCM35_PF_TIMG0_CCP1
#define GPIO_PWM_MOTOR_C1_IDX                                DL_TIMER_CC_1_INDEX



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMA1)
#define TIMER_0_INST_IRQHandler                                 TIMA1_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMA1_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                           (892U)



/* Defines for UART1 */
#define UART1_INST                                                         UART0
#define UART1_INST_FREQUENCY                                            40000000
#define UART1_INST_IRQHandler                                   UART0_IRQHandler
#define UART1_INST_INT_IRQN                                       UART0_INT_IRQn
#define GPIO_UART1_RX_PORT                                                 GPIOA
#define GPIO_UART1_TX_PORT                                                 GPIOA
#define GPIO_UART1_RX_PIN                                         DL_GPIO_PIN_11
#define GPIO_UART1_TX_PIN                                         DL_GPIO_PIN_10
#define GPIO_UART1_IOMUX_RX                                      (IOMUX_PINCM22)
#define GPIO_UART1_IOMUX_TX                                      (IOMUX_PINCM21)
#define GPIO_UART1_IOMUX_RX_FUNC                       IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART1_IOMUX_TX_FUNC                       IOMUX_PINCM21_PF_UART0_TX
#define UART1_BAUD_RATE                                                   (9600)
#define UART1_IBRD_40_MHZ_9600_BAUD                                        (260)
#define UART1_FBRD_40_MHZ_9600_BAUD                                         (27)
/* Defines for UART_WIT */
#define UART_WIT_INST                                                      UART2
#define UART_WIT_INST_FREQUENCY                                         40000000
#define UART_WIT_INST_IRQHandler                                UART2_IRQHandler
#define UART_WIT_INST_INT_IRQN                                    UART2_INT_IRQn
#define GPIO_UART_WIT_RX_PORT                                              GPIOA
#define GPIO_UART_WIT_RX_PIN                                      DL_GPIO_PIN_24
#define GPIO_UART_WIT_IOMUX_RX                                   (IOMUX_PINCM54)
#define GPIO_UART_WIT_IOMUX_RX_FUNC                    IOMUX_PINCM54_PF_UART2_RX
#define UART_WIT_BAUD_RATE                                                (9600)
#define UART_WIT_IBRD_40_MHZ_9600_BAUD                                     (260)
#define UART_WIT_FBRD_40_MHZ_9600_BAUD                                      (27)
/* Defines for UART_1 */
#define UART_1_INST                                                        UART1
#define UART_1_INST_FREQUENCY                                            4000000
#define UART_1_INST_IRQHandler                                  UART1_IRQHandler
#define UART_1_INST_INT_IRQN                                      UART1_INT_IRQn
#define GPIO_UART_1_RX_PORT                                                GPIOA
#define GPIO_UART_1_TX_PORT                                                GPIOA
#define GPIO_UART_1_RX_PIN                                        DL_GPIO_PIN_18
#define GPIO_UART_1_TX_PIN                                        DL_GPIO_PIN_17
#define GPIO_UART_1_IOMUX_RX                                     (IOMUX_PINCM40)
#define GPIO_UART_1_IOMUX_TX                                     (IOMUX_PINCM39)
#define GPIO_UART_1_IOMUX_RX_FUNC                      IOMUX_PINCM40_PF_UART1_RX
#define GPIO_UART_1_IOMUX_TX_FUNC                      IOMUX_PINCM39_PF_UART1_TX
#define UART_1_BAUD_RATE                                                (115200)
#define UART_1_IBRD_4_MHZ_115200_BAUD                                        (2)
#define UART_1_FBRD_4_MHZ_115200_BAUD                                       (11)





/* Defines for DMA_WIT */
#define DMA_WIT_CHAN_ID                                                      (0)
#define UART_WIT_INST_DMA_TRIGGER                            (DMA_UART2_RX_TRIG)


/* Port definition for Pin Group KEY */
#define KEY_PORT                                                         (GPIOB)

/* Defines for KEY1: GPIOB.21 with pinCMx 49 on package pin 20 */
#define KEY_KEY1_PIN                                            (DL_GPIO_PIN_21)
#define KEY_KEY1_IOMUX                                           (IOMUX_PINCM49)
/* Port definition for Pin Group LED */
#define LED_PORT                                                         (GPIOB)

/* Defines for LED1: GPIOB.22 with pinCMx 50 on package pin 21 */
#define LED_LED1_PIN                                            (DL_GPIO_PIN_22)
#define LED_LED1_IOMUX                                           (IOMUX_PINCM50)
/* Port definition for Pin Group BUZZ */
#define BUZZ_PORT                                                        (GPIOA)

/* Defines for BUZZ_1: GPIOA.5 with pinCMx 10 on package pin 45 */
#define BUZZ_BUZZ_1_PIN                                          (DL_GPIO_PIN_5)
#define BUZZ_BUZZ_1_IOMUX                                        (IOMUX_PINCM10)
/* Defines for LEFT_A: GPIOA.28 with pinCMx 3 on package pin 35 */
#define ENCODER_LEFT_A_PORT                                              (GPIOA)
// pins affected by this interrupt request:["LEFT_A"]
#define ENCODER_GPIOA_INT_IRQN                                  (GPIOA_INT_IRQn)
#define ENCODER_GPIOA_INT_IIDX                  (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define ENCODER_LEFT_A_IIDX                                 (DL_GPIO_IIDX_DIO28)
#define ENCODER_LEFT_A_PIN                                      (DL_GPIO_PIN_28)
#define ENCODER_LEFT_A_IOMUX                                      (IOMUX_PINCM3)
/* Defines for RIGHT_A: GPIOB.9 with pinCMx 26 on package pin 61 */
#define ENCODER_RIGHT_A_PORT                                             (GPIOB)
// pins affected by this interrupt request:["RIGHT_A"]
#define ENCODER_GPIOB_INT_IRQN                                  (GPIOB_INT_IRQn)
#define ENCODER_GPIOB_INT_IIDX                  (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define ENCODER_RIGHT_A_IIDX                                 (DL_GPIO_IIDX_DIO9)
#define ENCODER_RIGHT_A_PIN                                      (DL_GPIO_PIN_9)
#define ENCODER_RIGHT_A_IOMUX                                    (IOMUX_PINCM26)
/* Defines for LEFT_B: GPIOB.20 with pinCMx 48 on package pin 19 */
#define ENCODER_LEFT_B_PORT                                              (GPIOB)
#define ENCODER_LEFT_B_PIN                                      (DL_GPIO_PIN_20)
#define ENCODER_LEFT_B_IOMUX                                     (IOMUX_PINCM48)
/* Defines for RIGHT_B: GPIOB.3 with pinCMx 16 on package pin 51 */
#define ENCODER_RIGHT_B_PORT                                             (GPIOB)
#define ENCODER_RIGHT_B_PIN                                      (DL_GPIO_PIN_3)
#define ENCODER_RIGHT_B_IOMUX                                    (IOMUX_PINCM16)
/* Port definition for Pin Group MOTOR */
#define MOTOR_PORT                                                       (GPIOA)

/* Defines for AIN1: GPIOA.16 with pinCMx 38 on package pin 9 */
#define MOTOR_AIN1_PIN                                          (DL_GPIO_PIN_16)
#define MOTOR_AIN1_IOMUX                                         (IOMUX_PINCM38)
/* Defines for AIN2: GPIOA.14 with pinCMx 36 on package pin 7 */
#define MOTOR_AIN2_PIN                                          (DL_GPIO_PIN_14)
#define MOTOR_AIN2_IOMUX                                         (IOMUX_PINCM36)
/* Defines for BIN1: GPIOA.22 with pinCMx 47 on package pin 18 */
#define MOTOR_BIN1_PIN                                          (DL_GPIO_PIN_22)
#define MOTOR_BIN1_IOMUX                                         (IOMUX_PINCM47)
/* Defines for BIN2: GPIOA.15 with pinCMx 37 on package pin 8 */
#define MOTOR_BIN2_PIN                                          (DL_GPIO_PIN_15)
#define MOTOR_BIN2_IOMUX                                         (IOMUX_PINCM37)
/* Defines for INF1: GPIOB.8 with pinCMx 25 on package pin 60 */
#define INF_INF1_PORT                                                    (GPIOB)
#define INF_INF1_PIN                                             (DL_GPIO_PIN_8)
#define INF_INF1_IOMUX                                           (IOMUX_PINCM25)
/* Defines for INF2: GPIOB.26 with pinCMx 57 on package pin 28 */
#define INF_INF2_PORT                                                    (GPIOB)
#define INF_INF2_PIN                                            (DL_GPIO_PIN_26)
#define INF_INF2_IOMUX                                           (IOMUX_PINCM57)
/* Defines for INF3: GPIOB.25 with pinCMx 56 on package pin 27 */
#define INF_INF3_PORT                                                    (GPIOB)
#define INF_INF3_PIN                                            (DL_GPIO_PIN_25)
#define INF_INF3_IOMUX                                           (IOMUX_PINCM56)
/* Defines for INF4: GPIOA.8 with pinCMx 19 on package pin 54 */
#define INF_INF4_PORT                                                    (GPIOA)
#define INF_INF4_PIN                                             (DL_GPIO_PIN_8)
#define INF_INF4_IOMUX                                           (IOMUX_PINCM19)
/* Defines for INF5: GPIOB.19 with pinCMx 45 on package pin 16 */
#define INF_INF5_PORT                                                    (GPIOB)
#define INF_INF5_PIN                                            (DL_GPIO_PIN_19)
#define INF_INF5_IOMUX                                           (IOMUX_PINCM45)
/* Defines for INF6: GPIOB.18 with pinCMx 44 on package pin 15 */
#define INF_INF6_PORT                                                    (GPIOB)
#define INF_INF6_PIN                                            (DL_GPIO_PIN_18)
#define INF_INF6_IOMUX                                           (IOMUX_PINCM44)
/* Defines for INF7: GPIOB.6 with pinCMx 23 on package pin 58 */
#define INF_INF7_PORT                                                    (GPIOB)
#define INF_INF7_PIN                                             (DL_GPIO_PIN_6)
#define INF_INF7_IOMUX                                           (IOMUX_PINCM23)
/* Defines for INF8: GPIOA.9 with pinCMx 20 on package pin 55 */
#define INF_INF8_PORT                                                    (GPIOA)
#define INF_INF8_PIN                                             (DL_GPIO_PIN_9)
#define INF_INF8_IOMUX                                           (IOMUX_PINCM20)
/* Port definition for Pin Group OLED */
#define OLED_PORT                                                        (GPIOA)

/* Defines for SCL: GPIOA.0 with pinCMx 1 on package pin 33 */
#define OLED_SCL_PIN                                             (DL_GPIO_PIN_0)
#define OLED_SCL_IOMUX                                            (IOMUX_PINCM1)
/* Defines for SDA: GPIOA.1 with pinCMx 2 on package pin 34 */
#define OLED_SDA_PIN                                             (DL_GPIO_PIN_1)
#define OLED_SDA_IOMUX                                            (IOMUX_PINCM2)



/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_SYSCTL_CLK_init(void);
void SYSCFG_DL_PWM_MOTOR_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_UART1_init(void);
void SYSCFG_DL_UART_WIT_init(void);
void SYSCFG_DL_UART_1_init(void);
void SYSCFG_DL_DMA_init(void);

void SYSCFG_DL_SYSTICK_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
