## Compilacao e Carregamento

1. **Compilar o modulo**:

    ```bash
    make
    ```

2. **Carregar o modulo**:
    ```bash
    sudo insmod pid_v1.ko
    ```

3. **Verificar se o modulo esta carregado**:
    ```bash
    dmesg 
    ```
   Se o modulo estiver carregado corretamente, voce vera "/proc/pid Criado" como um modulo carregado.

## Testes

1. **Escrever um valor no /proc/pid**:
    Para escrever um valor de PID, use o comando echo. Substitua "1395" pelo valor desejado:

    utilise o comando pgrep "" para verificar um numero de PID ativo no kernel



    ```bash
    echo "copie o numero aqui" > /proc/pid
    ```

2. **Ler o valor formatado no `/proc/pid`**:

    Para ler o valor, use o comando `cat`:

    ```bash
    cat /proc/pid
    ```
    A saida devera ser algo como:
    ```plaintext
    command = [bash] pid = [numero] state = [1]
    ```

3. **Repetir com outro valor de PID**:
    Para testar com outro valor, repita o processo de escrita e leitura:
    ```bash
    echo "outro numero" > /proc/pid
    cat /proc/pid
    ```
    Saida esperada:
    ---
    command = [bash] pid = [2567] state = [1]
    ```

## Remover do Modulo

Para remover o modulo do kernel, use o comando:
```bash
sudo rmmod pid_v1

**Verificar se o modulo foi descarregado**:
    ```bash
    dmesg 
    ```
   Se o modulo estiver carregado corretamente, voco vera "/proc/pid Removido" como um modulo carregado.


