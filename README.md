# Trabalho Prático 1 - Estratégias de Comparação de Imagens

## Sobre
Este projeto tem como objetivo comparar imagens no formato PGM utilizando o método Local Binary Pattern (LBP) para extração de características. O programa processa imagens PGM nos formatos P2 (ASCII) e P5 (Binário), gera a transformação LBP, calcula histogramas e, por fim, compara imagens com base na distância euclidiana entre os histogramas resultantes.

## Funcionalidades do Projeto
* 1. Compara uma imagem de teste com todas as imagens da base de referência
* 2. Gera uma imagem LBP a partir de imagem imagem PGM

## Estrutura do Projeto
O projeto está estruturado nos seguintes arquivos:

* 1. lbp.c

Arquivo principal do programa, onde a execução começa. Ele recebe os parâmetros via linha de comando e coordena a leitura, escrita processamento e comparação das imagens.

**Função main:**
    Recebe argumentos que indicam o diretório de imagens e o nome da imagem de entrada.
    Abre a imagem especificada, lê os dados e calcula o LBP, gera o histograma, e dependendo da solicitação, gera a imagem LBP ou compara a imagem com outras imagens no diretório.

* 2.  processa_imagem.h

É o arquivo de cabeçalho que contém as declarações das funções e da estrutura da imagem PGM utilizada no projeto.
Define a estrutura imagemPGM e todas as funções mencionadas no arquivo processa_imagem.c.

* 3. processa_imagem.c

Contém as funções responsáveis por processar imagens no formato PGM e calcular LBP. Aqui são implementadas as funções que lidam com a leitura, escrita, transformação LBP, e manipulação de diretórios.São elas:

* **le_cabecalho_imagem:** Lê o cabeçalho de uma imagem PGM (formato, largura, altura, e valor máximo de pixel).
* **le_imagem:** Carrega a imagem completa em formato P2 (ASCII) ou P5 (Binário).
* **escreve_imagem_lbp:** Escreve a imagem resultante do cálculo LBP em arquivo.
* **calcula_lbp:** Calcula o LBP de uma imagem.
* **gera_histograma:** Gera o histograma da imagem LBP e armazena em arquivo.
* **le_histograma_lbp:** Lê o histograma de uma imagem LBP já processada.
* **calcula_distancia:** Calcula a distância Euclidiana entre dois histogramas.
* **manipula_diretorio:** Lê e processa todas as imagens de um diretório para gerar seus histogramas.
* **compara_imagens:** Compara a imagem de entrada com as imagens no diretório de histogramas e encontra a mais similar.
* **imagem_valida:** Verica se o arquivo é uma imagem PGM ou LBP.
* **aloca_lbp:** Aloca memória para a estrutura de imagem LBP.
* **aloca_matriz_imagem:** Aloca memória para a matriz de pixels da imagem.
* **libera_memoria:** Libera a memória alocada para a imagem.

## Compilação e Execução

Para compilar o programa use o comando `make`

Para comparar uma imagem de teste com as imagens da base de referência use:
` ./LBP -d ./base -i img1.pgm `

Para gerar um imagem LBP use:
` ./LBP -i img1.pgm -o img_out.pgm`
