

//To compile (win): gcc test.c -o main2.exe -std=c99
//To run (win): main2.exe

#include <stdlib.h>
#include <stdio.h>

//10*8 = 80
char a[80];
#define BMP_WIDTH 14
#define numByte 8



void set(char a[], int i, int j){

int area = ((i+1)*(j+1)+(i)*(BMP_WIDTH-(j+1)));

int index = area / numByte;
int numBit = area % numByte;

a[index] = a[index]^(1<<numBit);
}


int getBit(char a[], int i, int j){
    int area = ((i+1)*(j+1)+(i)*(BMP_WIDTH-(j+1)));

    int index = area / numByte;
    int numBit = area % numByte;

    if (a[index]&(1<<numBit)){
        return 1;
    }else{
        return 0;
    }
}





int main(){

set(a,10,5);

/* char b = (1<<0);
printf("%d",b); */

printf("%d",getBit(a,10,6));


return 0;
}
