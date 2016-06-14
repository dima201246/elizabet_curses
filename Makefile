VERSION_EL = 4.0

CC = g++
FLAGS = -Wall -c
ARCH = $(shell getconf LONG_BIT)

LDFLAGS	= -Llib -lconf_lang_x$(ARCH) -lcalc_x$(ARCH) -Wl,-rpath,lib -lncurses

vpath %.cpp ./header

EXE := elizabet_debug_x$(ARCH)
EXE_REL := elizabet_x$(ARCH)

MAIN := main.cpp
LR := last_result.cpp
AD := angry_debug.cpp
EXW := extern_work.cpp
TERMOP := terminal_op.cpp
UIN := user_input.cpp
SAP := sapper.cpp

MAINOBJ := $(patsubst %.cpp,%.o,$(MAIN))
LROBJ := $(patsubst %.cpp,%.o,$(LR))
ADOBJ := $(patsubst %.cpp,%.o,$(AD))
EXWOBJ := $(patsubst %.cpp,%.o,$(EXW))
TERMOPOBJ := $(patsubst %.cpp,%.o,$(TERMOP))
UINOBJ := $(patsubst %.cpp,%.o,$(UIN))
SAPOBJ := $(patsubst %.cpp,%.o,$(SAP))

all: $(EXE)

realise: $(EXE_REL) clean-all

start: all
		./$(EXE)

# $^ - имена всех пререквизитов.  $? - именна тех пререквизитов что были обновлены


$(EXE_REL): $(MAINOBJ) $(LROBJ) $(ADOBJ) $(EXWOBJ) $(TERMOPOBJ) $(UINOBJ) $(SAPOBJ)
		$(CC) -O2 $^ -o $@ $(LDFLAGS)  -m$(ARCH)
		chmod u=rwx,g=rx,o=rx ./$(EXE_REL)

$(EXE): $(MAINOBJ) $(LROBJ) $(ADOBJ) $(EXWOBJ) $(TERMOPOBJ) $(CALCOBJ) $(UINOBJ) $(SAPOBJ)
		$(CC) $^ -o $@ $(LDFLAGS)  -m$(ARCH)
		chmod u=rwx,g=rx,o=rx ./$(EXE)

$(MAINOBJ):	$(MAIN)
		$(CC) -Darch_l=\"$(ARCH)\" -D_version_el=\"$(VERSION_EL)\" $(FLAGS) $< -o $@  -m$(ARCH)

$(LROBJ): $(LR)
		$(CC) $(FLAGS) $< -o $@  -m$(ARCH)

$(ADOBJ): $(AD)
		$(CC) $(FLAGS) $< -o $@  -m$(ARCH)

$(EXWOBJ): $(EXW)
		$(CC) $(FLAGS) $< -o $@  -m$(ARCH)

$(TERMOPOBJ): $(TERMOP)
		$(CC) $(FLAGS) $< -o $@  -m$(ARCH)

$(UINOBJ): $(UIN)
		$(CC) $(FLAGS) $< -o $@ -m$(ARCH)

$(SAPOBJ): $(SAP)
		$(CC) $(FLAGS) $< -o $@ -m$(ARCH)

clean:
		rm -f *.o

clean-all:
		rm -f *.o
		rm $(EXE)