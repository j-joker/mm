#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
struct s_block {
        size_t size;
        struct s_block *next;
        struct s_block *prev;
        int free;
        void *ptr;
        char data[1];
};
void * base;
#define BLOCK_SIZE 20 
typedef struct s_block * t_block;

t_block extend_heap(t_block last ,size_t size){
        t_block b=sbrk(0);
        int sb=(int)sbrk(size+BLOCK_SIZE);
        if(sb<0)
                return NULL;
        b->size=size;
        b->prev=last;
        b->next=NULL;
        if(last)
                last->next=b;
        b->free=0;
        b->ptr=b->data;
        return b;
}

void split_block(t_block b ,size_t size){

        t_block new=b->data+size;
        new->free=1;
        new->next=b->next;
        new->ptr=new->data;
        if(new->next){
                new->next->prev=new;
        }
        b->next=new;
        b->size=size;
}
t_block find_block(size_t size,t_block *last){
        t_block b=base;
        while(b&&!(b->free&&b->size>size)){
                *last=b;
                b=b->next;
        }
        return b;
}

int align4(size_t x){
        return (((x-1) >> 2) <<2);
}

void *mm(size_t size){

        t_block last,b;
        size_t new_size=align4(size);
        if(base){
                last=base;
                b=find_block(new_size,&last);
                if(b!=NULL){
                        if(b->size-new_size>BLOCK_SIZE+4){
                                split_block(b,new_size);
                        }
                }else{
                        b=extend_heap(last,new_size);

                }
        }else{
                b=extend_heap(NULL,new_size);
                base=b;
        }       
        return b==NULL?b:b->data;
}
void fusion(t_block b){
        if(b->next&&b->next->free){
                b->size+=BLOCK_SIZE+b->next->size;
                b->next=b->next->next;
        }

}
void free(char* p){
        if(p<base||p>sbrk(0)){
        printf("wrong address");
        return;
        }
        t_block b=(t_block)(p-BLOCK_SIZE);
        if(p!=b->ptr){
                printf("not valid address");
                return;
        }
        b->free=1;
        if(b->next&&b->next->free){
                fusion(b);
        }
        if(b->prev&&b->prev->free){
                fusion(b->prev);
        }

}
int main(){
        void *p=NULL;
        p=mm(30);
        printf("%d %d\n",p ,base);
        free(p);
        printf("%d\n",((t_block)base)->free);
        return 0;
}

