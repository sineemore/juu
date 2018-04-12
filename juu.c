#include <stdio.h>
#include <stdlib.h> /* EXIT_SUCCESS */

#define BUF_SIZE (1024 * 4)

static void outc(char ch) {
	if (EOF == putchar(ch)) {
		exit(EXIT_FAILURE);
	}
}

static void outs(const char *str) {
	if (EOF == fputs(str, stdout)) {
		exit(EXIT_FAILURE);
	}
}

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
				outc(ch);
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
				outc(ch);
				outc('\n');
				i = ++indent;
				while (i-- > 0) outs(placeholder);
				break;
	
			case '}':
			case ']':
				outc('\n');
				i = --indent;
				while (i-- > 0) outs(placeholder);
				outc(ch);
				if (indent == 0) outc('\n');
				break;
				
			case ',':
				outc(',');
				outc('\n');
				i = indent;
				while (i-- > 0) outs(placeholder);
				break;

			case ':':
				outc(':');
				outc(' ');
				break;

			case '"':
				/* String/property key start, see if clause on top (line 22) */
				outc('"');
				is_string = 1;
				break;
	
			default:
				/* Numbers, true, false, null */
				outc(ch);
				break;
			}
		}
	}

	if (ferror(stdin)) {
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
