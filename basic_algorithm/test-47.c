#include<stdio.h>
 
#define abs(a) ((a) > 0 ? (a) : -(a))
 
double getCubeRoot(double input){
    int i, d1,d2;
    double num;
     
    for(i = 1; input>=i*i*i; i++);
    i--;
    num = i;
    // 假定输入均为整数
    for(d1 = 0; input>=(int)(num*num*num); num = 0.1*++d1 + i);
    d1--;
    return 0.1*d1 + i;
}
 
int main(){
    double num;
    if(scanf("%lf", &num)!= -1){
        printf("%.1f\n", getCubeRoot(num));
    }
}
