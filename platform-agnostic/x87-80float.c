// compiling with SSE2(x64) and disable SSE2(x86) makes different result

#include <stdio.h> 
#include <float.h> 

union dType{ 
    double d; 
    unsigned char c[8]; 
}; 

static char * dump_val(char *buf, union dType val) 
{ 

    int i; 
    for(i=0;i<8;i++) 
        { sprintf(&buf[i*2],"%02x", val.c[7-i]); } 
    buf[16] = '\0'; 

    return buf; 
} 

int main() 
{ 

     char buf1[20],buf2[20],buf3[20],buf4[20]; 

     union dType a,b,c; 

     a.d = 20.2; 

     b.d = 0.2; 

     c.d = a.d; 

     const double ZERO = 0.0; 

     double h = (a.d - b.d); 

     if((a.d - b.d) == 20.0)  

     // if you compile with -mfpmath=sse -msse2, value is eqal, if you complie with x87 instruction , it is not eqaul 

     { 

          printf("equal\n"); 

     } 

     else 

     { 

          printf("not equal\n"); 

     } 

     if(h == 20.0) 

     { 

          printf("equal\n"); 

     } 

     else 

     { 

          printf("not equal\n"); 

     } 

     if((a.d-c.d) == ZERO)  

     { 

          printf("equal\n"); 

     }else 

     { 

          printf("not equal\n"); 

     } 

     printf("%lf,%lf, %lf\n",a, b, (a.d-b.d)); 

     union dType e,f; 

     e.d = a.d - b.d; 

     f.d = 20.0; 

     printf("%s %s %s %s\n", dump_val(buf1,a), dump_val(buf2,b), dump_val(buf3,e),dump_val(buf4,f)); 

} 