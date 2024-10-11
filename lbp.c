#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processa_imagem.h"

int main() {
    FILE *file_in, *file_out;
    imagemPGM *imagem_in, *imagem_out;

    file_in = fopen("Apuleia1.pgm", "rb");
    imagem_in = (imagemPGM *)malloc(sizeof(imagemPGM));

    file_out = fopen("EscritaApuleia.pgm", "wb");
    imagem_out = (imagemPGM *)malloc(sizeof(imagemPGM));
    if (imagem_in == NULL) {
        fprintf(stderr, "Erro ao alocar memória para imagemPGM\n");
        return 1;
    }

    le_cabecalho_pgm(file_in, imagem_in);

    le_p5(file_in, imagem_in);
    escreve_p5(file_out, imagem_out);

    fclose(file_in);

    return 0;
}