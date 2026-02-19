/* Tell the linker we want a 64-bit x86 ELF output file */
OUTPUT_FORMAT(elf64-x86-64)

/* kernel_main is where execution begins */
ENTRY(kernel_main)

SECTIONS
{
    /*
     * Load the kernel into the top 2GB of virtual address space.
     * This is required by the Limine protocol — it won't load
     * kernels in the lower half (below 0xffffffff80000000).
     */
    . = 0xffffffff80000000;

    /*
     * The Limine requests section — Limine scans here to find
     * our framebuffer request, base revision, etc.
     * Must come first so Limine sees them before jumping to us.
     */
    .limine_requests : ALIGN(8) {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    }

    /* Executable code */
    .text : ALIGN(4K) { *(.text .text.*) }

    /* Read-only data (string literals, const variables) */
    .rodata : ALIGN(4K) { *(.rodata .rodata.*) }

    /* Writable data and zero-initialised globals */
    .data : ALIGN(4K) { *(.data .data.*) }
    .bss  : ALIGN(4K) { *(.bss  .bss.*)  *(COMMON) }

    /*
     * Throw away debug/exception frames — not needed in a
     * freestanding kernel and can cause issues on some hosts.
     */
    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
    }
}
