// Nicole Guobadia
// TCSS 333 C in System Programming - Assignment 3

/* This program uses command line arguments to do John Napier addition and
multiplication. Tested on CSS Gate.

For the addition algorithm, we simply go through both napier arrays and add their
contents together based on index. For the multiplication algorithm you simply add
together the indexes of the numbers to generate the products.

To reduce all of the values down to single letters we go cell by cell in the
resulting sum / product. If the cell[i] > 1, then we reduce that cell by 2, and
increase cell[i + 1] by one, as 2(2^n) = 2^(n + 1). We continue doing this until
the cell is 1 or 0, and we continue this process throughout the array.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// The maximum length of a napier expression
#define MAX_LENGTH 52

// Capital letter ascii offset
#define CAP_ASCII_OFFSET 39

// Lowercase letter ascii offset
#define LOWER_ASCII_OFFSET 97


// Checks to make sure that the operator is valid
// param: char
bool isOper(char);

// Checks to make sure there is the right number of operators
// param: int
bool isValidArgs(int);

// Checks to make sure what is being multiplied/added is is valid
// param: char *, int, char *, int
bool isValidOperand(char *, int, char *, int);

// Populates the napier arrays
// param: char *, int, char *. int, char *, char *
void populate(char *, int, char *, int, char *, char *);

// Tester method to help me check the array outputs
// param: char *, int
void checkArr(char *, int);

// Corrects the arrays so that there are no duplicate letters
// param: char *
void adjust(char *);

// Adds str1 + str2 = str3
// param: char *, char *, char *
void addition(char *, char *, char *);

// Multiplies str1 * str2 = str3	for (i = 0; i < MAX_LENGTH; i++) {
// param: char *. char *, char *
void multiplication(char *, char *, char *);

// Generates the result back into ASCII to be printed
// param: char *
void convertToASCII(char *);


/* Main method. Uses command line arguments to get two operands and an operators
 to do John Napier addition/multiplication, then outputs a completely reduced
 result. Any input that does not fit within the S o S (String operator String)
 format will produce an error. Any operators that are not addition '+' or
 multiplication 'x' will also produce an error. Only upper and lowercase letters
 are allowed. */
int main(int argc, char *argv[]) {

	bool validArgs = isValidArgs(argc);

	if (validArgs) {

		bool validOperator, validOperand;
		char * napier1 = (char *)calloc(52, sizeof(char));
		char * napier2 = (char *)calloc(52, sizeof(char));
		char * arg1;
		char * arg2;
		int operator, len1, len2;
		arg1 = &(argv[1][0]);
		arg2 = &(argv[3][0]);
		operator = * argv[2];
		len1 = strlen(arg1);
		len2 = strlen(arg2);

		validOperator = isOper(operator);
		validOperand = isValidOperand(arg1, len1, arg2, len2);

		if (validOperand && validOperator) {

			populate(arg1, len1, arg2, len2, napier1, napier2);
			adjust(napier1);
			adjust(napier2);

			if (operator == 'x') {

				char * product = (char *)calloc(52, sizeof(char));
				multiplication(napier1, napier2, product);
				convertToASCII(product);
				printf("%s x %s => %s\n", arg1, arg2, product);

				free(napier1);
				free(napier2);
				free(product);

			} else {

				char * sum = (char *)calloc(52, sizeof(char));
				addition(napier1, napier2, sum);
				convertToASCII(sum);
				printf("%s + %s => %s\n", arg1, arg2, sum);

				free(napier1);
				free(napier2);
				free(sum);

			}

		}

	}

}

/* Checks to make sure that the operator is valid
// param: char
//
// pre: character operator is passed through to verify it is valid
// post: boolean returned in accordance with whether the operator was valid or not
*/
bool isOper(char op) {

	if((op != '+') && (op != 'x')) {
		puts("Invalid operator. ");
		return false;

	}

	return true;

}

/* Checks to make sure there is the right number of operators
// param: int
//
// pre: number of arguments is passed through to verify there are only 4 arguments
// post: boolean returned in accordance with whether there were EXACTLY 4 arguments or not
*/
bool isValidArgs(int num) {

	if(num != 4) {

		puts("Invalid number of arguments.");
		return false;

	}
	return true;

}

/* Checks to make sure what is being multiplied/added is is valid
// param: char *, int, char *, int
//
// pre: str1 and str are char * representing each operand. l1 and l2 represent their respective lengths
// post: boolean returned in accordance with whether both operands were valid
*/
bool isValidOperand (char * str1, int l1, char * str2, int l2) {
	int i;
	for (i = 0; i < l1; i++) {
		if (!(str1[i] < 91 && str1[i] > 64) && !(str1[i] > 96 && str1[i] < 123)) {

			puts("Invalid operand. ");
			return false;

		}

	}

	for (i = 0; i < l2; i++) {
		if (!(str2[i] < 91 && str2[i] > 64) && !(str2[i] > 96 && str2[i] < 123)) {

			puts("Invalid operand. ");
			return false;

		}

	}
	return true;

}

