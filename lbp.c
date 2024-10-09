#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Estrutura da imagem PGM */
typedef struct imagemPGM {
    char formato[3]; /* p2 ou p5 */
    int largura; 
    int altura;
    int max_pix; /* tamanho da variação de cada pixel na escala de cinza */
    int **pixels;
} imagemPGM;

/* ------------------ Leitura da imagem ---------------------- */

/* Aloca memória para a imagem */
int **alocaImagem(int largura, int altura){
    int **pixels = (int **)malloc(altura * sizeof(int *));
    for (int i = 0; i < altura; i++) {
        pixels[i] = (int *)malloc(largura * sizeof(int));
    }
    return pixels;
}

/* Função para ler a imagem PGM (P2 ou P5) */
imagemPGM *leituraImagem(FILE *file_in) {
    char formato[3];
    int largura, altura, max_pix; 
    int **pixels;

    /* Ler o formato (P2 ou P5) */
    fscanf(file_in, "%s", formato);
    
    /* Ler as dimensões da imagem */
    fscanf(file_in, "%d %d", &largura, &altura);
    
    /* Ler o valor máximo do pixel */
    fscanf(file_in, "%d", &max_pix);

    /* Alocar memória para os pixels */
    pixels = alocaImagem(largura, altura);

    /* Verifica se o formato da imagem é P2 (ASCII) */
    if (strcmp(formato, "P2") == 0) {
        for (int i = 0; i < altura; i++) {
            for (int j = 0; j < largura; j++) {
                fscanf(file_in, "%d", &pixels[i][j]);
            }
        }
    }
    /* Verifica se o formato da imagem é P5 (Binário) */
    else if (strcmp(formato, "P5") == 0) {
        fgetc(file_in); 
        for (int i = 0; i < altura; i++) {
            for (int j = 0; j < largura; j++) {
                pixels[i][j] = fgetc(file_in);
            }
        }
    }
    else {
        printf("Formato de arquivo não suportado\n");
        fclose(file_in);
        return NULL;
    }

    /* Cria estrutura de imagem */
    imagemPGM *imagem = (imagemPGM *)malloc(sizeof(imagemPGM));
    strcpy(imagem->formato, formato);
    imagem->largura = largura;
    imagem->altura = altura;
    imagem->max_pix = max_pix;
    imagem->pixels = pixels;

    return imagem;
}

/* Calculo do LBP 
operacao de leitura para p2 e p5 sao diferentes
no calculo do lpb as bordas nao sao consideradas */


/* ------------------- Escrita da Imagem ----------------------- */

/* Função para escrever a imagem PGM (P2 ou P5) */
void escritaImagem(FILE *file_out, imagemPGM *imagem) {
    /* Escreve o cabeçalho */
    fprintf(file_out, "%s\n", imagem->formato);
    fprintf(file_out, "%d %d\n", imagem->largura, imagem->altura);
    fprintf(file_out, "%d\n", imagem->max_pix);

    /* Verifica se o formato da imagem é P2 (ASCII) */
    if (strcmp(imagem->formato, "P2") == 0) {
        for (int i = 0; i < imagem->altura; i++) {
            for (int j = 0; j < imagem->largura; j++) {
                fprintf(file_out, "%d ", imagem->pixels[i][j]);
            }
            fprintf(file_out, "\n");
        }
    }
    /* Verifica se o formato da imagem é P5 (Binário) */
    else if (strcmp(imagem->formato, "P5") == 0) {
        for (int i = 0; i < imagem->altura; i++) {
            for (int j = 0; j < imagem->largura; j++) {
                fputc(imagem->pixels[i][j], file_out);
            }
        }
    }
}

/* ------------------- Função principal ----------------------- */

int main(int argc, char *argv[]) {
    FILE *file_in;
    FILE *file_out;
    /*char base_dir[100];*/
    char input_file[100];
    char file_path[150];

    // Verifica se o número correto de argumentos foi passado
    if (argc != 5) {
        printf("Uso: %s -d <diretório da base> -i <imagem de entrada>\n", argv[0]);
        return 1;  // Encerra o programa
    }

    const char *baseDir = NULL;  // Para armazenar o diretório da base

    // Processa os argumentos fornecidos na linha de comando
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            baseDir = argv[++i];  // Armazena o diretório da base
        } else if (strcmp(argv[i], "-i") == 0) {
            strcpy(input_file, argv[++i]);  // Armazena o arquivo de entrada
        }
    }

    // Verifica se os argumentos foram fornecidos corretamente
    if (!baseDir || !input_file[0]) {
        printf("Por favor, forneça o diretório da base e o arquivo de entrada.\n");
        return 1;  // Encerra o programa
    }

    /* Construir o caminho completo do arquivo de entrada */
    sprintf(file_path, "%s/%s", baseDir, input_file);

    /* Abrir o arquivo PGM para leitura */
    file_in = fopen(file_path, "rb");
    if (!file_in) {
        printf("ERRO: Problemas ao abrir o arquivo %s!\n", file_path);
        return 1;
    }

    /* Abrir o arquivo de saída (mesmo nome, mas para escrita) */
    char output_file[150];
    sprintf(output_file, "%s/output_%s", baseDir, input_file);
    file_out = fopen(output_file, "wb");
    if (!file_out) {
        printf("ERRO: Problemas ao abrir o arquivo de saída %s para escrita!\n", output_file);
        fclose(file_in);
        return 1;
    }

    /* Chamar a função de leitura de imagem */
    imagemPGM *imagem = leituraImagem(file_in);

    /* Verificar se a imagem foi lida corretamente */
    if (imagem) {
        printf("Imagem lida com sucesso: %dx%d, Max Pixel: %d, Formato: %s\n", 
            imagem->largura, imagem->altura, imagem->max_pix, imagem->formato);
        
        /* Chamar a função de escrita da imagem */
        escritaImagem(file_out, imagem);

        /* Liberação de memória */
        for (int i = 0; i < imagem->altura; i++) {
            free(imagem->pixels[i]);
        }
        free(imagem->pixels);
        free(imagem);
    }

    /* Fechar os arquivos */
    fclose(file_in);
    fclose(file_out);

    return 0;
}
