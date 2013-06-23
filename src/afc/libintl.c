#include <stdlib.h>
#include <string.h>

// TODO remove this after binutils fix.
char* libintl_dgettext(const char *__domainname, const char *__msgid)
{
	char* result = malloc(strlen(__domainname) + strlen(__msgid) + 2);
	int j = 0;
	int i;
	for (i = 0;__domainname[i] != '\0'; ++i) {
		result[j++] = __domainname[i];
	}
	result[j++] = ':';
	for (i = 0; __msgid[i] != '\0'; ++i) {
		result[j++] = __msgid[i];
	}
	result[j] = '\0';
	return result;
}
