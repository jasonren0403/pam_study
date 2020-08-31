SRC_DIR = ./src
BUILD_DIR = ./build
OBJ_DIR = ./shared_so
PAM_DIR = /lib/x86_64-linux-gnu/security
CC = gcc
CFLAGS = -fPIC -shared -lpam -lcrypt -lbsd
# gcc -o ./shared_so/simplelogin.so -shared -fPIC ./src/simplelogin.c ./src/ban.c ./src/changepw.c ./src/readconf.c -lpam -lcrypt -lbsd && make copy
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.so)

all:
	@mkdir -p $(BUILD_DIR)
	gcc $(SRC) -fPIC -shared -o $(OBJ)
	# -lcrypt authDemo.c,simplelogin.c
getItem:
	@mkdir -p $(BUILD_DIR)
	gcc -o $(OBJ_DIR)/getitem.so -shared -fPIC ./src/demos/getItem.c -lpam
auth:
	@mkdir -p $(BUILD_DIR)
	gcc -o $(OBJ_DIR)/linux_auth.so -shared -fPIC ./src/auth.c -lpam -lcrypt
final_so:
	@mkdir -p $(BUILD_DIR)
	gcc -o $(OBJ_DIR)/simplelogin.so -shared -fPIC ./src/simplelogin.c ./src/ban.c ./src/config.c ./src/utils.c -lpam -lcrypt -lbsd -lccl
bancheck:
	@mkdir -p $(BUILD_DIR)
	gcc -o $(BUILD_DIR)/ban ./src/main/ban_main.c ./src/utils.c ./src/ban.c -lbsd
conftool:
	@mkdir -p $(BUILD_DIR)
	gcc -o $(BUILD_DIR)/readconf ./src/main/config_main.c ./src/config.c ./src/utils.c -lccl
clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(wildcard $(OBJ_DIR)/*.so)
	@sudo rm -rf $(PAM_DIR)/simplelogin.so
	@sudo rm -rf $(PAM_DIR)/linux_auth.so
	@sudo rm -rf $(PAM_DIR)/getitem.so
copy:
	@sudo cp -r ./conf/banned_list /etc/banned_list
	@sudo cp -r ./conf/mylogin.conf /etc/mylogin.conf
	@sudo cp -r $(wildcard $(OBJ_DIR)/*.so) /lib/x86_64-linux-gnu/security/
