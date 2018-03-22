/**
 * File:	char-kmod-jbn.c
 * Author:	Jorge Brandon Nunez
 * Class:	COP4600-SP18
 * Professor:	Dr. Gerber
 * Due Date:	2018-03-23
 */


#include <linux/module.h>	// Core header for modules.
#include <linux/device.h>	// Supports driver model.
#include <linux/kernel.h>	// Kernel header for convenient functions.
#include <linux/fs.h>		// File-system support.
#include <linux/uaccess.h>	// User access copy function support.
#define DEVICE_NAME "char-kmod"	// Device name.
#define CLASS_NAME "char-dev"	// Class name. This is a character device.


/**
 * Mod Info:	The module uses a GPL to avoid tainting the kernel.
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arati Banerjee, Huong Dang, and Jorge B. Nunez");


static int  major_number;
static char data[1024] = {'\0'};
static int  data_size;


int init_module(void)
{
	printk(KERN_INFO "Char-KMod: Installing module.\n");
	return 0;
}


void cleanup_module(void)
{
	printk(KERN_INFO "Removing module.\n");
	return;
}
