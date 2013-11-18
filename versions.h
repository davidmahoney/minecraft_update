static const char * ENOFILE = "0";
static const char * ENOTLINK = "-2";
static const char * ENOREAD = "-1";

struct MemoryStruct {
		char *memory;
		size_t size;
};

char * get_latest_version(const char* url);
int get_current_version(char **version);
char * parse_version(char *input);

