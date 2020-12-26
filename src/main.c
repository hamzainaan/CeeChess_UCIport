#include "stdio.h"
#include "defs.h"
#include "stdlib.h"
#include "string.h"

int main() {

		printf("%s %s by %s", engine_name, engine_ver, engine_author);
		AllInit();
		printf("\ninfo string Initialization successfully done!\n");
		S_BOARD pos[1];
    	S_SEARCHINFO info[1];
    	info->quit = FALSE;
		pos->HashTable->pTable = NULL;
    	InitHashTable(pos->HashTable, 256);
		setbuf(stdin, NULL);
    	setbuf(stdout, NULL);

	char line[256];
	while (TRUE) {
		memset(&line[0], 0, sizeof(line));

		fflush(stdout);
		if (!fgets(line, 256, stdin))
			continue;
		if (line[0] == '\n')
			continue;
		if (!strncmp(line, "uci",3)) {
			Uci_Loop(pos, info);
			if(info->quit == TRUE) break;
			continue;
		} else if(!strncmp(line, "quit",4))	{
			break;
		}
	}

	free(pos->HashTable->pTable);
	return 0;
}
