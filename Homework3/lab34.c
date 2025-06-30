#include <stdio.h>
#define Cout_Arr_Input 200
#define Cout_Arr_Sub_Input 20
int main(void)
{
	int i=0;
	int Cout_Sub=0;
	char Array[Cout_Arr_Input];
	char SubArray[Cout_Arr_Sub_Input];
	char *PrefArray=(char *)Array;
	char *PrefSubArray=(char *)SubArray;
	char *Flag = NULL;
	Flag = SubArray;
	printf("Input:");
	fgets(Array,Cout_Arr_Input,stdin);
	printf("Substring input:");
        fgets(SubArray,Cout_Arr_Sub_Input,stdin);
	while(*PrefArray!='\0')
	{
		if (*(PrefSubArray+1) == '\0')
		{
			PrefSubArray=NULL;
			break;
		}
		if(*PrefArray==*PrefSubArray)
		{
			Cout_Sub++;
			PrefSubArray++;
		}
		else
		{
			PrefSubArray=Flag;
		}
		PrefArray++;
	
}
	printf("Result: %d", Cout_Sub);
	return 0;
}