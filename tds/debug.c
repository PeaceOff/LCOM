#include "debug.h"
#include "stdio.h"


static FILE * f;

char Debug_File(){
	if(f!=NULL) return 1;

	f = fopen("home/lcom/log.txt", "w");

	if(f==NULL)
			return 0;

	fprintf(f,"LOG:\n");
	return 1;

}

void Debug_LOG(char* tag, char* value){
	if(Debug_File())
		fprintf(f, "%s : %s\n",tag, value);


}

void Debug_LOGHex(char* tag, long value){
	if(Debug_File())
		fprintf(f, "%s : %x\n",tag, value);

}

void Debug_LOGInt(char* tag, long value){
	if(Debug_File())
		fprintf(f, "%s : %d\n",tag, value);
}

