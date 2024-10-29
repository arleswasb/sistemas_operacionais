#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>

#define MAX_LINE 80 /* The maximum length command */
#define HIST_SIZE 20 /* Number of commands to store in history */

int main(void) {
    char *args[MAX_LINE / 2 + 1]; /* Command line arguments */
    char *history[HIST_SIZE]; /* Command history */
    int should_run = 1; /* Flag to determine when to exit program */
    int hist_index = 0; /* Index for command history */

    while (should_run) {
        printf("osh> ");
        fflush(stdout);

        // Lê a entrada do usuário
        char input[MAX_LINE];
        if (!fgets(input, MAX_LINE, stdin)) {
            break; // Saia se houver erro na leitura
        }

        // Remove a nova linha da entrada
        input[strcspn(input, "\n")] = 0;

        // Verifica se o usuário digitou "exit"
        if (strcmp(input, "exit") == 0) {
            should_run = 0; // Sai do loop
            continue;
        }

        // Adiciona o comando à história
        if (hist_index < HIST_SIZE) {
            history[hist_index++] = strdup(input); // Duplica a string para armazená-la
        } else {
            // Se o histórico estiver cheio, substitui o mais antigo
            free(history[0]); // Libera o mais antigo
            memmove(history, history + 1, (HIST_SIZE - 1) * sizeof(char *)); // Move os comandos
            history[HIST_SIZE - 1] = strdup(input); // Adiciona o novo comando
        }

        // Divide a entrada em argumentos e verifica por redirecionamentos
        char *input_file = NULL;
        char *output_file = NULL;
        int pipe_fd[2]; // Pipe para comunicação entre processos
        int has_pipe = 0; // Flag para verificar se há pipe

        char *token = strtok(input, " ");
        int i = 0;
        while (token != NULL) {
            // Verifica por redirecionamento de saída
            if (strcmp(token, ">") == 0) {
                output_file = strtok(NULL, " "); // Próximo token é o arquivo de saída
                if (output_file == NULL) {
                    fprintf(stderr, "Error: No output file specified after >\n");
                    break;
                }
            }
            // Verifica por redirecionamento de entrada
            else if (strcmp(token, "<") == 0) {
                input_file = strtok(NULL, " "); // Próximo token é o arquivo de entrada
                if (input_file == NULL) {
                    fprintf(stderr, "Error: No input file specified after <\n");
                    break;
                }
            }
            // Verifica por pipe
            else if (strcmp(token, "|") == 0) {
                has_pipe = 1; // Define flag para pipe
                break; // Para o loop, já que o comando atual terminou
            }
            else {
                args[i++] = token; // Adiciona o token aos argumentos
            }
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Termina a lista de argumentos

        // Cria um novo processo para o comando
        pid_t pid = fork();

        if (pid == 0) { // Processo filho
            // Se houver um pipe, cria o pipe
            if (has_pipe) {
                pipe(pipe_fd);
                pid_t pid2 = fork();

                if (pid2 == 0) { // Segundo filho
                    // Redireciona a saída padrão para o pipe
                    dup2(pipe_fd[1], STDOUT_FILENO);
                    close(pipe_fd[0]); // Fecha o lado de leitura do pipe
                    execvp(args[0], args);
                    perror("execvp failed");
                    exit(1);
                } else {
                    // Redireciona a entrada padrão do pipe
                    dup2(pipe_fd[0], STDIN_FILENO);
                    close(pipe_fd[1]); // Fecha o lado de escrita do pipe
                    execvp(strtok(NULL, " "), args + i + 1); // Executa o próximo comando
                    perror("execvp failed");
                    exit(1);
                }
            }

            // Redirecionamento de entrada
            if (input_file != NULL) {
                int fd = open(input_file, O_RDONLY);
                if (fd < 0) {
                    perror("Error opening input file");
                    exit(1);
                }
                dup2(fd, STDIN_FILENO); // Redireciona a entrada padrão
                close(fd);
            }

            // Redirecionamento de saída
            if (output_file != NULL) {
                int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("Error opening output file");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO); // Redireciona a saída padrão
                close(fd);
            }

            // Executa o comando
            execvp(args[0], args);
            perror("execvp failed"); // Se execvp falhar
            exit(1); // Sai do processo filho
        } else if (pid > 0) { // Processo pai
            // Espera o processo filho terminar
            wait(NULL);
        } else {
            perror("fork failed"); // Se fork falhar
        }

        // Exibe o histórico de comandos
        printf("\nCommand History:\n");
        for (int j = 0; j < hist_index; j++) {
            printf("%d: %s\n", j + 1, history[j]);
        }
    }

    // Libera a memória do histórico
    for (int j = 0; j < hist_index; j++) {
        free(history[j]);
    }

    return 0;
}