#include <stdio.h>
#include <limits.h>

int main(){

  printf("INT(32bit) max(%d)\n",INT_MAX);
  printf("This is not enought for 9999999999(10digits) with postive and negative\n");
  printf("10 digits was standard for academic and engineering filed in USA pre-computer era\n");
  printf("It is enought for 9 digits\n");



  printf("10 digits are 0x%lx 0x%lx\n",
    9999999999,-9999999999
  );

  printf("This is 34bit + 1 sign bit = minimun 35bit.\n");
}
