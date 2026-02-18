ENTRY(kernel_main)

SECTIONS
{
    . = 0xFFFFFFFF80000000; /* upper half — required by Limine v8 */

    /* limine scans these sections to find our requests */
    .limine_requests_start : ALIGN(8) { KEEP(*(.limine_requests_start)) }
    .limine_requests       : ALIGN(8) { KEEP(*(.limine_requests)) }
    .limine_requests_end   : ALIGN(8) { KEEP(*(.limine_requests_end)) }

    .text   : ALIGN(4K) { *(.text) *(.text.*) }
    .rodata : ALIGN(4K) { *(.rodata) *(.rodata.*) }
    .data   : ALIGN(4K) { *(.data) *(.data.*) }
    .bss    : ALIGN(4K) { *(.bss) *(.bss.*) *(COMMON) }
}
