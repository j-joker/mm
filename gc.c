#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
void *mm(size_t size){
void *p;
p=sbrk(0);
if(sbrk(size)== -1) 
        return NULL;
return p;
}
int main(){
void *p=mm(10);
printf("%d\n",p);
return 0;
}   
