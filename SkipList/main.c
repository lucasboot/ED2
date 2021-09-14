#include <stdio.h>
#include "skiplist.h"

int main (){
    SkipList* sk = createSkipList(5, 0.6);
    for (int i =0; i<10; i=i+2){
        int t = insertSkipList(sk, i);
        if(t == 0){
            printf("Erro na insercao\n");
        }
    }
    printSkipList(sk);
    printf("\n\n Teste se ha o valor 2: %d\n", searchSkipList(sk, 2));
    removeSkipList(sk, 4);
    printf("\n --Removido o 4--\n");
    printSkipList(sk);
    freeSkipList(sk);
return 0;
}
