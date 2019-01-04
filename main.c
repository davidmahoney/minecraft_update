#ifdef linux
#define _GNU_SOURCE
#endif
#include "curl/curl.h"
#include "curl/easy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "versions.h"
#include "update.h"

const char* version = "1.2";
const char* VERSIONS_URL = "https://launchermeta.mojang.com/mc/game/version_manifest.json";
const char* DOWNLOAD_DIR = "https://s3.amazonaws.com/Minecraft.Download/versions";
const char *SERVER_FILE = "minecraft_server.";
void print_usage(char *progname);
void print_version();

int main(int argc, char *argv[]) {
	if (argc > 1) {
		for (int i = 1; i < argc; ++i) {
			if (argv[i][0] != '-') {
				print_usage(argv[0]);
				return 1;
			}
			for (int j = 1; j < strlen(argv[i]); ++j) {
				switch (argv[i][j]) {
					case 'v': print_version(); break;
					case 'h': print_usage(argv[0]); break;
					default: print_usage(argv[0]); return 1;
				}
			}
		}
		return 0;
	}

	char *current_ver = NULL;
	Version latest_ver;

	latest_ver = get_latest_version(VERSIONS_URL);
	printf("Latest available version is %s\n", latest_ver.id);
	int success = get_current_version(&current_ver);
	if (success != 0 && success != 1) {
			fprintf(stderr, "current version failed with %d\n", success);
			return 1;
	}

	printf("Current version is %s\n", current_ver);

	if (current_ver == NULL || compare_versions(latest_ver.id, current_ver) > 0 ) { 
			/* should update */
		char * filename = NULL;
		asprintf(&filename, "%s%s.jar", SERVER_FILE, latest_ver.id);
		
		if (latest_ver.download_url == NULL) {
			puts("No download URL!\n");
		}
		success = update_minecraft(filename,
			latest_ver.download_url);
		if (success != 0) {
				fprintf(stderr, "Failed to update minecraft server: error %d\n", success);
		}
	}
	free(latest_ver.id);
	free(latest_ver.metadata_url);
	free(latest_ver.download_url);
	free(current_ver);
	return 0;
}

void print_usage(char *name) {
	fprintf(stderr, "Usage: %s [-hv]\n", name);
}

void print_version() {
	fprintf(stdout, "Version %s\n", version);
}
