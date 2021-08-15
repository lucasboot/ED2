#pragma once

#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

struct GrafoMatrizAdj {
    int** arestas;
    char** rotuloVertices;
    int verticesInseridos;
    int maxNumVertices;
};

/**
 * Se o grafo for ponderado, inicializamos cada posicao da matriz com INT_MAX.
 * Se o grafo for não ponderado, inicializamos cada posicao da matriz com 0.
 **/
struct GrafoMatrizAdj* inicializar(int numVertices, bool ponderado) {
    GrafoMatrizAdj* m = (struct GrafoMatrizAdj*)malloc(sizeof(struct GrafoMatrizAdj));
    m->verticesInseridos = 0;
    m->maxNumVertices = numVertices;
    m->rotuloVertices = (char**)malloc(numVertices * sizeof(char*));
    m->arestas = (int**)malloc(numVertices * sizeof(int*));
    for (int i = 0; i < numVertices; i++) {
        m->arestas[i] = (int*)malloc(numVertices*sizeof(int));
    }
    if (ponderado) {
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                m->arestas[i][j] = INT_MAX;
            }
        }
    }
    else {
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                m->arestas[i][j] = 0;
            }
        }
    }
    return m;
}

/**
 * A principio nao temos nenhuma ordenacao usando os rotulos.
 * Portanto, usaremos busca linear
 **/
 
 int obterIndiceVertice(struct GrafoMatrizAdj* grafo, char* rotuloVertice) {
     for (int i = 0; i < grafo->maxNumVertices; i++) {
         if (strcmp(grafo->rotuloVertices[i], rotuloVertice) == 0) {
             return i;
         }
     }
     return -1; //nao encontrado
 }

 /**
  * Se o grafo for ponderado, usamos a variavel peso para especificar o peso da aresta.
  * Se o grafo for não ponderado, passaremos o valor 1 para a variavel peso, de modo que represente existência da aresta.
  * Se um dos rotulos dos vertices nao existir, simplesmente nao faremos nada.
  **/
  
  void inserirAresta(struct GrafoMatrizAdj* grafo, char* rotuloVOrigem, char* rotuloVDestino, int peso) {
      int i = obterIndiceVertice(grafo, rotuloVDestino);
      int j = obterIndiceVertice(grafo, rotuloVOrigem);

      if (i != -1 && j != -1) {
              grafo->arestas[i][j] = peso;
              grafo->arestas[j][i] = peso;
              return;
      }
      else {
          return;
      }
      

  }

  /**
   * Usamos a variavel grafo->verticesInseridos para controlar a quantidade de vertices que ja foi inserido no grafo.
   * Logo, ela pode/deve ser usada para especificar em qual posicao o vertice a ser inserido sera alocado.
   **/
   
   void inserirVertice(struct GrafoMatrizAdj* grafo, char* rotuloVertice) {
       if (grafo->verticesInseridos <= grafo->maxNumVertices) { //tudo ok
          
           grafo->rotuloVertices[grafo->verticesInseridos] = rotuloVertice;
           grafo->verticesInseridos++;
       }
       return;

   }

   /**
    * Tomar cuidado com as diferenças semânticas entre haver conexão entre dois vertices em um grafo ponderado,
    * e haver conexão entre dois vertices em um grafo nao-ponderado.
    **/
    
    bool saoConectados(struct GrafoMatrizAdj* grafo, char* rotuloVOrigem, char* rotuloVDestino) {
        int i = obterIndiceVertice(grafo, rotuloVOrigem);
        int j = obterIndiceVertice(grafo, rotuloVDestino);
        
        if (i == -1 || j == -1) {
            return false;
        }
        else {
            int peso = grafo->arestas[i][j];
            int peso2 = grafo->arestas[j][i];
            if (peso == 0 || peso2==0) {
                return true;
            }
            else if(peso != INT_MAX || peso2 != INT_MAX){
                return true;
            }
            else {
                return false;
            }
        }
       }

    
