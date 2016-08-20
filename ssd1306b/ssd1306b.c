
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

// vcc gnd scl sda

//128x64
//iiiii iiiiiiiiii
//   X XXX XXX XXX
//   X X X X X X X
//   X XXX XXX XXX
//   X X X X X X X
//   X XXX XXX XXX

const unsigned char numdata[10][8]=
{
// .XXX
// .X.X
// .X.X
// .X.X
// .XXX
{0x07,0x05,0x05,0x05,0x07,0x00,0x00,0x00},
// ...X
// ...X
// ...X
// ...X
// ...X
{0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00},
// .XXX
// ...X
// .XXX
// .X..
// .XXX
{0x07,0x01,0x07,0x04,0x07,0x00,0x00,0x00},
// .XXX
// ...X
// .XXX
// ...X
// .XXX
{0x07,0x01,0x07,0x01,0x07,0x00,0x00,0x00},
// .X.X
// .X.X
// .XXX
// ...X
// ...X
{0x05,0x05,0x07,0x01,0x01,0x00,0x00,0x00},
// .XXX
// .X..
// .XXX
// ...X
// .XXX
{0x07,0x04,0x07,0x01,0x07,0x00,0x00,0x00},
// .XXX
// .X..
// .XXX
// .X.X
// .XXX
{0x07,0x04,0x07,0x05,0x07,0x00,0x00,0x00},
// .XXX
// ...X
// ...X
// ...X
// ...X
{0x07,0x01,0x01,0x01,0x01,0x00,0x00,0x00},
// .XXX
// .X.X
// .XXX
// .X.X
// .XXX
{0x07,0x05,0x07,0x05,0x07,0x00,0x00,0x00},
// .XXX
// .X.X
// .XXX
// ...X
// ...X
{0x07,0x05,0x07,0x01,0x01,0x00,0x00,0x00},
};


extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern unsigned int GETPC ( void );
extern void dummy ( unsigned int );


#define IO0PIN 0xE0028000
#define IO0SET 0xE0028004
#define IO0DIR 0xE0028008
#define IO0CLR 0xE002800C


#define IO1PIN 0xE0028010
#define IO1SET 0xE0028014
#define IO1DIR 0xE0028018
#define IO1CLR 0xE002801C

#define SCS     0xE01FC1A0
#define APBDIV  0xE01FC100

#define TIMERCOUNT 0x100000

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
static void uart_init ( void )
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
    PUT32(U0FDR,(0x08<<4)|(0x05<<0)); //mul 0x08, div 0x05
    PUT32(U0FCR,(1<<2)|(1<<1)|(1<<0)); //enable and reset fifos
    PUT32(U0TER,(1<<7)); //transmit enable
}
//-------------------------------------------------------------------
static void uart_putc ( unsigned int x )
{
    while (( GET32(U0LSR) & (1<<5)) == 0) continue;
    PUT32(U0THR,x);
}
//-------------------------------------------------------------------
//static unsigned int uart_getc ( void )
//{
    //while (( GET32(U0LSR) & (1<<0)) == 0) continue;
    //return(GET32(U0RBR));
//}
//-------------------------------------------------------------------
static void hexstring ( unsigned int d )
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
    uart_putc(0x0D);
    uart_putc(0x0A);
}



#define PADDR 0x78


#define SETCONTRAST         0x81
#define DISPLAYALLONRESUME  0xA4
#define DISPLAYALLON        0xA5
#define NORMALDISPLAY       0xA6
#define INVERTDISPLAY       0xA7
#define DISPLAYOFF          0xAE
#define DISPLAYON           0xAF
#define SETDISPLAYOFFSET    0xD3
#define SETCOMPINS          0xDA
#define SETVCOMDESELECT     0xDB
#define SETDISPLAYCLOCKDIV  0xD5
#define SETPRECHARGE        0xD9
#define SETMULTIPLEX        0xA8
#define SETLOWCOLUMN        0x00
#define SETHIGHCOLUMN       0x10
#define SETSTARTLINE        0x40
#define MEMORYMODE          0x20
#define COMSCANINC          0xC0
#define COMSCANDEC          0xC8
#define SEGREMAP            0xA0
#define CHARGEPUMP          0x8D
#define EXTERNALVCC         0x01
#define SWITCHCAPVCC        0x02


#define SDA 3
#define SCL 2

