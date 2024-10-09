CFLAGS = -Wall -Werror -Wextra -g  # flags de compilacao
LDFLAGS = -lm
ENTREGA = LBP

CC = gcc

# arquivos-objeto
	objects = lbp.o
     
all: lbp

lbp: lbp.o
	$(CC) -o lbp lbp.o $(LDFLAGS)

# arquivo TGZ para entregar
entrega: clean
	mkdir $(ENTREGA)
	cp *.c *.h makefile $(ENTREGA)
	chmod 600 $(ENTREGA)/*
	tar czvf $(ENTREGA).tgz $(ENTREGA)
	rm -rf $(ENTREGA)
	@echo "Arquivo $(ENTREGA).tgz criado para entrega"

clean:
	rm -f $(objects) lbp
