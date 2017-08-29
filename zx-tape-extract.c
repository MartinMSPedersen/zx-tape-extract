#include <stdio.h>
#include <stdlib.h>
#include <libspectrum.h>

char *blocktype2char(libspectrum_tape_type type) {
	switch (type) {
		case 0x10:
			return "ROM";
		case 0x11:
			return "TURBO";
		case 0x12:
			return "PURE_TONE";  
		case 0x13:
			return "PULSES";
		case 0x14:
			return "PURE_DATA";
		case 0x15:
			return "RAW_DATA";
		case 0x19:
			return "GENERALISED_DATA";
		case 0x20:
			return "PAUSE";
		case 0x21:
			return "GROUP_START";
		case 0x22:
			return "GROUP_END";
		case 0x23:
			return "JUMP";
		case 0x24:
			return "LOOP_START";
		case 0x25:
			return "LOOP_END";
		case 0x28:
			return "SELECT";
		case 0x2a:
			return "STOP48";
		case 0x2b:
			return "SET_SIGNAL_LEVEL";
		case 0x30:
			return "COMMENT";
		case 0x31:
			return "MESSAGE";
		case 0x32:
			return "ARCHIVE_INFO";
		case 0x33:
			return "HARDWARE";
		case 0x35:
			return "CUSTOM";
		case 0x5a:
			return "CONCAT";
		default:
			return "Unknown";
			/*
			   LIBSPECTRUM_TAPE_BLOCK_RLE_PULSE        
			   LIBSPECTRUM_TAPE_BLOCK_PULSE_SEQUENCE
			   LIBSPECTRUM_TAPE_BLOCK_DATA_BLOCK 
			 */
	}
	return "This should never happen";
}




int main(int argc, char **argv) {
	if (argc<2) {
		fprintf(stderr,"Missing tape-file argument\n");
		return EXIT_FAILURE;
	}
	const char *input_filename = argv[1];
	FILE *f;
	unsigned char *buf;
	size_t bufsize;
	libspectrum_id_t t;
	libspectrum_tape *thetape;
	libspectrum_error e;
	libspectrum_tape_iterator tape_iter;
	libspectrum_tape_block *tape_block;
	int block_count;
	int data_len;

	libspectrum_error libspectrum_init(void); 

	f=fopen(input_filename,"r");
	if (f==NULL) {
		fprintf(stderr,"Error opening file: %s\n",input_filename);
		return EXIT_FAILURE;
	}
	fseek(f, 0, SEEK_END);
	data_len = ftell(f);
	fseek(f, 0, SEEK_SET);
	buf = (libspectrum_byte *) malloc(data_len);
	if (!buf) return EXIT_FAILURE;
	bufsize=fread(buf,1,data_len,f);
	libspectrum_identify_file_raw(&t, input_filename, buf, bufsize);
	if (t!=LIBSPECTRUM_ID_TAPE_TAP) {
		fprintf(stderr,"Not a tape-file\n");
		return EXIT_FAILURE;
	}

	thetape=libspectrum_tape_alloc();
	e=libspectrum_tape_read(thetape,buf,bufsize,LIBSPECTRUM_ID_UNKNOWN,NULL);
	if (e!=LIBSPECTRUM_ERROR_NONE) {
		fprintf(stderr,"Error reading tape - %d\n",e);
	}
	free(buf);

	/*
	   Hurray. We got the tape
	 */
	block_count = 0;
	tape_block = libspectrum_tape_iterator_init(&tape_iter,thetape);
	libspectrum_byte* tape_data;
	long unsigned int tape_data_len;

	while (tape_block!=NULL) {
		printf("Block %d contains a ",block_count);
		printf("%s.\n",blocktype2char(libspectrum_tape_block_type(tape_block)));
		if (strcmp("ROM",blocktype2char(libspectrum_tape_block_type(tape_block)))==0) {
			tape_data_len = libspectrum_tape_block_data_length(tape_block);
			printf("length=%lu\n",tape_data_len);
			tape_data = (libspectrum_byte *)malloc(tape_data_len);
			if (tape_data_len==19) {
				for (int i=0; i<tape_data_len; i++) {
					printf("%02x",tape_data[i]);
				}
				printf("\n");
				for (int i=0; i<tape_data_len; i++) {
					printf("%c",tape_data[i]);
				}
				printf("\n");
			} 
		}
		block_count++;
		tape_block = libspectrum_tape_iterator_next(&tape_iter);
		printf("\n");
	}
	printf("Number of tape-blocks: %d\n",block_count);


	return EXIT_SUCCESS;


}
