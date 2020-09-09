

//To compile (win): gcc test.c -o main2.exe -std=c99
//To run (win): main2.exe

#include <stdlib.h>
#include <stdio.h>


void function(unsigned char integer[1],int int2){
    integer[0] = 1;
    int2 = 1;
}

unsigned char integer[1]={5};
int int2 = 5;



int main(){
function(integer,int2);
printf("%d",integer[0]);
printf("%d",int2);

return 0;
}
