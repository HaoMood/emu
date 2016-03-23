#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define GPJ0CON	0xE0200240
#define GPJ0DAT 0xE0200244

volatile unsigned int* pConf;
volatile unsigned int* pData;

int drv_open(struct inode* inode, struct file* filp)
{
	return 0;
}

int drv_release(struct inode* inode, struct file* filp)
{
	return 0;
}

int drv_write(struct inode* inode, char* buf, size_t length, loff_t *offset)
{
	char c;
	volatile int val;
	copy_from_user(&c, buf, 1);

	val = *pData;
	//val &= ~(7 << 3);
	//val |= (~c) << 3;
	val = ~(0 << 3);
	val = ~(c << 3);
	*pData = val;
	return 1;
}


struct file_operations fop =
{
	open : drv_open,
	write : drv_write,
	release : drv_release,
};

int init_module()
{
	int ret;
	
	pConf = ioremap(GPJ0CON, 4);
	pData = ioremap(GPJ0DAT, 4);
	
	if((ret = register_chrdev(240, "LED driver", &fop)) < 0)
	{
		printk("err\n");
		return -1;
	}

	*pConf &= 0xff000fff;
	*pConf |= 0xff111fff;

	*pData = ~(7 << 3);

	printk("LED: Hello from kernel space\n");

	return 0;
}

void cleanup_module(void)
{
	*pData = ~(0 << 3);

	iounmap(pConf);
	iounmap(pData);
	unregister_chrdev(240, "LED driver");

	printk("LED: I'm leaving, bye\n");
}

MODULE_LICENSE("GPL");
		
