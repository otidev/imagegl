all: compile run

compile: main.c
	gcc $^ -o program.exe
run:
	./program.exe
clean:
	rm *.exe *.png