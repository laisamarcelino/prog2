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
    int pixel;

    for(int i=0; i < imagem->largura; i++){
        for(int j=0; j < imagem->altura; j++){
            fscanf(arquivo, "%d", &pixel);
            imagem->pixels[i][j] = pixel;
        }
    }
    printf("%d", imagem->pixels[0][0]);
    return imagem;
}

