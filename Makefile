# Consts:
# IncludeDIR = *.hpp DIR
# CC = gcc or g++
# DB = gdb
# CFLAGS = comp line flags (-ansi = ansi law; -Wall, -Wextra = warnings; -I./ = *.h DIR)
# ObjDIR = *.o DIR
# LibDIR = lib DIR
# LibDIR -> enable when using, put $(LIBS) at end of gogo's command
LibDIREC =./lib/
LibDIR =./lib/
IncludeDIR =./include/
CppDIR=./src/
ObjDIR=./src/obj/
CC=g++
CFLAGS=-ansi -Wall -Wextra -I$(IncludeDIR)
CFLAGSTEST=-ansi -Wall -Wextra -I$(IncludeDIR)
# LIBS=-lgtest

# Vars:
# -- ADAPT THIS IN YOUR PROGRAM --
headers = 
mainObject = main
objects = 

# Set of *.hpp on which the *.cpp depend
_DEPS = $(headers)
DEPS = $(patsubst %,$(IncludeDIR)%,$(_DEPS))

# Set of *.o
_OBJ = $(mainObject).o $(objects)
OBJ = $(patsubst %,$(ObjDIR)%,$(_OBJ))

# Gathers *.o
$(ObjDIR)%.o: $(CppDIR)%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

# Creates executable (Linux)
$(mainObject): $(OBJ)
	$(CC) -g $^ $(CFLAGS) $(LIBS) -o aracne


# Gets the files and organize in the lib include src/obj structure
.PHONY: prepareDIR
prepareDIR:
	mkdir -p $(LibDIREC)
	mkdir -p $(IncludeDIR)
	mkdir -p $(CppDIR)
	mkdir -p $(ObjDIR)
	mv *.hpp $(IncludeDIR); true
	mv *.cpp $(CppDIR); true

# Call for run
.PHONY: run
run:
	./aracne

# Call for *.o clean up
.PHONY: clean
clean:
	rm -f $(ObjDIR)*.o
	rm -f ./aracne

	

# Call for help with this makefile's commands
.PHONY: help
help:
	@echo "\n\t Makefile\n"
	@echo " make............= compiles aracne "
	@echo " make prepareDIR.= prepares project in the "lib include src/obj" structure (use this if all files are with this makefile)"
	@echo " make clean......= removes objects from obj directory\n"
	@echo " make run........= roda aracne no formato padrao (porta 8228)" 
	@echo "\n\t End of Makefile Help\n"

