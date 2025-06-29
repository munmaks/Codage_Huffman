CC = gcc
# -ansi 
CFLAGS = -std=c17 -pedantic -Wall -Wfatal-errors -Werror -Wextra -finline-functions -funroll-loops
CFLAGS_OPTIONALS = -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
OPT = -O2
ARCHIVE = codage_HUFFMAN

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

EXE = huffman
RESULT = resultat.DEC
LIBRARY_MTRACK := $(shell echo $$MTRACK) 

SRC = $(wildcard $(SRC_DIR)/*.c)

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

INC = -I $(INC_DIR)


all: $(BIN_DIR)/$(EXE)

$(BIN_DIR)/$(EXE): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(CFLAGS) $(OPT) $(CFLAGS_OPTIONALS) $(LIBRARY_MTRACK)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(OPT) $(INC) -c -o $@ $< $(LIBRARY_MTRACK)


# $(OBJ_DIR):
# 	mkdir -p $(OBJ_DIR)



dist: mrproper
	mkdir -p $(ARCHIVE)
	cp -R makefile crime-et-chatiment.txt $(SRC_DIR) $(INC_DIR) $(ARCHIVE)
	zip -r $(ARCHIVE).zip $(ARCHIVE)
	rm -rf $(ARCHIVE)


clean:
	@$(RM) fichier_bin.HUF
	@$(RM) resultat.DEC
	@$(RM) $(OBJ_DIR)/*.o
	@$(RM) $(BIN_DIR)/$(EXE)

mrproper: clean
	@$(RM)r $(OBJ_DIR)
	@$(RM)r $(BIN_DIR)

uninstall: mrproper
	rm -f $(ARCHIVE).zip

.PHONY: all clean mrproper uninstall dist
