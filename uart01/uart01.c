
extern void dummy ( unsigned int );
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

#define IO1PIN 0xE0028010
#define IO1SET 0xE0028014
#define IO1DIR 0xE0028018
#define IO1CLR 0xE002801C

#define T0TCR  0xE0004004
#define T0TC   0xE0004008
#define T0PR   0xE000400C
#define T0CTCR 0xE0004070
#define T0MCR  0xE0004014
#define T0CCR  0xE0004028

#define SCS     0xE01FC1A0
#define APBDIV  0xE01FC100

#define TIMERCOUNT 0x1000000

#define PLL0CON  0xE01FC080
#define PLL0CFG  0xE01FC084
#define PLL0STAT 0xE01FC088
#define PLL0FEED 0xE01FC08C

#define MAMCR 0xE01FC000
#define MAMTIM 0xE01FC004


#define PINSEL0     0xE002C000
#define MAMCR       0xE01FC000
#define MAMTIM      0xE01FC004

#define U0ACR       0xE000C020
#define U0LCR       0xE000C00C
#define U0DLL       0xE000C000
#define U0DLM       0xE000C004
#define U0IER       0xE000C004
#define U0FDR       0xE000C028
#define U0FCR       0xE000C008
#define U0TER       0xE000C030
#define U0LSR       0xE000C014
#define U0THR       0xE000C000
#define U0RBR       0xE000C000

//-------------------------------------------------------------------
void uart_init ( void )
{
    unsigned int ra;

    ra=GET32(PINSEL0);
    ra&=(~0xF);
    ra|=0x5;
    PUT32(PINSEL0,ra);


//12000000 Hz PCLK 115200 baud
//dl 0x04 mul 0x08 div 0x05 baud 115385 diff 185
    PUT32(U0ACR,0x00); //no autobaud
    PUT32(U0LCR,0x83); //dlab=1; N81
    PUT32(U0DLL,0x04); //dl = 0x0004
    PUT32(U0DLM,0x00); //dl = 0x0004
    PUT32(U0IER,0x00); //no interrupts
    PUT32(U0LCR,0x03); //dlab=0; N81
    PUT32(U0IER,0x00); //no interrupts
    PUT32(U0FDR,(0x8<<4)|(0x5<<0)); //mul 0x08, div 0x05
    PUT32(U0FCR,(1<<2)|(1<<1)|(1<<0)); //enable and reset fifos
    PUT32(U0TER,(1<<7)); //transmit enable
}
//-------------------------------------------------------------------
void uart_putc ( unsigned int x )
{
    while (( GET32(U0LSR) & (1<<5)) == 0) continue;
    PUT32(U0THR,x);
}
//-------------------------------------------------------------------
unsigned int uart_getc ( void )
{
    while (( GET32(U0LSR) & (1<<0)) == 0) continue;
    return(GET32(U0RBR));
}
//-------------------------------------------------------------------
void hexstring ( unsigned int d, unsigned int cr )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_putc(rc);
        if(rb==0) break;
    }
    if(cr)
    {
        uart_putc(0x0D);
        uart_putc(0x0A);
    }
    else
    {
        uart_putc(0x20);
    }
}


void notmain ( void )
{
    unsigned int led;
    unsigned int ra;
    unsigned int rb;


    PUT32(SCS,0); //old fashioned lpc gpio
    PUT32(PLL0CON,0);
    PUT32(APBDIV,1); //APB clk = CCLK.
    PUT32(MAMTIM,1);
    PUT32(MAMCR,2);

    uart_init();
    hexstring(0x12345678,1);

    led=0x01000000; /* bit 24 */

    /* make P1.24 an output, 0xE0028018 */
    PUT32(IO1DIR,GET32(IO1DIR)|led);
     PUT32(IO1SET,led);

    PUT32(T0TCR,0);
    PUT32(T0TCR,2);
    for(ra=0;ra<1000;ra++) dummy(ra);
    PUT32(T0PR,0);
    PUT32(T0CTCR,0);
    PUT32(T0MCR,0);
    PUT32(T0CCR,0);
    PUT32(T0TCR,1);
    ra=GET32(T0TC);
    while(1)
    {
        PUT32(IO1CLR,led);
        uart_putc(0x31);
        while(1)
        {
            rb=GET32(T0TC);
            if((rb-ra)>=TIMERCOUNT) break;
        }
        ra+=TIMERCOUNT;

        PUT32(IO1SET,led);
        uart_putc(0x30);
        while(1)
        {
            rb=GET32(T0TC);
            if((rb-ra)>=TIMERCOUNT) break;
        }
        ra+=TIMERCOUNT;
    }

}

