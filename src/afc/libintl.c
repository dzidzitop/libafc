/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2013 Dźmitry Laŭčuk

libafc is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */
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
