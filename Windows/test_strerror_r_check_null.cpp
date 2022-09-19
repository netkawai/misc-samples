#include <cstdio>
#include <cstring>

// POSIX to Microsoft strerror_s
/*
 * POSIX definition of strerror_r() -- see http://pubs.opengroup.org/onlinepubs/9699919799/functions/strerror.html
 */

#define strerror_r(errno,buf,len) strerror_s(buf,len,errno)

void declare_cpp(void *a);

int main()
{
	printf("sizeof of ZEOR(%d)\n",sizeof(0));
	
	declare_cpp(0);
	

    /* some implementations fail if buf is less than 80 characters
       so size it appropriately */
    char errstr[128];
    /* some implementations fail if err == 0 */
    strerror_r(1, errstr, 127);
    return 0;
}

extern "C" void declare_c(void *a);

void declare_cpp(void *a)
{
	if(NULL == a)
		printf("a is NULL in CPP\n");
	else
		printf("a is NOT NULL in CPP\n");

	if(NULL == 0)
		printf("0 is NULL in CPP\n");
	else
		printf("0 is NOT NULL in CPP\n");

	if(sizeof(NULL) == sizeof(0))
		printf("sizeof NULL is the same size of 0\n");
	else
		printf("sizeof NULL is the different size of 0\n");

	declare_c(a);
}

extern "C"
{
	void declare_c(void *a)
	{

		if(NULL == a)
			printf("a is NULL in C\n");
		else
			printf("a is NOT NULL in C\n");

		if(sizeof(NULL) == sizeof(0))
			printf("sizeof NULL is the same size of 0\n");
		else
			printf("sizeof NULL is the different size of 0\n");		
	}
}