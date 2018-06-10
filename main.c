#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nvidia-service.h"


int main() {
	DBusConnection *connection;
	DBusError error;
	DBusObjectPathVTable vtable;

	dbus_error_init(&error);
	connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
	check_and_abort(&error);

	dbus_bus_request_name(connection, "it.nvidia.service.DBusService", 0, &error);
	check_and_abort(&error);

	vtable.message_function = tutorial_messages;
	vtable.unregister_function = NULL;
	
	dbus_connection_try_register_object_path(connection,
						 "/it/nvidia/DBusService",
						 &vtable,
						 NULL,
						 &error);
	check_and_abort(&error);

	while(1) {
		dbus_connection_read_write_dispatch(connection, 1000);
	}
	
	return 0;
}


