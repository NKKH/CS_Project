

//To compile (win): gcc test.c -o main2.exe -std=c99
//To run (win): main2.exe

#include <stdlib.h>
#include <stdio.h>


void function(int a, int b){
    
    for(int i = a; i< a+b; i++){
        printf("%d",i);
    }

}





int main(){
function(1,3);


return 0;
}
