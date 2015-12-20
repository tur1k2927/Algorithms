#include <stdio.h>
#include <stdlib.h>
#define N 100001
#define BASE 10
#define MIN_LENGTH 4

char* n1;
char* n2;
char* answer;
int n1_length, n2_length;

int karatsuba(char*, int, char*, int, char*);
int correct_sizes(char*, int*, char*, int*);
int char_to_digit(char);
int strcat(char*, int, char*, int);
char digit_to_char(int);
int sum(char*, int, char*, int, char*);
int substract(char*, int, char*, int, char*);
int reverse(char*);
void print_num(char*, int);
int get_nums(void);

int main(){
	
	n1 = (char *) malloc(N);
	n2 = (char *) malloc(N);
	answer = (char *) malloc(2*N);

	int was_zero = get_nums();
	if ( was_zero )
	{
		printf("0\n");
		return 0;
	}

	int result_length = karatsuba(n1, n1_length, n2, n2_length, answer);

	print_num(answer, result_length);
	
	free(n1);
	free(n2);
	free(answer);

	return 0;
}

int karatsuba(char* x, int x_length, char* y, int y_length, char* result){

	int result_length = 0;

	if ( x_length < MIN_LENGTH && y_length < MIN_LENGTH )
	{
		int first = 0, second = 0;
		int i;
		for ( i = 0; i < x_length; i++ )
		{
			first = first * 10 + char_to_digit(x[i]);
		}
		for ( i = 0; i < y_length; i++ )
		{
			second = second * 10 + char_to_digit(y[i]);
		}

		int product = first * second;
		for ( i = 0; product; i++ )
		{
			result[i] = digit_to_char(product % BASE);
			product /= BASE;
		}
		if ( i == 0 && product == 0 )
            result[i++] = '0';

		result[i] = '\0';
		result_length = reverse(result);

		return result_length;
	}



	int add = correct_sizes(x, &x_length, y, &y_length);

	int m = x_length / 2;
	m = x_length - m;

	char a[N];
	int a_length = karatsuba(x, m, y, m, a);

	char d[N];
	int d_length = karatsuba(x+m, x_length - m, y+m, y_length-m, d);

	char e[N], first_sum[N], second_sum[N], e_product[N];
	int first_sum_length = sum(x, m, x+m, x_length-m, first_sum);
	int second_sum_length = sum(y, m, y+m, y_length-m, second_sum);
	int e_product_length = karatsuba(first_sum, first_sum_length, second_sum, second_sum_length, e_product);

	int e_length = 0;
	e_length = strcat(e, e_length, e_product, e_product_length);

	e_length = substract(e, e_length, a, a_length, e);
	e_length = substract(e, e_length, d, d_length, e);

	result_length = strcat(result, result_length, a, a_length);

	int i; m = x_length - m;
    for ( i = 0; i < 2 * m; i++)
        result[result_length+i] = '0';
    result[result_length=result_length+i] = '\0';

    for ( i = 0; i < m; i++ )
        e[e_length+i] = '0';
    e[e_length=e_length+i] = '\0';

	result_length = sum(result, result_length, e, e_length, result);
	result_length = sum(result, result_length, d, d_length, result);

	for ( ; add > 0; add-- )
	{
		result[--result_length] = '\0';
	}

	return result_length;
}

int correct_sizes(char* x, int* x_length, char* y, int* y_length){

	int add = 0;

	if ( *x_length != *y_length )
	{
		if ( *x_length > *y_length )
		{
			while ( *y_length != *x_length )
			{
				y[(*y_length)++] = '0';
				add++;
			}
			y[*y_length] = '\0';
			n2_length = *y_length;
		}
		else
		{
			while ( *x_length != *y_length )
			{
				x[(*x_length)++] = '0';
				add++;
			}
			x[*x_length] = '\0';
			n1_length = *x_length;
		}
	}

	return add;
}

