#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>


int main()
{
	int fd, ret;
	int x, y;

	const char* evdev_path = "/dev/input/event3";
	struct input_event iev[3];

	fd = open(evdev_path, O_RDONLY);
	if(fd < 0) {
		perror("error: could not open evdev");
		return -1;
	}
	
	while(1)
	{
		ret = read(fd, iev, sizeof(struct input_event)*3);
		if(ret < 0) {
			perror("error: could not read input event");
			break;
		}
		
		if(iev[0].type == EV_REL && iev[0].code == REL_X)
			x = iev[0].value;
		if(iev[1].type == EV_REL && iev[1].code == REL_Y)
			y = iev[1].value;

		printf("%d, %d\n", x, y);
		printf("debug - %hu, %hu, %d\n", iev[0].type, iev[0].code, iev[0].value);
		printf("debug - %hu, %hu, %d\n", iev[1].type, iev[1].code, iev[1].value);
		printf("debug - %hu, %hu, %d\n", iev[2].type, iev[2].code, iev[2].value);
	}

	close(fd);
	return 0;
}
