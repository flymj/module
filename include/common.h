#include <linux/ioctl.h>
typedef struct
{
	unsigned long count;
}flymj_msleep_t;

#define FLYMJ_IOCTL_SLEEP_WITH_SCHEDULE _IOW('p', 0x1, flymj_msleep_t*)
#define FLYMJ_IOCTL_SLEEP_WITH_SCHEDULE_TIMEOUT _IOW('p',0x2, flymj_msleep_t*)
#define FLYMJ_IOCTL_SLEEP_WITH_BUSY_WAIT _IOW('p',0x3, flymj_msleep_t*)
