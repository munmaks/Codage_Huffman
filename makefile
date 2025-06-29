RED      := \e[0;31m
GREEN    := \e[0;32m
YELLOW   := \e[0;33m
BLUE     := \e[0;34m
PURPLE   := \e[0;35m
CYAN     := \e[0;36m
WHITE    := \e[0;37m
BOLD     := \e[1m
RESET    := \e[0m
ITALIC   := \e[3m
FLASH    := \e[5m
BG_RED   := \e[41m
BG_GREEN := \e[42m

CC = gcc
CFLAGS = -std=c17 -Wall -ansi
OPT := -O2
ARCHIVE = codage_HUFFMAN

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

INC = -I $(INC_DIR)

EXE = huffman
RESULT = resultat.DEC

LIBRARY_MTRACK = 
ADVANCED_CFLAGS = 

ifeq ($(DEV), 1)
	OPT := 
	ADVANCED_CFLAGS += -Wextra -Wfatal-errors -Werror -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align 
	ADVANCED_CFLAGS += -Wstrict-overflow=5 -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum 
	ADVANCED_CFLAGS += -Wconversion -Wunreachable-code -Winit-self -Werror-implicit-function-declaration 
	ADVANCED_CFLAGS += -finline-functions -funroll-loops -Wformat=2 -Wpacked -Wredundant-decls -Wstrict-prototypes 
	ADVANCED_CFLAGS += -Wwrite-strings -Wformat-nonliteral -Wformat-security -Wstringop-overflow -pedantic 
endif

ifeq ($(MEM), 1)
	CFLAGS = -std=c17 -Wall
	LIBRARY_MTRACK = $(shell echo $$MTRACK)
endif

DEPENDENCIES = $(OBJ_DIR)/bits_operations.o $(OBJ_DIR)/codage.o
DEPENDENCIES += $(OBJ_DIR)/decompres.o $(OBJ_DIR)/main.o

all: $(BIN_DIR)/$(EXE)

$(BIN_DIR)/$(EXE): $(DEPENDENCIES)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(CFLAGS) $(OPT) $(ADVANCED_CFLAGS) $(LIBRARY_MTRACK)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(ADVANCED_CFLAGS) $(OPT) $(INC) -c -o $@ $< $(LIBRARY_MTRACK)


# $(OBJ_DIR):
# 	mkdir -p $(OBJ_DIR)

dev:
	@echo "$(BLUE)$(BOLD)DEVELOPMENT MODE$(RESET)"
	@$(MAKE) -B DEV=1

mem:
	@echo "$(BLUE)$(BOLD)MEMORY TRACKING MODE$(RESET)"
	@$(MAKE) -B MEM=1

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

.PHONY: all clean mrproper uninstall dist dev mem

# end of makefile
