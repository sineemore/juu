#include <stdio.h>
#include <stdlib.h> /* EXIT_SUCCESS */

/* Buffer size for reading from stdin */
#define BUFFER_SIZE (1024 * 4)

static void outc(char ch) {
	if (EOF == putchar(ch)) {
		exit(EXIT_FAILURE);
	}
}

static void fputs_repeat(const char *str, size_t count) {
	while (count-- > 0) {
		int value = fputs(str, stdout);
		if (value == EOF) {
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char **argv) {
	
	char buffer[BUFFER_SIZE];
	const char placeholder[] = "  ";
	unsigned int indent = 0;
	char is_string = 0;
	char escaped = 0;

	/* Read stdin until EOF */
	while (!feof(stdin)) {
		
		size_t n = fread(&buffer, sizeof(char), BUFFER_SIZE, stdin);

		if (n != BUFFER_SIZE && ferror(stdin)) {
			/* Error reading stdin */
			exit(EXIT_FAILURE);
		}

		for (size_t k = 0; k < n; k++) {
			char ch = buffer[k];

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
				indent++;
				fputs_repeat(placeholder, indent);
				break;
	
			case '}':
			case ']':
				outc('\n');
				indent--;
				fputs_repeat(placeholder, indent);
				outc(ch);
				if (indent == 0) outc('\n');
				break;
				
			case ',':
				outc(',');
				outc('\n');
				fputs_repeat(placeholder, indent);
				break;

			case ':':
				outc(':');
				outc(' ');
				break;

			case '"':
				/* String/property key start, see if clause on top */
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

	return EXIT_SUCCESS;
}
