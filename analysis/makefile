CC      = c++ # use the compiler used for compiling Delphes

#===========================================================
# ideally no need to edit below
CFLAGS  = -O2 -pipe -Wall -W -Woverloaded-virtual 
CFLAGS += `root-config --cflags` 
CFLAGS += -I$(DELPHES)/external -I$(DELPHES) -std=c++0x  
LDFLAGS =  `root-config --glibs` -L$(DELPHES) -lDelphes -lEG 

SDIR = src
ODIR = obj
IDIR = src/include
CFLAGS += -I$(IDIR)

PROGRAM = RunDelphes
Files = RunDelphes.o \
		Analysis.o \
		Event.o \
		JetSub.o \
		PFlowJet.o

DEPS =  Event.h \
		Analysis.h \
		JetSub.h \
		PFlowJet.h

OBJ = $(patsubst %, $(ODIR)/%, $(Files))

VPATH = $(SDIR)	: $(IDIR)

all: $(PROGRAM)

$(PROGRAM): $(OBJ)  
	@$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(ODIR)/%.o : %.cc $(DEPS)
	$(CC) $(CFLAGS)  -c -o $@ $<

clean:; @rm -f $(OBJ) $(PROGRAM) core 
