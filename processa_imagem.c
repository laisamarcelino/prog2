#include "processa_imagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

imagemPGM *le_cabecalho_imagem(FILE *arquivo, imagemPGM *imagem){
    char formato[3];
    int largura, altura, max_pix;

    fscanf(arquivo, "%s", formato);
    fscanf(arquivo, "%d %d", &largura, &altura);
    fscanf(arquivo, "%d", &max_pix);

    strcpy(imagem->formato, formato);
    imagem->largura = largura;
    imagem->altura = altura;
    imagem->max_pix = max_pix;

    return imagem;
}

imagemPGM *le_imagem(FILE *arquivo, imagemPGM *imagem){
    unsigned char pixel;

    le_cabecalho_imagem(arquivo, imagem);
    imagem->pixels = aloca_imagem(imagem);

    if (strcmp(imagem->formato, "P2") == 0){
        for (int i = 0; i < imagem->altura; i++){
            for (int j = 0; j < imagem->largura; j++)
                fscanf(arquivo, "%d", &imagem->pixels[i][j]);
        }
    }

    else if (strcmp(imagem->formato, "P5") == 0){
        fgetc(arquivo);

        for (int i = 0; i < imagem->altura; i++){
            for (int j = 0; j < imagem->largura; j++){
                fread(&pixel, sizeof(unsigned char), 1, arquivo); // Le 1 byte por vez
                imagem->pixels[i][j] = (int)pixel;                // Armazena o valor como int
            }
        }
    }

    else
        printf("Formato de arquivo não suportado");

    return imagem;
}

imagemPGM *escreve_imagem_lbp(FILE *arquivo, imagemPGM *imagem){
    unsigned char pixel;
    
    // Escreve o cabeçalho no arquivo
    fprintf(arquivo, "%s\n%d %d\n%d\n", imagem->formato, imagem->largura, imagem->altura, imagem->max_pix);

    if (strcmp(imagem->formato, "P2") == 0){
        for (int i = 0; i < imagem->altura; i++){
            for (int j = 0; j < imagem->largura; j++)
                fprintf(arquivo, "%d ", imagem->pixels[i][j]);
            fprintf(arquivo, "\n");
        }
    }

    else if (strcmp(imagem->formato, "P5") == 0){
        for (int i = 0; i < imagem->altura; i++){
            for (int j = 0; j < imagem->largura; j++){
                pixel = (unsigned char)imagem->pixels[i][j]; // Converte o valor de volta para unsigned char
                fwrite(&pixel, sizeof(unsigned char), 1, arquivo);  // Escreve 1 byte por vez
            }
        }
    }

    else
        printf("Formato de arquivo não suportado");

    return imagem;
}

void calcula_lbp(imagemPGM *imagem, imagemPGM *lbp_imagem){
    int pc, lbp;

    // Percorre os pixels internos da imagem, ignorando as bordas
    for (int i = 1; i < imagem->altura - 1; i++) {
        for (int j = 1; j < imagem->largura - 1; j++) {
            pc = imagem->pixels[i][j];  // Pixel central
            lbp = 0;

            // Define os 8 vizinhos do pixel central
            int vizinho[8] = {
                imagem->pixels[i-1][j-1], imagem->pixels[i-1][j], 
                imagem->pixels[i-1][j+1], imagem->pixels[i][j+1], 
                imagem->pixels[i+1][j+1], imagem->pixels[i+1][j], 
                imagem->pixels[i+1][j-1], imagem->pixels[i][j-1]
            };

            // Calcula o padrão binário
            for (int x = 0; x < 8; x++) {
                if (vizinho[x] >= pc) {
                    lbp |= (1 << x);  // Atribui 1 se o vizinho é maior ou igual ao pixel central
                }
            }     

            lbp_imagem->pixels[i][j] = lbp;  // Armazena o valor LBP na nova imagem
        }
    }
}

// Arrumar negocio do nome do arquivo
void gera_histograma(imagemPGM *lbp_imagem, int *histograma){
    FILE *arquivo;
    int lbp_valor;

    arquivo = fopen("histograma_lbp", "wb");
    if (arquivo == NULL){
        fprintf(stderr, "Erro ao abrir o arquivo de entrada\n");
        return;
    }

    // Inicializa o histograma
    for (int i = 0; i < 256; i++) {
        histograma[i] = 0;
    }

    // Percorre a imagem lbp e conta as frequências de cada valor
    for (int i = 0; i < lbp_imagem->altura; i++) {
        for (int j = 0; j < lbp_imagem->largura; j++) {
            lbp_valor = lbp_imagem->pixels[i][j];
            histograma[lbp_valor]++;
        }
    }

    fwrite(histograma, sizeof(int), 256, arquivo);
    fclose(arquivo);
}


int calcula_distancia(int *histograma1, int *histograma2, int tam){
    int soma = 0;

    for (int i = 0; i < tam; i ++){
        soma += pow(histograma1[i] - histograma2[i], 2);
    }

    return sqrt(soma);
}

int **aloca_imagem(imagemPGM *imagem){
    int **pixels;

    // Aloca um vetor para as linhas
    pixels = (int **)malloc(imagem->altura * sizeof(int *));
    
    if (pixels == NULL){
        printf("Erro ao alocar memória para a imagem\n");
        return NULL;
    }

    // Aloca um vetor para as colunas
    for (int i = 0; i < imagem->altura; i++){
        pixels[i] = (int *)malloc(imagem->largura * sizeof(int));
        if (pixels[i] == NULL){
            printf("Erro ao alocar memória para a imagem %d\n", i);
            for (int j = 0; j < i; j++)
                free(pixels[j]);
            free(pixels);
            return NULL;
        }
    }

    return pixels;
}

void libera_memoria(imagemPGM *imagem){

    for (int i = 0; i < imagem->altura; i++)
        free(imagem->pixels[i]);
    free(imagem->pixels);

}