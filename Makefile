WATCOM = C:\WATCOM
CC = wcl386
CFLAGS = -bt=nt -zastd=c99 -zq -i="$(WATCOM)\h" -i="$(WATCOM)\h\nt" -i=src -i=deps\inih -i=deps\bearssl\inc -i=deps\bearssl\src

all: bin\9xget.exe .SYMBOLIC

bin\deps.lst:
	@if not exist bin mkdir bin
	@cmd /c "dir /s /b deps\*.c > bin\deps.lst"
	$(CC) -c $(CFLAGS) -fo=bin\ @bin\deps.lst

bin\9xget.exe: bin\deps.lst src\*.c src\*.h
	@if not exist bin mkdir bin
	$(CC) -c $(CFLAGS) -fo=bin\ src\*.c
	wlink name $@ system win95 runtime console=4.0 option quiet file bin\*.obj

clean: .SYMBOLIC
	-@if exist bin\* del /q bin\*
