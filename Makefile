WATCOM = C:\WATCOM
CC = wcl386
CFLAGS = -bt=nt -zastd=c99 -zq -i="$(WATCOM)\h" -i="$(WATCOM)\h\nt" -i=src

all: bin\9xget.exe .SYMBOLIC

bin\9xget.exe: src\*.c src\*.h
	@if not exist bin mkdir bin
	$(CC) -c $(CFLAGS) -fo=bin\ src\*.c
	wlink name $@ system win95 runtime console=4.0 option quiet file bin\*.obj

clean: .SYMBOLIC
	-@if exist bin\* del /q bin\*
