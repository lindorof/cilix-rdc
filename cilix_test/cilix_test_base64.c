#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"
#include "cilix_base64.h"

//Test values
///////////////////////////////////////////////////////////////
#define STRING_A "9TrJ"
#define STRING_B "9TrJUQ=="
#define STRING_C "9TrJUfA="

#define HEXNUM_A {0xF5,0x3A,0xC9}
#define HEXNUM_B {0xF5,0x3A,0xC9,0x51}
#define HEXNUM_C {0xF5,0x3A,0xC9,0x51,0xF0}

#define HEXSTR_A "0xF5 0x3A 0xC9"
#define HEXSTR_B "0xF5 0x3A 0xC9 0x51"
#define HEXSTR_C "0xF5 0x3A 0xC9 0x51 0xF0"

#define TEXT_STR "test1234567"
#define TEXT_B64 "dGVzdDEyMzQ1Njc="
///////////////////////////////////////////////////////////////

#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

//#define STATUS(x)  ((x>0)?"[PASS]":"[FAIL]")
#define STATUS(x)  score(x)
#define PERCENT(a,b)  ((float)((float)a/(float)b)*100)

int test_cilix_b64_encode(void);
int test_cilix_b64_decode(void);


int test_b64_encode(void);
int test_b64_decode(void);
int test_b64_encodef(void);
int test_b64_decodef(void);
void test_b64_text_encode(void);
void test_b64_text_decode(void);

int hexputs(const char* data, int len);
int hexprint(const char* data, int len);
int compare(char *a, char *b, int l);
void deepCompare(int pass, char *a, char *b, int len);
char *score(int x);

int testScore = 0;
int testTotal = 0;

void cilix_test_base64(void) {

	test_cilix_b64_decode();

	test_cilix_b64_encode();


	return ;
	
	puts("\nbase64.c [Test Data]");
	puts("------------------------------------");
	printf("%s           : %s\n",HEXSTR_A,STRING_A);
	printf("%s      : %s\n",HEXSTR_B,STRING_B);
	printf("%s : %s\n",HEXSTR_C,STRING_C);
	printf("%s              : %s\n",TEXT_STR,TEXT_B64);
	puts("\nTesting b64_encode() ...\n");
	test_b64_encode();
	puts("\nTesting b64_decode() ...\n");
	test_b64_decode();
	puts("\nTesting test_b64_encodef() ...\n");
	printf("%s\n",STATUS(test_b64_encodef()));
	puts("\nTesting test_b64_decodef() ...\n");
	printf("%s\n",STATUS(test_b64_decodef()));
	puts("\nTesting test_b64_text_encode() ...\n");
	test_b64_text_encode();
	puts("\nTesting test_b64_text_decode() ...\n");
	test_b64_text_decode();
	puts("------------------------------------");
	printf("\n[END] Test score: %g%% (%d/%d)\n",PERCENT(testScore,testTotal),testScore,testTotal);

}
int test_cilix_b64_encode(void){

	char test_a[] = HEXNUM_A;
	char test_b[] = HEXNUM_B;
	char test_c[] = HEXNUM_C;

	int size_a = NELEMS(test_a);
	int size_b = NELEMS(test_b);
	int size_c = NELEMS(test_c);

	int out_size_a = cilix_base64_ensize(size_a) + 1;
	int out_size_b = cilix_base64_ensize(size_b) + 1;
	int out_size_c = cilix_base64_ensize(size_c) + 1;

	char* Base64_a = malloc(sizeof(char*)*out_size_a);
	char* Base64_b = malloc(sizeof(char*)*out_size_b);
	char* Base64_c = malloc(sizeof(char*)*out_size_c);
	
	cilix_base64_encode(test_a,size_a,Base64_a);
	cilix_base64_encode(test_b,size_b,Base64_b);
	cilix_base64_encode(test_c,size_c,Base64_c);


	printf("\t%s\t%s\n",STATUS(strcmp((char*)Base64_a,STRING_A)==0),Base64_a);
	printf("\t%s\t%s\n",STATUS(strcmp((char*)Base64_b,STRING_B)==0),Base64_b);
	printf("\t%s\t%s\n",STATUS(strcmp((char*)Base64_c,STRING_C)==0),Base64_c);

	free(Base64_a);
	free(Base64_b);
	free(Base64_c);
	
	return 0;
}

