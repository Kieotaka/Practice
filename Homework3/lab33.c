#include <stdio.h>

int main(void)
{
	int Input_Array [10];
	int i;
	int *Output_Array=(int *)Input_Array;
	for (i=0; i<10; i++)
	{
		Input_Array[i]=i+1;
	}

	for (i=0; i<10; i++)
	{
		printf("%d ",*Output_Array);
		Output_Array++;
	}
	return 0;
}