#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h> 	// for socket(), bind(), connect()
#include <arpa/inet.h> 		// for sockaddr_in, inet_ntoa()
#include <netinet/tcp.h> 	// tcp nodelay
#include <sys/stat.h>	    // stat function
#include <sys/un.h>
#include <net/ethernet.h>

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>

