/**
 * Kernel module that communicates with /proc file system.
 *
 * This provides the base logic for Project 2 - displaying task information
 */

#include <linux/init.h>           // Para inicializacao e saida de modulo
#include <linux/slab.h>           // Para alocacao de memoria no kernel
#include <linux/sched.h>          // Para manipulacao de estruturas de processos
#include <linux/module.h>         // Para criacao de modulos do kernel
#include <linux/kernel.h>         // Para uso de macros do kernel
#include <linux/proc_fs.h>        // Para manipulacao do sistema de arquivos /proc
#include <linux/vmalloc.h>        // Para alocacao de memoria contigua
#include <asm/uaccess.h>          // Para transferencia de dados entre espaço do kernel e do usuario

#define BUFFER_SIZE 128		// Tamanho do buffer de leitura/escrita
#define PROC_NAME "pid"		// Nome da entrada no /proc


/* the current pid */
static long l_pid = 0;		/* Variável global para armazenar o PID atual */

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
        // creates the /proc/pid entry
        proc_create(PROC_NAME, 0666, NULL, &proc_ops);

        printk(KERN_INFO "/proc/%s Criada\n", PROC_NAME); // Informa no log do kernel que a entrada foi criada

	return 0;
}

/* This function is called when the module is removed. */

static void proc_exit(void)                 
{
        // removes the /proc/pid entry
        remove_proc_entry(PROC_NAME, NULL);

        printk( KERN_INFO "/proc/%s Removida\n", PROC_NAME);// Informa no log do kernel que a entrada foi removida
}

/**
 * A Função abaixo é chamada cada vez que a entrada /proc/pid é lida.
 *
 * Esta função é chamada repetidamente até retornar 0, então
 * há uma variável `completed` que garante que a função retorne 0
 * após uma leitura.
 */

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;				// Número de bytes lidos
        char buffer[BUFFER_SIZE];		// Buffer para armazenar dados temporariamente
        static int completed = 0;		// Variável para controlar a leitura
        struct task_struct *tsk = NULL;		// Estrutura de tsk para armazenar informações do processo

        if (completed) {
                completed = 0;
                return 0;			// Retorna 0 para indicar que a leitura foi concluída
        }

        // Obtém a estrutura da tsk (processo) associada ao PID armazenado em l_pid 
	// Busca na lista de processos 

	tsk = pid_task(find_vpid(l_pid), PIDTYPE_PID);

	// Se o processo não for encontrado, informa no buffer
	//Se for encontrado guarda as informações no buffer do kernel
	
	if (tsk == NULL) {
        	rv = snprintf(buffer, BUFFER_SIZE, "PID %ld nao encontrado\n", l_pid);
    	} else {
        	rv = snprintf(buffer, BUFFER_SIZE, "PID: %ld\nNome: %s\nEstado: %ld\n",l_pid, tsk->comm, tsk->state);
    	}

        completed = 1;			// Marca como completado para não repetir a leitura

        // copies the contents of kernel buffer to userspace usr_buf
	// Copia o conteúdo do buffer do kernel para o buffer do espaço do usuário (usr_buf) 
        
	if (copy_to_user(usr_buf, buffer, rv)) {
                rv = -EFAULT; 		// Retorna erro em caso de falha na cópia
        }

        return rv; 			// Retorna o número de bytes lidos
}

/**
 * Função chamada cada vez que escrevemos na entrada /proc.


 */static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
        char *k_mem;        // Ponteiro para armazenar dados temporarios no espaco do kernel
        int ret;            // Variavel para armazenar o valor de retorno de kstrtol

        // allocate kernel memory
	// Aloca memória para os dados recebidos do usuário (+1 para o caractere nulo)

        k_mem = kmalloc(count + 1, GFP_KERNEL); // Aloca memoria suficiente para o contendo + 1 byte para o caractere nulo
        
	if (!k_mem)
        return -ENOMEM;     		// Se a alocacao falhar, retorna erro de memoria insuficiente

        // Copia dados do buffer do usuário para o buffer do kernel

        if (copy_from_user(k_mem, usr_buf, count)) {
		printk( KERN_INFO "Error copying from user\n");
                kfree(k_mem);		// Libera a memória alocada em caso de erro
                return -EFAULT;		// Retorna erro de falha na cópia
        }
    
	k_mem[count] = '\0'; // Adiciona caractere nulo ao final da string


	
    // Converte a string de entrada para um valor inteiro do tipo long

    ret = kstrtol(k_mem, 10, &l_pid); 			// kstrtol() converte string para long, base 10 (decimal)
    if (ret < 0) {             				// Se houver erro na conversao
        kfree(k_mem);          				// Libera a memoria alocada
        return ret;            				// Retorna o codigo de erro de kstrtol
    }

    printk(KERN_INFO "Valor do PID Recebido: %ld\n", l_pid); 		// Imprime o valor de PID recebido no log do kernel

        kfree(k_mem);

        return count;
}

/* Macros para registrar os pontos de entrada e saída do módulo */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module");
MODULE_AUTHOR("SGG");