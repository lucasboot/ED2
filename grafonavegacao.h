#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
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
            if (vertices[i] == rotuloVertice) {  //if (vertices.at(i).compare(rotuloVertice) == 0) {
                return i;
            }
        }
        return -1;
    }
    
    /**
    * O argumento indicesVerticesVisitados serve para controlar quais
    * vertices ja foram visitados.
    * Lembrando que DFS eh uma funcaoo recursiva.
    **/
    void dfs(string rotuloVOrigem, bool* indicesVerticesVisitados) {
        int j = obterIndiceVertice(rotuloVOrigem);
      
        if (indicesVerticesVisitados[j] == true) {
          return;
        }
        else {
          indicesVerticesVisitados[j] = true;
          for (int i = 0; i < arestas[j].size(); i++) {
              dfs(vertices[arestas[j][i].first], indicesVerticesVisitados);
          }
         }
        
        
    }

public:
    /**
    * Lembrem-se:
    *       1) nao podemos inserir vertices com mesmo rotulo
    *       2) toda vez que inserirmos um novo vertice, precisaremos
    *          inserir um vetor para representar as conexoes daquele
    *          vertice na lista de adjacncias
    **/
    void inserirVertice(string rotuloVertice) {
        if (obterIndiceVertice(rotuloVertice) == -1) {
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
        pair<int, int> aresta = make_pair(j, peso);
        arestas[i].push_back(aresta);
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

        return false;
    }

    /**
    * Verifica se ha algum caminho entre vertice rotuloVOrigem e
    * vrtice rotuloVDestino.
    * A melhor forma de fazer isto  reusando a funcao dfs.
    * void dfs(string rotuloVOrigem, bool* indicesVerticesVisitados)
    **/
    bool haCaminho(string rotuloVOrigem, string rotuloVDestino) {
        bool* v = new bool[vertices.size()]{false};
        int i = obterIndiceVertice(rotuloVOrigem);
        int j = obterIndiceVertice(rotuloVDestino);
        if (!saoConectados(rotuloVOrigem, rotuloVDestino) && i==j) {
            return false;
        }
        else {
            dfs(rotuloVOrigem, v);
            return v[j];
        }
    }

    /**
    * Muda os rtulos do vrtices dos diferentes componentes para
    * nomes que representam cores.
    * Dica: procura componentes partindo do vrtice v0 ou v1, em ordem
    * crescente (mas voc pode usar outra ordem se desejar).
    * Retorna a quantidade de componentes.
    * A melhor forma de fazer isto  reusando a funcao dfs.
    **/
    int colorir() {
        //string cores[8] = { "amarelo", "roxo", "branco", "rosa", "vermelho", "verde", "preto", "coral" };
        int tam = vertices.size();
        bool* v = new bool[tam]{false};
        int componentes = 1;
        if (tam > 0) {
            for (int i = 0; i < vertices.size(); i++) {
                bool* visitados = new bool[tam] {false};
                if (!v[i]) {
                    dfs(vertices[i], visitados);
                    for (int j = 0; j < vertices.size(); j++) {
                        if (visitados[j] == true) {
                            vertices[j] = "corComp" + to_string(componentes);
                            v[j] = true;
                        }
                    }
                    componentes++;
                }
            }
        }
        else {
            return -1;
        }
        return componentes-1;
    }

    /**
    * Usa a abordagem de navegao BFS para listar as distncias
    * entre o vrtice rotuloVOrigem e cada um dos demais vrtices.
    * A funo BFS consegue listar a menor distncia entre um vrtice
    * e os demais se o grafo for NO-PONDERADO.
    * Retorna um vetor de inteiros com a quantidade de arestas
    * (distncia) e o vrtice rotuloVOrigem e cada um dos demais vrtices.
    * No  uma funo recursiva.
    *  necessrio utilizar a ED fila.
    **/
    int* bfs(string rotuloVOrigem) {
        int o = obterIndiceVertice(rotuloVOrigem);
        int tam = vertices.size();
        int* distancias = new int[tam]{0};
        distancias[o] = 0;
        bool* visitados = new bool[tam]{false};
        visitados[o] = true;
        queue<int> q;
        q.push(o);
        while (!q.empty()) {
            int indice = q.front();
            q.pop();
            for (auto i: arestas[indice]) {
                if (!visitados[i.first]) {
                    visitados[i.first] = true;
                    distancias[i.first] = distancias[indice] + 1;
                    q.push(i.first);
                }

             
            }

        }
        
        return distancias;
      
    }

    vector<string> getVertices() {
        return vertices;
    }

    vector<vector<pair<int, int>>> getArestas() {
        return arestas;
    }
};