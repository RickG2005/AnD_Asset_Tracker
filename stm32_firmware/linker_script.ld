/* Define the entry point of the application */
ENTRY(Reset_Handler)

/* Specify the memory areas for a standard STM32F103C8 (64KB Flash, 20KB RAM) */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
  RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
}

/* Define the top of the stack (End of RAM) */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  /* The vector table and code go into FLASH */
  .text :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Vector table (if you add one later) */
    *(.text)             /* Program code */
    *(.text*)            /* Merge all other code sections */
    *(.rodata)           /* Read-only data (constants, strings) */
    *(.rodata*)
    . = ALIGN(4);
  } > FLASH

  /* Used by the startup code to initialize data from FLASH to RAM */
  _sidata = LOADADDR(.data);

  /* Initialized data section goes into RAM, but its contents live in FLASH */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* Create a global symbol at data start */
    *(.data)           /* Initialized variables */
    *(.data*)
    . = ALIGN(4);
    _edata = .;        /* Create a global symbol at data end */
  } > RAM AT > FLASH

  /* Uninitialized data section (Zeroed out at startup) */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;         /* Create a global symbol at bss start */
    *(.bss)            /* Uninitialized variables */
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;         /* Create a global symbol at bss end */
  } > RAM

  _end = .;

  /* Remove information from the compiler libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }
}