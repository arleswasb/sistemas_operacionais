/*
entrar no shell osh_pipe

./osh_pipe




Testar Funcionalidades

a)Testar Comandos Simples
Comando Simples:

osh> ls

Isso deve listar os arquivos e diretorios no diretorio atual.

Comando com Argumentos:

osh> ls -l

Isso deve listar os arquivos em formato detalhado.

b) Testar Redirecionamento de Saida

Redirecionamento de Saida:

osh> ls > output.txt


Isso deve criar (ou sobrescrever) um arquivo chamado output.txt contendo a saida do comando ls.
Verifique o arquivo:

cat output.txt

c) Testar Redirecionamento de Entrada

Redirecionamento de Entrada: Primeiro, crie um arquivo de teste:

echo "banana" > in.txt
Depois, teste o comando:


osh> sort < input.txt
Isso deve exibir "banana" (ou a saida ordenada do arquivo, se houver mais palavras).

d)Testar Execucao em Segundo Plano

Execucao em Segundo Plano:

osh> sleep 10 &

Isso deve iniciar o comando sleep em segundo plano. Voce deve ver uma mensagem informando que o processo foi iniciado em segundo plano.
Verifique a execucao em segundo plano: Voce pode continuar a usar o shell enquanto o sleep esta em execucao.

e) Testar Pipes
Testar com Pipes:

osh> ls -l | less

Isso deve listar os arquivos e passar a saida para o comando less, permitindo que voce role a lista.


f)Testar Comandos de Historico

Repetir o ultimo Comando:

osh> !!
Isso deve repetir o ultimo comando que voce executou.
!!
Testar a Saida de Comandos Validos: Se voce executou um comando antes, deve ve-lo sendo executado novamente.

*/