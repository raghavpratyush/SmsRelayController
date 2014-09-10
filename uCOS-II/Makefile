#UCOS-II OS Folder Makefile
#--------------------------

# Define C Source files in $(PROJDIR)/uCOS-II OS Folder
UCOSCSRC = Source/ucos_ii.c
UCOSCSRC += Ports/AVR/Atmega32/os_cpu_c.c

#Define Asm Source files in $(PROJDIR)/uCOS-II OS Folder
UCOSASRC = Ports/AVR/Atmega32/os_cpu_a.asm

#Define include directories for system
#INCDIR += $(PROJDIR)/uCOS-II/Source
#INCDIR += $(PROJDIR)/uCOS-II/Ports/AVR/Atmega32

#Don't edit following parameters
UCOSMCSRC = $(addprefix $(PROJDIR)/uCOS-II/,$(UCOSCSRC))
UCOSMASRC = $(addprefix $(PROJDIR)/uCOS-II/,$(UCOSASRC))

#All object files in current folder
#will get compiled and will be put in objs/other folder
OBJ += $(UCOSMCSRC:.c=.o) $(UCOSMASRC:.asm=.o)
