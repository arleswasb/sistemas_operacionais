#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define MAX_LINE 80 /* The maximum length command */

int main(void) {
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */

    while (should_run) {
        printf("osh> ");
        fflush(stdout);

        // Lê a entrada do usuário
        char input[MAX_LINE];
        fgets(input, MAX_LINE, stdin);

        // Remove a nova linha da entrada
        input[strcspn(input, "\n")] = 0;

        // Verifica se o usuário digitou "exit"
        if (strcmp(input, "exit") == 0) {
            should_run = 0; // Sai do loop
            continue;
        }

        // Divide a entrada em argumentos
        char *token = strtok(input, " ");
        int i = 0;
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Termina a lista de argumentos

        // Cria um novo processo
        pid_t pid = fork();
        if (pid == 0) { // Processo filho
            // Executa o comando
            execvp(args[0], args);
            // Se execvp falhar, imprime um erro
            perror("execvp failed");
            exit(1); // Sai do processo filho
        } else if (pid > 0) { // Processo pai
            // Espera o processo filho terminar
            wait(NULL);
        } else {
            perror("fork failed"); // Se fork falhar
        }
    }
    return 0;
}