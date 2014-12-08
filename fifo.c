#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include<linux/string.h>
#include <asm/uaccess.h>
#include <linux/time.h>	
#include <linux/slab.h>	
#include <linux/wait.h>
#include<linux/sched.h>
#define fifoname "fifo"

MODULE_AUTHOR("Viswanath");
MODULE_DESCRIPTION("Lab 2 Solution");
MODULE_LICENSE("GPL");

static char _FIFO[2][11];
static int firstFIFOPointer=0;
static int secondFIFOPointer=0;
static int _FIFO_num;
static DECLARE_WAIT_QUEUE_HEAD(wq);
static int flag=0;

static ssize_t fifo_read(struct file *file, char *buf, size_t count,loff_t *ppos)
{
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	wait_event(wq, flag != 0);
	if (_FIFO[_FIFO_num][*ppos] == '\0') {
		printk(KERN_INFO "End of string, returning zero.\n");
		strcpy(_FIFO[_FIFO_num],"EOF\0");
		return 0;
	}
	copy_to_user(buf, &_FIFO[_FIFO_num][*ppos], 1);
	*ppos += 1;
	flag = 0;
	return 1;
}
// this method is executed when writing to the module
static ssize_t fifo_write( struct file *file, const char *buf, size_t count,loff_t *ppos )
{
	int i;
	int fifopointer = (_FIFO_num==0)?firstFIFOPointer:secondFIFOPointer;
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	for(i=0;i<count-1 && fifopointer<10;i++,fifopointer++)
	{
		_FIFO[_FIFO_num][fifopointer]=*(buf+i);
	}
	_FIFO[_FIFO_num][fifopointer]='\0';
	flag = 1;
	wake_up(&wq);
	return i;
}
// this method is called whenever the module is being used
// e.g. for both read and write operations
static int fifo_open(struct inode * inode, struct file * file)
{
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	_FIFO_num=MINOR(inode -> i_rdev)/2;
	return 0;
}
// this method releases the module and makes it available for new
// operations
static int fifo_release(struct inode * inode, struct file * file)
{
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	return 0;
}
// module's file operations
static struct file_operations fifo_fops = {
	.owner = THIS_MODULE,
	.read = fifo_read,
	.write = fifo_write,
	.open = fifo_open,
	.release = fifo_release,
};
// initialize module (executed when using insmod)
static int __init fifo_init(void)
{
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	register_chrdev(240,fifoname,&fifo_fops);
	_FIFO[0][0]='\0';
	_FIFO[1][0]='\0';
	return 0;
}
// cleanup module (executed when using rmmod)
static void __exit fifo_cleanup(void)
{
	unregister_chrdev(240,fifoname);
}
module_init(fifo_init);
module_exit(fifo_cleanup);
