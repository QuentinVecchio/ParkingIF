#*******************************************************************************#
#                           Makefile  -  description                     		#
#                            -------------------	            		 		#
#   début                : 12/01/2016											#
#   copyright            : (C) 2015 par Adrien Lepic, Marcin Quentin Vecchio	#
#*******************************************************************************#
CC=g++
EDL=g++
EXECNAME=Parking
DEBUG=#-DMAP
CPPFLAGS=-I -g -O2 -Wall -static -std=c++11
EDLFLAGS=-g -O2 -Wall
RM=@rm
ECHO=@echo
RMFLAGS=-f
OUTPUT=-o
INT= GestionClavier.h Mere.h Entree.h Sortie.h
CPP=$(INT:.h=.cpp)
OBJ=$(INT:.h=.o)
CLEAR=clear
LIBS=-L .
LIBPATH=-ltp -ltcl8.6 -lncurses
INCPATH=

all : $(EXECNAME)

$(EXECNAME) : $(OBJ)
	$(ECHO) "Edition des liens de $(EXECNAME)"
	$(EDL) $(EDLFLAGS) $(OBJ) $(LIBS) $(LIBPATH) $(OUTPUT) $(EXECNAME)

Mere.o : GestionClavier.h Entree.h Sortie.h

%.o : %.cpp
	$(ECHO) "Compilation de $<"
	$(CC) $(DEBUG) $(CPPFLAGS) $(INCPATH) -c $<

.PHONY : $(CLEAR)

$(CLEAR):
	$(RM) $(RMFLAGS) $(OBJ) Mere.o $(EXECNAME)
