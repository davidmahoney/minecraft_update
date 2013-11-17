#include "json/json.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

const char *SERVER_FILE = "minecraft_server.jar";

char * get_latest_version(char* input) {
		json_object *jobj;
		char * version = NULL;
		int version_len;
		jobj = json_tokener_parse(input);
		jobj = json_object_object_get(jobj, "latest");
		jobj = json_object_object_get(jobj, "release");
		version_len = json_object_get_string_len(jobj);
		version = realloc(version, version_len);
		memcpy(version, json_object_get_string(jobj), version_len);
		return version;
}

int get_current_version(char * version) {
		char *real_server_file = malloc(64);
		char * start;
		char * end;
		/* char *version; */
		size_t size;

		size = readlink(SERVER_FILE, real_server_file, 64);
		if (size == -1) {
			/* version = malloc(3); */
			version = realloc(version, 3);
			switch (errno) {
					case ENOENT: {
										 strncpy(version, "0", 2);
										 return 1;
										 break;
								 }
					case EACCES: {
										 strncpy(version, "-1", 3);
										 return 2;
										 break;
					}

					case EINVAL: {
										 strncpy(version, "-2", 3);
										 return 1;
										 break;
					}
					default: {
									 strncpy(version, "-9", 3);
									 return 9999;
									 break;
					}
			}
			return version;
		}
		start = strchr(real_server_file, '.') + 1;
		end = strrchr(start, '.');
		/* version = malloc(end - start + 1); */
		version = realloc(version, end - start + 1);
		strncpy(version, start, end - start);
		return 0;
}

