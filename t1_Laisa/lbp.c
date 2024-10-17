#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processa_imagem.h"

int main(int argc, char *argv[]) {
    char caminho_imagem[MAX_NOME];
    int opcao_escrita = 0;

    // Verifica se o número correto de argumentos foi passado
    if (argc != 5) {
        printf("Entrada inválida\n");
        return 1;
    }

    // Constroi o caminho da imagem
    if (strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-i") == 0)
        sprintf(caminho_imagem, "%s/%s", argv[2], argv[4]);
    else {
        sprintf(caminho_imagem, "%s", argv[2]);
        opcao_escrita = 1; //habilita a escrita da imagem
    }

    if (!opcao_escrita){//argv[2], argv[4]
        compara_imagens(argv[4]);
        printf("[DEBUG] Opcao de comparaçao\n");
        return 0;
    }

    printf("[DEBUG] Opcao de escrita");
    //escreve_imagens();
    return 0;
}