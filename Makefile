NAME = cricket
CC = g++ -fpermissive
CFLAGS = -Ihelper -fopenmp

LDFLAGS = -L./ -lpthread -lm -lrt -ldl -lsqlite3

# Check if the student directory is set
ifndef SDIR
SDIR = build
endif

# Compiler flags for build mode
ifeq ($(mode),debug)
	CFLAGS += -O0 -g
else
	mode = release
	CFLAGS += -O2
endif

SRC = main.c helper/db.c helper/vis.c cricket.c cricket_seq.c cricket_par.c
OBJ_SEQ = $(SDIR)/main.o $(SDIR)/db.o $(SDIR)/vis.o $(SDIR)/$(NAME).o $(SDIR)/$(NAME)_seq.o
OBJ_PAR = $(SDIR)/main.o $(SDIR)/db.o $(SDIR)/vis.o $(SDIR)/$(NAME).o $(SDIR)/$(NAME)_par.o

# The dependency file names
DEPS := $(OBJ_SEQ:.o=.d)

all: sequential parallel

sequential: $(SDIR)/$(NAME)_seq

parallel: $(SDIR)/$(NAME)_par

$(SDIR)/$(NAME)_seq : $(OBJ_SEQ)
	$(CC) -o $@ $(OBJ_SEQ) $(LDFLAGS)

$(SDIR)/$(NAME)_par : $(OBJ_PAR)
	$(CC) -o $@ $(OBJ_PAR) -fopenmp $(LDFLAGS)

$(SDIR)/%.o: %.c
	$(CC) -c -MMD -MP $< -o $@ $(CFLAGS)

$(SDIR)/%.o: helper/%.c
	$(CC) -c -MMD -MP $< -o $@ $(CFLAGS)

clean:
	rm -f $(SDIR)/$(NAME)_seq $(SDIR)/$(NAME)_par $(SDIR)/*.o $(SDIR)/*.d $(SDIR)/runtime

-include $(DEPS)

.PHONY : all sequential parallel unit_test checks clean
