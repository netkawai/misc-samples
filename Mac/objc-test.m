#include <stdio.h>
#import <objc/Object.h>

@interface SimpleInt 
{
  int val;
}
- (int)Value;

@end

@implementation SimpleInt
- (int)Value {
   return val;
}

@end

int main(void) {
  Class clsInst = objc_getClass("SimpleInt");
  printf("clsInt:%p \n", (void*)clsInst);
}