int sum(char* num1, int length1, char* num2, int length2, char* result){

    char temp_result[N];
	int additional_unit = 0;
	if ( length1 < length2 )
	{
		int temp = length1;
		length1 = length2;
		length2 = temp;

		char* temp_p = num1;
		num1 = num2;
		num2 = temp_p;
	}

	int i, j, r_index = 0, only_zeros = 1;
	for ( i = length2-1, j = length1-1; i >= 0 && j >= 0; i--, j-- )
	{
		int sum = char_to_digit(num1[j]) + char_to_digit(num2[i]);
		int new_sum = sum % 10 + additional_unit;
		additional_unit = 0;

		if ( sum / 10 || new_sum / 10 )
			additional_unit = 1;

        if ( only_zeros && new_sum % 10 )
            only_zeros = 0;

		temp_result[r_index++] = digit_to_char(new_sum % 10);
	}

	for ( ; j >= 0; j-- )
	{
		int sum = char_to_digit(num1[j]);
		int new_sum = sum % 10 + additional_unit;
		additional_unit = 0;

		if ( sum / 10 || new_sum / 10 )
			additional_unit = 1;

        if ( only_zeros && new_sum % 10 )
            only_zeros = 0;


		temp_result[r_index++] = digit_to_char(new_sum % 10);
	}

	if ( additional_unit )
    {
        temp_result[r_index++] = digit_to_char(additional_unit);
        additional_unit = only_zeros = 0;
    }

	temp_result[r_index] = '\0';
	for ( i = r_index-1; i > 0 && temp_result[i] == '0'; i-- )
	{
		temp_result[i] = '\0';
	}

	if ( only_zeros )
    {
        r_index = 1;
        temp_result[r_index] = '\0';
    }

	int _length = reverse(temp_result);

	_length = strcat(result, 0, temp_result, _length);

	return _length;
}

int substract(char* num1, int length1, char* num2, int length2, char* result){

    char temp_result[N];
	if ( length1 < length2 )
	{
		int temp = length1;
		length1 = length2;
		length2 = temp;

		char* temp_p = num1;
		num1 = num2;
		num2 = temp_p;
	}

	int i, j, r_index = 0;
	for ( i = length1-1, j = length2-1; i >= 0 && j >= 0; i--, j-- )
	{
		int a = char_to_digit(num1[i]);
		int b = char_to_digit(num2[j]);

		if ( a < b )
		{
		    int k;
		    for ( k = i - 1; k >= 0 && num1[k] == '0'; k-- );

            if ( k >= 0 && num1[k] > '0' )
            {
                num1[k]--;
                a += BASE;

                for ( k = k + 1; k < i; k++ )
                {
                    num1[k] = digit_to_char(BASE-1);
                }
            }

		}

		temp_result[r_index++] = digit_to_char(a-b);
	}

	for ( ; i >= 0; i-- )
	{
		temp_result[r_index++] = num1[i];
	}
	temp_result[r_index] = '\0';

    for ( i = r_index-1; i > 0 && temp_result[i] == '0'; i-- )
	{
		temp_result[i] = '\0';
	}


	int _length = reverse(temp_result);
	_length = strcat(result, 0, temp_result, _length);

	return _length;
}

int reverse(char* word){

	int _length;
	for ( _length = 0; word[_length]; _length++ )
		;

	int i, half = _length / 2;
	for ( i = 0; i < half; i++ )
	{
		char temp = word[i];
		word[i] = word[_length-i-1];
		word[_length-i-1] = temp;
	}

	return _length;
}

int strcat(char* line1, int line1_length, char* line2, int line2_length){

	line1 += line1_length;
	while (*line1++ = *line2++)
		;

	return line1_length + line2_length;
}

int char_to_digit(char a){

	return a >= 48 && a <= 57 ? a - 48 : -10000;
}

char digit_to_char(int a){

	return a + 48;
}

void print_num(char* num, int length){

	printf("\n\n");

	int ostatok = length % 3;

	int i;
	for ( i = 0; i < ostatok; i++ )
	{
		printf("%c", num[i]);
	}

	for (; i < length; i++ )
	{
		if ( (length - i ) % 3 == 0 )
			printf(" ");
		printf("%c", num[i]);
	}

	printf("\n");
}

int get_nums(void){

	int i; int only_zeros = 1, remember_was_zero = 0;
	for ( n1_length = 0; (n1[n1_length] = getchar()) != '\n'; n1_length++ )
	{
		if ( n1[n1_length] != '0' )
			only_zeros = 0;
	}
	n1[n1_length] = '\0';

	if ( only_zeros )
		remember_was_zero = 1;

	only_zeros = 1;
	for ( n2_length = 0; (n2[n2_length] = getchar()) != '\n'; n2_length++ )
	{
		if ( n2[n2_length] != '0' )
			only_zeros = 0;
	}
	n2[n2_length] = '\0';

	if ( only_zeros )
		remember_was_zero = 1;

	if ( remember_was_zero )
		return 1;

	return 0;
}
