#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processa_imagem.h"

int main() {
    FILE *file_in;
    imagemPGM *imagem;

    file_in = fopen("Apuleia1.pgm", "rb");
    imagem = (imagemPGM *)malloc(sizeof(imagemPGM));
    if (imagem == NULL) {
        fprintf(stderr, "Erro ao alocar memória para imagemPGM\n");
        return 1;
    }

    le_cabecalho_pgm(file_in, imagem);
    le_p2(file_in, imagem);
    
    return 0;
}