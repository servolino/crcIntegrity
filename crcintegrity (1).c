/*
 * Authors: Samuel Ervolino, Jorge B Nunez
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


typedef struct node {
	char* input_hex;
	char* input_bin;
	char* polynomial;
	char* mode;
	char* last_three;
	int* val;
} node;

char* trim_bin(char* input_bin){

    int len = strlen(input_bin) - 12;
    int i;
    char* trimmed_bin = malloc(sizeof(char) * len + 1);

    for(i = 0; i < len; i++)
        trimmed_bin[i] = input_bin[i];

    trimmed_bin[len] = '\0';

    return trimmed_bin;
}

int hex_to_int(char hexval)
{
	int retval;
	
	switch(hexval) {
	case '0':
		retval = 0;
		break;
	case '1':
		retval = 1;
		break;
	case '2':
		retval = 2;
		break;
	case '3':
		retval = 3;
		break;
	case '4':
		retval = 4;
		break;
	case '5':
		retval = 5;
		break;
	case '6':
		retval = 6;
		break;
	case '7':
		retval = 7;
		break;
	case '8':
		retval = 8;
		break;
	case '9':
		retval = 9;
		break;
	case 'A':
		retval = 10;
		break;
	case 'B':
		retval = 11;
		break;
	case 'C':
		retval = 12;
		break;
	case 'D':
		retval = 13;
		break;
	case 'E':
		retval = 14;
		break;
	case 'F':
		retval = 15;
		break;
	default:
		fprintf(stderr, "Error: %c passed into hex_to_int\n\n", hexval);
	}
	
	return retval;
}

char bin_to_hex(int* crc_val){

	if(crc_val[0] == 0){
		if(crc_val[1] == 0){
			if(crc_val[2] == 0){
				if(crc_val[3] == 0)
					return '0';
				else if(crc_val[3] == 1)
					return '1';
			}
			else if(crc_val[2] == 1){
				if(crc_val[3] == 0)
					return '2';
				else if(crc_val[3] == 1)
					return '3';
			}
		}
		else if(crc_val[1] == 1){
			if(crc_val[2] == 0){
				if(crc_val[3] == 0)
					return '4';
				else if(crc_val[3] == 1)
					return '5';
			}
			else if(crc_val[2] == 1){
				if(crc_val[3] == 0)
					return '6';
				else if(crc_val[3] == 1)
					return '7';
			}
		}
	}
	else if(crc_val[0] == 1){
		if(crc_val[1] == 0){
			if(crc_val[2] == 0){
				if(crc_val[3] == 0)
					return '8';
				else if(crc_val[3] == 1)
					return '9';
			}
			else if(crc_val[2] == 1){
				if(crc_val[3] == 0)
					return 'A';
				else if(crc_val[3] == 1)
					return 'B';
			}
		}
		else if(crc_val[1] == 1){
			if(crc_val[2] == 0){
				if(crc_val[3] == 0)
					return 'C';
				else if(crc_val[3] == 1)
					return 'D';
			}
			else if(crc_val[2] == 1){
				if(crc_val[3] == 0)
					return 'E';
				else if(crc_val[3] == 1)
					return 'F';
			}
		}

	}
}


void calculate(node* crc){

// ms1_idx corresponds with leading 1, or most significant 1.
	char* dividend_char;
	int ms1_idx = 0;
	int i, j, start;
	int len;
	int cur_len, carry;
	int* dividend_cur_13bits = NULL;
	int* dividend;
	int* poly = malloc(sizeof(int) * 13);
	int round = 1;
	int end = 0;
	int idx_tail = 0;
	int size = 0;
	int* crc_val;
	int** report;
	int** buffer;
	int* temp;
	char* crc_hex = malloc(sizeof(char) * 4);

	if(crc->mode[0] == 'v')
		dividend_char = trim_bin(crc->input_bin);
	else
		dividend_char = crc->input_bin;

	len = strlen(dividend_char);

	dividend = malloc(sizeof(int) * (len+12));

// Convert to ints. Insert padding after binary message
	for(i = 0; i < len+12; i++){
		if(i < len)
			dividend[i] = hex_to_int(dividend_char[i]);
		else
			dividend[i] = 0;
	}

// Convert polynomial to int
	for(i = 0; i < 13; i++)
		poly[i] = hex_to_int(crc->polynomial[i]);

	while(dividend[ms1_idx] == 0)
		ms1_idx++;

	len += 12;
	start = ms1_idx;
	ms1_idx = 0;

	printf("The binary string difference after each XOR step of the CRC calculation:\n\n");

// Division begins
	while(end != 1){

		if(round == 1){
			dividend_cur_13bits = malloc(sizeof(int) * 13);
			j = 0; 

			for(i = start; i < start + 13; i++){
				dividend_cur_13bits[j] = dividend[i];
				j++;
			}

			carry = start + 13;
		}

// XOR operation, need to append rest of divisor and report after this
		for(i = 0; i < 13; i++){
			//printf("before: %d ", dividend_cur_13bits[i]);
			dividend_cur_13bits[i] = dividend_cur_13bits[i] ^ poly[i];
			//printf("after: %d\n", dividend_cur_13bits[i]);
		}

// append and report results of XOR

		if(round == 1){

			for(i = 0; i < len; i++){
				if(i != 0 && i % 4 == 0)
					printf(" ");

				printf("%d", dividend[i]);
			}

			printf("\n");

			report = malloc(sizeof(int*) * 1);
			report[0] = malloc(sizeof(int) * len);
			for(i = 0; i < start; i++){
				report[0][i] = 0;
				//printf("0");
			}
			for(i = start; i < start + 13; i++){
				report[0][i] = dividend_cur_13bits[i - start];
				//printf("%d", )
			}
			for(i = start + 13; i < len; i++)
				report[0][i] = dividend[i];

			for(i = 0; i < len; i++){
				if(i != 0 && i % 4 == 0)
					printf(" ");

				printf("%d", report[0][i]);
			}

			printf("\n");


			idx_tail++;
			size = 1;
		}
		else{

			if(idx_tail >= size){
				size = idx_tail * 2;
				buffer = realloc(report, sizeof(int*) * size);
				report = buffer;
			}

			report[idx_tail] = malloc(sizeof(int) * len);

			for(i = 0; i < start; i++)
				report[idx_tail][i] = 0;
			for(i = start; i < start + 13; i++)
				report[idx_tail][i] = dividend_cur_13bits[i - start];
			for(i = start + 13; i < len; i++)
				report[idx_tail][i] = dividend[i];

			for(i = 0; i < len; i++){
				if(i != 0 && i % 4 == 0)
					printf(" ");

				printf("%d", report[round-1][i]);
			}

			printf("\n");


			idx_tail++;
		}
		
		round++;

// Identify most signficant 1 index in current 13 bits
		while(dividend_cur_13bits[ms1_idx] == 0)
			ms1_idx++;

		//printf("ms1 idx: %d\n", ms1_idx);

		cur_len = 13 - ms1_idx;

		if(cur_len != 13){
			i = 0;
			while(cur_len != 13){
				if(i + ms1_idx < 13)
					dividend_cur_13bits[i] = dividend_cur_13bits[i + ms1_idx];
				else{
					dividend_cur_13bits[i] = dividend[carry];
					carry++;
					cur_len++;
				}

				i++;

				if(carry > len){

					crc_val = malloc(sizeof(int) * 12);

					/*for(i = 0; i < 12; i++){
						crc_val[i] = report[round-2][i+12];
					}*/

					j = 12;
					for(i = 0; i < 12; i++){
						crc_val[i] = report[round-2][len-j];
						j--;
					}
					
					end = 1;
					break;
				}
			}

			start += ms1_idx;
		}

		else
			start += 13;

		ms1_idx = 0;
	}

	for(i = 0; i < 12; i++){
		if((i+1)%4 == 0){
			temp = malloc(sizeof(int) * 4);
			temp[0] = crc_val[i-3];
			temp[1] = crc_val[i-2];
			temp[2] = crc_val[i-1];
			temp[3] = crc_val[i];

	
			crc_hex[((i+1)/4)-1] = bin_to_hex(temp);
			free(temp);
		}

	}

	crc_hex[3] = '\0';

	printf("\nThe CRC computed from the input: ");
	for(i = 0; i < 12; i++){
		if(i % 4 == 0)
			printf(" ");
		printf("%d", crc_val[i]);
	}
	printf(" (bin) = ");

	for(i = 0; i < 3; i++)
		printf("%c", crc_hex[i]);

	printf(" (hex)\n");

	if(crc->mode[0] == 'v'){
		printf("\nDid the CRC check pass? (Yes or no): ");

		if(strcmp(crc_hex, crc->last_three) == 0)
			printf("Yes\n");
		else
			printf("No\n");
	}

	free(dividend_cur_13bits);
	free(buffer);

	return;
}

