# FormiguinhaZ

Bem vindo ao **FormiguinhaZ<sup><bold>TM</bold></sup>**, um simulador de formigas
altamente paralelizado!

## Requisitos

Para conseguir compilar o código fonte, é necessária a instalação de duas
bibliotecas:

* JsonCpp - utilizada para fazer o parsing dos arquivos de configuração;
* SDL - utilizada para construir a interface gráfica da simulação.

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

Com isso, será aberta uma janela gráfica onde a simulação será exibida.
No terminal, aparecerá um log com as principais estatísticas referentes à
simulação, como o número de formigas de cada colônia e a quantidade de
comida disponível em cada fonte.

## Customização

Caso deseje rodar a simulação com um outro conjunto de configurações,
basta copiar, dentro do diretório `configs`, um dos arquivos existentes
e renomeá-lo para `config_n.json`, onde `n` é um número inteiro positivo
à sua escolha. Então, edite o arquivo alterando os valores desejados e
execute `main` com `n` como argumento.


### Parâmetros configuráveis da simulação

#### Parâmetros Globais

* `nThreads`: Número de threads, além da principal que devem ser utilizadas
   para rodar a simulação. Inteiro maior que 0.
* `maxIteration`: Número de iterações que a simulação deve rodar antes de
  fechar. Inteiro maior que 0.
* `squareSize`: Largura de cada *Tile*, em pixels. Inteiro maior que 0.
* `height`: Altura do mapa, em *Tiles*. Inteiro maior que 0.
* `width`: Largura do mapa, em *Tiles*. Inteiro maior que 0.

**AVISO:** Observe que os parâmetros `squareSize`, `height`, `width` devem
ser pensados levando em consideração o tamanho da representação gráfica
das formigas e a resolução da tela do dispositivo em que você está.
Não recomendamos abaixar o `squareSize` para menos de 50, devido ao
tamanho das formigas. Os valores padrões de `height = 21` e `width = 38`
foram, então, pensados para preencher a maior área possível de um monitor
com resolução Full HD, ou seja, `1920 X 1080`.

* `vision`: Campo de visão de cada formiga. Cada formiga consegue ver um
  retângulo de largura `2*vision` e altura `vision` à sua frente. Inteiro
maior que 0.
* `pheroLifetime`: Vida de um único feromônio, medida em iterações.
  Inteiro maior que 0.
* `tickDuration`: Tempo de espera entre iterações, em milisegundos.
  Inteiro maior que 0.

#### Parâmetros para Formigueiros (`anthills`)

* `x`: Coordenada x do formigueiro. Medida da esquerda para a direta, a
  partir de 0. Inteiro maior que 0 e menor que `width`.
* `y`: Coordenada y do formigueiro. Medida de cima para baixo, a
  partir de 0. Inteiro maior que 0 e menor que `height`.
* `nAnts`: Número de formigas que habitam o formigueiro. Inteiro maior ou
  igual a 0.

#### Parâmetros para as Fontes de Comida (`Foods`)

* `x`: Coordenada x da `Food`. Medida da esquerda para a direta, a
  partir de 0. Inteiro maior que 0 e menor que `width`.
* `y`: Coordenada y da `Food`. Medida de cima para baixo, a
  partir de 0. Inteiro maior que 0 e menor que `height`.
* `initialQuantity`: Quantidade inicial de comida disponível. Inteiro
  maior que 0.
* `refillTime`: Tempo entre cada reabastecimento da comida, medido em
  iterações. Inteiro maior que 0.
* `refillQuantity`: Quantidade de comida que deve ser reposta a cada
  reabastecimento. Inteiro maior que 0.
* `numSticks`: Número de bastões disponíveis na fonte de comida para as
  formigas poderem coletá-la. Inteiro maior que 0.

