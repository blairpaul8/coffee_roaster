.syntax unified
.cpu cortex-m4
.fpu fpv4-sp-d16
.thumb

/* --------------------------------------------------------------------
 * External symbols from linker script
 * ------------------------------------------------------------------ */
.extern _estack
.extern _sidata
.extern _sdata
.extern _edata
.extern _sbss
.extern _ebss

.extern main
.extern EXTI0_IRQHandler

/* --------------------------------------------------------------------
 * Vector table
 * ------------------------------------------------------------------ */
.section .isr_vector, "a", %progbits
.align 2
.word _estack            /* Initial Stack Pointer */
.word Reset_Handler      /* Reset Handler */

.word Default_Handler    /* NMI_Handler */
.word Default_Handler    /* HardFault_Handler */
.word Default_Handler    /* MemManage_Handler */
.word Default_Handler    /* BusFault_Handler */
.word Default_Handler    /* UsageFault_Handler */
.word 0                  /* Reserved */
.word 0                  /* Reserved */
.word 0                  /* Reserved */
.word 0                  /* Reserved */
.word Default_Handler    /* SVCall_Handler */
.word Default_Handler    /* DebugMon_Handler */
.word 0                  /* Reserved */
.word Default_Handler    /* PendSV_Handler */
.word Default_Handler    /* SysTick_Handler */

.word EXTI0_IRQHandler   /* EXTI Line0 Interrupt */

.word Default_Handler    /* EXTI1 */
.word Default_Handler    /* EXTI2 */
.word Default_Handler    /* EXTI3 */
.word Default_Handler    /* EXTI4 */
.word Default_Handler    /* DMA1_Stream0 */
.word Default_Handler    /* DMA1_Stream1 */
.word Default_Handler    /* DMA1_Stream2 */
.word Default_Handler    /* DMA1_Stream3 */
.word Default_Handler    /* DMA1_Stream4 */
.word Default_Handler    /* DMA1_Stream5 */
.word Default_Handler    /* DMA1_Stream6 */
.word Default_Handler    /* ADC */
.word Default_Handler    /* CAN1_TX */
.word Default_Handler    /* CAN1_RX0 */
.word Default_Handler    /* CAN1_RX1 */
.word Default_Handler    /* CAN1_SCE */
.word Default_Handler    /* EXTI9_5 */
.word Default_Handler    /* TIM1_BRK_TIM9 */
.word Default_Handler    /* TIM1_UP_TIM10 */
.word Default_Handler    /* TIM1_TRG_COM_TIM11 */
.word Default_Handler    /* TIM1_CC */
.word Default_Handler    /* TIM2 */
.word Default_Handler    /* TIM3 */
.word Default_Handler    /* TIM4 */
.word Default_Handler    /* I2C1_EV */
.word Default_Handler    /* I2C1_ER */
.word Default_Handler    /* I2C2_EV */
.word Default_Handler    /* I2C2_ER */
.word Default_Handler    /* SPI1 */
.word Default_Handler    /* SPI2 */
.word Default_Handler    /* USART1 */
.word Default_Handler    /* USART2 */
.word Default_Handler    /* USART3 */
.word Default_Handler    /* EXTI15_10 */
.word Default_Handler    /* RTC_Alarm */
.word Default_Handler    /* OTG_FS_WKUP */
.word Default_Handler    /* TIM8_BRK_TIM12 */
.word Default_Handler    /* TIM8_UP_TIM13 */
.word Default_Handler    /* TIM8_TRG_COM_TIM14 */
.word Default_Handler    /* TIM8_CC */
.word Default_Handler    /* DMA1_Stream7 */
.word Default_Handler    /* FMC */
.word Default_Handler    /* SDIO */
.word Default_Handler    /* TIM5 */
.word Default_Handler    /* SPI3 */
.word Default_Handler    /* UART4 */
.word Default_Handler    /* UART5 */
.word Default_Handler    /* TIM6_DAC */
.word Default_Handler    /* TIM7 */
.word Default_Handler    /* DMA2_Stream0 */
.word Default_Handler    /* DMA2_Stream1 */
.word Default_Handler    /* DMA2_Stream2 */
.word Default_Handler    /* DMA2_Stream3 */
.word Default_Handler    /* DMA2_Stream4 */
.word Default_Handler    /* ETH */
.word Default_Handler    /* ETH_WKUP */
.word Default_Handler    /* CAN2_TX */
.word Default_Handler    /* CAN2_RX0 */
.word Default_Handler    /* CAN2_RX1 */
.word Default_Handler    /* CAN2_SCE */
.word Default_Handler    /* OTG_FS */
.word Default_Handler    /* DMA2_Stream5 */
.word Default_Handler    /* DMA2_Stream6 */
.word Default_Handler    /* DMA2_Stream7 */
.word Default_Handler    /* USART6 */
.word Default_Handler    /* I2C3_EV */
.word Default_Handler    /* I2C3_ER */
.word Default_Handler    /* OTG_HS_EP1_OUT */
.word Default_Handler    /* OTG_HS_EP1_IN */
.word Default_Handler    /* OTG_HS_WKUP */
.word Default_Handler    /* OTG_HS */
.word Default_Handler    /* DCMI */
.word Default_Handler    /* RNG */
.word Default_Handler    /* FPU */

/* --------------------------------------------------------------------
 * Reset handler
 * ------------------------------------------------------------------ */
.section .text
.thumb_func
.global Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    /* Enable FPU (CP10, CP11) */
    ldr r0, =0xE000ED88      /* SCB->CPACR */
    ldr r1, [r0]
    orr r1, r1, #(0xF << 20)
    str r1, [r0]
    dsb
    isb

    /* Copy .data from Flash to RAM */
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata
1:
    cmp r1, r2
    ittt lt
    ldrlt r3, [r0], #4
    strlt r3, [r1], #4
    blt 1b

    /* Zero .bss */
    ldr r0, =_sbss
    ldr r1, =_ebss
    movs r2, #0
2:
    cmp r0, r1
    it lt
    strlt r2, [r0], #4
    blt 2b

    /* Jump to main */
    bl main

    /* If main returns, loop forever */
3:
    b 3b

.size Reset_Handler, . - Reset_Handler

/* --------------------------------------------------------------------
 * Default interrupt handler
 * ------------------------------------------------------------------ */
.thumb_func
.global Default_Handler
.type Default_Handler, %function
Default_Handler:
4:
    b 4b
.size Default_Handler, . - Default_Handler

