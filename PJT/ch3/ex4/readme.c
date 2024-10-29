/*
Testar Funcionalidades

a)Testar Comandos Simples
Comando Simples:

osh> ls

Isso deve listar os arquivos e diretórios no diretório atual.

Comando com Argumentos:

osh> ls -l

Isso deve listar os arquivos em formato detalhado.

b) Testar Redirecionamento de Saída

Redirecionamento de Saída:

osh> ls > output.txt


Isso deve criar (ou sobrescrever) um arquivo chamado output.txt contendo a saída do comando ls.
Verifique o arquivo:

cat output.txt

c) Testar Redirecionamento de Entrada

Redirecionamento de Entrada: Primeiro, crie um arquivo de teste:

echo "banana" > in.txt
Depois, teste o comando:


osh> sort < input.txt
Isso deve exibir "banana" (ou a saída ordenada do arquivo, se houver mais palavras).

d)Testar Execução em Segundo Plano

Execução em Segundo Plano:

osh> sleep 10 &

Isso deve iniciar o comando sleep em segundo plano. Você deve ver uma mensagem informando que o processo foi iniciado em segundo plano.
Verifique a execução em segundo plano: Você pode continuar a usar o shell enquanto o sleep está em execução.

e) Testar Pipes
Testar com Pipes:

osh> ls -l | less

Isso deve listar os arquivos e passar a saída para o comando less, permitindo que você role a lista.


f)Testar Comandos de Histórico

Repetir o Último Comando:

osh> !!
Isso deve repetir o último comando que você executou.
!!
Testar a Saída de Comandos Válidos: Se você executou um comando antes, deve vê-lo sendo executado novamente.

*/