// pre: char * array to be checked and the length of the array to prevent going out of bounds
// post: outputs the array to analyze
void checkArr(char * arr, int len) {
	int i;
	for (i = 0; i < len; i++) {
		printf("%d, ", arr[i]);

	}

}

/* Populates the napier arrays. To do this we use an ascii offset, one for capital letters
// and the other for lowercase letters. Lowercase letters are offset by 97 to appear in the
// [0, 25] and capital letters are offset by 39 to appear in [26, 51]. This is done so that
// each letter's exponent corresponds to the index of the array, simplifying the addition
// and multiplication process.
// param: char *, int, char *, int, char *, char *
//
// pre: arg1 and arg2 represent the operands with len1 and len2 being their lengths respectively.
// n1 and n2 are the char * for all possible values (a - Z) to be allocated into, a total of 52 possible
// locations.
// post: n1 is allocated with the occurences of each letter from arg1, and likewise n2 is also
// allocated with the occurences of each letter from arg1
*/
void populate(char * arg1, int len1, char * arg2, int len2, char * n1, char * n2) {
	int i;
	for (i = 0; i < len1; i++) {
		if (arg1[i] <= 90) {
			n1[arg1[i] - CAP_ASCII_OFFSET]++;

		} else {
			n1[arg1[i] - LOWER_ASCII_OFFSET]++;

		}

	}

	for (i = 0; i < len2; i++) {
		if (arg2[i] <= 90) {
			n2[arg2[i] - CAP_ASCII_OFFSET]++;

		} else {
			n2[arg2[i] - LOWER_ASCII_OFFSET]++;

		}

	}

}

/* Corrects the arrays so that there are no duplicate letters. To do this, if any cell[n] > 1,
// the next cell[n + 1] increments by 1 and the cell[n] decrements by 2. This is because
// 2(2^n) = 2^(n + 1).
// param: char *
//
// pre: a char * with duplicate values (i.e. aaab)
// post: the same char * adjusted so that there are no more duplicates (i.e. ac)
*/
void adjust(char * arr) {
	int i;
	for (i = 0; i < MAX_LENGTH - 1; i++) {
		while (arr[i] > 1) {
			arr[i + 1]++;
			arr[i] = arr[i] - 2;

		}

	}

}

/* Adds str1 + str2 = str3. To do this simply add together str1[n] + str2[n] = str3[n]. The sum
// is then adjusted to that there are no duplicates.
// param: char *, char *, char *
//
// pre: 2 char * representing the operands and an empty char * of the same length
// post: both char * added together and stored into the empty char *. The resulting
// char * is also run through adjust() to ensure no duplicates
*/
void addition(char * str1, char * str2, char * sum) {
	int i;
	for (i = 0; i < MAX_LENGTH; i++) {
		sum[i] = str1[i] + str2[i];

	}
	adjust(sum);

}

/* Multiplies str1 * str2 = str3. To do this we simply increment the product[i + j] by 1
// where i and j are the exponents for the operands. For example, 2^(3) x 2^(5) = 2^(8),
// or str1[3] x str2[5] = product[8]++
// param: char *. char *, char *
//
// pre: 2 char * representing the operands and an empty char * of the same length
// post: both char * multiplied together and stored into the empty char *. The resulting
// char * is also run through adjust() to ensure no duplicates
*/
void multiplication(char * str1, char * str2, char * product) {
	int i, j;
	for (i = 0; i < MAX_LENGTH; i++) {
		if (str1[i] > 0) {
			for (j = 0; j < MAX_LENGTH;j++) {
				if (str2[j] > 0) {
					product[i + j]++;

				}

			}

		}

	}
	adjust(product);

}

/* Generates the result back into ASCII to be printed. This is done in a similar way as the populate()
// method, however instead of subtracting the offset we add the offset. A temporary char * is also created
// to shift all values down to the left. This is done because 0 represents a null terminating character,
// therefore there must be no gabs between ascii values in the array.
// param: char *
//
// pre: a char * with a sum / product with no duplicate values
// post: the same char * where its values have been converted to their ascii equivalent using the
// CAP_ASCII_OFFSET and LOWER_ASCII_OFFSET respectively. A temporary char * is created and later freed
// here
*/
void convertToASCII(char * result) {
	int i, count;
	char * temp = (char *)calloc(52, sizeof(char));
	count = 0;

	// puts the values back into their ascii values
	for (i = 0; i < MAX_LENGTH; i++) {
		if (result[i] == 0) {
			// DO NOTHING

		} else if (i < 26) {
			result[i] += LOWER_ASCII_OFFSET + i - 1;

		} else if (i >= 26){
			result[i] += CAP_ASCII_OFFSET + i - 1;

		}

	}

	// shifts over all of the values towards the left
	for (i = 0; i < MAX_LENGTH; i++) {
		if (result[i] > 0) {
			temp[count] = result[i];
			count++;

		}

	}
	memcpy(result, temp, MAX_LENGTH);
	free(temp);

}
