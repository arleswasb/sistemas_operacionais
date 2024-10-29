#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>  // Para acessar os jiffies
#include <asm/uaccess.h>

#define PROC_NAME "jiffies"

/* Função de leitura para o arquivo /proc/jiffies */
ssize_t jiffies_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    int rv = 0;
    char buffer[128];
    static int completed = 0;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;

    // Escreve o valor atual de jiffies no buffer
    rv = sprintf(buffer, "%lu\n", jiffies);

    // Copia o conteúdo do buffer do kernel para o espaço de usuário
    if (copy_to_user(usr_buf, buffer, rv)) {
        return -EFAULT;
    }

    return rv;
}

/* Definição das operações para o arquivo /proc/jiffies */
static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = jiffies_read,
};

/* Função chamada quando o módulo é carregado */
int proc_init(void) {
    // Cria a entrada /proc/jiffies
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s criado\n", PROC_NAME);
    return 0;
}

/* Função chamada quando o módulo é removido */
void proc_exit(void) {
    // Remove a entrada /proc/jiffies
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removido\n", PROC_NAME);
}

/* Macros para registrar os pontos de entrada e saída do módulo */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modulo que exibe o valor atual de jiffies");
MODULE_AUTHOR("arles");