# FormiguinhaZ

Bem vindo ao *FormiguinhaZ<sup>TM</sup>*, um simulador de formigas
altamente paralelizado!

## Requisitos

Para conseguir compilar o código fonte, é necessária a instalação de duas
bibliotecas:

* JsonCpp -- utilizada para fazer o parsing dos arquivos de configuração;
* SDL -- utilizada para construir a interface gráfica da simulação.

Ambas estão disponíveis nos repositórios padrão do Ubuntu e podem ser
instaladas com os seguintes comandos:

```sh
    sudo apt install libjsoncpp-dev
    sudo apt install libsdl2-dev
```

## Compilação e execução

Pelo terminal, navegue até o diretório onde este `README` se encontra e
digite o comando

```sh
    make
```

Após o término da compilação, você terá, neste repositório, o executável
denominado `main`.

Observe, agora, os arquivos no diretório `./configs`.
Eles são exemplos de configurações possíveis para a simulação. Escolha um
e execute `main` com o número da config como argumento. Por exemplo, se
quero rodar a simulação com a configuração do arquivo `config_1.json`,
devo rodar o comando

```sh
    ./main 1
```

## Customização

Caso deseje rodar a simulação com um outro conjunto de configurações,
basta copiar um dos arquivos existentes e renomeá-lo para `config_`*n*`.json`,
onde *n* é um número inteiro positivo à sua escolha. Então, edite o
arquivo alterando os valores desejados e execute `main` com *n* como
argumento.


### Parâmetros configuráveis da simulação
