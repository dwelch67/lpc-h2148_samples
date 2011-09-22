
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

#define TIMERCOUNT 0x4000000

#define PLL0CON  0xE01FC080
#define PLL0CFG  0xE01FC084
#define PLL0STAT 0xE01FC088
#define PLL0FEED 0xE01FC08C

#define MAMCR 0xE01FC000
#define MAMTIM 0xE01FC004

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
        while(1)
        {
            rb=GET32(T0TC);
            if((rb-ra)>=TIMERCOUNT) break;
        }
        ra+=TIMERCOUNT;

        PUT32(IO1SET,led);
        while(1)
        {
            rb=GET32(T0TC);
            if((rb-ra)>=TIMERCOUNT) break;
        }
        ra+=TIMERCOUNT;
    }

}

