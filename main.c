#include "curl/curl.h"
#include "curl/easy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "versions.h"
#include "update.h"

const char* VERSIONS_URL = "https://s3.amazonaws.com/Minecraft.Download/versions/versions.json";
const char* DOWNLOAD_DIR = "https://s3.amazonaws.com/Minecraft.Download/versions";
const char *SERVER_FILE = "minecraft_server.";

int main(int argc, char *argv[]) {
	char *current_ver = NULL;
	char *latest_ver = NULL;
	char *download_url;


	latest_ver = get_latest_version(VERSIONS_URL);
	printf("Latest available version is %s\n", latest_ver);
	int success = get_current_version(&current_ver);
	if (success != 0 && success != 1) {
			fprintf(stderr, "current version failed with %d\n", success);
			return 1;
	}

	printf("Current version is %s\n", current_ver);
	printf ("compare: %i\n", compare_versions(latest_ver, current_ver));

	if (current_ver == NULL || compare_versions(latest_ver, current_ver) > 0 ) { 
			/* should update */

			asprintf(
							&download_url,
							"%s/%s/%s%s.jar",
							DOWNLOAD_DIR, 
							latest_ver, 
							SERVER_FILE, 
							latest_ver);
		
		success = update_minecraft(
				download_url + strlen(DOWNLOAD_DIR) + strlen(latest_ver) + 2, 
				download_url);
		if (success != 0) {
				fprintf(stderr, "Failed to update minecraft server: error %d\n", success);
		}
	}
	free(latest_ver);
	free(current_ver);
	return 0;
}
