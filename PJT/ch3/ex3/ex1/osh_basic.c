#include <stdio.h>      // Biblioteca padr�o de entrada e sa�da (usada para printf, fgets, etc.)
#include <unistd.h>     // Biblioteca que inclui fun��es para cria��o e manipula��o de processos, como fork e execvp
#include <string.h>     // Biblioteca para manipula��o de strings (usada para strtok, strcmp, etc.)
#include <sys/wait.h>   // Biblioteca para fun��es relacionadas � espera de processos, como wait

#define MAX_LINE 80     // Define o tamanho m�ximo da linha de comando (80 caracteres)

int main(void) {
    char *args[MAX_LINE / 2 + 1]; 	// Array de ponteiros para armazenar os argumentos do comando
    int should_run = 1; 			// Flag que determina quando sair do programa (1 para continuar, 0 para parar)

    while (should_run) { 			// Loop principal do shell
        printf("osh> "); 			// Exibe o prompt "osh>"
        fflush(stdout);  			// Garante que o prompt seja exibido imediatamente, limpando o buffer de sa�da

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

        char input[MAX_LINE];                                       // String para armazenar a entrada do usu�rio
        if (fgets(input, MAX_LINE, stdin) == NULL) { 				// L� a entrada do usu�rio e verifica erros
            perror("Error reading input"); 					        // Imprime erro caso a leitura falhe
            continue; 								                // Volta ao in�cio do loop
        }

        input[strcspn(input, "\n")] = 0; 					// Remove o caractere de nova linha (\n) que o fgets adiciona no final da string

        if (strcmp(input, "exit") == 0) {					 // Verifica se o comando digitado foi "exit"
            should_run = 0;                         // Se for "exit", define should_run como 0 para encerrar o loop
            continue; 								// Sai do loop principal
        }

        									                // Quebra a linha de comando em tokens/argumentos
        int i = 0;
        args[i] = strtok(input, " "); 						// Usa strtok para pegar o primeiro token (separado por espa�o)
        while (args[i] != NULL) { 						// Continua a pegar tokens at� o final da string
            i++;
            args[i] = strtok(NULL, " "); 					// Pega os tokens subsequentes
        }

        // Verifica se o �ltimo argumento � "&" (indicando execu��o em segundo plano)

        int run_in_background = 0; 						// Flag para execu��o em segundo plano
        if (i > 0 && strcmp(args[i - 1], "&") == 0) { 				// Se o �ltimo argumento for "&"
            run_in_background = 1; 						// Marca para rodar em segundo plano
            args[i - 1] = NULL; 						// Remove o "&" da lista de argumentos
        }

        // Cria um processo filho

        pid_t pid = fork(); 							// fork cria um novo processo (retorna 0 no processo filho, e o PID do filho no processo pai)
        if (pid < 0) {								    // Se fork retornar um valor negativo, houve erro na cria��o do processo filho
            perror("Fork failed");                      // Exibe a mensagem de erro
            return 1;                                   // Retorna 1 para indicar falha
        } else if (pid == 0) {                          // Processo filho
            									                // No processo filho, executa o comando usando execvp
            if (execvp(args[0], args) == -1) { 					// execvp executa o comando que foi digitado
                perror("Error executing command"); 				// Se houver erro na execu��o do comando, imprime uma mensagem
            }
            return 1; 								// Sai do processo filho (evita continuar a execu��o do shell no processo filho)
        } else {                                    // Processo pai
            if (!run_in_background) { 				// Se N�O for para rodar em segundo plano (n�o tem "&")
                wait(NULL); 							// Espera o processo filho terminar antes de continuar
            } else { 								// Se for para rodar em segundo plano (tem "&")
                printf("[Processo em segundo plano iniciado]\n"); 		// Exibe uma mensagem informando que o processo est� rodando em segundo plano
            }
        }
    }

    return 0; // Retorna 0 ao final da execu��o do programa
}