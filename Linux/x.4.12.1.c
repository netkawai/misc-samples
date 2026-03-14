#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	    char           pathname[] = "/tmp/myfile";
	        struct stat    buf;

		    if (stat(pathname, &buf) == -1)
			            printf("stat error for %s\n", pathname);
		        else
				        printf("inode:%ld\nsize:%d\nblocksize:%d\nblocks:%d\nused size:%d\n",
							        buf.st_ino, buf.st_size, buf.st_blksize, buf.st_blocks, buf.st_blksize * buf.st_blocks);

			    exit(0);

}

