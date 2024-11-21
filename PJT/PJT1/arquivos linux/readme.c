/*
Testar Funcionalidades

a)Testar Comandos Simples
Comando Simples:

osh> ls

Isso deve listar os arquivos e diret�rios no diret�rio atual.

Comando com Argumentos:

osh> ls -l

Isso deve listar os arquivos em formato detalhado.

b) Testar Redirecionamento de Sa�da

Redirecionamento de Sa�da:

osh> ls > output.txt


Isso deve criar (ou sobrescrever) um arquivo chamado output.txt contendo a sa�da do comando ls.
Verifique o arquivo:

cat output.txt

c) Testar Redirecionamento de Entrada

Redirecionamento de Entrada: Primeiro, crie um arquivo de teste:

echo "banana" > in.txt
Depois, teste o comando:


osh> sort < input.txt
Isso deve exibir "banana" (ou a sa�da ordenada do arquivo, se houver mais palavras).

d)Testar Execu��o em Segundo Plano

Execu��o em Segundo Plano:

osh> sleep 10 &

Isso deve iniciar o comando sleep em segundo plano. Voc� deve ver uma mensagem informando que o processo foi iniciado em segundo plano.
Verifique a execu��o em segundo plano: Voc� pode continuar a usar o shell enquanto o sleep est� em execu��o.

e) Testar Pipes
Testar com Pipes:

osh> ls -l | less

Isso deve listar os arquivos e passar a sa�da para o comando less, permitindo que voc� role a lista.


f)Testar Comandos de Hist�rico

Repetir o �ltimo Comando:

osh> !!
Isso deve repetir o �ltimo comando que voc� executou.
!!
Testar a Sa�da de Comandos V�lidos: Se voc� executou um comando antes, deve v�-lo sendo executado novamente.

*/