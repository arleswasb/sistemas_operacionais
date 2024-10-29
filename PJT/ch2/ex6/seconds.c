#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <linux/unistd.h>  // Para HZ
#include <asm/uaccess.h>

#define PROC_NAME "seconds"

static unsigned long start_jiffies;  // Armazena o valor de jiffies quando o módulo é carregado

/* Função de leitura para o arquivo /proc/seconds */
ssize_t seconds_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    char buffer[128];
    unsigned long elapsed_seconds;

    // Calcula o número de segundos que se passaram desde que o módulo foi carregado
    elapsed_seconds = (jiffies - start_jiffies) / HZ;

    // Formata o número de segundos em uma string
    int len = sprintf(buffer, "%lu\n", elapsed_seconds);

    // Copia o conteúdo do buffer do kernel para o espaço de usuário
    if (copy_to_user(usr_buf, buffer, len)) {
        return -EFAULT;
    }

    return len;
}

/* Definição das operações para o arquivo /proc/seconds */
static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = seconds_read,
};

/* Função chamada quando o módulo é carregado */
int proc_init(void) {
    // Armazena o valor atual de jiffies no momento em que o módulo é carregado
    start_jiffies = jiffies;

    // Cria a entrada /proc/seconds
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s criado\n", PROC_NAME);
    return 0;
}

/* Função chamada quando o módulo é removido */
void proc_exit(void) {
    // Remove a entrada /proc/seconds
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removido\n", PROC_NAME);
}

/* Macros para registrar os pontos de entrada e saída do módulo */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modulo que exibe o número de segundos desde que foi carregado");
MODULE_AUTHOR("arleswasb");