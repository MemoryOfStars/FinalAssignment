#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/gpio.h>
#include <mach/regs-gpioj.h>

#define DEVICE_NAME     "one_key"

struct button_irq_desc {
    int irq;
    int pin;
    int pin_setting;
    char *name;	
};

static struct button_irq_desc button_irq = {IRQ_EINT0, S3C2410_GPF(0) ,  S3C2410_GPF0_EINT0 , "KEY"};

static DECLARE_WAIT_QUEUE_HEAD(button_waitq);

static volatile int ev_press = 0;

static irqreturn_t buttons_interrupt(int irq, void *dev_id)
{
    struct button_irq_desc *button_irqs = (struct button_irq_desc *)dev_id;
    int down;
    int i,t;
    down = !s3c2410_gpio_getpin(button_irqs->pin);

	if(down == 1){
		udelay(100);
		down = !s3c2410_gpio_getpin(button_irqs->pin);
		if((down == 1)&&(ev_press == 0)) //按键按下
		{
			ev_press = 1;
		}
       wake_up_interruptible(&button_waitq);
    } 
    return IRQ_RETVAL(IRQ_HANDLED);
}


static int s3c24xx_buttons_open(struct inode *inode, struct file *file)
{
    int err = 0;
	err = request_irq(button_irq.irq, buttons_interrupt, IRQ_TYPE_LEVEL_LOW,button_irq.name, (void *)&button_irq);//中断注册

    if (err) {//中断注册出错处理
		disable_irq(button_irq.irq);
        free_irq(button_irq.irq, (void *)&button_irq);
        return -EBUSY;
    }
    ev_press = 1;
    return 0;
}


static int s3c24xx_buttons_close(struct inode *inode, struct file *file)
{
	free_irq(button_irq.irq, (void *)&button_irq);
    return 0;
}


static int s3c24xx_buttons_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
    unsigned long err;
	char user_value = '0';

    if (!ev_press) {
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		else
			wait_event_interruptible(button_waitq, ev_press);
    }

	err = copy_to_user(buff, (const void *)&user_value, 1);
	ev_press = 0;
    return err ? -EFAULT : count;
}

static unsigned int s3c24xx_buttons_poll( struct file *file, struct poll_table_struct *wait)
{
    unsigned int mask = 0;
    poll_wait(file, &button_waitq, wait);
    if (ev_press)
        mask |= POLLIN | POLLRDNORM;
    return mask;
}


static struct file_operations dev_fops = {
    .owner   =   THIS_MODULE,
    .open    =   s3c24xx_buttons_open,
    .release =   s3c24xx_buttons_close, 
    .read    =   s3c24xx_buttons_read,
    .poll    =   s3c24xx_buttons_poll,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

static int __init dev_init(void)
{
	int ret;

	ret = misc_register(&misc);

	printk (DEVICE_NAME"\tinitialized\n");

	return ret;
}

static void __exit dev_exit(void)
{
	misc_deregister(&misc);
	printk (DEVICE_NAME"\exit\n");
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("FriendlyARM Inc.");