int test_b64_encode(void) {
	
	unsigned char test_a[] = HEXNUM_A;
	unsigned char test_b[] = HEXNUM_B;
	unsigned char test_c[] = HEXNUM_C;

	unsigned int size_a = NELEMS(test_a);
	unsigned int size_b = NELEMS(test_b);
	unsigned int size_c = NELEMS(test_c);

	int out_size_a = b64e_size(size_a) + 1;
	int out_size_b = b64e_size(size_b) + 1;
	int out_size_c = b64e_size(size_c) + 1;

	unsigned char *out_a = malloc( (sizeof(char) * out_size_a) );
	unsigned char *out_b = malloc( (sizeof(char) * out_size_b) );
	unsigned char *out_c = malloc( (sizeof(char) * out_size_c) );
	
	out_size_a = b64_encode(test_a,size_a,out_a);
	out_size_b = b64_encode(test_b,size_b,out_b);
	out_size_c = b64_encode(test_c,size_c,out_c);

	printf("\t%s\t%s\n",STATUS(strcmp((char*)out_a,STRING_A)==0),out_a);
	printf("\t%s\t%s\n",STATUS(strcmp((char*)out_b,STRING_B)==0),out_b);
	printf("\t%s\t%s\n",STATUS(strcmp((char*)out_c,STRING_C)==0),out_c);
	
	free(out_a);
	free(out_b);
	free(out_c);
	
	return 0;
}

int test_cilix_b64_decode(void){

	char test_a[] = STRING_A;
	char test_b[] = STRING_B;
	char test_c[] = STRING_C;

	int len_a = strlen(test_a);
	int len_b = strlen(test_b);
	int len_c = strlen(test_c);

	int out_size_a = cilix_base64_desize(len_a);
	int out_size_b = cilix_base64_desize(len_b);
	int out_size_c = cilix_base64_desize(len_c);


	char *out_a = malloc( (sizeof(char) * out_size_a) +1);
	char *out_b = malloc( (sizeof(char) * out_size_b) +1);
	char *out_c = malloc( (sizeof(char) * out_size_c) +1);

	cilix_base64_decode(test_a,out_a,&out_size_a);
	cilix_base64_decode(test_b,out_b,&out_size_b);
	cilix_base64_decode(test_c,out_c,&out_size_c);
	
	char r_a[] = HEXNUM_A;
	char r_b[] = HEXNUM_B;
	char r_c[] = HEXNUM_C;
	
	printf("\t%s\t",STATUS(compare(r_a,(char*)out_a,NELEMS(r_a)))); hexputs((char*)out_a,out_size_a);
	printf("\t%s\t",STATUS(compare(r_b,(char*)out_b,NELEMS(r_b)))); hexputs((char*)out_b,out_size_b);
	printf("\t%s\t",STATUS(compare(r_c,(char*)out_c,NELEMS(r_c)))); hexputs((char*)out_c,out_size_c);
	
	free(out_a);
	free(out_b);
	free(out_c);
	
	return 0;
}
int test_b64_decode(void) {
	
	unsigned char test_a[] = STRING_A;
	unsigned char test_b[] = STRING_B;
	unsigned char test_c[] = STRING_C;

	unsigned int len_a = strlen((char*)test_a);
	unsigned int len_b = strlen((char*)test_b);
	unsigned int len_c = strlen((char*)test_c);

	int out_size_a = b64d_size(len_a);
	int out_size_b = b64d_size(len_b);
	int out_size_c = b64d_size(len_c);

	unsigned char *out_a = malloc( (sizeof(char) * out_size_a) +1);
	unsigned char *out_b = malloc( (sizeof(char) * out_size_b) +1);
	unsigned char *out_c = malloc( (sizeof(char) * out_size_c) +1);
	
	out_size_a = b64_decode(test_a,len_a,out_a);
	out_size_b = b64_decode(test_b,len_b,out_b);
	out_size_c = b64_decode(test_c,len_c,out_c);
	
	char r_a[] = HEXNUM_A;
	char r_b[] = HEXNUM_B;
	char r_c[] = HEXNUM_C;
	
	printf("\t%s\t",STATUS(compare(r_a,(char*)out_a,NELEMS(r_a)))); hexputs((char*)out_a,out_size_a);
	printf("\t%s\t",STATUS(compare(r_b,(char*)out_b,NELEMS(r_b)))); hexputs((char*)out_b,out_size_b);
	printf("\t%s\t",STATUS(compare(r_c,(char*)out_c,NELEMS(r_c)))); hexputs((char*)out_c,out_size_c);
	
	free(out_a);
	free(out_b);
	free(out_c);
	
	return 0;
}

