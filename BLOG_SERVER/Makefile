EXE:=testver
DIR:=./SERVER_RUN/
DIR2:=./cJSON/
SOURCE2:=$(wildcard ./cJSON/*.c *.c)
SOURCE:=$(notdir $(SOURCE2))
OBJES:=$(patsubst %.c,%.o, $(SOURCE))
MOBJES:=$(addprefix $(DIR),$(OBJES))
$(DIR)$(EXE):$(MOBJES)
	@$(CXX) -o $@ $^ -lpthread -lm 
	@rm -rf $(DIR)*.o
$(DIR)%.o:%.c
	@$(CXX) -o $@ -c $^
$(DIR)%.o:$(DIR2)%.c
	@$(CXX) -o $@ -c $^
.PHONY:clean rebuild
clean:
	@rm -rf $(DIR)*
rebuild:clean $(DIR)$(EXE)

