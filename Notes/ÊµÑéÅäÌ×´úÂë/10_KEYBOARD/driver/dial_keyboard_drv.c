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

#define DEVICE_NAME     "dial_key"

struct button_irq_desc {
    int irq;
    int pin;
    int pin_setting;
    int number;
    char *name;	
};
struct col_desc
{
	int pin;
	int pin_setting;
};

static struct col_desc col_table [] = {
	{S3C2410_GPG(7) , S3C2410_GPIO_OUTPUT},
	{S3C2410_GPF(5) , S3C2410_GPIO_OUTPUT},
	{S3C2410_GPF(4) , S3C2410_GPIO_OUTPUT},
	{S3C2410_GPF(3) , S3C2410_GPIO_OUTPUT},
};

static struct button_irq_desc button_irqs [] = {
    {IRQ_EINT2, S3C2410_GPF(2) ,  S3C2410_GPF2_EINT2 , 0 , "KEY1"},
    {IRQ_EINT1, S3C2410_GPF(1) ,  S3C2410_GPF1_EINT1 , 1 , "KEY2"},
    {IRQ_EINT13, S3C2410_GPG(5) ,  S3C2410_GPG5_EINT13 , 2 , "KEY3"},
    {IRQ_EINT14, S3C2410_GPG(6) ,  S3C2410_GPG6_EINT14 , 3 , "KEY4"},
};
static volatile char key_values [] = {'0', '0', '0', '0'};
static volatile char user_values [] = {'0', '0'};
static DECLARE_WAIT_QUEUE_HEAD(button_waitq);

static volatile int ev_press = 0;
static volatile int row = -1;
static volatile int col = -1;

static irqreturn_t buttons_interrupt(int irq, void *dev_id)
{
    struct button_irq_desc *button_irqs = (struct button_irq_desc *)dev_id;
    int down;
    int i,t=10;
    down = !s3c2410_gpio_getpin(button_irqs->pin);

	if(down == 1){
		udelay(100);
		down = !s3c2410_gpio_getpin(button_irqs->pin);
		if((down == 1)&&(ev_press == 0))
		{
			key_values[button_irqs->number] = '0' + down;
        		ev_press = 1;
        		for(i = 0; i < 4; i++)
        		{
        			s3c2410_gpio_setpin(col_table[i].pin, 1);
				udelay(10);
				t = s3c2410_gpio_getpin(button_irqs->pin);
				switch(t)
				{
					case 2:
					case 4:
					case 32:
					case 64:row = button_irqs->number;col = i;break;
					default:row = -1;col = -1;break;
				}
				s3c2410_gpio_setpin(col_table[i].pin, 0);
				if((row >= 0) && (col >= 0))
				{
					break;
				}
			}
		}
       wake_up_interruptible(&button_waitq);
    } 
    return IRQ_RETVAL(IRQ_HANDLED);
}


static int s3c24xx_buttons_open(struct inode *inode, struct file *file)
{
    int i, j;
    int err = 0;
    for (i = 0; i < sizeof(button_irqs)/sizeof(button_irqs[0]); i++) {
	if (button_irqs[i].irq < 0) {
		continue;
	}
        err = request_irq(button_irqs[i].irq, buttons_interrupt, IRQ_TYPE_LEVEL_LOW, 
                          button_irqs[i].name, (void *)&button_irqs[i]);
        if (err)
            break;
    }
   for(j = 0; j < 4; j++)
   {
	   s3c2410_gpio_cfgpin(col_table[j].pin, col_table[j].pin_setting);
   }

    if (err) {
        i--;
        for (; i >= 0; i--) {
	    if (button_irqs[i].irq < 0) {
		continue;
	    }
	    disable_irq(button_irqs[i].irq);
            free_irq(button_irqs[i].irq, (void *)&button_irqs[i]);
        }
        return -EBUSY;
    }

    ev_press = 1;
    //ÁÐÏßÀ­µÍ
    for(i = 0; i< 4; i++)
    {
    	s3c2410_gpio_setpin(col_table[i].pin, 0);
    }
    return 0;
}


static int s3c24xx_buttons_close(struct inode *inode, struct file *file)
{
    int i;
    
    for (i = 0; i < sizeof(button_irqs)/sizeof(button_irqs[0]); i++) {
	if (button_irqs[i].irq < 0) {
	    continue;
	}
	free_irq(button_irqs[i].irq, (void *)&button_irqs[i]);
    }

    return 0;
}


static int s3c24xx_buttons_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
    unsigned long err;

    if (!ev_press) {
	if (filp->f_flags & O_NONBLOCK)
	    return -EAGAIN;
	else
	    wait_event_interruptible(button_waitq, ev_press);
    }
    
		user_values[0] = row+48;
		user_values[1] = col+48;
		err = copy_to_user(buff, (const void *)user_values, 2);
		user_values[0] = '0';
		user_values[1] = '0';
		col = -1;
		row = -1;
		ev_press = 0;
    return err ? -EFAULT : min(sizeof(key_values), count);
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
