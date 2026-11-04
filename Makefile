!ifdef __UNIX__
WATCOM_DEF = /opt/watcom
MKDIR = mkdir -p bin
RM = -rm -f bin/*
S = /
LINK_FILES = file { bin/*.o }
!else
WATCOM_DEF = C:\WATCOM
MKDIR = @if not exist bin mkdir bin
RM = -@if exist bin\* del /q bin\*
S = \
LINK_FILES = file bin\*.obj
!endif

!ifndef WATCOM
!ifdef %WATCOM
WATCOM = $(%WATCOM)
!else
WATCOM = $(WATCOM_DEF)
!endif
!endif

CC = wcl386
CFLAGS = -bt=nt -zastd=c99 -zq -i="$(WATCOM)$(S)h" -i="$(WATCOM)$(S)h$(S)nt" -i=src -i=deps$(S)inih

!ifdef SKIP_SYSTEM_CHECK
CFLAGS += -dSKIP_SYSTEM_CHECK
!endif

all: bin$(S)9xget.exe .SYMBOLIC

bin$(S)deps.marker:
	$(MKDIR)
	$(CC) -c $(CFLAGS) -fo=bin$(S) deps$(S)inih$(S)*.c
	@echo 1 > bin$(S)deps.marker

bin$(S)9xget.exe: bin$(S)deps.marker src$(S)*.c src$(S)*.h
	$(MKDIR)
	$(CC) -c $(CFLAGS) -fo=bin$(S) src$(S)*.c
	wlink name $@ system win95 runtime console=4.0 option quiet library wsock32.lib $(LINK_FILES)

clean: .SYMBOLIC
	$(RM)
