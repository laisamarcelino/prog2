#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processa_imagem.h"

int main() {
    FILE *file_in;

    file_in = fopen("Apuleia1.pgm", "rb");

    le_cabecalho_pgm(file_in);

    return 0;
}