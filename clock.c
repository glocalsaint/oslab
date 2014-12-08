#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include<linux/time.h>
static char timestring[100];

static ssize_t clock_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
        if (timestring[*ppos] == '\0') {
        	printk(KERN_INFO "End of string, returning zero.\n");
        	return 0;
    	}

    	copy_to_user(buf, &timestring[*ppos], 1);
    	*ppos += 1;
    	return 1;  // returned a single character
}
static int clock_open(struct inode * inode, struct file * file)
{
        printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
        
	signed long get_time;
        int sec, hr, min, tmp;
        struct timeval tv;
        
	do_gettimeofday(&tv);
        tmp = tv.tv_sec;
        sec = tmp % 60;
        tmp = tmp / 60;
        min = tmp % 60;
        tmp = tmp / 60;
        hr = tmp % 24;
        
        sprintf(timestring, "time ::  %d:%d:%d of Year: %d\n\0",hr,min,sec,tmp/(24*365)+1970 );
        
        return 0;
}
// this method releases the module and makes it available for new
// operations
static int clock_release(struct inode * inode, struct file * file)
{
        printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
        return 0;
}

static const struct file_operations clock_fops = {
        .owner = THIS_MODULE,
        .read = clock_read,
        .open = clock_open,
        .release = clock_release,
};
static int __init clock_init(void) {
        proc_create("clock", 0, NULL, &clock_fops);
        return 0;
}

static void __exit clock_exit(void) {
        remove_proc_entry("clock", NULL);
}

MODULE_LICENSE("GPL");
module_init(clock_init);
module_exit(clock_exit);
