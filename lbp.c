#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processa_imagem.h"

int main() {
    FILE *file_in, *file_out;
    imagemPGM *imagem_in, *imagem_lbp;
    int *histograma;

    // Abre o arquivo de entrada
    file_in = fopen("Apuleia1.pgm", "r");
    if (file_in == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada\n");
        return 1;
    }

    // Aloca memória para a estrutura imagem de entrada
    imagem_in = (imagemPGM *)malloc(sizeof(imagemPGM));
    if (imagem_in == NULL) {
        fprintf(stderr, "Erro ao alocar memória para imagemPGM\n");
        fclose(file_in);
        return 1;
    }

    // Lê o cabeçalho e os dados da imagem
    le_imagem(file_in, imagem_in); 

    // Aloca memória para a estrutura da imagem LBP
    imagem_lbp = (imagemPGM *)malloc(sizeof(imagemPGM));
    if (imagem_lbp == NULL) {
        fprintf(stderr, "Erro ao alocar memória para imagem LBP\n");
        fclose(file_in);
        free(imagem_in);  // Libera a memória alocada antes de sair
        return 1;
    }

    // Aloca a matriz de pixels para a imagem LBP
    imagem_lbp->largura = imagem_in->largura;
    imagem_lbp->altura = imagem_in->altura;
    imagem_lbp->max_pix = imagem_in->max_pix;
    strcpy(imagem_lbp->formato, imagem_in->formato);
    imagem_lbp->pixels = aloca_imagem(imagem_lbp);

    // Calcula o LBP
    calcula_lbp(imagem_in, imagem_lbp);

    // Abre o arquivo de saída
    file_out = fopen("teste_lbp2.pgm", "w");
    if (file_out == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de saída\n");
        fclose(file_in);
        free(imagem_in);
        free(imagem_lbp);  // Libera a memória alocada antes de sair
        return 1;
    }

    // Escreve a imagem com LBP no arquivo de saída
    escreve_imagem_lbp(file_out, imagem_lbp);

    histograma = (int *)malloc(256 * sizeof(int));
    if (histograma == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o histograma\n");
        fclose(file_in);
        free(imagem_in);
        free(imagem_lbp);
        return 1;
    }


    gera_histograma(imagem_lbp, histograma);

    // Fecha os arquivos e libera a memória alocada
    fclose(file_in);
    fclose(file_out);
    free(imagem_in);
    libera_memoria(imagem_lbp);

    return 0;
}
