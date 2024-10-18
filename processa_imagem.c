#include "processa_imagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <math.h>
#include <unistd.h>

int le_pgm(char caminho_img[], char *formato, int *largura, int *altura, int *max_pix, unsigned char **pixels){
    FILE *arquivo;
    int c, pixel;
    char formato_img[3];

    arquivo = fopen(caminho_img, "r");
    if (arquivo == NULL){
        return 1;
    }

    // Le formato da imagem
    fscanf(arquivo, "%s", formato_img);
    strcpy(formato, formato_img);

    // Ignora comentários
    while ((c = fgetc(arquivo)) == '#') {
        while (fgetc(arquivo) != '\n');
    }
    ungetc(c, arquivo);

    fscanf(arquivo, "%d %d", largura, altura);
    fscanf(arquivo, "%d", max_pix);

    if (strcmp(formato_img, "P2") == 0){
        *pixels = (unsigned char *)malloc((*largura) * (*altura) * sizeof(unsigned char)); 
        if (*pixels == NULL) {
            //printf("Erro ao alocar memória para pixels.\n");
            fclose(arquivo);
            return 1;
        }

        for (int i = 0; i < (*largura) * (*altura); i++){
            fscanf(arquivo, "%d", &pixel);
            (*pixels)[i] = (unsigned char)pixel;
        }
    }

    else if (strcmp(formato_img, "P5") == 0){
        fgetc(arquivo);
        *pixels = (unsigned char *)malloc((*largura) * (*altura) * sizeof(unsigned char)); 
        if (*pixels == NULL) {
            //printf("Erro ao alocar memória para pixels.\n");
            fclose(arquivo);
            return 1;
        }

        fread(*pixels, sizeof(unsigned char), (*largura) * (*altura), arquivo);
    }

    else {
        //printf("Formato de arquivo não suportado: %s\n", formato);
        fclose(arquivo);
        return 1;
    }

    fclose(arquivo);
    return 0;
}

void calcula_lbp(imagemPGM *img_entrada, unsigned char **img_lbp){
    int largura, altura;
    unsigned char pixel_c, lbp;

    largura = img_entrada->largura;  
    altura = img_entrada->altura; 

    *img_lbp = (unsigned char *)calloc(largura * altura, sizeof(unsigned char)); 
    if (*img_lbp == NULL) {
            //printf("Erro ao alocar memória para pixels da imagem LBP.\n");
            return;
        }

    // Loop sobre cada pixel (exceto as bordas)
    for (int y = 1; y < altura - 1; y++) {
        for (int x = 1; x < largura - 1; x++) {
            pixel_c = img_entrada->pixels[y * largura + x];  // Pixel central
            lbp = 0; 

            // Calcula o LBP considerando os 8 vizinhos do pixel central
            lbp |= (img_entrada->pixels[(y-1) * largura + (x-1)] >= pixel_c) << 7; // Vizinhos: Cima-Esquerda
            lbp |= (img_entrada->pixels[(y-1) * largura + x] >= pixel_c) << 6; // Vizinhos: Cima
            lbp |= (img_entrada->pixels[(y-1) * largura + (x+1)] >= pixel_c) << 5; // Vizinhos: Cima-Direita
            lbp |= (img_entrada->pixels[y * largura + (x+1)] >= pixel_c) << 4; // Vizinhos: Direita
            lbp |= (img_entrada->pixels[(y+1) * largura + (x+1)] >= pixel_c) << 3; // Vizinhos: Baixo-Direita
            lbp |= (img_entrada->pixels[(y+1) * largura + x] >= pixel_c) << 2; // Vizinhos: Baixo
            lbp |= (img_entrada->pixels[(y+1) * largura + (x-1)] >= pixel_c) << 1; // Vizinhos: Baixo-Esquerda
            lbp |= (img_entrada->pixels[y * largura + (x-1)] >= pixel_c) << 0; // Vizinhos: Esquerda

            (*img_lbp)[y * largura + x] = lbp;  // Armazena o valor LBP calculado
        }
    }
}

