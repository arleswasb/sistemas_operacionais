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

static unsigned long start_jiffies;  // Variável global para armazenar o valor de jiffies na inicialização

/* This function is called when the module is loaded. */
int simple_init(void)
{
       printk(KERN_INFO "Loading Module\n");
       
       // Capturar o valor de jiffies no momento do carregamento do módulo
        start_jiffies = jiffies;
        printk(KERN_INFO "Start jiffies: %lu\n", start_jiffies);

        // Imprimir o valor de HZ (taxa de ticks por segundo)
        printk(KERN_INFO "HZ (timer tick rate): %d\n", HZ);


       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {

    unsigned long end_jiffies = jiffies;  // Capturar o valor de jiffies na remoção
    unsigned long elapsed_jiffies = end_jiffies - start_jiffies;  // Calcular a diferença
    unsigned long elapsed_seconds = elapsed_jiffies / HZ;  // Converter para segundos
    
	printk(KERN_INFO "Removing Module\n");

    // Imprimir o valor de jiffies no momento da remoção
        printk(KERN_INFO "End jiffies: %lu\n", end_jiffies);

    // Imprimir o tempo decorrido em jiffies e em segundos
        printk(KERN_INFO "Elapsed jiffies: %lu\n", elapsed_jiffies);
        printk(KERN_INFO "Elapsed time: %lu seconds\n", elapsed_seconds);
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module with jiffies and time calculation");
MODULE_AUTHOR("SGG");

