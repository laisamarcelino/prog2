#include "processa_imagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
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
    int pc, lbp;

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

void gera_histograma(imagemPGM *lbp_imagem, int *histograma, char *nome_pgm){
    FILE *arquivo;
    int lbp_valor;
    char nome_lbp[100], *extensao;
    const char *diretorio_lbp = "histogramas";

    /* Cria um diretorio para salvar os histogramas se ele nao existir
       Da permissoes de leitura, escrita e execucao */
    if (mkdir(diretorio_lbp, 0777) != 0) { 
        if (errno != EEXIST) {
            fprintf(stderr, "Erro ao criar o diretório");
            return;
        }
    }

    // Atualiza a extensao .pgm para .lbp
    extensao = strrchr(nome_pgm, '.');
    if (extensao != NULL)
        *extensao = '\0'; // Remove a extensao .pgm
    sprintf(nome_lbp, "%s/%s.lbp", diretorio_lbp, nome_pgm);

    arquivo = fopen(nome_lbp, "wb");
    if (arquivo == NULL){
        fprintf(stderr, "Erro ao abrir o arquivo do histograma\n");
        return;
    }

    // Inicializa o histograma
    for (int i = 0; i < 256; i++) {
        histograma[i] = 0;
    }

    // Percorre a imagem lbp e conta as frequências de cada valor
    for (int i = 0; i < lbp_imagem->altura; i++) {
        for (int j = 0; j < lbp_imagem->largura; j++) {
            lbp_valor = lbp_imagem->pixels[i][j];
            histograma[lbp_valor]++;
        }
    }

    fwrite(histograma, sizeof(int), 256, arquivo);
    fclose(arquivo);
}

//arrumar esse fopen
int le_histograma_lbp (int *histograma, char *nome_lbp){
    FILE *arquivo;
    int valor, tam = 0, i = 0;

    arquivo = fopen(nome_lbp, "rb");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        return 1;
    }

    while((fread(&valor, sizeof(int), 1, arquivo)) == 1){
        histograma[i] = valor;
        printf("%d ", histograma[i]);
        tam++;
        i++;
    }

    fclose(arquivo);
    return tam;
}


int calcula_distancia(int *histograma1, int *histograma2, int tam){
    int soma = 0;

    for (int i = 0; i < tam; i ++){
        soma += pow(histograma1[i] - histograma2[i], 2);
    }

    return sqrt(soma);
}

void compara_imagens (imagemPGM *ima){
    int tam;

    

    //se n existir img_base.lbp calcular e salvar lbp no diretorio
    // se existir comparar distancias, base com todos arquivos do diretorio histogramas


}

int imagem_valida(char *nome_pgm) {
    char *extensao = strrchr(nome_pgm, '.');
    if (extensao != NULL && strcmp(extensao, ".pgm") == 0) {
        return 1;
    }
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