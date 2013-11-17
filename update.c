#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "curl/curl.h"
#include "curl/easy.h"

struct HttpFile {
		char *filename;
		FILE *stream;
};


static size_t write_server_file(void *contents, size_t size, size_t nmemb, void *userdata) {
		struct HttpFile *out = (struct HttpFile *)userdata;
		if (out && !out->stream) {
				out->stream=fopen(out->filename, "wb");
				if (!out->stream)
						return -1; /* can't open the file to write */
		}
		return fwrite(contents, size, nmemb, out->stream);
}


int update_minecraft(char *newversion) {
		char * url;
		char *filename;
		int success;
		CURL *curl;
		CURLcode res;

		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();

		asprintf(&url, "https://s3.amazonaws.com/Minecraft.Download/versions/%s/minecraft_server.%s.jar", newversion, newversion);
		asprintf(&filename, "minecraft_server.%s.jar", newversion);

		struct HttpFile server_file = {
				filename,
				NULL
		};

		if (curl) {
				curl_easy_setopt(curl, CURLOPT_URL, url );
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
								&write_server_file);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&server_file);
				curl_easy_setopt(curl, CURLOPT_USERAGENT,
								"libcurl-agent/1.0");
		}
		else {
				fprintf(stderr, "failed to initialize curl.\n");
				return 1;
		}

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "Could not fetch new server file: %s\n", res);
		}

		success = unlink("minecraft_server.jar");
		if (success != 0 && errno != ENOENT) {
				fprintf(stderr, "Could not remove minecraft_server.jar: %s (%d)\n", strerror(errno), errno);
				return -1;
		}

		success = symlink(filename, "minecraft_server.jar");
		if (success != 0) {
			fprintf(stderr, "Could not create link: %s\n", strerror(errno));
			return -1;
		}

		curl_easy_cleanup(curl);

		if (server_file.stream)
				fclose(server_file.stream);

		curl_global_cleanup();
		
		free(filename);
		free(url);
}
