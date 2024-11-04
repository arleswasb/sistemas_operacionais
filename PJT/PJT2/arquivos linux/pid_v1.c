/**
 * Kernel module that communicates with /proc file system.
 *
 * This provides the base logic for Project 2 - displaying task information
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "pid"

/* the current pid */
static long l_pid = 0;

/**
 * Function prototypes
 */
static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,                  // Funcao de leitura para a entrada /proc
        .write = proc_write,                // Funcao de escrita para a entrada /proc
};

/* This function is called when the module is loaded. */
static int proc_init(void)                  // Cria a entrada /proc/pid com permissoes 0666 (leitura e escrita)
{
        // creates the /proc/procfs entry
        proc_create(PROC_NAME, 0666, NULL, &proc_ops);

        printk(KERN_INFO "/proc/%s Criada\n", PROC_NAME); // Informa no log do kernel que a entrada foi criada

	return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void)                 
{
        // removes the /proc/procfs entry
        remove_proc_entry(PROC_NAME, NULL);

        printk( KERN_INFO "/proc/%s Removida\n", PROC_NAME);// Informa no log do kernel que a entrada foi removida
}

/**
 * This function is called each time the /proc/pid is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;
        struct task_struct *tsk = NULL;

        if (completed) {
                completed = 0;
                return 0;
        }

        tsk = pid_task(find_vpid(l_pid), PIDTYPE_PID);

	 
	if (tsk == NULL) {
        	rv = snprintf(buffer, BUFFER_SIZE, "PID %ld nao encontrado\n", l_pid);
    	} else {
        	rv = snprintf(buffer, BUFFER_SIZE, "PID: %ld\nNome: %s\nEstado: %ld\n",l_pid, tsk->comm, tsk->state);
    	}

        completed = 1;

        // copies the contents of kernel buffer to userspace usr_buf 
        if (copy_to_user(usr_buf, buffer, rv)) {
                rv = -EFAULT;
        }

        return rv;
}

/**
 * This function is called each time we write to the /proc file system.
 */
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
        char *k_mem;        // Ponteiro para armazenar dados temporarios no espaco do kernel
        int ret;            // Variavel para armazenar o valor de retorno de kstrtol

        // allocate kernel memory
        k_mem = kmalloc(count+1, GFP_KERNEL); // Aloca memoria suficiente para o contendo + 1 byte para o caractere nulo
        if (!k_mem)
        return -ENOMEM;     // Se a alocacao falhar, retorna erro de memoria insuficiente

        /* copies user space usr_buf to kernel buffer */

        if (copy_from_user(k_mem, usr_buf, count)) {
		printk( KERN_INFO "Error copying from user\n");
                kfree(k_mem);
                return -EFAULT;
        }
    k_mem[count] = '\0';      						 // Adiciona um caractere nulo ao final da string para evitar problemas de buffer
	
    // Converte a string de entrada para um valor inteiro do tipo long

    ret = kstrtol(k_mem, 10, &l_pid); 			// kstrtol() converte string para long, base 10 (decimal)
    if (ret < 0) {             				// Se houver erro na conversao
        kfree(k_mem);          				// Libera a memoria alocada
        return ret;            				// Retorna o codigo de erro de kstrtol
    }

    printk(KERN_INFO "Received PID value: %ld\n", l_pid); 		// Imprime o valor de PID recebido no log do kernel

        kfree(k_mem);

        return count;
}

/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module");
MODULE_AUTHOR("SGG");