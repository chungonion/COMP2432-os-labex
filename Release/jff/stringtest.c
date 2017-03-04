#include <stdio.h>
#include <stdlib.h>

int main(){
    char* string_aray[1000];
    string_aray[0]="Something Interesting";
    printf("%c\n",string_aray[0][2]);
    printf("%s\n",string_aray[0]);


    return 0;
}
