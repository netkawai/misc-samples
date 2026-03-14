#include <stdio.h>
#include <time.h>
#include <wchar.h>

int main(void)
{
  struct tm* timeptr;
  wchar_t dest[100];
  time_t temp;
  size_t rc;

  printf("size wchar_t is %lu\n",sizeof(wchar_t));

  temp = time(NULL);
  timeptr = localtime(&temp);
  rc = wcsftime(dest, sizeof(dest), L"Today is %A,"
                                    L" %b %d.\n Time: %I:%M %p",timeptr);
  printf("%zu characters placed in string to make:\n\n%ls\n",rc,dest);
}