char* mini_bin(int intval)
{
	int idx;
	char* mini_bin;
	
	mini_bin = malloc(sizeof(char) * 5);
	
	for (idx = 0; idx < 4; idx++)
		mini_bin[idx] = '0';
	mini_bin[5] = '\0';

	if (intval >= 8) {
		mini_bin[0] = '1';
		intval -= 8;
	}
	if (intval >= 4) {
		mini_bin[1] = '1';
		intval -= 4;
	}
	if (intval >= 2) {
		mini_bin[2] = '1';
		intval -= 2;
	}
	if (intval >= 1) {
		mini_bin[3] = '1';
		intval -= 1;
	}
	
	return mini_bin;
}


char* hex_to_bin(char* input_hex)
{
	int bin_len, idx, jdx, ibuff;
	char* input_bin, *sbuff;

	bin_len = strlen(input_hex) * 4;
	input_bin = malloc(sizeof(char) * (bin_len + 12 + 1));

	for (idx = 0; idx < bin_len + 12 + 1; idx++)
		input_bin[idx] = '\0';

	for (idx = 0; idx < strlen(input_hex); idx++) {
		ibuff = hex_to_int(input_hex[idx]);
		sbuff = mini_bin(ibuff);
		for (jdx = 0; jdx < 4; jdx++) {
			input_bin[4*idx + jdx] = sbuff[jdx];
		}
		free(sbuff);
	}
	
	return input_bin;
}


