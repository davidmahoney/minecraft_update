static const char * ENOFILE = "0";
static const char * ENOTLINK = "-2";
static const char * ENOREAD = "-1";

typedef struct {
	char *id;
	char *metadata_url;
	char *download_url;
} Version;

Version get_latest_version(const char* url);
int get_current_version(char **version);
int compare_versions(const char *version1, const char* version2);


