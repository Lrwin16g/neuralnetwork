# Makefile

CC = g++
CFLAGS = -O

EXEDIR = ./bin
OBJDIR = ./obj
SRCDIR = ./src

TARGET1 = $(EXEDIR)/train_bpnetwork
#TARGET2 = $(EXEDIR)/train_haar_adaboost

OBJ1 = $(OBJDIR)/train_bpnetwork.o $(OBJDIR)/bpnetwork.o $(OBJDIR)/sample.o $(OBJDIR)/filelib.o $(OBJDIR)/mllib.o
#OBJ2 = $(OBJDIR)/train_haar_adaboost.o $(OBJDIR)/adaboost.o $(OBJDIR)/haar.o $(OBJDIR)/filelib.o

all: $(TARGET1) #$(TARGET2)

$(TARGET1): $(OBJ1)
	$(CC) -o $(TARGET1) $^

#$(TARGET2): $(OBJ2)
#	$(CC) $(LIBS) -o $(TARGET2) $^

$(OBJDIR)/train_bpnetwork.o: $(SRCDIR)/train_bpnetwork.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/bpnetwork.o: $(SRCDIR)/bpnetwork.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/sample.o: $(SRCDIR)/sample.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/filelib.o: $(SRCDIR)/filelib.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/mllib.o: $(SRCDIR)/mllib.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET1) $(OBJDIR)/*.o
