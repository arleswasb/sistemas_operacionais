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
#include <linux/hash.h>  // Para GOLDEN_RATIO_PRIME
#include <linux/gcd.h>   // Para a função gcd()


/* This function is called when the module is loaded. */
int simple_init(void)
{
       printk(KERN_INFO "Loading Module\n");
       
       // 1. Imprimir o valor de GOLDEN_RATIO_PRIME
       printk(KERN_INFO "GOLDEN_RATIO_PRIME: %lu\n", GOLDEN_RATIO_PRIME);


       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");

       // 2. Calcular e imprimir o MDC (gcd) de 3300 e 24
       unsigned long result_gcd = gcd(3300, 24);
       printk(KERN_INFO "GCD of 3300 and 24 is: %lu\n", result_gcd);
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module with GOLDEN_RATIO_PRIME and GCD");
MODULE_AUTHOR("SGG");

