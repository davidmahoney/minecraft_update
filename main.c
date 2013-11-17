#include "curl/curl.h"
#include "curl/easy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "versions.h"
#include "update.h"

char* VERSIONS_URL = "https://s3.amazonaws.com/Minecraft.Download/versions/versions.json";

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

int main(int argc, char *argv[]) {
	CURL *curl;
	CURLcode res;
	struct MemoryStruct chunk;
	char *current_ver = NULL;
	char *latest_ver = NULL;
	chunk.memory = malloc(1);
	chunk.size = 0;

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, VERSIONS_URL);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
							&read_version_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
			curl_easy_setopt(curl, CURLOPT_USERAGENT,
							"libcurl-agent/1.0");
	}
	else {
		fprintf(stderr, "failed to initialize curl.\n");
		return 1;
	}
	
	res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform(): failed %s\n",
			curl_easy_strerror(res));
			return 2;
	}

	latest_ver = get_latest_version(chunk.memory);
	printf("Latest available version is %s\n", latest_ver);
	int success = get_current_version(&current_ver);
	if (success != 0 && success != 1) {
			fprintf(stderr, "current version failed with %d\n", success);
			return 1;
	}

	printf("Current version is %s\n", current_ver);

	if (chunk.memory)
			free(chunk.memory);
	curl_easy_cleanup(curl);

	if (current_ver == NULL || strcmp(latest_ver,current_ver) > 0) {
			/* should update */
		success = update_minecraft(latest_ver);
	}
	free(latest_ver);
	free(current_ver);
	return 0;
}
