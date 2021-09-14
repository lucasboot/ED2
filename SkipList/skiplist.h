#ifndef SKIPLIST_H_INCLUDED
#define SKIPLIST_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
typedef struct SkipList SkipList;
struct node{
    int key;
    struct node **next;
    int level;
};

struct SkipList{
    int MaxLevel;
    float P;
    int level;
    struct node *start;

};
struct node* NewNode(int key, int level){
    struct node* newnode = malloc(sizeof(struct node));
    if(newnode != NULL){
        newnode->key = key;
        newnode->level = level;
        newnode->next = malloc((level+1)*sizeof(struct node*)); //se forem 5 niveis, 0 até o nível 5
        int i;
        for(int i = 0; i<level+1;i++){
            newnode->next[i] = NULL;
        }
        return newnode;
    }
}

SkipList* createSkipList(int MaxLevel, float P){
    SkipList *sk = (SkipList*) malloc(sizeof(SkipList));
    if(sk != NULL){
        sk->MaxLevel = MaxLevel;
        sk->P = P; //fracao dos nos
        sk->level = 0; //total atualmente
        sk->start = NewNode(-1, MaxLevel);

    }

}

void freeSkipList(SkipList* sk){
    if(sk!=NULL){
        struct node *no, *crr;
        crr = sk->start->next[0];
        while(crr != NULL){
            no = crr;
            crr = crr->next[0];
            free(no->next);
            free(no);
        }
        free(sk->start);
        free(sk);

    } else {
        return;
    }
}

int searchSkipList(SkipList *sk, int key){
    if(sk == NULL)
        return 0;
    struct node *crr = sk->start;
    for(int i = sk->level; i>=0; i--){
        while(crr->next[i] != NULL &&
                crr->next[i]->key < key){
                    crr = crr->next[i];
                }
    }
    crr = crr->next[0];
    if(crr != NULL && crr->key ==key){
        return 1;
    } else {
        return 0;
    }
}

int randomLevel (SkipList *sk){
    float r = (float)rand()/RAND_MAX;
    int level = 0;
    while (r< sk->P && level < sk->MaxLevel){
        level++;
        r = (float)rand()/RAND_MAX;
    }
    return level;
}

int insertSkipList(SkipList *sk, int key){
    if(sk == NULL)
        return 0;
    struct node *crr = sk->start;
    struct node **aux;
    aux = malloc((sk->MaxLevel+1)*sizeof(struct node*));
    for(int i = 0; i<= sk->MaxLevel; i++){
        aux[i] = NULL;
    }
    //encontrar o vetor de numeros anteriores para cada possivel nivel do novo valor
    for(int i = sk->level; i>=0; i--){
        while(crr->next[i] != NULL &&
                crr->next[i]->key<key){
                    crr = crr->next[i];
        }
        aux[i] = crr;
    }
    crr = crr->next[0];
    if(crr == NULL || crr->key != key){
        int newLevel = randomLevel(sk);
        struct node* newnode = NewNode(key, newLevel);
        if(newnode == NULL){ //erro na insercao
            free(aux);
            return 0;
        }
        //Se for um nivel maior que os anteriores, o no com os -1 precisa ser adicionado como anteiores aos novos
        if(newLevel > sk->level){
            for(int i =sk->level+1; i<= newLevel; i++){
                aux[i] = sk->start;
            }
            sk->level = newLevel;
        }
        for(int i =0; i<= newLevel; i++){
            newnode->next[i] = aux[i]->next[i];
            aux[i]->next[i] = newnode;
        }
        free(aux);
        return 1;
    }
}

int removeSkipList(SkipList *sk, int key){
    if(sk == NULL)
        return 0;
    struct node *crr = sk->start;
    struct node **aux;
    aux = malloc((sk->MaxLevel+1)*sizeof(struct node*));
    for(int i = 0; i<= sk->MaxLevel; i++){
        aux[i] = NULL;
    }
    //encontrar o vetor de numeros anteriores para cada possivel nivel do novo valor
    for(int i = sk->level; i>=0; i--){
        while(crr->next[i] != NULL &&
                crr->next[i]->key<key){
                    crr = crr->next[i];
        }
        aux[i] = crr;
    }
    crr = crr->next[0];
    if(crr != NULL && crr->key == key){
        for(int i =0; i<= sk->level; i++){
            if(aux[i]->next[i] != crr) //o que esta sendo removido nao esta em tal nivel
                break;
            aux[i]->next[i] = crr->next[i];
        }
        while(sk->level > 0 && sk->start->next[sk->level] == NULL){
            sk->level--;
        }
        free(crr->next);
        free(crr);
        free(aux);
        return 1;
    }
    free(aux);
    return 0; //o valor nao existe na SK

}

void printSkipList(SkipList* sk){
    int i, j;
    struct node *aux = NULL;
    aux = sk->start;
    while(aux != NULL){
        for(j = aux->level; j>=0; j--){
            printf("%d\t", aux->key);
        }
        printf("\n");
        aux = aux->next[0];
    }
}

#endif // SKIPLIST_H_INCLUDED
