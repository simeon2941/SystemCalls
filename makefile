
#

# Compiler variables
CCFLAGS = -Wall

# Rule to link object code files to create executable file
assignment1.exe: assignment2.cc
	g++ $(CCFLAGS) assignment2.cc -o assignment2.exe

clean:
	-rm *.o assignment2.exe

