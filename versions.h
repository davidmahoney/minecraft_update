/* The file was not found */
static const char * ENOFILE = "0";
/* The file was not a symlink */
static const char * ENOTLINK = "-2";
/* The file was there, but could not be read */
static const char * ENOREAD = "-1";

/*
Holds the data necessary to get the latest version of the server file.
*/
typedef struct {
	char *id;
	char *metadata_url;
	char *download_url;
} Version;

/*
get the latest server version metadata
*/
Version get_latest_version(const char* url);
/*
get the current version of the server. It looks for the symlink minecraft_server.jar,
which should point at a file named like minecraft_server.x.x.x.jar

return values can be 0 (success), 1 (success) or something else (failure).
*/
int get_current_version(char **version);

/* 
Compare two version strings in the form of <major>.<minor>.<revision>. Returns 0 if they
are equal, 1 if version1 is newer than version2, and -1 otherwise. This function doesn't
check if a version string is well formed.
*/
int compare_versions(const char *version1, const char* version2);


