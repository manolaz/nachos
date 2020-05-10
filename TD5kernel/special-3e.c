#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

static unsigned length = 1024;
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

	mutex_lock(&mutex);

	if (*offp >= length) {
		mutex_unlock(&mutex);
		return 0;
	}

	if (count + *offp > length)
		count = length - *offp;

	for(i=0; i<count; i++) {
		ret = copy_to_user(userbuff+i, &buffer[*offp+i], 1);
		if (ret) {
			*offp += i;
			mutex_unlock(&mutex);
			return -EFAULT;
		}
		buffer[*offp+i] = '0';
	}

	mutex_unlock(&mutex);

	*offp += count;
	return count;
}

static ssize_t
special_miscdev_write(struct file* filp, const char __user * userbuff, size_t count, loff_t* offp)
{
	int i;

	printk(KERN_INFO "special: writing %lu bytes at offset %ld\n", (unsigned long) count, (unsigned long) *offp);

	mutex_lock(&mutex);

	if (*offp >= length) {
		mutex_unlock(&mutex);
		return -ENOSPC;
	}

	if (count + *offp > length)
		count = length - *offp;

	for(i=0; i<count; i++)
		buffer[*offp+i]++;

	mutex_unlock(&mutex);

	*offp += count;
	return count;
}

static long
special_miscdev_ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	printk(KERN_INFO "special: ioctl with command %u\n", cmd);

	switch (cmd) {
	case 1: {
		char *newbuffer = kmalloc(arg, GFP_KERNEL);
		if (!newbuffer) {
			printk(KERN_ERR "special: failed to reallocate buffer\n");
			return -ENOMEM;
		}
		mutex_lock(&mutex);
		if (arg < length) {
			memcpy(newbuffer, buffer, arg);
		} else {
			memcpy(newbuffer, buffer, length);
			memset(newbuffer+length, '0', arg-length);
		}
		kfree(buffer);
		buffer = newbuffer;
		length = arg;
		mutex_unlock(&mutex);
		return 0;
	}

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

	buffer = kmalloc(length, GFP_KERNEL);
	if (!buffer) {
		printk(KERN_ERR "special: failed to allocate buffer\n");
		misc_deregister(&special_miscdev);
		return -ENOMEM;
	}

	memset(buffer, '0', length);

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
