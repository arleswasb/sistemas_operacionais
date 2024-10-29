/*
linha de comando para criar o executavel osh

gcc -o meu_osh osh_basic.c

teste de funcionalidade normal e com & 


Primeiro, execute um comando sem & para verificar o comportamento padrão, onde o shell 

espera o término do processo antes de permitir que novos comandos sejam inseridos.

Por exemplo, o comando abaixo exibirá o conteúdo do diretório e esperará até a conclusão:


 meu_osh> ls -l

O prompt (meu_osh>) só aparecerá novamente depois que o comando ls -l for concluído.

3. Testar execução em segundo plano (com &)

Agora, teste a funcionalidade de execução em segundo plano, onde o shell não espera que o processo termine para aceitar novos comandos.

Tente executar o seguinte comando com & no final:

osh> sleep 5 &

O comando sleep 5 faz o processo "dormir" por 5 segundos antes de terminar, mas como você colocou o &, ele será executado em segundo plano.

Comportamento esperado:

Assim que você pressionar Enter, o shell não esperará os 5 segundos e imediatamente mostrará o prompt osh> para que você possa inserir outro comando.
Enquanto isso, o comando sleep será executado em segundo plano.



*/