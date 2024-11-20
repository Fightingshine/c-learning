#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<stdbool.h>

#define const1 5417.7530
#define conste 2.718281828
typedef struct input_struct
{
    double numT;
    bool flagT;
    double numH;
    bool flagH;
    double numD;
    bool flagD;
} input, *input_p;
void copy(char a, double a_num, input_p p);
void compute_H(input_p input);
void compute_T(input_p input);
void compute_D(input_p input);
void init(input_p p);

int main()
{
    input_p test_p = (input_p)malloc(sizeof(input));
    while (1)
    {
        init(test_p);
        char fir, sec;
        double fir_num, sec_num;
        scanf("%c", &fir);
        if (fir == 'E')
        {
            break;
        }
        scanf("%lf %c %lf", &fir_num, &sec, &sec_num);
        getchar();

        copy(fir, fir_num, test_p);
        copy(sec, sec_num, test_p);

        if (test_p->flagH == true && test_p->flagT == true)
        {
            compute_D(test_p);
        }
        else if (test_p->flagD == true && test_p->flagT == true)
        {
            compute_H(test_p);
        }
        else if (test_p->flagH == true && test_p->flagD == true)
        {
            compute_T(test_p);
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

void init(input_p p)
{
    p->numT = 0;
    p->flagT = false;
    p->numD = 0;
    p->flagD = false;
    p->numH = 0;
    p->flagH = false;
}

void compute_H(input_p input)
{
    double e = (6.11) * (pow(conste, const1 * ((1.0 / 273.16) - (1.0 / (input->numD + 273.16)))));
    double h = 0.5555 * (e - 10.0);
    input->numH = (input->numT) + h;
    printf("%c %.1lf %c %.1lf %c %.1lf\n", 'T', input->numT, 'D', input->numD, 'H', input->numH);
}
void compute_T(input_p input)
{
    double e = (6.11) * (pow(conste, const1 * ((1.0 / 273.16) - (1.0 / (input->numD + 273.16)))));
    double h = 0.5555 * (e - 10.0);
    input->numT = input->numH - h;
    printf("%c %.1lf %c %.1lf %c %.1lf\n", 'T', input->numT, 'D', input->numD, 'H', input->numH);
}
void compute_D(input_p input)
{
    double mid1 = ((input->numH - input->numT) / 0.5555 + 10.0) / 6.11;
    double mid2 = log(mid1) / const1;
    double mid3 = (1.0) / ((1.0) / 273.16 - mid2);
    input->numD = mid3 - 273.16;
    printf("%c %.1lf %c %.1lf %c %.1lf\n", 'T', input->numT, 'D', input->numD, 'H', input->numH);
}

void copy(char a, double a_num, input_p p)
{
    if (a == 'T')
    {
        p->numT = a_num;
        p->flagT = true;
    }
    if (a == 'H')
    {
        p->numH = a_num;
        p->flagH = true;
    }
    if (a == 'D')
    {
        p->numD = a_num;
        p->flagD = true;
    }
}