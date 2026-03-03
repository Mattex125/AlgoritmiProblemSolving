# Variabili
# Makefile per un progetto C che utilizza moduli esterni
# con librerie mod1 e mod2
# Questo Makefile compila i file .c presenti nella cartella corrente
# e li linka con le librerie mod1 e mod2.
# Utilizza il compilatore gcc e le opzioni di debug e warning
# per garantire una compilazione sicura e informativa.
# Variabili per il compilatore, le opzioni di compilazione e link
# e i file sorgente e oggetto

# variabile per il compilatore C
CC = g++  
# opzioni di compilazione
# -Wall abilita tutti i warning, -g aggiunge informazioni di debug
CFLAGS = -Wall -g 
 # linker
LD = g++
# opzioni di link
# -lm linka la libreria matematica
LDFLAGS = -lm 
# File sorgente e oggetto
# Utilizza wildcard per prendere tutti i file .c nella cartella corrente
# e crea i file .o corrispondenti
SRC = $(wildcard *.c)
# Sostituisce l'estensione .cc con .o per i file oggetto
OBJ = $(SRC:.c=.o)
INC = $(wildcard *.h)
TARGET = $(basename $(firstword $(SRC)))
# Librerie esterne
# Le librerie mod1 e mod2 sono necessarie per il progetto
# LIB = -lmod1 -lmod2

# Regole di compilazione e link
# Compila i file .cc in file .o e poi linka i file .o in un eseguibile
# $< è il primo prerequisito della regola ( ovvero un file sorgente) , $@ è il nome del target

# Regola per compilare i file .cc in file .o
# Questa regola utilizza il compilatore C e le opzioni di compilazione
# per creare i file oggetto .o dai file sorgente .cc
# La regola è applicata a tutti i file .cc presenti nella cartella corrente
# e crea i file .o corrispondenti nella stessa cartella



%.o: %.cc $(INC) #indica che per ogni file sorgente con estensione .cc, verrà generato un file oggetto con estensione .o
	$(CC) $(CFLAGS) -c $< -o $@



# Pattern Matching con %:
# Il simbolo % è un wildcard che rappresenta una parte generica del nome del file.
#  Ad esempio, se hai un file main.cc, il % corrisponderà a main, e la regola genererà main.o.




$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $@

# Regola di link
all: $(TARGET)


# Regola di pulizia
clean:
	rm -f $(OBJ) $(TARGET)