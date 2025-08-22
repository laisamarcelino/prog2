# 🖼️ Estratégias de Comparação de Imagens

## 📌 Sobre
Este projeto tem como objetivo **comparar imagens no formato PGM** utilizando o método **Local Binary Pattern (LBP)** para **extração de características**.  

O programa:
- Processa imagens PGM nos formatos **P2 (ASCII)** e **P5 (Binário)**.  
- Gera a **transformação LBP** da imagem.  
- Calcula **histogramas de padrões locais**.  
- Compara imagens com base na **distância euclidiana** entre os histogramas resultantes.  

---

## 🚀 Funcionalidades
- Comparar uma **imagem de teste** com todas as imagens da base de referência.  
- Gerar uma **imagem LBP** a partir de uma imagem PGM.  
- Calcular **histogramas LBP** para uso em comparações.  

---

## 📂 Estrutura do Projeto

### `lbp.c`
Arquivo principal do programa, onde a execução inicia.  
- Recebe parâmetros via **linha de comando**.  
- Controla a **comparação entre imagens** ou a **geração da imagem LBP**.  

### `processa_imagem.h`
- Arquivo de **cabeçalho**.  
- Declara funções auxiliares e a **estrutura `imagemPGM`**, usada para representar imagens.  

### `processa_imagem.c`
Implementa todas as funções de processamento:  

- **`le_pgm`** → Lê imagens nos formatos P2 e P5.  
- **`calcula_lbp`** → Calcula o LBP da imagem:  
  - Para cada pixel (exceto bordas), compara o valor com seus 8 vizinhos.  
  - Define bits como `1` (vizinho ≥ pixel central) ou `0` (vizinho < pixel central).  
  - Combina os bits em um número de 8 bits que representa o **padrão local**.  
- **`escreve_imagem_lbp`** → Salva a imagem LBP em arquivo binário.  
- **`le_lbp`** → Lê uma imagem LBP salva em arquivo.  
- **`calcula_dist`** → Calcula a **distância euclidiana** entre dois histogramas.  
- **`manipula_dir`** → Lê todas as imagens de um diretório, gera histogramas e prepara comparações.  
- **`compara_imagens`** → Compara a imagem de entrada com a base e retorna a mais similar.  
- **`escreve_imagens`** → Gera e salva a imagem LBP correspondente a uma imagem de entrada.  

---

## ⚙️ Compilação e Execução

### 🔨 Compilação
```bash
make
```

Para comparar uma imagem de teste com as imagens da base de referência use
```bash
./LBP -d ./base -i img1.pgm 
```

Para gerar um imagem LBP use:
```bash
./LBP -i img1.pgm -o img_out.pgm
```
