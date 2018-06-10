#include "nvidia-service.h"
#include <stdbool.h>

void check_and_abort(DBusError *error) {
	if (dbus_error_is_set(error)) {
		puts(error->message);
		abort();
	}
}

DBusHandlerResult tutorial_messages(DBusConnection *connection, DBusMessage *message, void *user_data) {
	const char *interface_name = dbus_message_get_interface(message);
	const char *member_name = dbus_message_get_member(message);
	
	if (0==strcmp("org.freedesktop.DBus.Introspectable", interface_name) &&
	    0==strcmp("Introspect", member_name)) {

		respond_to_introspect(connection, message);
		return DBUS_HANDLER_RESULT_HANDLED;
	} else if (0==strcmp("it.nvidia.service.DBusService", interface_name) &&
		   0==strcmp("Sum", member_name)) {
		
		respond_to_sum(connection, message);
		return DBUS_HANDLER_RESULT_HANDLED;
	} else if(0==strcmp("it.nvidia.service.DBusService", interface_name)&&
		   0==strcmp("DlDriver", member_name)){
        respond_to_driver_installation(connection,message);
    
	}  else if(0==strcmp("it.nvidia.service.DBusService", interface_name)&&
		   0==strcmp("BitChecking", member_name)){
        respond_to_bit_checking(connection,message);
    }
    
    else {
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}
}

void respond_to_introspect(DBusConnection *connection, DBusMessage *request) {
	DBusMessage *reply;

	const char *introspection_data =
		" <!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\" "
		"\"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">"
		" <!-- dbus-sharp 0.8.1 -->"
		" <node>"
		"   <interface name=\"org.freedesktop.DBus.Introspectable\">"
		"     <method name=\"Introspect\">"
		"       <arg name=\"data\" direction=\"out\" type=\"s\" />"
		"     </method>"
		"   </interface>"
		"   <interface name=\"it.nvidia.service.DBusService\">"
		"     <method name=\"Sum\">"
		"       <arg name=\"a\" direction=\"in\" type=\"i\" />"
		"       <arg name=\"b\" direction=\"in\" type=\"i\" />"
		"       <arg name=\"ret\" direction=\"out\" type=\"i\" />"
		"     </method>"
        "     <method name=\"BitChecking\">"
		"       <arg name=\"a\" direction=\"in\" type=\"y\" />"
		"		<arg name=\"b\" direction=\"in\" type=\"i\" />"
		"       <arg name=\"ret\" direction=\"out\" type=\"i\" />"
		"     </method>"
        "     <method name=\"DlDriver\">"
		"       <arg name=\"ret\" direction=\"out\" type=\"i\" />"
		"     </method>"
		"   </interface>"
		" </node>";
	
	reply = dbus_message_new_method_return(request);
	dbus_message_append_args(reply,
				 DBUS_TYPE_STRING, &introspection_data,
				 DBUS_TYPE_INVALID);
	dbus_connection_send(connection, reply, NULL);
	dbus_message_unref(reply);
}


void respond_to_sum(DBusConnection *connection, DBusMessage *request) {
	DBusMessage *reply;
	DBusError error;
	int a=0, b=0, ret=0;

	dbus_error_init(&error);

	dbus_message_get_args(request, &error,
			      DBUS_TYPE_INT32, &a,
			      DBUS_TYPE_INT32, &b,
			      DBUS_TYPE_INVALID);
	if (dbus_error_is_set(&error)) {
		reply = dbus_message_new_error(request, "wrong_arguments", "Illegal arguments to Sum");
		dbus_connection_send(connection, reply, NULL);
		dbus_message_unref(reply);
		return;
	}

	ret = a+b;
	
	reply = dbus_message_new_method_return(request);
	dbus_message_append_args(reply,
				 DBUS_TYPE_INT32, &ret,
				 DBUS_TYPE_INVALID);
	dbus_connection_send(connection, reply, NULL);
	dbus_message_unref(reply);
}