//------------------------------------------------------------------------
static void i2c_delay ( void )
{
    //unsigned int ra;

    //for(ra=0;ra<5;ra++) dummy(ra);
    dummy(0);
}
//------------------------------------------------------------------------
static void scl_high ( void )
{
    PUT32(IO0SET,1<<SCL);
}
//------------------------------------------------------------------------
static void sda_high ( void )
{
    PUT32(IO0SET,1<<SDA);
}
//------------------------------------------------------------------------
static void scl_low ( void )
{
    PUT32(IO0CLR,1<<SCL);
}
//------------------------------------------------------------------------
static void sda_low ( void )
{
    PUT32(IO0CLR,1<<SDA);
}
//------------------------------------------------------------------------
static unsigned int sda_read ( void )
{
    unsigned int ra;

    ra=GET32(IO0PIN);
    ra&=(1<<SDA);
    return(ra);
}
//------------------------------------------------------------------------
static void sda_input ( void )
{
    unsigned int ra;

    ra=GET32(IO0DIR);
    ra&=~(1<<SDA);
    PUT32(IO0DIR,ra);
}
//------------------------------------------------------------------------
static void sda_output ( void )
{
    unsigned int ra;

    ra=GET32(IO0DIR);
    ra|=1<<SDA;
    PUT32(IO0DIR,ra);
}
//------------------------------------------------------------------------
static void i2c_start ( void )
{
    i2c_delay();
    i2c_delay();
    sda_low();
    i2c_delay();
    i2c_delay();
    scl_low();
    i2c_delay();
}
//------------------------------------------------------------------------
static void i2c_stop ( void )
{
    i2c_delay();
    scl_high();
    i2c_delay();
    sda_high();
    i2c_delay();
    i2c_delay();
}
//------------------------------------------------------------------------
static unsigned int i2c_write_byte ( unsigned int b )
{
    unsigned int ra;
    for(ra=0x80;ra;ra>>=1)
    {
        i2c_delay();
        if(ra&b) sda_high();
        else     sda_low();
        i2c_delay();
        scl_high();
        i2c_delay();
        scl_low();
        i2c_delay();
        sda_low();
        i2c_delay();
    }
    i2c_delay();
    sda_input();
    i2c_delay();
    scl_high();
    i2c_delay();
    ra=sda_read();
    i2c_delay();
    scl_low();
    i2c_delay();
    sda_output();
    i2c_delay();
    return(ra);
}
//------------------------------------------------------------------------
//static unsigned int i2c_read_byte ( unsigned int *b )
//{
    //unsigned int ra;
    //unsigned int rb;
    //i2c_delay();
    //sda_input();
    //rb=0;
    //for(ra=0;ra<9;ra++)
    //{
        //i2c_delay();
        //scl_high();
        //i2c_delay();
        //rb<<=1;
        //if(sda_read()) rb|=1;
        //i2c_delay();
        //scl_low();
        //i2c_delay();
    //}
    //sda_output();
    //i2c_delay();
    //ra=rb&1;
    //*b=rb>>1;
    //return(ra);
//}
//------------------------------------------------------------------------
static void send_command ( unsigned int cmd )
{
//    PUT32(GPCLR0,1<<25); //D/C = 0 for command
    //spi_one_byte(cmd);
    i2c_start();
    if(i2c_write_byte(PADDR))
    {
        i2c_stop();
        hexstring(0xBADBAD00);
        return;
    }
    //not continuing
    //want D/C a zero
    if(i2c_write_byte(0x00))
    {
        //i2c_stop();
        //hexstring(0xBADBAD00);
        //return;
    }
    if(i2c_write_byte(cmd))
    {
        //i2c_stop();
        //hexstring(0xBADBAD00);
        //return;
    }
    i2c_stop();
}
//------------------------------------------------------------------------
static void send_data ( unsigned int data )
{
    //PUT32(GPSET0,1<<25); //D/C = 1 for data
    //spi_one_byte(data);
    i2c_start();
    if(i2c_write_byte(PADDR))
    {
        i2c_stop();
        hexstring(0xBADBAD00);
        return;
    }
    //not continuing
    //want D/C a one
    if(i2c_write_byte(0x40))
    {
        //i2c_stop();
        //hexstring(0xBADBAD00);
        //return;
    }
    if(i2c_write_byte(data))
    {
        //i2c_stop();
        //hexstring(0xBADBAD00);
        //return;
    }
    i2c_stop();
}
//------------------------------------------------------------------------
static void SetPageStart ( unsigned int x )
{
    send_command(0xB0|(x&0x07));
}
//------------------------------------------------------------------------
static void SetColumn ( unsigned int x )
{
    //x+=0x20;
    send_command(0x10|((x>>4)&0x0F));
    send_command(0x00|((x>>0)&0x0F));
}
//------------------------------------------------------------------------
static void ClearScreen ( void )
{
    unsigned int ra;
    unsigned int rb;

    for(ra=0;ra<8;ra++)
    {
        SetPageStart(ra);
        SetColumn(0);
        for(rb=0;rb<0x80;rb++) send_data(0);
    }
}

