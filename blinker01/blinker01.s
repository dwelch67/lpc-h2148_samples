
/*blinker01.s*/

.globl _start
_start:
    b reset
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang
    b hang

hang:   b hang

reset:
    ldr sp,=0x40002000

    ldr r0,=0xE0028010 /* I/O port 0 legacy base address */

    mov r1,#0x01000000 /* bit 24 */

    /* make P0.24 an output, 0xE0028018 */
    ldr r2,[r0,#0x08]
    orr r2,r2,r1
    str r2,[r0,#0x08]
    /* blink led */

loop:
    str r1,[r0,#0x04] /* IO0SET 0xE0028014 */
    mov r2,#0x100000
loop0:
    subs r2,r2,#1
    bne loop0

    str r1,[r0,#0x0C] /* IO0CLR 0xE002801C */
    mov r2,#0x100000
loop1:
    subs r2,r2,#1
    bne loop1

    b loop
