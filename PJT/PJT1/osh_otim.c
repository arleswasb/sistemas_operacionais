#include <stdio.h>                                                          // Inclui a biblioteca para operações de entrada e saída
#include <unistd.h>                                                         // Inclui funções de sistema como fork() e execvp()
#include <string.h>                                                         // Inclui funções para manipulação de strings, como strtok()
#include <sys/wait.h>                                                       // Inclui funções para gerenciar processos (ex: wait())
#include <fcntl.h>                                                          // Inclui funções para manipulação de arquivos, como open()

#define MAX_LINE 80                                                         // Define o tamanho máximo da linha de comando

// Função para dividir a linha de comando em tokens usando delimitadores


void tokenizar(char *input, char *args[], const char *delim) {
    int i = 0;
    args[i] = strtok(input, delim);                                         // Divide a string 'input' pelo delimitador e armazena o primeiro token em args[0]
    while (args[i] != NULL) {                                               // Continua a dividir enquanto houver tokens
        i++;
        args[i] = strtok(NULL, delim);                                      // Divide o próximo token
    }
}

// Função para lidar com redirecionamento de entrada e saída

int redirecionamento_manual(char *args[], int *input_fd, int *output_fd) {
    for (int j = 0; args[j] != NULL; j++) {                                             // Percorre os tokens para verificar redirecionamento
        if (strcmp(args[j], ">") == 0 && args[j + 1] != NULL) {                         // Se encontrar ">", redireciona a saída
            *output_fd = open(args[j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);         // Abre o arquivo para escrita
            args[j] = NULL;                                                             // Remove ">" dos argumentos
        } else if (strcmp(args[j], "<") == 0 && args[j + 1] != NULL) {                  // Se encontrar "<", redireciona a entrada
            *input_fd = open(args[j + 1], O_RDONLY);                                    // Abre o arquivo para leitura
            args[j] = NULL;                                                             // Remove "<" dos argumentos
        }
    }
    return (*input_fd < 0 || *output_fd < 0) ? -1 : 0;                                  // Retorna erro se algum arquivo não abrir com descritor ternario
}

// Função para executar um comando, com redirecionamento e execução em segundo plano


void execute_command(char *args[], int input_fd, int output_fd, int run_in_background) {
    pid_t pid = fork();                                                                 // Cria um novo processo
    if (pid == 0) {                                                                     // Código executado pelo processo filho
        if (input_fd >= 0) dup2(input_fd, STDIN_FILENO);                                // Redireciona a entrada, se necessário
        if (output_fd >= 0) dup2(output_fd, STDOUT_FILENO);                             // Redireciona a saída, se necessário
        execvp(args[0], args);                                                          // Executa o comando especificado em args
        perror("Error executing command");                                              // Exibe erro caso execvp falhe
        _exit(1);                                                                       // Termina o processo filho em caso de erro
    } else if (!run_in_background) {                                                    // Código executado pelo processo pai
        waitpid(pid, NULL, 0);                                                          // Espera o processo filho terminar, se não estiver em segundo plano
    } else {
        printf("[Processo em segundo plano iniciado]\n"); // Indica que o processo está em segundo plano
    }
}

int main(void) {
    char *args[MAX_LINE / 2 + 1];                      // Array para armazenar os argumentos do comando
    int should_run = 1;                                // Flag para manter o shell em execução
    char last_command[MAX_LINE] = "";                  // Armazena o último comando digitado
    int pipe_index = -1, input_fd = -1, output_fd = -1, run_in_background = 0;                // Inicializa variáveis

    while (should_run) {                               // Loop principal do shell
        printf("osh> ");                               // Prompt de comando
        fflush(stdout);                                // Garante que o prompt seja exibido

        char input[MAX_LINE];                          // String para armazenar a entrada do usuário
        if (fgets(input, MAX_LINE, stdin) == NULL) {   // Lê a entrada do usuário e verifica erros
            perror("Error reading input");
            continue;                                  // Reinicia o loop em caso de erro
        }
        input[strcspn(input, "\n")] = 0;               // Remove o caractere de nova linha da entrada

        if (strcmp(input, "!!") == 0) {                // Comando especial "!!" para repetir o último comando
            if (strlen(last_command) == 0) {           // Verifica se há um comando no histórico
                printf("Sem comandos no historico.\n");
                continue;
            }
            strcpy(input, last_command);               // Copia o último comando para 'input'
            printf("%s\n", last_command);              // Exibe o último comando na tela
        } else {
            strcpy(last_command, input);               // Armazena o comando atual no histórico
        }

        if (strcmp(input, "exit") == 0) break;         // Sai do shell se o comando for "exit"

        tokenizar(input, args, " ");                    // Divide a entrada em tokens usando espaços como delimitadores

        


        for (int j = 0; args[j] != NULL; j++) {        // Verifica cada token
            if (strcmp(args[j], "|") == 0) {           // Identifica o operador de pipe "|"
                pipe_index = j;                        // Armazena o índice do pipe
                args[j] = NULL;                        // Remove o "|" da lista de argumentos
            }
            if (strcmp(args[j], "&") == 0 && args[j + 1] == NULL) { // Verifica execução em segundo plano
                run_in_background = 1;                 // Marca para execução em segundo plano
                args[j] = NULL;                        // Remove "&" dos argumentos
            }
        }

        if (redirecionamento_manual(args, &input_fd, &output_fd) < 0) { // Trata redirecionamento de entrada e saída
            perror("Erro ao abrir arquivo");
            continue;
        }

        if (pipe_index != -1) {                        // Se houver um pipe, trata o comando com dois processos
            int pipefd[2];
            if (pipe(pipefd) == -1) {                  // Cria um pipe para comunicação entre processos
                perror("Erro ao criar pipe");
                continue;
            }

            pid_t pid1 = fork();                       // Cria o primeiro processo
            if (pid1 == 0) {                           // Código do primeiro processo filho
                dup2(pipefd[1], STDOUT_FILENO);        // Redireciona a saída para o pipe
                close(pipefd[0]);                      // Fecha o lado de leitura do pipe
                close(pipefd[1]);                      // Fecha o lado de escrita do pipe após redirecionar
                execvp(args[0], args);                 // Executa o primeiro comando
                perror("Error executing command");     // Exibe erro se execvp falhar
                _exit(1);                              // Termina o processo filho em caso de erro
            }

            pid_t pid2 = fork();                       // Cria o segundo processo
            if (pid2 == 0) {                           // Código do segundo processo filho
                dup2(pipefd[0], STDIN_FILENO);         // Redireciona a entrada para o pipe
                close(pipefd[0]);                      // Fecha o lado de leitura do pipe
                close(pipefd[1]);                      // Fecha o lado de escrita do pipe após redirecionar
                execvp(args[pipe_index + 1], &args[pipe_index + 1]); // Executa o segundo comando
                perror("Error executing command");     // Exibe erro se execvp falhar
                _exit(1);                              // Termina o processo filho em caso de erro
            }

            close(pipefd[0]);                          // Fecha o lado de leitura do pipe no processo pai
            close(pipefd[1]);                          // Fecha o lado de escrita do pipe no processo pai
            if (!run_in_background) {                  // Se não for segundo plano, espera ambos terminarem
                waitpid(pid1, NULL, 0);
                waitpid(pid2, NULL, 0);
            }
        } else {
            execute_command(args, input_fd, output_fd, run_in_background); // Executa o comando sem pipe
        }
    }
