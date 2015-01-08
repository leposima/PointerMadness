// PointerMadness.cpp : Defines the entry point for the console application.
// This program figures out how many 'unused' bits there are in a pointer and asks user to fill these unused bits
// with boolean values. Then the pointer is used as masked out and at the end the boolean values are read from the pointer
// Yes, this project makes no sense, as memory is virtually free and there is humongous amounts of it available.
// perhaps in very mem limited embedded system could use something like this, as boolean variable is probably one byte
// but at least the win32 compiler did not easily allow editing the mem address that the pointer is pointing directly,
// only via casting it to variable and casting it back. 

// yes, as the project name tells, this is pure madness. Only a masochist would read further :)
// 

#include "stdafx.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 4048
typedef int VARIABLE_TYPE;

int _tmain(int argc, _TCHAR* argv[])
{
	size_t sizeOfVariable = sizeof(VARIABLE_TYPE);
	char userInputBuffer[BUFF_SIZE] = { 0 };
	size_t temppi = sizeOfVariable;
	VARIABLE_TYPE variable = 21313155;
	auto pointerToVariable = &variable;
	auto freeBitsInPointer = (unsigned int)0;
	auto bitMask = (unsigned int)0x0;

	bool* booleanStorage = reinterpret_cast<bool*>(malloc(sizeof(bool)* freeBitsInPointer));
	memset(booleanStorage, 0, sizeof(bool)* freeBitsInPointer);

	// calculate how many free bits are in the pointer and generate bitmask, to mask out used pointer bits
	while (temppi % 2 == 0) {
		freeBitsInPointer++;
		temppi /= 2;
		if (!bitMask)
			bitMask = 1;
		else
			bitMask |= (bitMask << 1);
	}

	printf("Size of our VARIABLE_TYPE is %d bytes on this system.\n Bit shift  1 << 1 will produce: %d \n", sizeOfVariable, 1 << 1);
	printf("Diff between to VARIABLE_TYPE pointers are %d \n", (VARIABLE_TYPE)(pointerToVariable - 1) - (VARIABLE_TYPE)(pointerToVariable));
	printf("At the start we had %d at our pointers location\n", *pointerToVariable);
	// ask user a boolean for every 'free' bit
	for (auto i = 0; i < freeBitsInPointer; i++) {

		do {
			printf("Enter %d:th boolean to pass within the pointer [ true / false ] \n", i + 1);
			gets_s(userInputBuffer, BUFF_SIZE);
			// check if we got valid answer, redo query if not. 
			if (stricmp(userInputBuffer, "true") != 0 && stricmp(userInputBuffer, "false") != 0) {
				continue;
			}
			else
			{
				break;
			}

		} while (true);

		auto boolean = stricmp(userInputBuffer, "true") == 0;

		if (boolean)
		{
			VARIABLE_TYPE temp_var = (VARIABLE_TYPE)pointerToVariable;
			temp_var |= (0x1 << i);
			pointerToVariable = (VARIABLE_TYPE*)temp_var;
		}
		else
		{
			VARIABLE_TYPE temp_var = (VARIABLE_TYPE)pointerToVariable;
			temp_var &= ~(0x1 << i);
			pointerToVariable = (VARIABLE_TYPE*)temp_var;
		}

	}
	// mask out the booleans in temporary variable use the pointer
	VARIABLE_TYPE temppi_var = (VARIABLE_TYPE)pointerToVariable & ~bitMask;
	auto maskedOutPointer = (VARIABLE_TYPE*)temppi_var;
	printf("At the middle we have %d at the masked out pointer \n", *maskedOutPointer);

	// read booleans from pointer
	for (auto i = 0; i < freeBitsInPointer; i++) {
		VARIABLE_TYPE temp_var = (VARIABLE_TYPE)pointerToVariable;
		booleanStorage[i] = (temp_var & (0x1 << i)) != 0;
		printf("Read boolean %s from the pointer\n", booleanStorage[i] ? "true" : "false");
		// zero out the boolean bits in the pointer to make it 'pure' again
		temp_var &= ~(0x1 << i);
		pointerToVariable = (VARIABLE_TYPE*)temp_var;

	}

	printf("In the end we have an variable %d from the end of the pointer \n", *pointerToVariable);

	// do not close program until user input
	gets(userInputBuffer);
	free(booleanStorage);
	return 0;
}

