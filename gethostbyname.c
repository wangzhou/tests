/*
 *  uname -a
 *	Linux kllp05 4.15.0-50-generic #54-Ubuntu SMP Mon May 6 18:46:08 UTC 2019 x86_64 x86_64 x86_64 GNU/Linux
 *  test result shows that: gethostbyname will return host IPs in /etc/hosts
 */
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(int ac, char *av[])
{
	char ip_addr_s[INET_ADDRSTRLEN];
	struct hostent *host;
	char **ip_addr;
	const char *ip_dst;
	
	if (ac != 2)
		exit(3);

	if (!(host = gethostbyname(av[1]))) {
		perror("host");
		exit(1);
	}

	/* only test IPv4 */
	for (ip_addr = host->h_addr_list; *ip_addr; ip_addr++) {
		ip_dst = inet_ntop(AF_INET, *ip_addr, ip_addr_s, sizeof(ip_addr_s));	
		if (!ip_dst) {
			perror("inet_ntop");
			exit(2);
		}

		printf("ip addr: %s\n", ip_dst);
	}

	return 0;
}
