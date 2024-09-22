#include <stdio.h>
#include <math.h>
#define ZERO 0
#define MINUS_ONE -1
#define ONE 1
#define TWO 2
#define LIMIT 1.0*pow(10,-6)

/*this program will compute the cosine values up to a certain number*/
 

double my_cos(double x);
double factorial (double x);

int main (){
double num;
printf("Please enter a number:\n");
scanf("%lf", &num);
printf("Recevied number is: %.2lf\n", num);
printf("Cosine value is: %.2lf\n", cos(num));
printf("Computed number is: %.2lf\n", my_cos(num));

}


double my_cos(double x){
double i = ZERO, sum = ZERO, error = ZERO;
error = pow(MINUS_ONE, i)*pow(x,TWO*i)/(factorial(TWO*i));
while (fabs(error) > LIMIT){
	sum += error;
	i++;
	error = pow(MINUS_ONE, i)*pow(x,TWO*i)/(factorial(TWO*i));
	}
return sum;
}

double factorial (double x){
double i, fact = ONE;
for (i = ONE; i <= x; i++)
	fact *= i;
//printf("fact is: %.2lf\n", fact);
return fact;
}
