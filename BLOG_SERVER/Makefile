EXE:=testver
DIR:=./SERVER_RUN/
SOURCE:=$(wildcard *.c)
OBJES:=$(patsubst %.c,%.o, $(SOURCE))
MOBJES:=$(addprefix $(DIR),$(OBJES))

$(DIR)$(EXE):$(MOBJES)
	@$(CC) -o $@ $^ -lpthread
	@rm -rf $(DIR)*.o
$(DIR)%.o:%.c
	@$(CC) -o $@ -c $^
.PHONY:clean rebuild
clean:
	@rm -rf $(DIR)*
rebuild:clean $(DIR)$(EXE)
	
