CFLAGS = -Wall -Werror -Wextra -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = lbp.o processa_imagem.o
     
all: LBP

LBP: lbp.o processa_imagem.o
	$(CC) -o LBP lbp.o processa_imagem.o $(LDFLAGS)

lpb.o: lbp.c
	$(CC) -c $(CFLAGS) lbp.c

processa_imagem.o: processa_imagem.c
	$(CC) -c $(CFLAGS) processa_imagem.c

clean:
	rm -f $(objects) LBP

