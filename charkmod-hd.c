/**
 * File:	char-kmod-jbn.c
 * Author:	Jorge Brandon Nunez
 * Class:	COP4600-SP18
 * Professor:	Dr. Gerber
 * Due Date:	2018-03-23
 * Reference: http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
 */


#include <linux/module.h>	// Core header for modules.
#include <linux/device.h>	// Supports driver model.
#include <linux/kernel.h>	// Kernel header for convenient functions.
#include <linux/fs.h>		// File-system support.
#include <linux/uaccess.h>	// User access copy function support.
#define DEVICE_NAME "charkmod"	// Device name.
#define CLASS_NAME  "chardev"	// Class name. This is a character device.


/**
 * Mod Info:	The module uses a GPL to avoid tainting the kernel.
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arati Banerjee, Huong Dang, and Jorge B. Nunez");
MODULE_DESCRIPTION("Character device driver");
MODULE_VERSION("0.1");

#define MAX_BUFFER_SIZE 1024

/**
 * Important variables that store data and keep track of relevant information.
 */
static int  major_number;
static char data[MAX_BUFFER_SIZE] = {'\0'};
static int  data_size;
static int  bufferLocation = 0;
static int  bufferRead = 0;

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


int init_module(void)
{
	printk(KERN_INFO "charkmod: installing module.\n");

	// Allocate a major number for the device.
	major_number = register_chrdev(0, DEVICE_NAME, &fops);
	if (major_number < 0) {
		printk(KERN_ALERT "charkmod could not register number.\n");
		return major_number;
	}
	printk(KERN_INFO "charkmod: registered correctly with major number %d\n", major_number);

	return 0;
}


void cleanup_module(void)
{
	printk(KERN_INFO "charkmod: removing module.\n");
	unregister_chrdev(major_number, DEVICE_NAME);
	return;
}


static int open(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "charkmod: device opened.\n");
	return 0;
}


static int close(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "charkmod: device closed.\n");
	return 0;
}


static ssize_t read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int errorcount = 0;
	int seen = 0; // # of bytes already seen by user 

  // If not enough data is available to service a read request, 
  // the driver must respond with only the amount available (including 0 bytes)
	while (seen < len && bufferLocation > 0)
		// Send to user from our data buffer 
		errorcount = copy_to_user(buffer[seen], data[bufferRead], 1);
    bufferLocation--;
    bufferRead++;
    seen++;
    
    // If we go over max buffer
    if (bufferRead > MAX_BUFFER_SIZE)
      break;
  }
  
  if (error_count==0){           
     printk(KERN_INFO "charkmod: Sent %d characters to the user\n", seen);
     return seen;  // clear the position to the start and return 0
  }
  else {
     printk(KERN_INFO "charkmod: Failed to send %d characters to the user\n", error_count);
     return -EFAULT;              
  }
}


static ssize_t write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	printk(KERN_INFO "charkmod: something wrote to device.\n");
	return 0;
}
