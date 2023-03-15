#include <sys/types.h>
#include <sys/param.h>

 int
 main ()
 {


 #if BYTE_ORDER != BIG_ENDIAN
                not big endian
 #endif

   printf("%d,%d",BYTE_ORDER,BIG_ENDIAN);
   return 0;
 }
