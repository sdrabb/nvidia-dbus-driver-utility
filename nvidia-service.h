#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* somewhat unix-specific */ 
#include <sys/time.h>
#include <unistd.h>

#include <dbus/dbus.h>
#include <curl/curl.h>
 


#define URL_NVIDIA_DRIVER "http://it.download.nvidia.com/XFree86/Linux-x86_64/390.67/NVIDIA-Linux-x86_64-390.67.run"
#define DL_PATH "./build/nvidia-driver.sh"

#define CMD_CHANGE_SYSTEMD_TARGET "systemctl isolate runlevel2.target"
#define CMD_RUN_DRIVER "./build/nvidia-driver.sh"
#define DATA_SIZE 200

void check_and_abort(DBusError *error);





void respond_to_introspect(DBusConnection *connection, DBusMessage *request);
DBusHandlerResult tutorial_messages(DBusConnection *connection, DBusMessage *message, void *user_data);


// dbus methods implementations


// sum a and b
void respond_to_sum(DBusConnection *connection, DBusMessage *request);


// avoid loop in login page ubuntu on nvidia driver update
void respond_to_driver_installation(DBusConnection *connection, DBusMessage *request);
int download_link(const char * link, const char * dl_path);
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
int change_systemd_target_and_run_driver();

// given the byte a check if in position b  the parameter a has a bit setted to one
void respond_to_bit_checking(DBusConnection *connection, DBusMessage *request);


typedef struct cfg {
    const char * nvidia_dl_link;
    const char * nvidia_dl_path;
} DlCfg;

