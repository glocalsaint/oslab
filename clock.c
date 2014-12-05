#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>


static ssize_t clock_read(struct file *file, char *buf, size_t count,
                                                        loff_t *ppos)
{
copy_to_user(buf,"hi viswanath\n",12*sizeof(char));
return 13*sizeof(char);
}
static const struct file_operations clock_fops = {
  .owner = THIS_MODULE,
  .read = clock_read,
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

