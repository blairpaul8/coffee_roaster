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

/* --------------------------------------------------------------------
 * Vector table
 * ------------------------------------------------------------------ */
.section .isr_vector, "a", %progbits
.align 2
.word _estack            /* Initial stack pointer */
.word Reset_Handler      /* Reset handler */

/* Optional: basic fault handlers */
.word Default_Handler    /* NMI */
.word Default_Handler    /* HardFault */
.word Default_Handler    /* MemManage */
.word Default_Handler    /* BusFault */
.word Default_Handler    /* UsageFault */
.word 0
.word 0
.word 0
.word 0
.word Default_Handler    /* SVCall */
.word Default_Handler    /* DebugMon */
.word 0
.word Default_Handler    /* PendSV */
.word Default_Handler    /* SysTick */

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

