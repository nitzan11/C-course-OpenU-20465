#include <stdio.h>
#define ZERO 0
#define ONE 1

/*this program will receive dollar rate and nis values, and will output the respective dollar value, including the total sum*/
int main ()
{
float dollarVal;
float rate, sum = ZERO;

printf("$\t\tIS\n");
scanf("%f",&rate); //reading dollar rate
while (scanf("%f", &dollarVal) == ONE){ //printing dollar values in requested format
	printf("%.2f\t\t%.2f\n", dollarVal, dollarVal*rate);
	sum += dollarVal;
	}
printf("%.2f\t\t%.2f\n", sum, sum*rate);
printf("Program finished successfully\n");	
}



