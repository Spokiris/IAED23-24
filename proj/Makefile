# Nome do arquivo de saída
OUTPUT = projname

# Arquivos de origem
SOURCES = main.c app.c

# Compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall -Wextra -Wno-implicit-function-declaration 

# Regra padrão
all: $(OUTPUT)

# Regra para compilar o programa
$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para limpar os arquivos compilados
clean:
	rm -f $(OUTPUT)