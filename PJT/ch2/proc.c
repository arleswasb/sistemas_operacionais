#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#define BUFFER_SIZE 128
#define PROC_NAME "hello"

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

/* Definir as operações para a entrada do /proc */
static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};

/* Esta função é chamada quando o módulo é carregado */
int proc_init(void) {
    // Cria a entrada /proc/hello
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

/* Esta função é chamada quando o módulo é removido */
void proc_exit(void) {
    // Remove a entrada /proc/hello
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/* Função de leitura para a entrada /proc/hello */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;

    rv = sprintf(buffer, "Hello World\n");

    // Copia dados do buffer do kernel para o espaço do usuário
    if (copy_to_user(usr_buf, buffer, rv)) {
        return -EFAULT;
    }

    return rv;
}

/* Macros para registrar os pontos de entrada e saída do módulo */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple /proc/hello Module");
MODULE_AUTHOR("Your Name");