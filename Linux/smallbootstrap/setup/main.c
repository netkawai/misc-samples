typedef struct _REGS
{
  short ax;
  short bx;
  short cx;
  short dx;
} REGS;

typedef struct _SREGS
{
  short ds;
  short es;
  short ss;
} SREGS;


extern putchar(int);
extern int intx(int,REGS*,REGS*,SREGS*);

void print(char* string)
{
  int c;
  
  for(;;)
    {
      c = *string;
      if(!c)
	return;
      putchar(c);
      
      string++;
    }
}

REGS inregs,outregs;
SREGS segregs;
char ascii[256];

void digit_to_ascii(int digit)
{
  int high,low;
  int j;
  int count = 0;
  int i=0;
  low = digit;

  for(;;){
    high=low/16;
    count++;
    if(0< high <10){
      break;
    }else {
      low=high;
    }
  }

  ascii[i]=high+0x30;
  i++;
  for(j=count ; j>0 ; j--){
    low = low - high*16;
    ascii[i]=low+0x30;
    i++;
  }
  ascii[i]=0;
}
  
void _main()
{
  int ret;
  ret = intx(0x15,&inregs,&outregs,&segregs);
  digit_to_ascii(ret);
  print("Hello World");
  print(ascii);
}


