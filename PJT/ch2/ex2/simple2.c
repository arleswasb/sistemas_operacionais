/**
 * simple.c
 *
 * A simple kernel module. 
 * 
 * To compile, run makefile by entering "make"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>   // Para acessar o valor de jiffies
#include <asm/param.h>       // Para acessar o valor de HZ

/* This function is called when the module is loaded. */
int simple_init(void)
{
       printk(KERN_INFO "Loading Module\n");
       
       // 1. Imprimir o valor de jiffies e HZ
        printk(KERN_INFO "jiffies: %lu\n", jiffies);
        printk(KERN_INFO "HZ (timer tick rate): %d\n", HZ);


       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");

       // 2. Imprimir o valor de jiffies ao sair
        printk(KERN_INFO "jiffies: %lu\n", jiffies);
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module with jiffies and HZ");
MODULE_AUTHOR("SGG");

