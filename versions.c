#include "json-c/json.h"
#include "curl/curl.h"
#include "curl/easy.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

const char *SERVER_LINK = "minecraft_server.jar";

struct MemoryStruct {
		char *memory;
		size_t size;
};


static size_t read_version_callback(void *contents, size_t size, size_t nmemb, void *userdata) {
		size_t realsize = size * nmemb;
		struct MemoryStruct *mem = (struct MemoryStruct *)userdata;

		mem->memory = realloc(mem->memory, mem->size + realsize + 1);
		if (mem->memory == NULL) {
				/* out of memory */
				printf("not enough memory (realloc returned NUL\n");
				return 0;
				}
		memcpy(&(mem->memory[mem->size]), contents, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;

		return realsize;
}


bool parse_version(char* input, char** output) {
		json_object *jobj;
		char * version = NULL;
		bool ok;
		int version_len;
		jobj = json_tokener_parse(input);
		if (!json_object_object_get_ex(jobj, "latest", &jobj))
			return false;	
		if (!json_object_object_get_ex(jobj, "release", &jobj))
			return false;
		version_len = json_object_get_string_len(jobj);
		version = json_object_get_string(jobj);
		mersion = realloc(version, version_len);
		*output = version;
		return true;
}



char *get_latest_version(const char *url) {
	CURL *curl;
	CURLcode res;
	struct MemoryStruct chunk;
	char *latest_ver = NULL;
	chunk.memory = malloc(1);
	chunk.size = 0;

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
						&read_version_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
		curl_easy_setopt(curl, CURLOPT_USERAGENT,
						"libcurl-agent/1.0");
	}
	else {
		fprintf(stderr, "failed to initialize curl.\n");
		return NULL;
	}

	res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform(): failed %s\n",
						curl_easy_strerror(res));
		return NULL;
	}

	if(!parse_version(chunk.memory, &latest_ver)) {
		fprintf(stderr, 
				"Tried to get the lasest version, but didn't understand the server's reply.");
		return NULL;
	}
	if (chunk.memory)
			free(chunk.memory);
	curl_easy_cleanup(curl);

	return latest_ver;
}

int get_current_version(char ** version) {
		char *real_server_file = malloc(64);
		char * start;
		char * end;
		size_t size;

		size = readlink(SERVER_LINK, real_server_file, 64);
		if (size == -1) {
			*version = realloc(*version, 3);
			switch (errno) {
					case ENOENT: {
										 strncpy(*version, "0", 2);
										 return 1;
										 break;
								 }
					case EACCES: {
										 strncpy(*version, "-1", 3);
										 return 2;
										 break;
					}

					case EINVAL: {
										 strncpy(*version, "-2", 3);
										 return 1;
										 break;
					}
					default: {
									 strncpy(*version, "-9", 3);
									 return 9999;
									 break;
					}
			}
		}
		/* readlink doesn't append the null byte to the end of the string */
		real_server_file[size + 1] = '\0';
		start = strchr(real_server_file, '.') + 1;
		end = strrchr(start, '.');
		/* if not enough dots (.) are found return version 0 */
		if (start == NULL || end == NULL || end - start == 0) {
				*version = realloc(*version, 2);
				strncpy(*version, "0", 2);
		}
		else {
				*version = realloc(*version, end - start + 1);
				strncpy(*version, start, end - start);
		}
		return 0;
}

