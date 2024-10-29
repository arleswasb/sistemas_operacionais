#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define MAX_LINE 80 /* The maximum length command */

int main(void) {
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */
    char last_command[MAX_LINE] = ""; /* String para armazenar o �ltimo comando executado */

    while (should_run) {
        printf("osh> "); // Prompt de comando
        fflush(stdout);  						// Limpa o buffer de sa�da para garantir que o prompt seja exibido

        char input[MAX_LINE]; // String para armazenar a entrada do usu�rio
        if (fgets(input, MAX_LINE, stdin) == NULL) { 			// L� a entrada do usu�rio e verifica se h� erros
            perror("Error reading input");
            continue; // Volta ao loop principal em caso de erro
        }

        input[strcspn(input, "\n")] = 0; 				// Remove o caractere de nova linha da entrada (adicionado por fgets)

        // Verifica se o comando � "!!", para repetir o �ltimo comando
        if (strcmp(input, "!!") == 0) {
            if (strlen(last_command) == 0) { 				// Verifica se h� algum comando no hist�rico
                printf("Sem comandos no historico.\n"); 		// Mensagem de erro se n�o houver hist�rico
                continue; 						// Volta ao in�cio do loop sem executar nada
            } else {
                printf("%s\n", last_command); 				// Exibe o �ltimo comando na tela (como se o usu�rio tivesse digitado)
                strcpy(input, last_command); 				// Copia o �ltimo comando para a string `input` para ser executado novamente
            }
        } else {
            strcpy(last_command, input); 				// Salva o comando atual como o �ltimo comando executado
        }

        // Verifica se o comando � "exit" para encerrar o shell
        if (strcmp(input, "exit") == 0) {
            should_run = 0; 						// Define a flag para terminar o loop
            continue; 							// Sai do loop principal
        }

        // Quebra a linha de comando em tokens/argumentos
        int i = 0;
        args[i] = strtok(input, " "); 					// Usa strtok para separar o primeiro token (argumento)
        while (args[i] != NULL) { 					// Continua a separar os tokens at� o final da string
            i++;
            args[i] = strtok(NULL, " ");
        }

        // Verifica se o �ltimo argumento � "&" (indicando execu��o em segundo plano)
        int run_in_background = 0; 					// Flag para execu��o em segundo plano
        if (i > 0 && strcmp(args[i - 1], "&") == 0) { 			// Se o �ltimo argumento for "&"
            run_in_background = 1; 					// Marca para rodar o comando em segundo plano
            args[i - 1] = NULL; 					// Remove o "&" da lista de argumentos para evitar problemas com execvp
        }

        // Cria um processo filho para executar o comando
        pid_t pid = fork(); // Cria o processo filho
        if (pid < 0) { 							// Se fork retornar um valor negativo, houve erro ao criar o processo filho
            perror("Fork failed");
            return 1; 							// Retorna erro e encerra o programa
        } else if (pid == 0) { // C�digo do processo filho
            if (execvp(args[0], args) == -1) { 				// Executa o comando digitado
                perror("Error executing command"); 			// Se houver erro na execu��o, exibe mensagem
            }
            return 1; // Sai do processo filho
        } else { // C�digo do processo pai
            if (!run_in_background) { 					// Se o comando n�o estiver em segundo plano
                wait(NULL); 						// O processo pai espera o processo filho terminar
            } else {
                printf("[Processo em segundo plano iniciado]\n"); 	// Mensagem de que o processo est� em segundo plano
            }
        }
    }

    return 0; // Sai do programa
}