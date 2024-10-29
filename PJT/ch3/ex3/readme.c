/*
Passo 1: Criar Arquivos de Teste no pronpt de comando osc

Crie um arquivo de entrada (por exemplo, in.txt):

echo -e "banana\nmaçã\nlaranja" > in.txt

Crie um arquivo para a saída (por exemplo, out.txt):

Você pode criar um arquivo vazio:

touch out.txt

Passo 3: Compilar o Código

gcc -o osh_in_out osh_in_out.c

Passo 4: Executar o Shell

Inicie o shell:

./osh_in_out

Passo 5: Realizar Testes

a) Testes de Redirecionamento de Saída

	Redirecionar a saída de um comando para um arquivo:

	Execute o seguinte comando dentro do shell:
	
ls > out.txt

Isso deve direcionar a saída do comando ls para out.txt.

Verifique o conteúdo de out.txt:


cat out.txt

O conteúdo do diretório atual deve ser exibido.

b)Testes de Redirecionamento de Entrada

Usar um arquivo como entrada:

Execute o seguinte comando dentro do shell:

sort < in.txt

O comando sort deve ler os dados de in.txt e exibir a lista classificada.

c) Testes Combinados

Executar um comando com redirecionamento de saída e em segundo plano:

Execute:

ls > out.txt &

Verifique a saída do shell para garantir que o processo em segundo plano foi iniciado.

Verifique o conteúdo do arquivo novamente:


cat out.txt

d) Testes de Comandos Recorrentes

Executar um comando novamente usando !!:

Execute:

echo "Hello World"

Agora, digite !! para repetir o último comando.

Verifique se a mensagem "Hello World" é exibida novamente.

e) Verificar Erros

Testar redirecionamentos com arquivos que não existem:

Tente executar:

ls > none/out.txt

Verifique se a mensagem de erro apropriada é exibida.

Testar redirecionamento de entrada com um arquivo que não existe:

Tente executar:

sort < none.txt
./
Novamente, verifique se a mensagem de erro correta é exibida.

*/