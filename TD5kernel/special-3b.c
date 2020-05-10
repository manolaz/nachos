#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#define LENGTH 1024
static char * buffer;
struct mutex mutex;

static int
special_miscdev_open(struct inode * inode, struct file * file)
{
	printk(KERN_INFO "special: opening file %p\n", file);
	return 0;
}

static int
special_miscdev_release(struct inode * inode, struct file * file)
{
	printk(KERN_INFO "special: closing file %p\n", file);
	return 0;
}

static ssize_t
special_miscdev_read(struct file* filp, char __user * userbuff, size_t count, loff_t* offp)
{
	int i, ret;

	printk(KERN_INFO "special: reading %lu bytes at offset %ld\n", (unsigned long) count, (unsigned long) *offp);

	if (*offp > LENGTH) {
		printk("lu 0\n");
		return 0;
	}

	if (count > LENGTH - *offp)
		count = LENGTH - *offp;

#if 0
	for(i=0; i<count; i++) {
		ret = copy_to_user(userbuff+i, buffer+i+*offp, 1);
		if (ret)
			return -EFAULT;
	}
#else
	/* version factorisée */
	ret = copy_to_user(userbuff, buffer+*offp, count);
	if (ret)
		return -EFAULT;
#endif

	*offp += count;
	printk("lu %lu\n", count);
	return count;
}

static ssize_t
special_miscdev_write(struct file* filp, const char __user * userbuff, size_t count, loff_t* offp)
{
	int i, ret;

	printk(KERN_INFO "special: writing %lu bytes at offset %ld\n", (unsigned long) count, (unsigned long) *offp);

	if (*offp > LENGTH) {
		printk("écrit 0\n");
		return 0;
	}

	if (count > LENGTH - *offp)
		count = LENGTH - *offp;

#if 0
	for(i=0; i<count; i++) {
		ret = copy_from_user(buffer+i+*offp, userbuff+i, 1);
		if (ret)
			return -EFAULT;
	}
#else
	/* version factorisée */
	ret = copy_from_user(buffer+*offp, userbuff, count);
	if (ret)
		return -EFAULT;
#endif

	*offp += count;
	printk("écrit %lu\n", count);
	return count;
}

static long
special_miscdev_ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	printk(KERN_INFO "special: ioctl with command %u\n", cmd);

	switch (cmd) {

	default:
		printk(KERN_ERR "special: don't know what to do with ioctl command %d", cmd);
		return -ENOSYS;
	}

	return 0;
}


static struct file_operations
special_miscdev_fops = {
	.owner = THIS_MODULE,
	.open = special_miscdev_open,
	.release = special_miscdev_release,
	.read = special_miscdev_read,
	.write = special_miscdev_write,
	.unlocked_ioctl = special_miscdev_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = special_miscdev_ioctl,
#endif
};

static struct miscdevice
special_miscdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "special",
	.fops = &special_miscdev_fops,
};

MODULE_ALIAS_MISCDEV(MISC_DYNAMIC_MINOR);
MODULE_ALIAS("devname:special");

int
special_init(void)
{
	int err;

	printk(KERN_INFO "special: registering misc device...\n");

	mutex_init(&mutex);

	err = misc_register(&special_miscdev);
	if (err < 0) {
		printk(KERN_ERR "special: failed to register misc device, error %d\n", err);
		return err;
	}

	buffer = kmalloc(LENGTH, GFP_KERNEL);
	if (!buffer) {
		printk(KERN_ERR "special: failed to allocate buffer\n");
		misc_deregister(&special_miscdev);
		return -ENOMEM;
	}

	/* on initialise le buffer à des caractères '0' (pas \0), sauf le premier */
	memset(buffer, '0', LENGTH);
	buffer[0] = 'x';

	printk(KERN_INFO "special ready\n");
	return 0;
}

void
special_exit(void)
{
	printk(KERN_INFO "special: exiting...\n");
	kfree(buffer);
	misc_deregister(&special_miscdev);
	printk(KERN_INFO "special: exited\n");
}

module_init(special_init);
module_exit(special_exit);
MODULE_LICENSE("GPL");