char* last_three(char* input_hex)
{
	int hex_len;
	char* last_three;
	
	hex_len = strlen(input_hex);
	last_three = malloc(sizeof(char) * 4);
	
	last_three[0] = input_hex[hex_len - 3];
	last_three[1] = input_hex[hex_len - 2];
	last_three[2] = input_hex[hex_len - 1];
	last_three[3] = '\0';
	
	return last_three;
}


node* init(node* crc, int argc, char** argv)
{
	int idx;
	int** XOR_reports;
	char cbuff, diff;
	
	// Check command line argument count.
	if (argc != 3) {
		fprintf(stderr, "Error: Invalid command line syntax\n");
		fprintf(stderr, "Proper syntax is as follows:\n");
		fprintf(stderr, "Compile: gcc -lm -o CrcIntegrity crcintegrity.c\n");
		fprintf(stderr, "Linux:   ./CrcIntegrity <mode> <hexval>\n");
		fprintf(stderr, "Windows: .\\CrcIntegrity <mode> <hexval>\n\n");
		return NULL;
	}
	
	// Force all alphabetic characters in the first argument to lowercase.
	// Unnecessary, but we like writing secure code.
	for (idx = 0; idx < strlen(argv[1]); idx++) {
		argv[1][idx] = tolower(argv[1][idx]);
	}

	// Force all alphabetic characters in the second argument to uppercase.
	// Likewise unnecessary, but we still like writing secure code.
	for (idx = 0; idx < strlen(argv[2]); idx++) {
		argv[2][idx] = toupper(argv[2][idx]);
	}
	
	// Check validity of CRC mode argument
	if ((strcmp(argv[1], "c") != 0) && (strcmp(argv[1], "v") != 0)) {
		fprintf(stderr, "Error: Invalid CRC mode\n");
		fprintf(stderr, "Specify \"c\" to calculate CRC integrity, or ");
		fprintf(stderr, "\"v\" to verify CRC integrity\n\n");
		return NULL;
	}

	// Check validity of hex string argument length.
	if ((strlen(argv[2]) < 3) || (strlen(argv[2]) > 40)) {
		fprintf(stderr, "Error: Invalid hex input\n");
		fprintf(stderr, "Hex input must be at least 3 digits long, or ");
		fprintf(stderr, "at most 40 digits long.\n\n");
		return NULL;
	}
	
	// Check validity of hex string argument representation.
	// If invalid digits are encountered, do NOT proceed.
	for (idx = 0; idx < strlen(argv[2]); idx++) {
		cbuff = argv[2][idx];
		if (!isalnum(cbuff)) {
			fprintf(stderr, "Error: Invalid hex input\n");
			fprintf(stderr, "\'%c\' is not a valid hex digit\n\n", cbuff);
			return NULL;
		}
		if (isalpha(cbuff)) {
			diff = cbuff - 'A';
			if ((diff > 5) || (diff < 0)) {
				fprintf(stderr, "Error: Invalid hex input\n");
				fprintf(stderr, "\'%c\' is not a valid hex digit\n\n", cbuff);
				return NULL;
			}
		}
	}
	
	// It is safe to allocate memory for the node.
	crc = malloc(sizeof(node));
	crc->input_hex = malloc(sizeof(char) * (strlen(argv[2]) + 1));
	crc->input_bin = NULL;
	crc->polynomial = malloc(sizeof(char) * (13 + 1));
	crc->mode = malloc(sizeof(char) * (strlen(argv[1]) + 1));
	//crc->val = malloc(sizeof(int) * 12);
	crc->last_three = NULL;

	// Make copies of the command line input and store the polynomial.
	strcpy(crc->input_hex, argv[2]);
	crc->input_bin = hex_to_bin(crc->input_hex);
	strcpy(crc->polynomial, "1100110110101");
	strcpy(crc->mode, argv[1]);

	crc->last_three = last_three(crc->input_hex);

	// Return fully-initialized node to main.
	return crc;
}


