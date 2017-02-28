#include <stdio.h>
#include <stdlib.h>

int checkLeap(int year){
    if(year%400==0 || (year%4==0 && year%100!=0)){
        return 1;
    }else{
        return 0;
    }
}


int main(int argc, char const *argv[]) {
    /* code */
    int status;
    status = checkLeap(2016);
    printf("%d\n",status );
    return 0;
}
