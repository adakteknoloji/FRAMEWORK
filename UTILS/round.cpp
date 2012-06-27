#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
double ROUND            ( double deger, int digit );
double ROUND_UP         ( double deger, int digit );
double ROUND_DOWN       ( double deger, int digit );
#define NUM_OF_DEGERS 10

int main()
{
    double sayilar[NUM_OF_DEGERS] = {  1.2345,  19.992 ,  19.01111 ,  8.759 , 9.4301 ,
                           -1.2345, -19.992 , -19.01111 , -8.759 , -9.4301 } ;

    for (int i=0; i < NUM_OF_DEGERS; i++) {
        printf ("ROUND (%f) is : %f \n", sayilar[i] , ROUND (sayilar[i], 2));
    }

    printf ("\n");

    for (int i=0; i < NUM_OF_DEGERS; i++) {
        printf ("ROUND_UP (%f) is : %f \n", sayilar[i] , ROUND_UP (sayilar[i], 2));
    }

    printf ("\n");

    for (int i=0; i < NUM_OF_DEGERS; i++) {
        printf ("ROUND_DOWN (%f) is : %f \n", sayilar[i] , ROUND_DOWN (sayilar[i], 2));
    }

    return 0;
}
*/

#ifdef MSVC
    double trunc (double d)
    {
        return (d > 0) ? floor (d) : ceil (d);
    }
#endif


/**************************************************************************************
                   ROUND
***************************************************************************************/

double ROUND ( double deger, int digit )
{

    double          carpan = 1.0;
    double          yeni_deger = deger;

    if (deger < 0.0) {
        carpan = -1.0;
    }

    for (int i=0; i < abs(digit); i++) {
        carpan *= 10.0;
    }
    yeni_deger *= carpan;
    yeni_deger += 0.5;
    yeni_deger = trunc (yeni_deger);
    yeni_deger /= carpan;

    return yeni_deger;
}


/**************************************************************************************
                   ROUND_UP
***************************************************************************************/

double ROUND_UP ( double deger, int digit )
{

    double          carpan = 1.0;
    double          yeni_deger = deger;

    if (deger < 0.0) {
        carpan = -1.0;
    }

    for (int i=0; i < abs(digit); i++) {
        carpan *= 10.0;
    }
    yeni_deger *= carpan;
    yeni_deger = ceil (yeni_deger);
    yeni_deger /= carpan;

    return yeni_deger;
}

/**************************************************************************************
                   ROUND_DOWN
***************************************************************************************/

double ROUND_DOWN ( double deger, int digit )
{

    double          carpan = 1.0;
    double          yeni_deger = deger;

    if (deger < 0.0) {
        carpan = -1.0;
    }

    for (int i=0; i < abs(digit); i++) {
        carpan *= 10.0;
    }
    yeni_deger *= carpan;
    yeni_deger = floor (yeni_deger);
    yeni_deger /= carpan;

    return yeni_deger;
}

