/*
Passo 1: Criar Arquivos de Teste no pronpt de comando osc

Crie um arquivo de entrada (por exemplo, in.txt):

echo -e "banana\nma��\nlaranja" > in.txt

Crie um arquivo para a sa�da (por exemplo, out.txt):

Voc� pode criar um arquivo vazio:

touch out.txt

Passo 3: Compilar o C�digo

gcc -o osh_in_out osh_in_out.c

Passo 4: Executar o Shell

Inicie o shell:

./osh_in_out

Passo 5: Realizar Testes

a) Testes de Redirecionamento de Sa�da

	Redirecionar a sa�da de um comando para um arquivo:

	Execute o seguinte comando dentro do shell:
	
ls > out.txt

Isso deve direcionar a sa�da do comando ls para out.txt.

Verifique o conte�do de out.txt:


cat out.txt

O conte�do do diret�rio atual deve ser exibido.

b)Testes de Redirecionamento de Entrada

Usar um arquivo como entrada:

Execute o seguinte comando dentro do shell:

sort < in.txt

O comando sort deve ler os dados de in.txt e exibir a lista classificada.

c) Testes Combinados

Executar um comando com redirecionamento de sa�da e em segundo plano:

Execute:

ls > out.txt &

Verifique a sa�da do shell para garantir que o processo em segundo plano foi iniciado.

Verifique o conte�do do arquivo novamente:


cat out.txt

d) Testes de Comandos Recorrentes

Executar um comando novamente usando !!:

Execute:

echo "Hello World"

Agora, digite !! para repetir o �ltimo comando.

Verifique se a mensagem "Hello World" � exibida novamente.

e) Verificar Erros

Testar redirecionamentos com arquivos que n�o existem:

Tente executar:

ls > none/out.txt

Verifique se a mensagem de erro apropriada � exibida.

Testar redirecionamento de entrada com um arquivo que n�o existe:

Tente executar:

sort < none.txt
./
Novamente, verifique se a mensagem de erro correta � exibida.

*/