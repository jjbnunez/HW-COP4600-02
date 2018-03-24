/**
 * File:	charkmod.c
 * Author:	Jorge Brandon Nunez, Arati Banerjee, Huong Dang
 * Class:	COP4600-SP18
 * Professor:	Dr. Gerber
 * Due Date:	2018-03-23
 */


#include <linux/module.h>	// Core header for modules.
#include <linux/device.h>	// Supports driver model.
#include <linux/kernel.h>	// Kernel header for convenient functions.
#include <linux/fs.h>		// File-system support.
#include <linux/uaccess.h>	// User access copy function support.
#define DEVICE_NAME "charkmod"	// Device name.
#define MAX_SIZE    1024	// Max buffer size.


/**
 * Mod Info:	The module uses a GPL to avoid tainting the kernel.
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arati Banerjee, Huong Dang, and Jorge B. Nunez");


/**
 * Important variables that store data and keep track of relevant information.
 */
static int  major_number;
static char temp[MAX_SIZE];
static char data[MAX_SIZE];
static int  data_size;


/**
 * Prototype functions for file operations.
 */
static int     open(struct inode *, struct file *);
static int     close(struct inode *, struct file *);
static ssize_t read(struct file *, char *, size_t, loff_t *);
static ssize_t write(struct file *, const char *, size_t, loff_t *);


/**
 * File operations structure and the functions it points to.
 */
static struct file_operations fops =
{
	.owner   = THIS_MODULE,
	.open    = open,
	.release = close,
	.read    = read,
	.write   = write,
};

/**
 * Initializes module at installation
 */
int init_module(void)
{
	int i;

	printk(KERN_INFO "charkmod: installing module.\n");

	// Allocate a major number for the device.
	major_number = register_chrdev(0, DEVICE_NAME, &fops);
	if (major_number < 0) {
		printk(KERN_ALERT "charkmod could not register number.\n");
		return major_number;
	}
	printk(KERN_INFO "charkmod: registered correctly with major number %d\n", major_number);

	// Initialize all data bytes to '\0'.
	data_size = 0;
	for (i = 0; i < MAX_SIZE; i++) {
		data[i] = '\0';
	}

	return 0;
}

/*
 * Removes module, sends appropriate message to kernel
*/
void cleanup_module(void)
{
	printk(KERN_INFO "charkmod: removing module.\n");
	unregister_chrdev(major_number, DEVICE_NAME);
	return;
}

/*
 * Opens device module, sends appropriate message to kernel
*/
static int open(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "charkmod: device opened.\n");
	return 0;
}

/*
 * Closes device module, sends appropriate message to kernel
*/
static int close(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "charkmod: device closed.\n");
	return 0;
}

/*
 * Reads from device, displays in userspace, and deletes the read data
*/
static ssize_t read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int i, j;
	int limit = MAX_SIZE, error = 0;

	printk(KERN_INFO "charkmod: something read from device.\n");

	// Sends message to kernel when empty buffer is read
	if (data_size == 0) {
		printk(KERN_INFO "charkmod: tried to read empty buffer!\n");
	}

  // The limit of how far is read is based on the data size
	if (len < data_size) {
		limit = len;
		data_size = data_size - len;
	} else {
		limit = data_size;
		data_size = 0;
	}

	// Sends the appropriate data to the userspace
	error = copy_to_user(buffer, data, limit);
	if (error != 0) {
		printk(KERN_INFO "charkmod: error copying to user!\n");
		return -EFAULT;
	}

	// Copies remaining data into a temporary array
	for (i = limit, j = 0; i < MAX_SIZE; i++, j++) {
		temp[j] = data[i];
	}

  // Fills the rest of the array with null characters
	for ( ; j < MAX_SIZE; j++) {
		temp[j] = '\0';
	}

  // Fills data with the temp array
	for (i = 0; i < MAX_SIZE; i++) {
		data[i] = temp[i];
	}

  // Returns the amount that was read to userspace
	return limit;
}

/*
 * Writes to the device
*/
static ssize_t write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	int i;

	printk(KERN_INFO "charkmod: something wrote to device.\n");

  // Sends message to kernel when there is less space than offered data
	if (len > MAX_SIZE) {
		printk(KERN_INFO "charkmod: not enough space! Dropping what's left.\n");
	}

  // Writes the data to the device
	for (i = 0, data_size = 0; i < MAX_SIZE; i++) {
		if (i >= len)
			data[i] = '\0';
		else {
			data[i] = buffer[i];
			data_size++;
		}
	}

  // Returns the count of the number of bytes attempted to be written
	return len;
}
