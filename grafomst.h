#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include "stdbool.h"
#include <algorithm>

using namespace std;

class GrafoListaAdj {
private:
    vector<string> vertices;

    //first é o indice do vertice, second é o peso (caso o grafo seja ponderado)
    vector<vector<pair<int, int>>> arestas;

    /**
    * A principio nao temos nenhuma ordenacao usando os rotulos.
    * Portanto, usaremos busca linear.
    **/
    int obterIndiceVertice(string rotuloVertice) {
        //reusar implementacao de exercicios anteriores
        for (int i = 0; i < vertices.size(); i++) {
            if (vertices[i] == rotuloVertice) {  //if (vertices.at(i).compare(rotuloVertice) == 0) {
                return i;
            }
        }
        return -1;
    }

public:
    /**
    * Lembrem-se:
    *       1) nao podemos inserir vertices com mesmo rotulo
    *       2) toda vez que inserirmos um novo vértice, precisaremos
    *          inserir um vetor para representar as conexões daquele
    *          vértice na lista de adjacências
    **/
    void inserirVertice(string rotuloVertice) {
        //reusar implementacao de exercicios anteriores
        if (obterIndiceVertice(rotuloVertice) == -1) {
            vertices.push_back(rotuloVertice);
            vector<pair<int, int>> m;
            arestas.push_back(m);
        }
        return;
    }
    void inserirVertices(GrafoListaAdj* grafo, int ini, int fim) {
        for (int i = ini; i <= fim; i++) {
            string rotulo;
            stringstream sstm;
            sstm << "v" << i;
            rotulo = sstm.str();
            grafo->inserirVertice(rotulo);
        }
    }
    /**
     * Sempre que o grafo for não ponderado, adicionaremos o peso 1,
     * por conveniência.
     **/
    void inserirArestaDirecionada(string rotuloVOrigem, string rotuloVDestino) {
        inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, 1);
    }

    void inserirArestaNaoDirecionada(string rotuloVOrigem, string rotuloVDestino) {
        inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, 1);
        inserirArestaDirecionada(rotuloVDestino, rotuloVOrigem, 1);
    }

    void inserirArestaNaoDirecionada(string rotuloVOrigem, string rotuloVDestino, int peso) {
        inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, peso);
        inserirArestaDirecionada(rotuloVDestino, rotuloVOrigem, peso);
    }

    /**
    * Inserir uma aresta entre rotuloVOrigem e rotuloVDestino com o peso
    * especificado.
    **/
    void inserirArestaDirecionada(string rotuloVOrigem, string rotuloVDestino, int peso) {
        //reusar implementacao de exercicios anteriores
        int i = obterIndiceVertice(rotuloVOrigem);
        int j = obterIndiceVertice(rotuloVDestino);
        pair<int, int> aresta = make_pair(j, peso);
        arestas[i].push_back(aresta);
    }

    //Grupo do union-find
    class Grupo {
    public:
        int pai;
        int tamanho;
    };

    //encontrar raiz 
    //pode ser com ou sem path compression
    int encontrarRaiz(Grupo* grupos, int i) {
        //IMPLEMENTAR
        while (grupos[i].pai != i) {
            i = grupos[i].pai;
        }
        return grupos[i].pai;
    }

    bool mesmoGrupo(Grupo* grupos, int a, int b) {
        //IMPLEMENTAR
        if (encontrarRaiz(grupos, a) == encontrarRaiz(grupos, b))
            return true;
        else
            return false;
    }

    int tamanhoGrupo(Grupo* grupos, int a) {
        //IMPLEMENTAR
        return grupos[a].tamanho;
    }

    void unirGrupos(Grupo* grupos, int a, int b) {
        //IMPLEMENTAR
        a = encontrarRaiz(grupos, a);
        b = encontrarRaiz(grupos, b);

        grupos[a].pai = grupos[b].pai;
        grupos[a].tamanho = grupos[a].tamanho + grupos[b].tamanho;
        grupos[b].tamanho = grupos[a].tamanho; //elementos da colisao?
    }

    //criei essa classe pra facilitar a ordenacao
    //das arestas com menor peso (que faz parte do
    //algoritmo de Kruskal)
    class Aresta {
    public:
        int origem;
        int destino;
        int peso;
        Aresta(int origem, int destino, int peso)
            : origem(origem), destino(destino), peso(peso)
        {}
    };

    //sobrescrever operator< para que a priority_queue
    //ordene como desejamos
    friend bool operator>(const Aresta& a1, const Aresta& a2) {
        //IMPLEMENTAR
        if (a1.peso > a2.peso)
            return true;
        else
            return false;
    }

    GrafoListaAdj* KruskalMST() {
        //IMPLEMENTAR

        //instanciamos um novo grafo, que armazenara
        //apenas as arestas da MST
        GrafoListaAdj* mst = new GrafoListaAdj();
        inserirVertices(mst, 1, 10);

        //adicionar vertices
        //inicializar estrutura do union-find (array de grupos)]
        int tam = vertices.size();
        Grupo* grupos = new Grupo[tam];
        for (int i = 0; i < tam; i++) {
            grupos[i].tamanho = 1;
            grupos[i].pai = obterIndiceVertice(vertices[i]);
        }

        //inicializar priority_queue
        priority_queue<Aresta, vector<Aresta>, greater<Aresta>> arestasMenorPeso;
       
        //adicionar arestas na fila    
        for (int i = 0; i < tam; i++) {
            for (int j = 0; j < arestas[i].size(); j++) {
                Aresta a(obterIndiceVertice(vertices[i]), arestas[i][j].first, arestas[i][j].second);
                arestasMenorPeso.push(a);
            }
        }
        //iterar na fila de arestas
            //se os vertices da arestas nao pertencem ao mesmo grupo
            //una-os, e adicione a aresta no MST
        while(!arestasMenorPeso.empty()) {
            if (!mesmoGrupo(grupos, arestasMenorPeso.top().origem, arestasMenorPeso.top().destino)) {
                unirGrupos(grupos, arestasMenorPeso.top().origem, arestasMenorPeso.top().destino);
                mst->inserirArestaNaoDirecionada(vertices[arestasMenorPeso.top().origem], vertices[arestasMenorPeso.top().destino], arestasMenorPeso.top().peso);
                arestasMenorPeso.pop();
            }
            else {
                arestasMenorPeso.pop();
            }
        }
        return mst;
    }

    vector<string> getVertices() {
        return vertices;
    }

    vector<vector<pair<int, int>>> getArestas() {
        return arestas;
    }
};