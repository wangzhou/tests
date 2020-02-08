#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int ac, char *av[])
{
	int fd;

	/* indeed, we have no /test */
	open("/test", O_RDONLY);
	perror("test");

	return -1;
}
