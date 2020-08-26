SRC_DIR = ./src
BUILD_DIR = ./build
OBJ_DIR = ./shared_so
CC = gcc
CFLAGS = -fPIC -shared -lpam -lcrypt -lbsd
# gcc -o ./shared_so/simplelogin.so -shared -fPIC ./src/simplelogin.c ./src/ban.c ./src/changepw.c ./src/readconf.c -lpam -lcrypt -lbsd && make copy
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.so)

all:
	gcc $(SRC) -fPIC -shared -o $(OBJ)
	# -lcrypt authDemo.c,simplelogin.c
final_so:
	gcc -o ./shared_so/simplelogin.so -shared -fPIC ./src/simplelogin.c ./src/ban.c ./src/changepw.c ./src/config.c ./src/utils.c -lpam -lcrypt -lbsd -lccl
$(OBJ_DIR)/%.so: $(SRC_DIR)/%.c 
	@echo + CC $<
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c -o $@ $<
bancheck:
	gcc -o ./ban ./src/ban_main.c ./src/utils.c ./src/ban.c
conftool:
	gcc -o ./readconf ./src/config.c ./src/utils.c -lccl
clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(wildcard $(OBJ_DIR)/*.so)
	@sudo rm -rf /lib/x86_64-linux-gnu/security/simplelogin.so
copy:
	@sudo cp -r ./conf/banned_list /etc/banned_list
	@sudo cp -r ./conf/mylogin.conf /etc/mylogin.conf
	@sudo cp -r $(wildcard $(OBJ_DIR)/*.so) /lib/x86_64-linux-gnu/security/
