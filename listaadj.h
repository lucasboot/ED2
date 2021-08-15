#pragma once
#include <iostream>
#include <vector>
using namespace std;

class GrafoListaAdj {
private:
    vector<string> vertices;

    //first  o indice do vertice, second  o peso (caso o grafo seja ponderado)
    vector<vector<pair<int, int>>> arestas;

    /**
    * A principio nao temos nenhuma ordenacao usando os rotulos.
    * Portanto, usaremos busca linear.
    **/
    int obterIndiceVertice(string rotuloVertice) {
        for (int i = 0; i < vertices.size(); i++) {
            if(vertices[i] == rotuloVertice){  //if (vertices.at(i).compare(rotuloVertice) == 0) {
                return i;
            }
        }
        return -1;
    }

public:
    /**
    * Lembrem-se:
    *       1) nao podemos inserir vertices com mesmo rotulo
    *       2) toda vez que inserirmos um novo vertice, precisaremos
    *          inserir um vetor para representar as conexoes daquele
    *          vertice na lista de adjacencias
    **/
    void inserirVertice(string rotuloVertice) {
        if (obterIndiceVertice(rotuloVertice) != -1) {
            vertices.push_back(rotuloVertice);
            vector<pair<int, int>> m;
            arestas.push_back(m);
        }
        return;
    }

    /**
    * Sempre que o grafo for nao ponderado, adicionaremos o peso 1,
    * por conveniencia.
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
        int i = obterIndiceVertice(rotuloVOrigem);
        int j = obterIndiceVertice(rotuloVDestino);
        arestas[i].push_back(make_pair(j, peso));
    }

    /**
    * Verifica se vertice rotuloVOrigem e vertice rotuloVDestino sao
    * conectados (vizinhos).
    **/
    bool saoConectados(string rotuloVOrigem, string rotuloVDestino) {
        int i = obterIndiceVertice(rotuloVOrigem);
        int k = obterIndiceVertice(rotuloVDestino);
        for (int j = 0; j < arestas[i].size(); j++) {
            if (arestas[i][j].first == k)
                return true;
        }
        for (int j = 0; j < arestas[k].size(); j++) {
            if (arestas[k][j].first == i)
                return true;
        }
        return false;
    }

    vector<string> getVertices() {
        return vertices;
    }

    vector<vector<pair<int, int>>> getArestas() {
        return arestas;
    }
};