void respond_to_bit_checking(DBusConnection *connection, DBusMessage *request) {

	DBusMessage *reply;
	DBusError error;
	int ret=0;
	char a = 0;
	int b = 0;

    char bit = 0x01;
	int bits = sizeof(char) * 8 ;
	int i;

	fprintf(stdout, "NVIDIA DRIVER WILL BE DOWNLOADED IN ---> %d \n",bits);

	dbus_error_init(&error);

	dbus_message_get_args(request, &error,
			      DBUS_TYPE_BYTE, &a,
				  DBUS_TYPE_INT32, &b,
			      DBUS_TYPE_INVALID);
	if (dbus_error_is_set(&error)) {
		reply = dbus_message_new_error(request, "wrong_arguments", "Illegal arguments to Sum");
		dbus_connection_send(connection, reply, NULL);
		dbus_message_unref(reply);
		return;
	}

	
	for (i=0; i<bits; i++ ){
		
		if (((a & bit) == 1) && i==b){
			ret = 1;
			break;
		}
		a = a >> 1;

	}

	reply = dbus_message_new_method_return(request);
	dbus_message_append_args(reply,
				 DBUS_TYPE_INT32, &ret,
				 DBUS_TYPE_INVALID);
	dbus_connection_send(connection, reply, NULL);
	dbus_message_unref(reply);
}


void respond_to_driver_installation(DBusConnection *connection, DBusMessage *request) {
	DBusMessage *reply;
	DBusError error;
	int ret;

    fprintf(stderr, "test ok \n");

    // download link

    
    DlCfg * dl_cfg;
    dl_cfg->nvidia_dl_link = URL_NVIDIA_DRIVER;
    dl_cfg->nvidia_dl_path = DL_PATH;


	fprintf(stdout, "NVIDIA DRIVER WILL BE DOWNLOADED IN --->  %s \n", DL_PATH );
	fprintf(stdout, "REDIRECTED TO SYSTEMD RUN LEVEL 2 THEN YOU CAN INSTALL NEW NVIDIA DRIVER \n");
    ret = download_link(dl_cfg->nvidia_dl_link, dl_cfg->nvidia_dl_path);
	change_systemd_target_and_run_driver();



	dbus_error_init(&error);

	dbus_message_get_args(request, &error,
			      DBUS_TYPE_INVALID);
	if (dbus_error_is_set(&error)) {
		reply = dbus_message_new_error(request, "wrong_arguments", "Illegal arguments to Sum");
		dbus_connection_send(connection, reply, NULL);
		dbus_message_unref(reply);
		return;
	}
	
	reply = dbus_message_new_method_return(request);
	dbus_message_append_args(reply,
				 DBUS_TYPE_INT32, &ret,
				 DBUS_TYPE_INVALID);
	dbus_connection_send(connection, reply, NULL);
	dbus_message_unref(reply);
}



int download_link(const char * link, const char * dl_path)
{
  CURL *curl_handle;
  const char *pagefilename = dl_path;
  FILE *pagefile;
 
  
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */ 
  curl_handle = curl_easy_init();
 
  /* set URL to get here */ 
  curl_easy_setopt(curl_handle, CURLOPT_URL, link);
 
  /* Switch on full protocol/debug output while testing */ 
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
 
  /* disable progress meter, set to 0L to enable and disable debug output */ 
  curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
 
  /* send all data to this function  */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
 
  /* open the file */ 
  pagefile = fopen(pagefilename, "wb");
  if(pagefile) {
 
    /* write the page body to this file handle */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
 
    /* get it! */ 
    curl_easy_perform(curl_handle);
 
    /* close the header file */ 
    fclose(pagefile);
  }
 
  /* cleanup curl stuff */ 
  curl_easy_cleanup(curl_handle);
 
  curl_global_cleanup();
 
  return 0;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

int change_systemd_target_and_run_driver(){

    int got_error = 0;
    FILE *pf;
    char data[DATA_SIZE];

    // Setup our pipe for reading and execute our command.
    pf = popen(CMD_CHANGE_SYSTEMD_TARGET , "w");

    if(!pf){
         fprintf(stderr, "Could not open pipe for output.\n");
         got_error = 1;
         return got_error;
    }

    // Grab data from process execution
    fgets(data, DATA_SIZE , pf);

    // Print grabbed data to the screen.
    fprintf(stdout, "-%s-\n",data);

    if (pclose(pf) != 0){
        fprintf(stderr," Error: Failed to close command stream \n");
        got_error = 1;
        return got_error;
    }

    // run driver bash
    system(CMD_RUN_DRIVER);

    return got_error;
}


