#include <stdio.h>

int main(void)
{
	float x=5;
	printf("x=%f",x);
	float y=6;
	printf("\ny=%f\n",y);
	float *xp=&y;
	float *yp=&y;
	printf("Output:%f\n",*xp+*yp);
	return 0;
}