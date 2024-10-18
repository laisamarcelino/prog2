# Trabalho Prático 1 - Estratégias de Comparação de Imagens

## Sobre
Este projeto tem como objetivo comparar imagens no formato PGM utilizando o método Local Binary Pattern (LBP) para extração de características. O programa processa imagens PGM nos formatos P2 (ASCII) e P5 (Binário), gera a transformação LBP, calcula histogramas e, por fim, compara imagens com base na distância euclidiana entre os histogramas resultantes.

## Funcionalidades do Projeto
* Compara uma imagem de teste com todas as imagens da base de referência
* Gera uma imagem LBP a partir de imagem imagem PGM

## Estrutura do Projeto
O projeto está estruturado nos seguintes arquivos:

### lbp.c

Arquivo principal do programa, onde a execução começa. Ele recebe os parâmetros via linha de comando e coordena a comparação ou escrita de imagens.

* **Função main:**
    Recebe argumentos que indicam o diretório de imagens e o nome da imagem de entrada.
    Chama as funções de comparação e geração de imagens.

### processa_imagem.h

É o arquivo de cabeçalho que contém as declarações das funções e da estrutura da imagem PGM utilizada no projeto.
Define a estrutura imagemPGM e todas as funções mencionadas no arquivo processa_imagem.c.

### processa_imagem.c

Contém as funções responsáveis por processar imagens no formato PGM e calcular LBP. Aqui são implementadas as funções que lidam com a leitura, escrita, transformação LBP, e manipulação de diretórios. São elas:

* **le_pgm:** Abre um arquivo de imagem PGM e realiza a leitura desses para formatos P2 e P5 de imagens. Retorna 1 caso haja algum erro.

* **calcula_lbp:** Realiza o cálculo do LBP (Local Binary Patterns).

* **escreve_imagem_lbp:** Escreve a imagem resultante do cálculo LBP em arquivo.

* **calcula_lbp:** Calcula o LBP de uma imagem. Para cada pixel, exceto os da borda, o LBP é calculado comparando o valor do pixel central com seus 8 vizinhos ao redor. Se o valor de um vizinho é maior ou igual ao valor do pixel central, um bit correspondente é definido como 1; caso contrário, é 0. Os bits são então combinados para formar um número de 8 bits que representa o padrão de textura local ao redor do pixel.O resultado é armazenado em img_lbp, uma nova imagem que contém o LBP de cada pixel da imagem original.

* **escreve_lbp:** Crie e escreve a img_lbp em um arquivo binário.

* **le_lbp:** Realiza a leitura de um arquivo LBP binário.

* **calcula_dist:** Calcula a distância Euclidiana entre dois histogramas.

* **manipula_dir:** Lê e processa todas as imagens de um diretório para gerar seus histogramas.

* **compara_imagens:** Compara a imagem de entrada com as imagens lbp e encontra a mais similar a partir da distancia de seus histogramas.

* **escreve_imagens:** Lê uma imagem de entrada do diretorio raiz, calcula o LBP dela e gera uma imagem LBP no diretorio raiz.

## Compilação e Execução

Para compilar o programa use o comando: `make`

Para comparar uma imagem de teste com as imagens da base de referência use: 
` ./LBP -d ./base -i img1.pgm `
O

Para gerar um imagem LBP use: 
` ./LBP -i img1.pgm -o img_out.pgm`
