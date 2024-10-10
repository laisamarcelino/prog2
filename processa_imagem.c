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
        for(int j = 0; j < imagem->largura; j++){
            fscanf(arquivo, "%d", &imagem->pixels[i][j]);
            printf("%d ", imagem->pixels[i][j]);
        }
    }
    
    return imagem;
}

imagemPGM *le_p5(FILE *arquivo){
    
}

int **aloca_imagem(imagemPGM *imagem){

    imagem->pixels = (int **)malloc(imagem->altura * sizeof(int *));
    for (int i = 0; i < imagem->altura; i++) {
        imagem->pixels[i] = (int *)malloc(imagem->largura * sizeof(int));
    }
    return imagem->pixels;
}

