/*
	input event driver for test

*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/platform_device.h>


const char* pdev_name = "ietest";
struct platform_device	*pdev;
struct ev_test {
	struct input_dev	*input_dev;
	struct task_struct 	*task;
};

int read_data_from_sensor(void)
{
	static int roll = 0;

	return roll++%2 ? 3 : 4;
}

void set_input_device_property(struct input_dev *dev)
{
	set_bit(EV_REL, dev->evbit);
	set_bit(REL_X, dev->relbit);
	set_bit(REL_Y, dev->relbit);
}

void send_event_msg_to_evdev(struct input_dev *dev)
{
	input_report_rel(dev, REL_X, read_data_from_sensor());
	input_report_rel(dev, REL_Y, read_data_from_sensor());
	input_sync(dev);
}

static int hw_fake_int_handler(void *arg)
{
	struct ev_test *event_ctrl = (struct ev_test*)arg;
	struct input_dev *idev = event_ctrl->input_dev;
	
	while(!kthread_should_stop()) 	//check stop signal
	{
		send_event_msg_to_evdev(idev);
		msleep_interruptible(300);
	}

	return 0;
}

static int inputevent_test_init(void)
{
	int ret;
	struct ev_test 		*event_ctrl = NULL;
	struct task_struct	*task;
	struct input_dev	*idev;


	/* platform or i2c or spi or serio or etc... */
	pdev = platform_device_register_simple(pdev_name, -1, NULL, 0);
	if(IS_ERR(pdev)) {
		ret = PTR_ERR(pdev);
		printk("error: platfrom register failed\n");
		return ret;
	}
	
	event_ctrl = kzalloc((sizeof(struct ev_test)), GFP_KERNEL);
	if(event_ctrl == NULL) {
		return -ENOMEM;
	}

	idev = input_allocate_device();
	if(!idev) {
		printk("error: not enough memory\n");
		ret = -ENOMEM;
		goto init_err1;
	}

	set_input_device_property(idev);
	
	ret = input_register_device(idev);
	if(ret) {
		printk("error: input register device failed\n");
		goto init_err1;
	}

	task = kthread_run(hw_fake_int_handler, event_ctrl, "int_%s_#%d", pdev_name, 1);
	if(IS_ERR_VALUE(task)) {
		printk("error: could not bind fake interrupt handler\n");
		ret = -ENODEV;
		goto init_err2;
	}

	event_ctrl->input_dev = idev; 
	event_ctrl->task = task;
	dev_set_drvdata(&pdev->dev, event_ctrl);

	return 0;

init_err2:
	input_unregister_device(idev);

init_err1:
	kfree(event_ctrl);

	return ret; 
}


static void inputevent_test_exit(void)
{
	struct ev_test 	*event_ctrl = (struct ev_test*) dev_get_drvdata(&pdev->dev);
	kthread_stop(event_ctrl->task);

	input_unregister_device(event_ctrl->input_dev);
	platform_device_unregister(pdev);
}



module_init(inputevent_test_init);
module_exit(inputevent_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chocokeki@gmail.com");
