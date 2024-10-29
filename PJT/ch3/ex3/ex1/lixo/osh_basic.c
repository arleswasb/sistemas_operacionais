
#include <stdio.h> 			//Biblioteca padrão para entrada e saída (funções como printf e fgets).
#include <unistd.h> 			//Fornece acesso a várias funções do sistema operacional, como fork e execvp.
#include <string.h> 			//Inclui funções para manipulação de strings, como strtok e strcmp.
#include <sys/wait.h> 			//Necessário para usar a função wait, que permite que o processo pai espere pelo término do processo filho.

#define MAX_LINE 80 /* The maximum length command */ 				//Define o tamanho máximo do comando de entrada, ou seja, o shell aceitará comandos de até 80 caracteres.

int main(void) {
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */  		//Um array de ponteiros para armazenar os argumentos do comando, assumindo que os argumentos não ocuparão mais de 40 espaços (dividindo 80 por 2, no pior caso, considerando o formato "comando + argumentos").
    int should_run = 1; /* flag to determine when to exit program */ 		//Um flag que controla o loop do shell, permitindo que ele continue executando até que o usuário digite "exit".

    while (should_run) {		//O programa entra em um loop contínuo (while(should_run)) que só é interrompido se o comando exit for inserido.
        printf("osh> "); 		// Prompt de comando
        fflush(stdout);   		// Limpa a sai­da para garantir que o prompt seja exibido

        // Le a entrada do usuario
        char input[MAX_LINE];
        if (fgets(input, MAX_LINE, stdin) == NULL) { 				//Lê o comando digitado pelo usuário, limitado a 80 caracteres (definido pela constante MAX_LINE).
            perror("Error reading input");
            continue;
        }

        // Remove o caractere de nova linha da entrada
        input[strcspn(input, "\n")] = 0;

        // Verifica se o comando é "exit" para encerrar o shell
        if (strcmp(input, "exit") == 0) {
            should_run = 0;
            continue;
        }

        // Quebra a linha de comando em tokens/argumentos
        int i = 0;
        args[i] = strtok(input, " "); 						//Divide o comando em palavras, usando o espaço como delimitador. Cada parte do comando (o comando em si e seus argumentos) é armazenada no array args.
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

	// Verifica se o último argumento é "&" (indicando execução em segundo plano)
        int run_in_background = 0; // Flag para execução em segundo plano
        if (i > 0 && strcmp(args[i - 1], "&") == 0) { // Se o último argumento for "&"
            run_in_background = 1; // Marca para rodar em segundo plano
            args[i - 1] = NULL; // Remove o "&" da lista de argumentos
        }

        // Cria um processo filho
        pid_t pid = fork();
        if (pid < 0) {
            // Erro ao criar processo filho
            perror("Fork failed");
            return 1;
        } else if (pid == 0) {
            // Processo filho: executa o comando usando execvp
            if (execvp(args[0], args) == -1) {
                perror("Error executing command");
            }
            return 1; // Sai do processo filho
        } else {
           if (!run_in_background) {
                // Se NÃO for em segundo plano, espera o filho terminar
                wait(NULL);
            } else {
                // Se for em segundo plano, não espera
                printf("[Processo em segundo plano iniciado]\n");
            }
        }
    }
    
    return 0;
}