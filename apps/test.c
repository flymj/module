#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "common.h"
int main()
{
	int fd = open("/dev/flymj", O_RDWR);
	int i = 100;
	struct timezone tz;
	struct timeval tv, tvEnd;

	flymj_msleep_t msleep;
	msleep.count = 1;
    gettimeofday(&tv, &tz);
	while(i--)
	{
		ioctl(fd, FLYMJ_IOCTL_SLEEP_WITH_SCHEDULE_TIMEOUT, &msleep);
	}
	gettimeofday(&tvEnd, &tz);
	printf("after 100 loop of 1us sleep with schedule_timeout, we get timeout = %ld us\n", 1000000* (tvEnd.tv_sec - tv.tv_sec) + tvEnd.tv_usec - tv.tv_usec);

	i = 100;
    gettimeofday(&tv, &tz);
	while(i--)
	{
		ioctl(fd, FLYMJ_IOCTL_SLEEP_WITH_SCHEDULE, &msleep);
	}
	gettimeofday(&tvEnd, &tz);
	printf("after 100 loop of 1us sleep with schedule, we get timeout = %ld us\n", 1000000* (tvEnd.tv_sec - tv.tv_sec) + tvEnd.tv_usec - tv.tv_usec);
	
	i = 100;
    gettimeofday(&tv, &tz);
	while(i--)
	{
		ioctl(fd, FLYMJ_IOCTL_SLEEP_WITH_BUSY_WAIT, &msleep);
	}
	gettimeofday(&tvEnd, &tz);
	printf("after 100 loop of 1us sleep with schedule, we get timeout = %ld us\n", 1000000* (tvEnd.tv_sec - tv.tv_sec) + tvEnd.tv_usec - tv.tv_usec);
	return 1;	
}
