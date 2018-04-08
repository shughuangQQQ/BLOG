EXE:=testver
DIR:=./SERVER_RUN/
DIR2:=./cJSON/
SOURCE2:=$(wildcard ./cJSON/*.c *.c)
SOURCE:=$(notdir $(SOURCE2))
OBJES:=$(patsubst %.c,%.o, $(SOURCE))
MOBJES:=$(addprefix $(DIR),$(OBJES))
$(DIR)$(EXE):$(MOBJES)
	@$(CC) -o $@ $^ -lpthread -lm 
	@rm -rf $(DIR)*.o
$(DIR)%.o:%.c
	@$(CC) -o $@ -c $^
$(DIR)%.o:$(DIR2)%.c
	@$(CC) -o $@ -c $^
.PHONY:clean rebuild
clean:
	@rm -rf $(DIR)*
rebuild:clean $(DIR)$(EXE)

