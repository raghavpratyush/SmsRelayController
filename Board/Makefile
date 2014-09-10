#Define C Source files in $(PROJDIR)/Board
BOARDCSRC = boardinit.c
BOARDCSRC += Uart/uart.c
BOARDCSRC += Lcd/lcd.c
BOARDCSRC += Gsm/gsm.c


#Define Asm Source files in $(PROJDIR)/Board
BOARDASRC = 

#Define include directories for system
#INCDIR +=

#Don't edit following parameters
BOARDMCSRC = $(addprefix $(PROJDIR)/Board/,$(BOARDCSRC))
BOARDMASRC = $(addprefix $(PROJDIR)/uCOS-II/,$(BOARDASRC))

#All object files in current folder
#will get compiled and will be put in objs/other folder
OBJ += $(BOARDMCSRC:.c=.o) $(BOARDMASRC:.asm=.o)
