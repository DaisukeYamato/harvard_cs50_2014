#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void){
    /* variables  */
    double money_d;
    int q=0, d=0, n=0, c=0;
    /* user input */
    printf("O hai!  ");
    do {
        printf("How much change is owed? ");
        money_d = GetFloat();
    } while (money_d < 0);
    /* cast int */
    int money = round(money_d * 100);
    int rest = money;
    q = rest / 25; rest = rest % 25;
    d = rest / 10; rest = rest % 10;
    n = rest / 5;  rest = rest % 5;
    c = rest;
    
    printf("%d\n", q+d+n+c);
}
