#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processa_imagem.h"

int main() {
    FILE *file_in, *file_out;
    imagemPGM *imagem_in;

    // Abre o arquivo de entrada
    file_in = fopen("Apuleia1.pgm", "r");
    if (file_in == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada\n");
        return 1;
    }

    // Aloca memória para a estrutura imagem
    imagem_in = (imagemPGM *)malloc(sizeof(imagemPGM));
    if (imagem_in == NULL) {
        fprintf(stderr, "Erro ao alocar memória para imagemPGM\n");
        fclose(file_in);
        return 1;
    }

    // Lê o cabeçalho e os dados da imagem
    le_cabecalho_pgm(file_in, imagem_in);
    le_imagem(file_in, imagem_in); 

    // Abre o arquivo de saída
    file_out = fopen("EscritaApuleia2.pgm", "w");
    if (file_out == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída\n");
        fclose(file_in);
        free(imagem_in);  // Libera a memória alocada antes de sair
        return 1;
    }

    // Escreve a imagem lida no arquivo de saída
    escreve_imagem(file_out, imagem_in);

    // Fecha os arquivos e libera a memória alocada
    fclose(file_in);
    fclose(file_out);
    free(imagem_in);

    return 0;
}
