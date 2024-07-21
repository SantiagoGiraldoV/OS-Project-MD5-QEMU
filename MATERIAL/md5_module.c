#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "md5_device"
#define CLASS_NAME "md5_class"

MODULE_LICENSE("GPL");

static int majorNumber;
static struct class* md5_class = NULL;
static struct device* md5_device = NULL;
static char *message = NULL;

static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "MD5 Device: opened\n");
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "MD5 Device: closed\n");
    return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    message = kmalloc(len + 1, GFP_KERNEL);
    if (!message) {
        printk(KERN_ALERT "MD5 Device: Memory allocation failed\n");
        return -ENOMEM;
    }

    if (copy_from_user(message, buffer, len)) {
        kfree(message);
        printk(KERN_ALERT "MD5 Device: Failed to copy data from user space\n");
        return -EFAULT;
    }
    message[len] = '\0';

    printk(KERN_INFO "MD5 Device: Received %zu characters from the user: %s\n", len, message);
    kfree(message);
    return len;
}

static struct file_operations fops = {
    .open = dev_open,
    .release = dev_release,
    .write = dev_write,
};

static int __init md5_init(void) {
    printk(KERN_INFO "MD5 Module: Initializing\n");

    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        printk(KERN_ALERT "MD5 Module: Failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "MD5 Module: Registered correctly with major number %d\n", majorNumber);

    md5_class = class_create(CLASS_NAME);
    if (IS_ERR(md5_class)) {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "MD5 Module: Failed to register device class\n");
        return PTR_ERR(md5_class);
    }
    printk(KERN_INFO "MD5 Module: Device class registered correctly\n");

    md5_device = device_create(md5_class, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(md5_device)) {
        class_destroy(md5_class);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "MD5 Module: Failed to create the device\n");
        return PTR_ERR(md5_device);
    }
    printk(KERN_INFO "MD5 Module: Device created correctly\n");

    return 0;
}

static void __exit md5_exit(void) {
    device_destroy(md5_class, MKDEV(majorNumber, 0));
    class_unregister(md5_class);
    class_destroy(md5_class);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "MD5 Module: Goodbye from the LKM!\n");
}

module_init(md5_init);
module_exit(md5_exit);
