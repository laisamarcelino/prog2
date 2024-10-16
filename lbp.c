#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processa_imagem.h"

int main(int argc, char *argv[]) {
    FILE *arq_entrada, *arq_saida;
    char caminho_imagem[300];
    imagemPGM *imagem_entrada, *imagem_saida;
    int opcao_escrita = 0;

    // Verifica se o número correto de argumentos foi passado
    if (argc != 5) {
        fprintf(stderr, "Entrada inválida\n");
        return 1;
    }

    // Constroi o caminho da imagem
    if (strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-i") == 0)
        sprintf(caminho_imagem, "%s/%s", argv[2], argv[4]);
    else {
        sprintf(caminho_imagem, "%s", argv[2]);
        opcao_escrita = 1;
    }

    // Abre o arquivo de entrada
    arq_entrada = fopen(caminho_imagem, "r");
    if (arq_entrada == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada\n");
        return 1;
    }

    // Aloca memória para a estrutura imagem de entrada
    imagem_entrada = (imagemPGM *)malloc(sizeof(imagemPGM));
    if (imagem_entrada == NULL) {
        fprintf(stderr, "Erro ao alocar memória para imagemPGM\n");
        fclose(arq_entrada);
        return 1;
    }

    // Lê o cabeçalho e os dados da imagem de entrada
    le_imagem(arq_entrada, imagem_entrada);
    
    if (opcao_escrita){
        imagem_saida = (imagemPGM *)malloc(sizeof(imagemPGM)); 
        if (imagem_saida == NULL) {
            fprintf(stderr, "Erro ao alocar memória para imagem_saida\n");
            fclose(arq_entrada);
            free(imagem_entrada);
            return 1;
        }

        aloca_lbp(imagem_entrada, imagem_saida);
        calcula_lbp(imagem_entrada, imagem_saida);

        // Abre o arquivo de saída
        arq_saida = fopen(argv[4], "w");
        if (arq_saida == NULL) {
            fprintf(stderr, "Erro ao abrir o arquivo de saída\n");
            fclose(arq_entrada);
            libera_memoria(imagem_entrada);
            free(imagem_entrada);
            libera_memoria(imagem_saida);
            free(imagem_saida);
            return 1;
        }

        // Escreve a imagem com LBP no arquivo de saída
        escreve_imagem_lbp(arq_saida, imagem_saida);
        
        fclose(arq_saida);
        fclose(arq_entrada);
        libera_memoria(imagem_entrada);
        free(imagem_entrada);
        libera_memoria(imagem_saida);
        free(imagem_saida);
        return 0;
    }

    manipula_diretorio(imagem_entrada, argv[2]);

    // Compara a imagem de entrada com as demais 
    compara_imagens(imagem_entrada, argv[4], argv[2]);

    // Fecha os arquivos e libera a memória alocada
    fclose(arq_entrada);
    libera_memoria(imagem_entrada);
    free(imagem_entrada);
   

    return 0;
}