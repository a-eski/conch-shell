#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define SHL_BUFFER_SIZE 1024
#define SHL_MAX_BUFFER_SIZE 20480
#define SHL_TOKEN_BUFFER_SIZE 64
#define SHL_MAX_TOKEN_BUFFER_SIZE 1280
#define SHL_LINES_SIZE 10

char* shl_string_copy(char* dest, char* source, const uint_fast32_t bufferSize)
{
	char* originalStringToSave = source;

	for (uint_fast32_t i = 0;
		i < bufferSize && (*dest = *source) != '\0';
		i++, ++dest, ++source) {}

	return (originalStringToSave);
}

bool shl_is_delimiter(char ch)
{
	switch (ch)
	{
		case ' ':
			return true;
		case '\t':
			return true;
		case '\r':
			return true;
		case '\n':
			return true;
		case '\a':
			return true;
		case EOF:
			return true;
		case '\0':
			return true;
		default:
			return false;
	}
}

int shl_string_compare(char* stringOne, char* stringTwo, uint_fast32_t maxBufferSize)
{
	const unsigned char *p1 = ( const unsigned char * )stringOne;
	const unsigned char *p2 = ( const unsigned char * )stringTwo;

	for (uint_fast32_t i = 0; i <= maxBufferSize && *p1 && *p1 == *p2; i++)
	{
		if (i == maxBufferSize)
			return -1;
		
		++p1, ++p2;
	}

	return ( *p1 > *p2 ) - ( *p2  > *p1 );
}

int main(void)
{
	char* copyOne = "hello"; 
	char* copyOneBuffer = malloc(sizeof(char) * SHL_TOKEN_BUFFER_SIZE);
	copyOne = shl_string_copy(copyOneBuffer, copyOne, SHL_TOKEN_BUFFER_SIZE);
	assert(strcmp(copyOne,copyOneBuffer) == 0);
	free(copyOneBuffer);
	printf("shl_string_copy test 1 passed.\n");

	char* copyTwo = "hello there";
	char* copyTwoBuffer = malloc(sizeof(char) * SHL_TOKEN_BUFFER_SIZE);
	copyTwo = shl_string_copy(copyTwoBuffer, copyTwo, SHL_TOKEN_BUFFER_SIZE);
	assert(strcmp(copyTwo,copyTwoBuffer) == 0);
	free(copyTwoBuffer);
	printf("shl_string_copy test 2 passed.\n");

	int resultOne = shl_string_compare("Hello", "Hello", SHL_TOKEN_BUFFER_SIZE);
	assert(resultOne == 0);
	printf("shl_string_compare test 1 passed.\n");

	int resultTwo = shl_string_compare("Hello", "Hello there", SHL_TOKEN_BUFFER_SIZE);
	assert(resultTwo != 0);
	printf("shl_string_compare test 2 passed.\n");

	int resultThree = shl_string_compare("Hello", "Hello there", 3);
	assert(resultThree == -1);
	printf("shl_string_compare test 3 passed.\n");

}
