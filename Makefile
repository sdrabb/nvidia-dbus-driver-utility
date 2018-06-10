.PHONY: clean

BUILD_FOLDER = ./build
BINARY_NAME = nvidia-service

all: binary configure

binary:
	mkdir -p $(BUILD_FOLDER)
	gcc -Wextra -Wall main.c nvidia-service.c -o $(BUILD_FOLDER)/$(BINARY_NAME) \
		-ldbus-1 \
		-I/usr/include/dbus-1.0 \
		-I/usr/lib/x86_64-linux-gnu/dbus-1.0/include \
		-I/usr/include/glib-2.0 \
		-I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ \
		-ldbus-glib-1 \
		-lcurl
		

configure:
	cp -f $(BINARY_NAME).conf /etc/dbus-1/system.d/
	
	

clean:
	rm -r $(BUILD_FOLDER)




