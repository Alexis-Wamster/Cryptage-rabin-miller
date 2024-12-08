# CHAPITRE 1: BUT FINAL
but: key_gen crypt_rsa crypt_rsa_file

# CHAPITRE 2: VARIABLES
CC = gcc
CFLAGS = -Wall -g
FLAG_LIB = -I./lib
LDFLAGS = -lm
BUILD = build
SRC = src
LIB = lib

# CHAPITRE 3: DEPENDANCES (REGLES IMPLICITES)
$(BUILD)/big.o: $(LIB)/big.c $(LIB)/big.h
	$(CC) $(CFLAGS) $(FLAG_LIB) -c $(LIB)/big.c -o $(BUILD)/big.o

$(BUILD)/utile.o: $(SRC)/utile.c $(SRC)/utile.h
	$(CC) $(CFLAGS) $(FLAG_LIB) -c $(SRC)/utile.c -o $(BUILD)/utile.o

$(BUILD)/key_gen.o: $(SRC)/key_gen.c $(SRC)/utile.c $(SRC)/utile.h
	$(CC) $(CFLAGS) $(FLAG_LIB) -c $(SRC)/key_gen.c -o $(BUILD)/key_gen.o

$(BUILD)/crypt_rsa.o: $(SRC)/crypt_rsa.c $(SRC)/utile.c $(SRC)/utile.h
	$(CC) $(CFLAGS) $(FLAG_LIB) -c $(SRC)/crypt_rsa.c -o $(BUILD)/crypt_rsa.o

$(BUILD)/crypt_rsa_file.o: $(SRC)/crypt_rsa_file.c $(SRC)/utile.c $(SRC)/utile.h
	$(CC) $(CFLAGS) $(FLAG_LIB) -c $(SRC)/crypt_rsa_file.c -o $(BUILD)/crypt_rsa_file.o


# CHAPITRE 4: DEPENDANCES AVEC COMMANDES
key_gen: $(BUILD)/key_gen.o $(BUILD)/big.o $(BUILD)/utile.o
	$(CC) $(CFLAGS) $(BUILD)/key_gen.o $(BUILD)/big.o $(BUILD)/utile.o $(LDFLAGS) -o key_gen

crypt_rsa: $(BUILD)/crypt_rsa.o $(BUILD)/big.o $(BUILD)/utile.o
	$(CC) $(CFLAGS) $(BUILD)/crypt_rsa.o $(BUILD)/big.o $(BUILD)/utile.o $(LDFLAGS) -o crypt_rsa

crypt_rsa: $(BUILD)/crypt_rsa_file.o $(BUILD)/big.o $(BUILD)/utile.o
	$(CC) $(CFLAGS) $(BUILD)/crypt_rsa_file.o $(BUILD)/big.o $(BUILD)/utile.o $(LDFLAGS) -o crypt_rsa_file

# CHAPITRE 5: NETTOYAGE DES FICHIERS GENERES
clean:
	rm -f key_gen crypt_rsa crypt_rsa_file $(BUILD)/*.o

# CHAPITRE 6: BUTS PHONY
.PHONY: but clean
