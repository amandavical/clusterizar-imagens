CC = gcc
CFLAGS = -Wall -O3 -march=native


SRC = main.c pgm.c utils.c kmeans.c


OBJ = $(SRC:.c=.o)


EXEC = prog.e


all: $(EXEC)


$(EXEC): $(OBJ)
	@echo "Compilando executável..."
	@$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)
	@echo "Compilação concluída!"


%.o: %.c
	@echo "Compilando $<..."
	@$(CC) $(CFLAGS) -c $<


clean:
	@echo "Limpando arquivos..."
	@rm -f $(OBJ) $(EXEC)