static unsigned int tim[4];
//------------------------------------------------------------------------
static void show_time ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int re;
    unsigned int rf;

    for(rb=0;rb<5;rb++)
    {
        SetPageStart(rb);
        SetColumn(0);
        for(ra=0;ra<4;ra++)
        {
            rc=numdata[tim[ra]][rb];
            for(rd=8;rd;rd>>=1)
            {
                if(rd&rc)
                {
                    re=0xFF;
                }
                else
                {
                    re=0x00;
                }
                for(rf=0;rf<8;rf++) send_data(re);
            }
        }
    }
}


//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;



    PUT32(SCS,0); //old fashioned lpc gpio
    PUT32(PLL0FEED,0xAA);
    PUT32(PLL0FEED,0x55);
    PUT32(PLL0CON,0);
    PUT32(APBDIV,1); //APB clk = CCLK.
    PUT32(MAMTIM,1);
    PUT32(MAMCR,2);

    uart_init();
    hexstring(0x12345678);

    ra=GET32(IO0DIR);
    ra|=1<<SDA;
    ra|=1<<SCL;
    PUT32(IO0DIR,ra);

    scl_high();
    sda_high();
    i2c_delay();
    i2c_delay();
    i2c_delay();

if(0)
{
    //find who acks
    for(ra=0;ra<0x100;ra+=2)
    {
        unsigned int rb;
        //hexstrings(ra);
        i2c_start();
        rb=i2c_write_byte(ra|0);
        i2c_stop();
        //hexstring(rb);
        if(rb==0) hexstring(ra);
    }
    //0x78 is the address
}
if(1)
{

    // Display Init sequence for 64x48 OLED module
    send_command(DISPLAYOFF);           // 0xAE
    send_command(SETDISPLAYCLOCKDIV);   // 0xD5
    send_command(0x80);                 // the suggested ratio 0x80
    send_command(SETMULTIPLEX);         // 0xA8
    send_command(0x3F);
    send_command(SETDISPLAYOFFSET);     // 0xD3
    send_command(0x0);                  // no offset
    send_command(SETSTARTLINE | 0x0);   // line #0
    send_command(CHARGEPUMP);           // enable charge pump
    send_command(0x14);
    send_command(MEMORYMODE);
    send_command(0x02);
    send_command(SEGREMAP | 0x1);
    send_command(COMSCANDEC);
    send_command(SETCOMPINS);           // 0xDA
    send_command(0x12);
    send_command(SETCONTRAST);          // 0x81
    send_command(0x7F);
    send_command(SETPRECHARGE);         // 0xd9
    send_command(0xF1);
    send_command(SETVCOMDESELECT);          // 0xDB
    send_command(0x40);
    send_command(NORMALDISPLAY);            // 0xA6
    send_command(DISPLAYALLONRESUME);   // 0xA4
    ClearScreen();
    send_command(DISPLAYON);                //--turn on oled panel
    //send_command(DISPLAYALLON);

    tim[0]=1;
    tim[1]=2;
    tim[2]=3;
    tim[3]=4;

    while(1)
    {
        show_time();
        tim[3]++;
        if(tim[3]>9)
        {
            tim[3]=0;
            tim[2]++;
            if(tim[2]>9)
            {
                tim[2]=0;
                tim[1]++;
                if(tim[1]>9)
                {
                    tim[1]=0;
                    tim[0]++;
                    if(tim[0]>9)
                    {
                        tim[0]=0;
                    }
                }
            }
        }
    }

}
    hexstring(0x12341234);

    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//
// Copyright (c) 2014 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