void escreve_lbp(const char *caminho_lbp, unsigned char *img_lbp, int tam){
    FILE *arquivo;

    arquivo = fopen(caminho_lbp, "wb");
    if (arquivo == NULL){
        //printf("Erro ao criar arquivo LBP: %s\n", caminho_lbp);
        return;
    }
    fwrite(img_lbp, sizeof(unsigned char), tam, arquivo);
    fclose(arquivo);
}

void le_lbp(const char *caminho_lbp, unsigned char **img_lbp, int tam_vet){
    FILE *arquivo;

    arquivo = fopen(caminho_lbp, "rb");
    if (arquivo == NULL){
        //printf("Erro ao abrir o arquivo LBP: %s\n", caminho_lbp);
        return;
    }
    
    // Aloca memória para os dados LBP
    *img_lbp = (unsigned char *)malloc(tam_vet * sizeof(unsigned char));
    fread(*img_lbp, sizeof(unsigned char), tam_vet, arquivo);
    fclose(arquivo); 
}

double calcula_dist(float *hist1, float *hist2, int tam_vet) {
    double soma = 0.0, diferenca;

    if (tam_vet == 0) {
        fprintf(stderr, "O tamanho do histograma é zero\n");
        return 1;
    }

    for (int i = 0; i < tam_vet; i++) {
        diferenca = hist1[i] - hist2[i];
        soma += diferenca * diferenca;
    }

    return sqrt(soma);
}

void manipula_dir(const char *nome_dir){
    DIR *diretorio;
    struct dirent *entrada;
    char caminho_img[2 * MAX_NOME], caminho_lbp[2 * MAX_NOME];
    char nome_lbp[MAX_NOME], *ext, formato;
    unsigned char *img_lbp = NULL;
    int max_pix;

    // Abre o diretório especificado
    diretorio = opendir(nome_dir);
    if (diretorio == NULL) {
        //printf("Erro ao abrir o diretório %s\n", nome_dir);
        return;
    }
    
    // Percorre todos os arquivos do diretorio
    while ((entrada = readdir(diretorio)) != NULL) {
        if(strstr(entrada->d_name, ".pgm") != NULL){

            sprintf(caminho_img, "%s/%s", nome_dir, entrada->d_name);

            // Gera nome do arquivo .lbp
            ext = strrchr(entrada->d_name, '.');
            if (ext != NULL)
                *ext = '\0'; 
            sprintf(nome_lbp, "%s.lbp", entrada->d_name);

            sprintf(caminho_lbp, "%s/%s", nome_dir, nome_lbp);

            // Gera o arquivo LBP se ele não existir
            if (access(caminho_lbp, F_OK) == -1) {
                    imagemPGM imagem;
                    le_pgm(caminho_img, &formato, &imagem.largura, 
                            &imagem.altura, &max_pix, &imagem.pixels);
                    calcula_lbp(&imagem, &img_lbp);
                    escreve_lbp(caminho_lbp, img_lbp, imagem.largura * imagem.altura);

                    free(img_lbp);
                    free(imagem.pixels);
            }
        }
    }

    closedir(diretorio);
}

