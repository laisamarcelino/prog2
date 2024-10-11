#include "processa_imagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

imagemPGM *le_cabecalho_pgm(FILE *arquivo, imagemPGM *imagem){
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

imagemPGM *le_p2(FILE *arquivo, imagemPGM *imagem){

    imagem->pixels = aloca_imagem(imagem);

    for(int i = 0; i < imagem->altura; i++){
        for(int j = 0; j < imagem->largura; j++)
            fscanf(arquivo, "%d", &imagem->pixels[i][j]);
    }
    
    return imagem;
}

imagemPGM *le_p5(FILE *arquivo, imagemPGM *imagem){
    unsigned char pixel;

    imagem->pixels = aloca_imagem(imagem);
    fgetc(arquivo);

    for (int i = 0; i < imagem->altura; i++) {
        for (int j = 0; j < imagem->largura; j++) {
            fread(&pixel, sizeof(unsigned char), 1, arquivo);  // Ler 1 byte por vez
            imagem->pixels[i][j] = (int)pixel;  // Armazenar o valor como int
        }
    }

    return imagem;
}

imagemPGM *escreve_p5(FILE *arquivo, imagemPGM *imagem){

    imagem->pixels = aloca_imagem(imagem);
    

    for (int i = 0; i < imagem->altura; i++) {
        for (int j = 0; j < imagem->largura; j++) {
            unsigned char pixel;
            fwrite(&pixel, sizeof(unsigned char), 1, arquivo);
            imagem->pixels[i][j] = (int)pixel;  
            printf("%d ", imagem->pixels[i][j]);
        }
    }

    return imagem;
}

int **aloca_imagem(imagemPGM *imagem) {
    int **pixels = (int **)malloc(imagem->altura * sizeof(int *));
    if (pixels == NULL) {
        printf("Erro ao alocar memória para imagem\n");
        return NULL;
    }

    for (int i = 0; i < imagem->altura; i++) {
        pixels[i] = (int *)malloc(imagem->largura * sizeof(int));
        if (pixels[i] == NULL) {
            printf("Erro ao alocar memória para a linha %d\n", i);
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            free(pixels);
            return NULL;
        }
    }

    return pixels;
}
