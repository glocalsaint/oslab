#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>

MODULE_AUTHOR("Viswanath");
MODULE_DESCRIPTION("Lab 2 Solution");
MODULE_LICENSE("GPL");

// this method is executed when reading from the module
static int x=34;
static ssize_t fifo_read(struct file *file, char *buf, size_t count,
                                                        loff_t *ppos)
{
printk(KERN_ALERT "Inside the x=%d:%s function\n",x, __FUNCTION__);
x=x+1;
return 0;
}

// this method is executed when writing to the module
static ssize_t fifo_write( struct file *file, const char *buf, size_t count,
                                                 loff_t *ppos )
{
printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
return 0;
}

// this method is called whenever the module is being used
// e.g. for both read and write operations
static int fifo_open(struct inode * inode, struct file * file)
{
printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
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
        .owner =        THIS_MODULE,
        .read =         fifo_read,
        .write =        fifo_write,
        .open =         fifo_open,
        .release =      fifo_release,
};

// initialize module (executed when using insmod)
static int __init fifo_init(void)
{
printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);

register_chrdev(240,"FIFO",&fifo_fops);
return 0;
}

// cleanup module (executed when using rmmod)
static void __exit fifo_cleanup(void)
{
unregister_chrdev(240,"FIFO");
}

module_init(fifo_init);
module_exit(fifo_cleanup);