void compara_imagens(char caminho_img[], const char *nome_dir) { 
    DIR *diretorio;
    struct dirent *entrada;
    imagemPGM img_entrada;
    unsigned char *img_lbp = NULL, *img_lbp_base = NULL;
    char formato;
    double dist = 0, menor_dist = INFINITY;
    char caminho_lbp[2 * MAX_NOME] = "", nome_img_similar[MAX_NOME] = "";
    int tam_vet, max_pix;
    float histograma_normalizado_entrada[TAM];

    le_pgm(caminho_img, &formato, &img_entrada.largura, &img_entrada.altura, &max_pix, &img_entrada.pixels);
    calcula_lbp(&img_entrada, &img_lbp); 
    manipula_dir(nome_dir);    
    
    // Gera o histograma normalizado da imagem de entrada
    for (int i = 0; i < TAM; i++) {
        histograma_normalizado_entrada[i] = (float)0; // Inicializa com zero
    }

    // Calculo do histograma normalizado da imagem de entrada
    for (int i = 0; i < img_entrada.largura * img_entrada.altura; i++) {
        histograma_normalizado_entrada[img_lbp[i]]++;
    }
    
    for (int i = 0; i < TAM; i++) {
        histograma_normalizado_entrada[i] /= (img_entrada.largura * img_entrada.altura);
    }

    // Percorre o diretorio para comparacao
    diretorio = opendir(nome_dir);
    if (diretorio == NULL) {
        //printf("Erro ao abrir o diretório %s\n", nome_dir);
        return;
    }

    while ((entrada = readdir(diretorio)) != NULL) {
        if (strstr(entrada->d_name, ".lbp") != NULL) {

            // Obtém o nome do arquivo sem extensão
            char *extensao_lbp = strrchr(nome_img_similar, '.');
            if (extensao_lbp != NULL) {
                *extensao_lbp = '\0';  // Remove a extensão ".lbp"
            }

            // Pula a comparação se for o mesmo arquivo
            if (strcmp(nome_img_similar, caminho_img) == 0) {
                continue;  
            }

            sprintf(caminho_lbp, "%s/%s", nome_dir, entrada->d_name);

            tam_vet = img_entrada.largura * img_entrada.altura;

            // Le os dados LBP do arquivo
            le_lbp(caminho_lbp, &img_lbp_base, tam_vet);

            // Normaliza o histograma da imagem base
            float histograma_normalizado_base[TAM];
            for (int i = 0; i < TAM; i++) {
                histograma_normalizado_base[i] = (float)0; 
            }

            for (int i = 0; i < tam_vet; i++) {
                histograma_normalizado_base[img_lbp_base[i]]++;
            }
            
            for (int i = 0; i < TAM; i++) {
                histograma_normalizado_base[i] /= tam_vet; 
            }

            // Calcula a distância utilizando os histogramas normalizados
            dist = calcula_dist(histograma_normalizado_entrada, histograma_normalizado_base, TAM);


            if (dist < menor_dist && dist != 0.0) {
                menor_dist = dist;
                strcpy(nome_img_similar, entrada->d_name);
            }

            free(img_lbp_base);
        }
    }

    closedir(diretorio);

    printf("Imagem mais similar: %s %.6f\n", nome_img_similar, menor_dist);

    // Libera memória após o uso
    if (img_entrada.pixels != NULL) {
        free(img_entrada.pixels);
        img_entrada.pixels = NULL; 
    }
    free(img_entrada.pixels);
    free(img_lbp);
}

void escreve_imagens(char caminho_img[], char caminho_saida[]){
    imagemPGM img_entrada;
    unsigned char *img_saida = NULL;
    FILE *arquivo;
    char formato[3]; 
    int largura, altura, max_pix, erro;

    // Lê a imagem de entrada
    erro = le_pgm(caminho_img, formato, &img_entrada.largura, &img_entrada.altura, 
           &max_pix, &img_entrada.pixels);

    if (erro) {
        return;
    }

    // Calcula o LBP da imagem de entrada
    calcula_lbp(&img_entrada, &img_saida);

    largura = img_entrada.largura;
    altura = img_entrada.altura;

    // Abre o arquivo de saída para escrita
    arquivo = fopen(caminho_saida, "wb");
    if (arquivo == NULL) {
        //printf("Erro ao abrir o arquivo %s\n", caminho_saida);
        return;
    }

    // Escreve o cabeçalho PGM no arquivo de saída
    fprintf(arquivo, "%s\n%d %d\n%d\n", formato, largura, altura, max_pix);

    // Escreve os pixels no formato correspondente (P2 ou P5)
    if (strcmp(formato, "P2") == 0) {
        for (int i = 0; i < largura * altura; i++) {
            fprintf(arquivo, "%d ", img_saida[i]);
            if ((i + 1) % largura == 0) {
                fprintf(arquivo, "\n"); 
            }
        }
    } 

    else if (strcmp(formato, "P5") == 0) {
        fwrite(img_saida, sizeof(unsigned char), largura * altura, arquivo);
    } 

    else {
        fclose(arquivo);
        return;
    }

    // Libera a memória usada pelos pixels
    free(img_saida);
    free(img_entrada.pixels);


    fclose(arquivo);
}
