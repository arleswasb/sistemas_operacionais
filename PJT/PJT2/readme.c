## Compilação e Carregamento

1. **Compilar o módulo**:
    ```bash
    make
    ```

2. **Carregar o módulo**:
    ```bash
    sudo insmod pid2.ko
    ```

3. **Verificar se o módulo está carregado**:
    ```bash
    dmesg 
    ```
   Se o módulo estiver carregado corretamente, você verá /proc/pid Criado como um modulo carregado.

## Testes

1. **Escrever um valor no /proc/pid**:
    Para escrever um valor de PID, use o comando echo. Substitua "1395" pelo valor desejado:

    ```bash
    echo "1395" > /proc/pid
    ```

2. **Ler o valor formatado no `/proc/pid`**:

    Para ler o valor, use o comando `cat`:

    ```bash
    cat /proc/pid
    ```
    A saída deverá ser algo como:
    ```plaintext
    command = [bash] pid = [1395] state = [1]
    ```

3. **Repetir com outro valor de PID**:
    Para testar com outro valor, repita o processo de escrita e leitura:
    ```bash
    echo "2567" > /proc/pid
    cat /proc/pid
    ```
    Saída esperada:
    ---
    command = [bash] pid = [2567] state = [1]
    ```

## Remoção do Módulo

Para remover o módulo do kernel, use o comando:
```bash
sudo rmmod pid2

**Verificar se o módulo foi descarregado**:
    ```bash
    dmesg 
    ```
   Se o módulo estiver carregado corretamente, você verá /proc/pid Removido como um modulo carregado.


