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
#include <linux/list.h>


/* This function is called when the module is loaded. */

Struct birthday {
int day;
int month;
int year;
struct list_head list;
}

macro LIST_HEAD ( )
static LIST_HEAD (birthday_list);

struct birthday *person;
person = kmalloc(sizeof(*person), GFP KERNEL);
person->day = 2;
person->month= 8;
person->year = 1995;

INIT_LIST_HEAD(&person->list);
list_add_tail(&person->list, &birthday_list);

struct birthday *ptr;
list_for_each_entry(ptr, &birthday list, list) {
/* a cada iteração prt aponta */
/* para próxima estrutura birthday */
}

struct birthday *ptr, *next
list_for_each_entry_safe(ptr,next,&birthday list,list) {
/* a cada iteração prt aponta */
/* para a próxima estrutura birthday */
list_del(&ptr->list);
kfree(ptr);
}

int birthday_init(void)
{
       printk(KERN_INFO "Loading Module\n");

       return 0;
}

/* This function is called when the module is removed. */
void birthday_exit(void) {
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( birthday_init );
module_exit( birthday_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("birthday Module");
MODULE_AUTHOR("SGG");

