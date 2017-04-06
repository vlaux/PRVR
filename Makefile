all: build run

build:
	/usr/local/bin/g++-6 main.cpp leitor_arquivos.cpp

run:
	./a.out