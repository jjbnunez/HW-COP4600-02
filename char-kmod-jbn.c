/**
 * File:	char-kmod-jbn.c
 * Author:	Jorge Brandon Nunez
 * Class:	COP4600-SP18
 * Professor:	Dr. Gerber
 * Due Date:	2018-03-23
 */


#include <linux/module.h>	// Core header for modules.
#include <linux/kernel.h>	// Kernel header for convenient functions.


/**
 * Module information.
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arati Banerjee, Huong Dang, and Jorge B. Nunez");


static char *string = "boiyyyy";


int init_module(void) {
	printk(KERN_INFO "Installing module. Whaddap %s!!!\n", string);
	return 0;
}


void cleanup_module(void) {
	printk(KERN_INFO "Removing module. Peace out %s!!!\n", string);
}
