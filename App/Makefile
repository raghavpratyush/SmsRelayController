#Define C Source files in $(PROJDIR)/App
APPCSRC = appinit.c
#APPCSRC += file.c

#Define Asm Source files in $(PROJDIR)/App
APPASRC = 

#Define include directories for system
#INCDIR +=

#Don't edit following parameters
APPMCSRC = $(addprefix $(PROJDIR)/App/,$(APPCSRC))
APPMASRC = $(addprefix $(PROJDIR)/App/,$(APPASRC))

#All object files in current folder
#will get compiled and will be put in objs/other folder
OBJ += $(APPMCSRC:.c=.o) $(APPMASRC:.asm=.o)
