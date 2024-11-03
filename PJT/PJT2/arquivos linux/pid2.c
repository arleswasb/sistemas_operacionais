#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>    // Para kmalloc e kfree
#include <asm/uaccess.h>
#include <linux/string.h>  // Para kstrtol

#define BUFFER_SIZE 128
#define PROC_NAME "pid"

static long pid_value = 0; // Variável para armazenar o valor convertido

// Função de escrita para a entrada /proc/pid
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos) {
    char *k_mem;
    int ret;

    // Aloca memória do kernel
    k_mem = kmalloc(count + 1, GFP_KERNEL); // +1 para caractere nulo
    if (!k_mem)
        return -ENOMEM;  // Erro de memória insuficiente

    // Copia do espaço do usuário para a memória do kernel
    if (copy_from_user(k_mem, usr_buf, count)) {
        kfree(k_mem);
        return -EFAULT;
    }
    k_mem[count] = '\0'; // Adiciona caractere nulo ao final da string

    // Converte a string para um valor inteiro usando kstrtol()
    ret = kstrtol(k_mem, 10, &pid_value); // Base 10 para decimal
    if (ret < 0) {
        kfree(k_mem);
        return ret;
    }

    printk(KERN_INFO "Received PID value: %ld\n", pid_value);

    // Libera memória do kernel
    kfree(k_mem);
    return count;
}

// Função de leitura para a entrada /proc/pid
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;

    // Formata a saída para incluir o comando fictício, PID e estado
    rv = snprintf(buffer, BUFFER_SIZE, "command = [bash] pid = [%ld] state = [1]\n", pid_value);

    // Copia dados do buffer do kernel para o espaço do usuário
    if (copy_to_user(usr_buf, buffer, rv)) {
        return -EFAULT;
    }

    return rv;
}

// Define as operações para a entrada /proc
static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
    .write = proc_write,
};

// Função chamada quando o módulo é carregado
static int proc_init(void) {
    // Cria a entrada /proc/pid
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s Criado\n", PROC_NAME);
    return 0;
}

// Função chamada quando o módulo é removido
static void proc_exit(void) {
    // Remove a entrada /proc/pid
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s Removido\n", PROC_NAME);
}

// Macros para registrar os pontos de entrada e saída do módulo
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple /proc/pid Module with Integer Parsing");
MODULE_AUTHOR("Your Name");