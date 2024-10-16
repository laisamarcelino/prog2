#include "processa_imagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

imagemPGM *le_cabecalho_imagem(FILE *arquivo, imagemPGM *imagem){
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

imagemPGM *le_imagem(FILE *arquivo, imagemPGM *imagem){
    unsigned char pixel;

    le_cabecalho_imagem(arquivo, imagem);
    imagem->pixels = aloca_matriz_imagem(imagem);

    if (strcmp(imagem->formato, "P2") == 0){
        for (int i = 0; i < imagem->altura; i++){
            for (int j = 0; j < imagem->largura; j++)
                fscanf(arquivo, "%d", &imagem->pixels[i][j]);
        }
    }

    else if (strcmp(imagem->formato, "P5") == 0){
        fgetc(arquivo);

        for (int i = 0; i < imagem->altura; i++){
            for (int j = 0; j < imagem->largura; j++){
                fread(&pixel, sizeof(unsigned char), 1, arquivo);
                imagem->pixels[i][j] = (int)pixel;                
            }
        }
    }

    else
        printf("Formato de arquivo não suportado");

    return imagem;
}

imagemPGM *escreve_imagem_lbp(FILE *arquivo, imagemPGM *imagem){
    unsigned char pixel;
    
    // Escreve o cabeçalho no arquivo
    fprintf(arquivo, "%s\n%d %d\n%d\n", imagem->formato, imagem->largura, imagem->altura, imagem->max_pix);

    if (strcmp(imagem->formato, "P2") == 0){
        for (int i = 0; i < imagem->altura; i++){
            for (int j = 0; j < imagem->largura; j++)
                fprintf(arquivo, "%d ", imagem->pixels[i][j]);
            fprintf(arquivo, "\n");
        }
    }

    else if (strcmp(imagem->formato, "P5") == 0){
        for (int i = 0; i < imagem->altura; i++){
            for (int j = 0; j < imagem->largura; j++){
                pixel = (unsigned char)imagem->pixels[i][j]; 
                fwrite(&pixel, sizeof(unsigned char), 1, arquivo);  
            }
        }
    }

    else
        printf("Formato de arquivo não suportado");

    return imagem;
}

void calcula_lbp(imagemPGM *imagem, imagemPGM *lbp_imagem){
    unsigned char pc, lbp;

    // Percorre os pixels internos da imagem, ignorando as bordas
    for (int i = 1; i < imagem->altura - 1; i++) {
        for (int j = 1; j < imagem->largura - 1; j++) {
            pc = imagem->pixels[i][j];  // Pixel central
            lbp = 0;

            // Define os 8 vizinhos do pixel central
            int vizinho[8] = {
                imagem->pixels[i-1][j-1], imagem->pixels[i-1][j], 
                imagem->pixels[i-1][j+1], imagem->pixels[i][j+1], 
                imagem->pixels[i+1][j+1], imagem->pixels[i+1][j], 
                imagem->pixels[i+1][j-1], imagem->pixels[i][j-1]
            };

            // Calcula o padrão binário
            for (int x = 0; x < 8; x++) {
                if (vizinho[x] >= pc) {
                    lbp |= (1 << x);  // Atribui 1 se o vizinho é maior ou igual ao pixel central
                }
            }     

            lbp_imagem->pixels[i][j] = lbp;  // Armazena o valor lbp na nova imagem
        }
    }
}

void gera_histograma(imagemPGM *lbp_imagem, double *histograma, char *nome_img_entrada){
    FILE *arquivo;
    unsigned char lbp_valor;
    char nome_lbp[100], *extensao;
    const char *diretorio_lbp = "histogramas";

    if (mkdir(diretorio_lbp, 0777) != 0) { 
        if (errno != EEXIST) {
            fprintf(stderr, "Erro ao criar o diretório");
            return;
        }
    }

    extensao = strrchr(nome_img_entrada, '.');
    if (extensao != NULL)
        *extensao = '\0'; 
    sprintf(nome_lbp, "%s/%s.lbp", diretorio_lbp, nome_img_entrada);

    arquivo = fopen(nome_lbp, "wb");
    if (arquivo == NULL){
        fprintf(stderr, "Erro ao abrir o arquivo do histograma\n");
        return;
    }

    for (int i = 0; i < TAM; i++) {
        histograma[i] = 0;
    }

    for (int i = 0; i < lbp_imagem->altura; i++) {
        for (int j = 0; j < lbp_imagem->largura; j++) {
            lbp_valor = lbp_imagem->pixels[i][j];
            histograma[lbp_valor]++;
        }
    }

    for (int i = 0; i < TAM; i++) {
        histograma[i] = (double)histograma[i] / (double)(lbp_imagem->altura * lbp_imagem->largura);
    }

    fwrite(histograma, sizeof(double), TAM, arquivo);
    fclose(arquivo);
}

int le_histograma_lbp (double *histograma, char *nome_lbp){
    FILE *arquivo;

    arquivo = fopen(nome_lbp, "rb");
    if (arquivo == NULL)
        return 0;

    fread(histograma, sizeof(double), TAM, arquivo);
    fclose(arquivo);
    
    return TAM;
}

double calcula_distancia(double *histograma1, double *histograma2, int tam){
    double soma = 0.0, diferenca;

    for (int i = 0; i < tam; i++){
        diferenca = histograma1[i] - histograma2[i];
        soma += diferenca * diferenca;
    }

    return sqrt(soma);
}


int manipula_diretorio (imagemPGM *imagem, char *nome_diretorio){
    DIR *diretorio;
    struct dirent *entrada;
    char caminho_imagem[300];
    FILE *arq_diretorio;
    imagemPGM *imagem_diretorio;
    double *histograma;

    // Abre o diretorio ./base
    diretorio = opendir(nome_diretorio);
    if (diretorio == NULL){
        fprintf(stderr, "Erro ao abrir diretorio ./base");
        return 1;
    }

    while ((entrada = readdir(diretorio)) != NULL){
        if (imagem_valida(entrada->d_name)){

            sprintf(caminho_imagem, "%s/%s", nome_diretorio, entrada->d_name);

            // Abre o arquivo no diretorio
            arq_diretorio = fopen(caminho_imagem, "r");
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
            fclose(arq_diretorio);
            aloca_lbp(imagem, imagem_diretorio);
            calcula_lbp(imagem, imagem_diretorio);

            histograma = (double *)malloc(TAM * sizeof(double));
            if (histograma == NULL) {
                fprintf(stderr, "Erro ao alocar memória para o histograma\n");
                free(imagem);
                free(imagem_diretorio);
                return 1;
            }

            gera_histograma(imagem_diretorio, histograma, entrada->d_name);

            libera_memoria(imagem_diretorio);
            free(histograma);
        }
    }

    return 0;
}

void compara_imagens (imagemPGM *imagem, char *nome_img_entrada, 
            char *nome_diretorio){
    char *extensao, nome_lbp[100], caminho_lbp[300], nome_similar[100];
    double *histograma_base, *histograma_similar, tam;
    double distancia, menor_distancia = INFINITY;
    DIR *diretorio;
    struct dirent *entrada;

    extensao = strrchr(nome_img_entrada, '.');
    if (extensao != NULL)
        *extensao = '\0'; 
    sprintf(nome_lbp, "./histogramas/%s.lbp", nome_img_entrada);

    // Inicializa os ponteiros de histograma
    histograma_base = (double *)malloc(TAM * sizeof(double));    if (histograma_base == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o histograma base\n");
        return;
    }
    histograma_similar = (double *)malloc(TAM * sizeof(double));    if (histograma_similar == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o histograma similar\n");
        return;
    }

    // Caso o histograma não exista, gera o histograma e salva no diretorio
    if (!le_histograma_lbp(histograma_base, nome_lbp)) {
        manipula_diretorio(imagem, nome_diretorio);
    }

    diretorio = opendir("histogramas");
    if (diretorio == NULL) {
        fprintf(stderr, "Erro ao abrir o diretório de histogramas\n");
        return;
    }

    // Percorre os histogramas no diretório
    while ((entrada = readdir(diretorio)) != NULL) {
        if (imagem_valida(entrada->d_name) == 2) {

            sprintf(caminho_lbp, "histogramas/%s", entrada->d_name);

            tam = le_histograma_lbp(histograma_similar, caminho_lbp);

            distancia = calcula_distancia(histograma_base, histograma_similar, tam);

            if (distancia < menor_distancia) {
                menor_distancia = distancia;
                strcpy(nome_similar, entrada->d_name);
            }
        }
    }

    closedir(diretorio);

    printf("Imagem mais similar: %s %f\n", nome_similar, menor_distancia);
}

int imagem_valida(char *nome_img_entrada) {
    char *extensao = strrchr(nome_img_entrada, '.');

    if (extensao != NULL && strcmp(extensao, ".pgm") == 0)
        return 1;
    else if (extensao != NULL && strcmp(extensao, ".lbp") == 0)
        return 2;

    return 0;
}

void aloca_lbp(imagemPGM *imagem, imagemPGM *lbp_imagem) {
    lbp_imagem->largura = imagem->largura;
    lbp_imagem->altura = imagem->altura;
    lbp_imagem->max_pix = imagem->max_pix;
    strcpy(lbp_imagem->formato, imagem->formato);
    lbp_imagem->pixels = aloca_matriz_imagem(lbp_imagem);

    if (lbp_imagem->pixels == NULL) {
        fprintf(stderr, "Erro ao alocar matriz de pixels para imagem LBP\n");
        free(imagem); 
        return;
    }
    
    // Inicializa os valores da matriz de pixels da imagem lbp
    for (int i = 0; i < lbp_imagem->altura; i++) {
        for (int j = 0; j < lbp_imagem->largura; j++) {
            lbp_imagem->pixels[i][j] = 0; 
        }
    }
}

int **aloca_matriz_imagem (imagemPGM *imagem){
    int **pixels;

    // Aloca um vetor para as linhas
    pixels = (int **)malloc(imagem->altura * sizeof(int *));
    
    if (pixels == NULL){
        printf("Erro ao alocar memória para a imagem\n");
        return NULL;
    }

    // Aloca um vetor para as colunas
    for (int i = 0; i < imagem->altura; i++){
        pixels[i] = (int *)malloc(imagem->largura * sizeof(int));
        if (pixels[i] == NULL){
            printf("Erro ao alocar memória para a imagem %d\n", i);
            for (int j = 0; j < i; j++)
                free(pixels[j]);
            free(pixels);
            return NULL;
        }
    }

    return pixels;
}

void libera_memoria(imagemPGM *imagem){
    for (int i = 0; i < imagem->altura; i++)
        free(imagem->pixels[i]);
    free(imagem->pixels);

}