int test_b64_encodef(void) {
	
	FILE *pFile;
	pFile = fopen("B64_TEST01A.tmp","wb");
	if (pFile==NULL)
		return 0;
	
	unsigned int i, j=0;
	unsigned int test_a[] = HEXNUM_B;
	unsigned int size_a = NELEMS(test_a);
	
	for (i=0;i<size_a;i++) {
		fputc(test_a[i],pFile);
	}
	fclose(pFile);
	
	j = b64_encodef("B64_TEST01A.tmp","B64_TEST01B.tmp");
	remove("B64_TEST01A.tmp");
	
	if (!j)
		return 0;
	
	pFile = fopen("B64_TEST01B.tmp","rb");
	if (pFile==NULL)
		return 0;
	
	char *out = malloc(j+1);
	fgets(out,j+1,pFile);
	fclose(pFile);
	remove("B64_TEST01B.tmp");
	printf("\tComparing \"%s\" to \"%s\" : ",STRING_B,out);
	if (strcmp(STRING_B,out)==0)
		return 1;
	
	return 0;
}

int test_b64_decodef(void) {
	
	FILE *pFile;
	pFile = fopen("B64_TEST02A.tmp","wb");
	if (pFile==NULL)
		return 0;
	
	int j=0;
	
	fputs(STRING_B,pFile);
	fclose(pFile);
	
	j = b64_decodef("B64_TEST02A.tmp","B64_TEST02B.tmp");
	remove("B64_TEST02A.tmp");
	
	if (!j)
		return 0;
	
	pFile = fopen("B64_TEST02B.tmp","rb");
	if (pFile==NULL)
		return 0;
	
	int c, l=0;
	char* out = (char*)malloc(j + 1);
	while(c!=EOF) {
		c=fgetc(pFile);
		if (c==EOF)
		break;
		out[l++] = c;
	}
	fclose(pFile);
	remove("B64_TEST02B.tmp");
	printf("\tComparing \"%s\" to \"",HEXSTR_B); hexprint(out,j); printf("\" : ");
	char r_b[] = HEXNUM_B;
	if (compare(r_b, out, j)) {
		free(out);
		return 1;

	}free(out);
	return 0;
}

void test_b64_text_encode(void) {
	char *test_str = TEXT_STR;
	unsigned int length = strlen(test_str);
	unsigned char *out_a = (unsigned char *)malloc(1 + (sizeof(char) * b64e_size(length)));
	int out_size_a = b64_encode((unsigned char*)test_str,length,out_a);
	
	int test_passed = (strcmp((char*)out_a,TEXT_B64)==0);
	printf("\t%s\t%s\n",STATUS(test_passed),out_a);

	deepCompare(test_passed,(char*)out_a,TEXT_B64,out_size_a);
	
	free(out_a);
}

void test_b64_text_decode(void) {
	char *test_str = TEXT_B64;
	unsigned int length = strlen(test_str) + 1;
	unsigned char *out_a = malloc( b64d_size(length)+1 );
	int out_size_a = b64_decode((unsigned char *)test_str,length,out_a);
	out_a[out_size_a] = '\0';
	
	int test_passed = (strcmp((char*)out_a,TEXT_STR)==0);
	printf("\t%s\t%s\n",STATUS(test_passed),out_a);

	deepCompare(test_passed,(char*)out_a,TEXT_STR,out_size_a);
	
	free(out_a);
}

int hexputs(const char* data, int len) {
	hexprint(data,len);
	printf("\n");
	return 0;
}

int hexprint(const char* data, int len) {
	int i;
	for (i=0;i<len;i++) {
		printf("0x%X",data[i]&255);
		if (i<len-1)
			printf(" ");
	}
	return 0;
}

int compare(char *a, char *b, int l) {
	int i;
	for (i=0;i<l;i++) {
		if (a[i]!=b[i])
			return 0;
	}
	return 1;
}

void deepCompare(int pass, char *a, char *b, int len){
	if (!pass) {
		for(int j=0;j<len;j++){
			printf("\t\'%c\' == \'%c\'",a[j],b[j]);
			if (a[j] != b[j])
				printf(" <- Error.");
			printf("\n");
		}
	}
}

char *score(int x) {
	testScore+=(!!x)*1;
	testTotal+=1;
	return ((x>0)?"[PASS]":"[FAIL]");
}