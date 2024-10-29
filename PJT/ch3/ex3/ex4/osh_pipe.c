#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h> // Para manipula��o de arquivos
#define MAX_LINE 80 /* O comprimento m�ximo do comando */

int main(void) {
    char *args[MAX_LINE / 2 + 1]; /* Argumentos da linha de comando */
    int should_run = 1; /* Flag para determinar quando sair do programa */
    char last_command[MAX_LINE] = ""; /* String para armazenar o �ltimo comando executado */

    while (should_run) {
        printf("osh> "); // Prompt de comando
        fflush(stdout); // Limpa o buffer de sa�da para garantir que o prompt seja exibido

        char input[MAX_LINE]; // String para armazenar a entrada do usu�rio
        if (fgets(input, MAX_LINE, stdin) == NULL) { // L� a entrada do usu�rio e verifica se h� erros
            perror("Error reading input");
            continue; // Volta ao loop principal em caso de erro
        }

        input[strcspn(input, "\n")] = 0; // Remove o caractere de nova linha da entrada

        // Verifica se o comando � "!!" para repetir o �ltimo comando
        if (strcmp(input, "!!") == 0) {
            if (strlen(last_command) == 0) { // Verifica se h� algum comando no hist�rico
                printf("Sem comandos no historico.\n"); // Mensagem de erro se n�o houver hist�rico
                continue; // Volta ao in�cio do loop sem executar nada
            } else {
                printf("%s\n", last_command); // Exibe o �ltimo comando na tela
                strcpy(input, last_command); // Copia o �ltimo comando para a string `input` para ser executado novamente
            }
        } else {
            strcpy(last_command, input); // Salva o comando atual como o �ltimo comando executado
        }

        // Verifica se o comando � "exit" para encerrar o shell
        if (strcmp(input, "exit") == 0) {
            should_run = 0; // Define a flag para terminar o loop
            continue; // Sai do loop principal
        }

        // Quebra a linha de comando em tokens/argumentos
        int i = 0;
        args[i] = strtok(input, " "); // Usa strtok para separar o primeiro token (argumento)
        while (args[i] != NULL) { // Continua a separar os tokens at� o final da string
            i++;
            args[i] = strtok(NULL, " ");
        }

        // Verifica se h� um pipe "|"
        int pipe_index = -1;
        for (int j = 0; j < i; j++) {
            if (args[j] != NULL && strcmp(args[j], "|") == 0) {
                pipe_index = j; // Armazena o �ndice do pipe
                args[j] = NULL; // Remove o pipe da lista de argumentos
                break;
            }
        }

        // Verifica se h� redirecionamento de sa�da ">"
        int output_redirected = 0;
        int output_fd = -1;
        for (int j = 0; j < i; j++) {
            if (args[j] != NULL && strcmp(args[j], ">") == 0) {
                if (args[j + 1] != NULL) {
                    output_fd = open(args[j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (output_fd < 0) {
                        perror("Erro ao abrir arquivo para redirecionamento de sa�da");
                        output_redirected = 0; // Se n�o abrir, reseta
                    } else {
                        output_redirected = 1; // Se abriu com sucesso
                        args[j] = NULL; // Remove ">" da lista de argumentos
                    }
                }
                break;
            }
        }

        // Verifica se h� redirecionamento de entrada "<"
        int input_redirected = 0;
        int input_fd = -1;
        for (int j = 0; j < i; j++) {
            if (args[j] != NULL && strcmp(args[j], "<") == 0) {
                if (args[j + 1] != NULL) {
                    input_fd = open(args[j + 1], O_RDONLY);
                    if (input_fd < 0) {
                        perror("Erro ao abrir arquivo para redirecionamento de entrada");
                        input_redirected = 0; // Se n�o abrir, reseta
                    } else {
                        input_redirected = 1; // Se abriu com sucesso
                        args[j] = NULL; // Remove "<" da lista de argumentos
                    }
                }
                break;
            }
        }

        // Verifica se o �ltimo argumento � "&" (indicando execu��o em segundo plano)
        int run_in_background = 0; // Flag para execu��o em segundo plano
        if (i > 0 && strcmp(args[i - 1], "&") == 0) { // Se o �ltimo argumento for "&"
            run_in_background = 1; // Marca para rodar o comando em segundo plano
            args[i - 1] = NULL; // Remove o "&" da lista de argumentos para evitar problemas com execvp
        }

        // Se houver um pipe, criamos dois processos
        if (pipe_index != -1) {
            int pipefd[2]; // Cria um array para os descritores do pipe
            if (pipe(pipefd) == -1) { // Cria o pipe
                perror("Erro ao criar pipe");
                continue; // Se falhar, continua o loop
            }

            // Cria o primeiro processo (antes do pipe)
            pid_t pid1 = fork();
            if (pid1 < 0) {
                perror("Fork failed");
                return 1;
            } else if (pid1 == 0) { // C�digo do primeiro filho
                // Redireciona a sa�da para o pipe
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]); // Fecha o lado de leitura do pipe
                close(pipefd[1]); // Fecha o lado de escrita do pipe
                if (execvp(args[0], args) == -1) {
                    perror("Error executing command");
                }
                return 1;
            }

            // Cria o segundo processo (depois do pipe)
            pid_t pid2 = fork();
            if (pid2 < 0) {
                perror("Fork failed");
                return 1;
            } else if (pid2 == 0) { // C�digo do segundo filho
                // Redireciona a entrada para o pipe
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]); // Fecha o lado de leitura do pipe
                close(pipefd[1]); // Fecha o lado de escrita do pipe
                if (execvp(args[pipe_index + 1], &args[pipe_index + 1]) == -1) {
                    perror("Error executing command");
                }
                return 1;
            }

            // C�digo do processo pai
            close(pipefd[0]); // Fecha o lado de leitura do pipe
            close(pipefd[1]); // Fecha o lado de escrita do pipe
            if (!run_in_background) { // Espera ambos os filhos terminarem
                waitpid(pid1, NULL, 0);
                waitpid(pid2, NULL, 0);
            } else {
                printf("[Processo em segundo plano iniciado]\n"); // Mensagem de que o processo est� em segundo plano
            }

        } else { // Sem pipe, executa normalmente
            // Cria um processo filho para executar o comando
            pid_t pid = fork(); // Cria o processo filho
            if (pid < 0) { // Se fork retornar um valor negativo, houve erro ao criar o processo filho
                perror("Fork failed");
                return 1; // Retorna erro e encerra o programa
            } else if (pid == 0) { // C�digo do processo filho
                // Redirecionamento de sa�da
                if (output_redirected) {
                    dup2(output_fd, STDOUT_FILENO); // Duplica o fd para STDOUT
                    close(output_fd); // Fecha o fd original
                }

                // Redirecionamento de entrada
                if (input_redirected) {
                    dup2(input_fd, STDIN_FILENO); // Duplica o fd para STDIN
                    close(input_fd); // Fecha o fd original
                }

                // Executa o comando digitado
                if (execvp(args[0], args) == -1) { // Tenta executar o comando
                    perror("Error executing command"); // Se houver erro na execu��o, exibe mensagem
                }
                return 1; // Sai do processo filho
            } else { // C�digo do processo pai
                if (!run_in_background) { // Se o comando n�o estiver em segundo plano
                    wait(NULL); // O processo pai espera o processo filho terminar
                } else {
                    printf("[Processo em segundo plano iniciado]\n"); // Mensagem de que o processo est� em segundo plano
                }
            }
        }
    }

    return 0; // Sai do programa
}