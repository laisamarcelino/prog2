#include "processa_imagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

/* Le um arquivo PGM*/
void le_pgm(const char *nome_arq, int *largura, int *altura, unsigned char **pixels){
    FILE *arquivo;
    char formato[3]; 
    int c, max_pix, pixel;

    arquivo = fopen(nome_arq, "r");
    if (arquivo == NULL){
        printf("Erro ao abrir arquivo: %s\n", nome_arq);
        return;
    }

    // Le cabecalho da imagem
    fscanf(arquivo, "%s", formato);

    // Ignora comentários
    while ((c = fgetc(arquivo)) == '#') {
        while (fgetc(arquivo) != '\n');
    }
    ungetc(c, arquivo);

    fscanf(arquivo, "%d %d", largura, altura);
    fscanf(arquivo, "%d", &max_pix);

    if (strcmp(formato, "P2") == 0){
        *pixels = (unsigned char *)malloc((*largura) * (*altura) * sizeof(unsigned char));  // Aloca memória para os dados da imagem
        for (int i = 0; i < (*largura) * (*altura); i++){
            fscanf(arquivo, "%d", &pixel);
            (*pixels)[i] = (unsigned char)pixel;
        }
    }

    else if (strcmp(formato, "P5") == 0){
        fgetc(arquivo);
        *pixels = (unsigned char *)malloc((*largura) * (*altura) * sizeof(unsigned char));  // Aloca memória para os dados da imagem
        fread(pixels, sizeof(unsigned char), (*largura) * (*altura), arquivo);
    }

    else {
        printf("Formato de arquivo não suportado: %s\n", formato);
        fclose(arquivo);
        return;
    }

    printf("[DEBUG] Arquivo %s lido\n", nome_arq);
    fclose(arquivo);

}

/* Compara duas imagens considerando a distancia Euclediana */
void compara_imagens(const char *nome_arq){
    //DIR *diretorio;
    //struct dirent *entrada;
    imagemPGM imagem;
    //unsigned char *imagem_lbp = NULL;

    le_pgm(nome_arq, &imagem.largura, &imagem.altura, &imagem.pixels);
    //calcula_lbp();
}