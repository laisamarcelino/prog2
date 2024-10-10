#include "processa_imagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

imagemPGM *le_cabecalho_pgm(FILE *arquivo){
    char formato[3];
    int largura, altura, max_pix;
    imagemPGM *imagem;

    fscanf(arquivo, "%s", formato);
    fscanf(arquivo, "%d %d", &largura, &altura);
    fscanf(arquivo, "%d", &max_pix);

    imagem = (imagemPGM *)malloc(sizeof(imagemPGM));
    if (imagem == NULL) {
        fprintf(stderr, "Erro ao alocar memória para imagemPGM\n");
        return NULL;
    }

    strcpy(imagem->formato, formato);
    imagem->largura = largura;
    imagem->altura = altura;
    imagem->max_pix = max_pix;

    return imagem;
}

