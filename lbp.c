#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "processa_imagem.h"

int main(int argc, char *argv[]) {
    FILE *arq_entrada, *arq_diretorio, *arq_saida;
    DIR *diretorio;
    char caminho_imagem[300], caminho_imagem2[300];
    //double distancia;
    struct dirent *entrada; // Struct definida pela biblioteca <dirent.h>
    imagemPGM *imagem_entrada, *imagem_diretorio, *imagem_saida;
    int *histograma, opcao_escrita = 0;

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
    arq_entrada = fopen(caminho_imagem, "rb");
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
        arq_saida = fopen(argv[4], "wb");
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

    // Abre o diretorio ./base
    diretorio = opendir(argv[2]);
    if (diretorio == NULL){
        fprintf(stderr, "Erro ao abrir diretorio ./base");
        return 1;
    }

    // Le as imagens do diretorio e calcula o lbp para todas as imagens
    while ((entrada = readdir(diretorio)) != NULL){
        if (imagem_valida(entrada->d_name)){

            sprintf(caminho_imagem2, "%s/%s", argv[2], entrada->d_name);

            // Abre o arquivo no diretorio
            arq_diretorio = fopen(caminho_imagem2, "rb");
            if (arq_diretorio == NULL) {
                fprintf(stderr, "Erro ao abrir o arquivo de entrada\n");
                return 1;
            }

            // Aloca memória para a estrutura imagem de entrada
            imagem_diretorio = (imagemPGM *)malloc(sizeof(imagemPGM));
            if (imagem_diretorio == NULL) {
                fprintf(stderr, "Erro ao alocar memória para imagemPGM\n");
                fclose(arq_diretorio);
                return 1;
            }

            // Lê o cabeçalho e os dados da imagem
            le_imagem(arq_diretorio, imagem_diretorio);
            aloca_lbp(imagem_entrada, imagem_diretorio);
            calcula_lbp(imagem_entrada, imagem_diretorio);

            histograma = (int *)malloc(256 * sizeof(int));
            if (histograma == NULL) {
                fprintf(stderr, "Erro ao alocar memória para o histograma\n");
                fclose(arq_entrada);
                free(imagem_entrada);
                free(imagem_diretorio);
                return 1;
            }

            gera_histograma(imagem_diretorio, histograma, entrada->d_name);
            
            libera_memoria(imagem_diretorio);
            free(histograma);
            fclose(arq_diretorio);
        }
    }
    //compara imagens
    //printf("Imagem mais similar: %s %f", img_similar, distancia);
    
    // Abre o diretorio ./histogramas
    /*diretorio = opendir("histogramas");
    if (diretorio == NULL){
        fprintf(stderr, "Erro ao abrir diretorio ./histogramas");
        return 1;
    }*/
    //compara_imagens(imagem_entrada, diretorio);

    // Fecha os arquivos e libera a memória alocada
    fclose(arq_entrada);
    libera_memoria(imagem_entrada);
    free(imagem_entrada);
   

    return 0;
}