#include <stdio.h>
#include <stdlib.h> /* EXIT_SUCCESS */

#define BUF_SIZE (1024 * 4)

int main(int argc, char **argv) {
	
	char buf[BUF_SIZE];
	const char placeholder[] = "  ";
	unsigned int indent = 0;
	unsigned int i = 0;
	unsigned int k = 0;
	char is_string = 0;
	char escaped = 0;
	char ch;
	size_t n;
	
	while (0 < (n = fread(&buf, sizeof(char), BUF_SIZE, stdin))) {
		for (k = 0; k < n; k++) {
			ch = buf[k];

			if (is_string) {
				/* Inside quoted string */
				putchar(ch);
				if (! escaped) {
					if (ch == '"') {
						/* Unescaped quote, string just ended */
						is_string = 0;
					} else if (ch == '\\') {
						escaped = 1;
					}
				} else {
					escaped = 0;
				}
				continue;
			}
	
			switch (ch) {
	
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				/* Ignoring original formatting */
				break;
		
			case '{':
			case '[':
				putchar(ch);
				putchar('\n');
				i = ++indent;
				while (i-- > 0) fputs(placeholder, stdout);
				break;
	
			case '}':
			case ']':
				putchar('\n');
				i = --indent;
				while (i-- > 0) fputs(placeholder, stdout);
				putchar(ch);
				if (indent == 0) putchar('\n');
				break;
				
			case ',':
				putchar(',');
				putchar('\n');
				i = indent;
				while (i-- > 0) fputs(placeholder, stdout);
				break;

			case ':':
				putchar(':');
				putchar(' ');
				break;

			case '"':
				/* String/property key start, see if clause on top (line 22) */
				putchar('"');
				is_string = 1;
				break;
	
			default:
				/* Numbers, true, false, null */
				putchar(ch);
				break;
			}
		}
	}
	
	return EXIT_SUCCESS;
}
