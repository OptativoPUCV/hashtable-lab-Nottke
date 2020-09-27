#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct Pair Pair;
typedef struct HashMap HashMap;
int enlarge_called=0;

struct Pair {
     char * key;
     void * value;
};

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

void insertMap(HashMap * map, char * key, void * value) {
    int pos=hash(key,map->capacity);
    struct Pair *n=(struct Pair*)malloc(sizeof(struct Pair));
    while (map->buckets[pos]!=NULL&&is_equal(map->buckets[pos]->key,key)==0){
      pos++;
      if(pos==map->capacity){
        pos=0;
      }
    }
    n->key=key;
    n->value=value;
    map->buckets[pos]=n;
    map->size++;
    map->current=pos;
}

void enlarge(HashMap * map){
    enlarge_called = 1; //no borrar (testing purposes)
    int pos=0;
    map->size=0;
    Pair**oldbu=(Pair**)malloc(map->capacity*sizeof(Pair));
    oldbu=map->buckets;
    map->capacity=map->capacity*2;
    map->buckets=(Pair**)malloc(map->capacity*sizeof(Pair));
    while(pos<map->capacity/2){
      if(oldbu[pos]!=NULL&&oldbu[pos]->key!=NULL){
        insertMap(map,oldbu[pos]->key,oldbu[pos]->value);
        map->size++;
      }
      pos++;
    }
}

HashMap * createMap(long capacity) {
    HashMap *map =(HashMap *) malloc(sizeof(HashMap));
    map->buckets=(Pair**) calloc(capacity,sizeof(Pair*));
    map->capacity=capacity;
    map->size=0;
    map->current=-1;
    return map;
}

void eraseMap(HashMap * map,  char * key) {
    int pos=hash(key,map->capacity);
    do{
      if(is_equal(map->buckets[pos]->key,key)){
        map->buckets[pos]->key=NULL;
        map->size--;
        return;
      }      
      pos++;
    }while(map->buckets[pos]!=NULL);
}

void * searchMap(HashMap * map,  char * key) {
    int pos=hash(key,map->capacity);
    do{
      if(is_equal(map->buckets[pos]->key,key)==1){
        map->current=pos;
        return map->buckets[pos]->value;
      }
      pos++;
    }while(map->buckets[pos]!=NULL);
    return NULL;
}

void * firstMap(HashMap * map) {
    int pos=map->current;
    do{
      if((map->buckets[pos]!=NULL)&&(map->buckets[pos]->key!=NULL)){
        map->current=pos;
        return map->buckets[pos]->value;
      }
      pos++;
      if(map->current==map->capacity){
        pos=0;
      }
    }while(map->current<map->capacity);
    return NULL;
}

void * nextMap(HashMap * map) {
    int pos=map->current;
    do{
      int aux=pos+1;
      if((map->buckets[aux]!=NULL&&(map->buckets[aux]->key!=NULL))){
        map->current=aux;
        return map->buckets[aux]->value;
      }
      pos++;
      if(map->current==map->capacity){
        pos=0;
      }
    }while(pos<map->capacity);
    return NULL;
}
