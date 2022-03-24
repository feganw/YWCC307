#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sha256.h"

#define BUFFERSIZE 1

int main(int argc, char * argv[]){

	unsigned char buffer[BUFFERSIZE];
	FILE *instream;
	int bytes_read = 0;
	int buffer_size = 0;
	
	buffer_size = sizeof(unsigned char) * BUFFERSIZE;
	instream = fopen("./701_3685.png", "rb");
	
	fseek(instream, 0, SEEK_END);
	size_t fs = ftell(instream);
	fseek(instream, 0, SEEK_SET);
	
	unsigned char* message = (unsigned char*) malloc(fs);
	size_t read = 0;
	if(instream != NULL){
		while((bytes_read = fread(&buffer, buffer_size, 1, instream)) == buffer_size){
			message[read] = buffer[0];
			read += bytes_read;
		}
	} else{
		printf("Could not open file.\n");
	}
	
	if(read == 0){
		printf("%s\n", strerror(errno));
	}
	clock_t t_start, t_stop;
	t_start = clock();
	std::cout << sha256hash(message, read) << std::endl;
	t_stop = clock();
	printf("elapsed time: %f10 (s)\n", (double)(t_stop - t_start) / CLOCKS_PER_SEC);
	
	free(message);

	return 0;
}