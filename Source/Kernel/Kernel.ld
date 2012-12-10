OUTPUT_FORMAT("binary")

SECTIONS
{   
    . = 0xFFFFFFFFC0100000;

    .text ALIGN (0x1000) :
    {
        *(.text)
        *(.gnu.linkonce.t*)
    }

    .rodata ALIGN (0x1000) :
    {
		__CTOR_LIST__ = .; LONG((__CTOR_END__ - __CTOR_LIST__) / 8 - 1) *(.ctors) LONG(0) __CTOR_END__ = .;
		__DTOR_LIST__ = .; LONG((__DTOR_END__ - __DTOR_LIST__) / 8 - 1) *(.dtors) LONG(0) __DTOR_END__ = .;
		*(.data)
		*(.rodata)
    }

    .data ALIGN (0x1000) :
    {
        *(.data)
        *(.gnu.linkonce.d*)
    }

    .bss :
    {
        sbss = .;
        *(COMMON)
        *(.bss)
        *(.gnu.linkonce.b*)
        ebss = .;
    }

    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame) /* discard this, unless you are implementing runtime support for C++ exceptions. */
    }
    _end = .;
}