/* STM32F411 Linker Script
 * Flash: 512 KB @ 0x08000000
 * RAM:   128 KB @ 0x20000000
 */

ENTRY(Reset_Handler)

/* Memory layout from datasheet */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/* End of RAM = initial stack pointer */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /* ------------------------------------------------
     * Vector table + program code (Flash)
     * ------------------------------------------------ */
    .text :
    {
        KEEP(*(.isr_vector))   /* Vector table */
        *(.text*)              /* Program code */
        *(.rodata*)            /* Read-only data */

        . = ALIGN(4);
    } > FLASH

    /* ------------------------------------------------
     * Initialized data (RAM, load from Flash)
     * ------------------------------------------------ */
    .data : AT(LOADADDR(.text) + SIZEOF(.text))
    {
        . = ALIGN(4);
        _sdata = .;

        *(.data*)

        . = ALIGN(4);
        _edata = .;
    } > RAM

    /* Flash location of initialized data */
    _sidata = LOADADDR(.data);

    /* ------------------------------------------------
     * Zero-initialized data (RAM)
     * ------------------------------------------------ */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;

        *(.bss*)
        *(COMMON)

        . = ALIGN(4);
        _ebss = .;
    } > RAM

    /* ------------------------------------------------
     * Optional heap (unused for now)
     * ------------------------------------------------ */
    _end = .;
}