void header_print(node* crc)
{
    int idx, jdx;
    int** report;
    char *temp_bin;

    // Initial header specifying program name and the authors of the program.
    printf("--------------------------------------------------------------");
    printf("\n\n");
    printf("CIS3360 Fall 2017 Integrity Checking Using CRC\n");
    printf("Author: Samuel Ervolino, Jorge B Nunez\n\n");
    
    // Specifies the input (in hex and binary).
    printf("The input string (hex): %s\n", crc->input_hex);
    printf("The input string (bin): ");
    for (idx = 0; idx < strlen(crc->input_bin); idx++) {
        if ((idx != 0) && (idx % 4 == 0)) {
            printf(" ");
        }
        printf("%c", crc->input_bin[idx]);
    }
    printf("\n\n");

    // Specifies the polynomial.
    printf("The polynomial used (binary bit string): ");
    for (idx = 0; idx < strlen(crc->polynomial); idx++) {
        if ((idx != 0) && (idx % 4 == 0)) {
            printf(" ");
        }
        printf("%c", crc->polynomial[idx]);
    }
    printf("\n\n");
    
    // Specifies the mode of operation and its behavior.
    printf("Mode of operation: ");
    if (strcmp(crc->mode, "c") == 0) {
        printf("calculation\n\n");
        printf("Number of zeroes that will be appended to the binary input:");
        printf(" 12\n\n");
    }
    if (strcmp(crc->mode, "v") == 0) {
        printf("verification\n\n");
        printf("The CRC observed at the end of the input: ");
        temp_bin = hex_to_bin(crc->last_three);
        for (idx = 0; idx < strlen(temp_bin); idx++) {
            if ((idx != 0) && (idx % 4 == 0)) {
                printf(" ");
            }
            printf("%c", temp_bin[idx]);
        }
        free(temp_bin);
        printf(" (bin) = %s (hex)\n\n", crc->last_three);
    }
    
    return;
}

int main(int argc, char** argv)
{
	node* crc = NULL;
	
	crc = init(crc, argc, argv);
	
	if (crc != NULL) {
		header_print(crc);
		calculate(crc);
	}

	return 0;
}
