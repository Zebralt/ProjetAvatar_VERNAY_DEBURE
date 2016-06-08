INCPATH = -I /usr/local/include -I /usr/X11R6/include -I ./src
LIBPATH = -L /usr/local/lib -L /usr/X11R6/lib
LIBRARY = -lglut -lGLU -lGL -lX11 -lXext -lXi -lXmu -lm -ljpeg -lpng

CFLAGS = $(INCPATH)
LDFALGS = $(LIBPATH) $(LIBRARY)
GCC=/usr/bin/gcc
SRC=src

all:
	@echo "Specifiez une cible"

%.o:$(SRC)/%.c
	$(GCC) $(CFLAGS) -o $@ -c $<

%:%.o $(SRC)/%.c
	$(GCC) $(LDFALGS) -o $@ texjpeg.o texpng.o $< 

# cette commande permet de supprimer tous les objets
clean:
	@rm -f `/bin/ls *.o | grep -v Makefile | grep -v $(SRC)`