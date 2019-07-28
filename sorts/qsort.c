#include <stdio.h>
#include <stdlib.h>

int compare(const void* a, const void* b)
{
    const int *arg1 = a;
    const int *arg2 = b;

    return *arg1 - *arg2;
}

int main()
{
	int arr[8] = {0, 11, 2, 4 ,3 ,15 ,19 ,8};
	int size_a = sizeof(arr) / sizeof(arr[0]);

	for(int i = 0; i < size_a; i++) {
		printf("Value array %d: %d\n", i, arr[i]);
	}
	
	qsort(arr, size_a, sizeof(arr[0]), compare);

	printf("Array is sorting end\n");

	for(int i = 0; i < size_a; i++) {
		printf("Value array %d: %d\n", i, arr[i]);
	}

	return 0;
}