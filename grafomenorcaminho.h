#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <tuple>
using namespace std;

#define POS_INF 1000000000
#define NEG_INF -1000000000

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
        //IMPLEMENTAR
        for (int i = 0; i < vertices.size(); i++) {
            if (vertices[i] == rotuloVertice) {  //if (vertices.at(i).compare(rotuloVertice) == 0) {
                return i;
            }
        }
        return -1;

    }

    /**
    * O argumento indicesVerticesVisitados serve para controlar quais
    * vertices já foram visitados.
    * Lembrando que DFS é uma função recursiva.
    **/
    void dfs(string rotuloVOrigem, bool* indicesVerticesVisitados) {
        //IMPLEMENTAR
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
    *       2) toda vez que inserirmos um novo vértice, precisaremos
    *          inserir um vetor para representar as conexões daquele
    *          vértice na lista de adjacências
    **/
    void inserirVertice(string rotuloVertice) {
        //IMPLEMENTAR
        if (obterIndiceVertice(rotuloVertice) == -1) {
            vertices.push_back(rotuloVertice);
            vector<pair<int, int>> m;
            arestas.push_back(m);
        }
        return;
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
        //IMPLEMENTAR
        int i = obterIndiceVertice(rotuloVOrigem);
        int j = obterIndiceVertice(rotuloVDestino);
        pair<int, int> aresta = make_pair(j, peso);
        arestas[i].push_back(aresta);
    }

    /**
    * Verifica se vértice rotuloVOrigem e vértice rotuloVDestino são
    * conectados (vizinhos).
    **/
    bool saoConectados(string rotuloVOrigem, string rotuloVDestino) {
        //IMPLEMENTAR
        int i = obterIndiceVertice(rotuloVOrigem);
        int k = obterIndiceVertice(rotuloVDestino);
        for (int j = 0; j < arestas[i].size(); j++) {
            if (arestas[i][j].first == k)
                return true;
        }

        return false;

    }

    /**
    * Verifica se há algum caminho entre vértice rotuloVOrigem e
    * vértice rotuloVDestino.
    * A melhor forma de fazer isto é reusando a função dfs.
    **/
    bool haCaminho(string rotuloVOrigem, string rotuloVDestino) {
        //IMPLEMENTAR
        bool* v = new bool[vertices.size()]{ false };
        int i = obterIndiceVertice(rotuloVOrigem);
        int j = obterIndiceVertice(rotuloVDestino);
        if (!saoConectados(rotuloVOrigem, rotuloVDestino) && i == j) {
            return false;
        }
        else {
            dfs(rotuloVOrigem, v);
            return v[j];
        }
    }

    /**
    * Muda os rótulos do vértices dos diferentes componentes para
    * nomes que representam cores.
    * Dica: procura componentes partindo do vértice v0 ou v1, em ordem
    * crescente (mas você pode usar outra ordem se desejar).
    * Retorna a quantidade de componentes.
    * A melhor forma de fazer isto é reusando a função dfs.
    **/
    int colorir() {
        //IMPLEMENTAR
        int tam = vertices.size();
        bool* v = new bool[tam] {false};
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
        return componentes - 1;
    }

    /**
    * Usa a abordagem de navegação BFS para listar as distâncias
    * entre o vértice rotuloVOrigem e cada um dos demais vértices.
    * A função BFS consegue listar a menor distância entre um vértice
    * e os demais se o grafo for NÃO-PONDERADO.
    * Retorna um vetor de inteiros com a quantidade de arestas
    * (distância) e o vértice rotuloVOrigem e cada um dos demais vértices.
    * Não é uma função recursiva.
    * É necessário utilizar a ED fila.
    **/
    int* bfs(string rotuloVOrigem) {
        //IMPLEMENTAR
        int o = obterIndiceVertice(rotuloVOrigem);
        int tam = vertices.size();
        int* distancias = new int[tam] {0};
        distancias[o] = 0;
        bool* visitados = new bool[tam] {false};
        visitados[o] = true;
        queue<int> q;
        q.push(o);
        while (!q.empty()) {
            int indice = q.front();
            q.pop();
            for (auto i : arestas[indice]) {
                if (!visitados[i.first]) {
                    visitados[i.first] = true;
                    distancias[i.first] = distancias[indice] + 1;
                    q.push(i.first);
                }


            }

        }

        return distancias;
    }

    /**
    * Usamos o BellmanFord para encontrar menor caminho
    * em grafos com arestas com pesos negativos.
    * O BellmanFord consegue detectar ciclos negativos
    * e atribuir o valor NEG_INF (constante definida nesse arquivo)
    * aos vértices que pertençam ao ciclo.
    * POS_INF deve ser atribuído aos vértices inalcançáveis.
    * O aspecto negativo é sua complexidade de tempo: O(V*E).
    * Isto acontece pois, como possui arestas negativas, cada vértice
    * do grafo precisa ser processado V vezes.
    * Pseudo-código: https://github.com/eduardolfalcao/edii/blob/master/conteudos/Grafos.md#bellman-ford
    **/
    int* bellmanFord(string rotuloVOrigem) {
        //IMPLEMENTAR
        int tam = vertices.size();
        int x = obterIndiceVertice(rotuloVOrigem);
        int* distancias = new int[tam] {POS_INF};
        std::fill_n(distancias, tam, POS_INF);
        distancias[x] = 0;

        for (int i = 0; i < tam-1; i++) {
            for(int j = 0; j<tam; j++){
                for (auto e : arestas[j]) {
                    if (distancias[j] + e.second < distancias[e.first]) {
                        distancias[e.first] = distancias[j] + e.second;
                    }
                }
            }
        }
        for (int i = 0; i < tam - 1; i++) {
            for (int j = 0; j < tam; j++) {
                for (auto e : arestas[j]) {
                    if (distancias[j] + e.second < distancias[e.first]) {
                        distancias[e.first] = NEG_INF;
                    }
                }
            }
        }

        return distancias;


    }

    /**
    * Usamos o Dijkstra para encontrar menor caminho
    * em grafos sem arestas com pesos negativos.
    * O Dijkstra retorna respostas incorretas caso o grafo
    * possua arestas negativas, e portanto não consegue
    * detectar ciclos negativos. Este é o aspecto negativo.
    * POS_INF deve ser atribuído aos vértices inalcançáveis.
    * O aspecto positivo é sua complexidade de tempo: O(V+E).
    * Isto acontece pois, como o grafo não possui arestas negativas,
    * cada vértice do grafo precisa ser processado apenas 1 vez.
    * Pseudo-código: https://github.com/eduardolfalcao/edii/blob/master/conteudos/Grafos.md#dijkstra
    * Ilustração: https://docs.google.com/drawings/d/1NmkJPHpcg8uVcDZ24FQiYs3uHR5n-rdm1AZwD74WiMY/edit?usp=sharing
    **/


    
    int* dijkstra(string rotuloVOrigem) {
        //IMPLEMENTAR
        priority_queue<pair<int, int>> fila;
        int x = obterIndiceVertice(rotuloVOrigem);
        int tam = vertices.size();
        int* distancias = new int[tam] {POS_INF};
        std::fill(distancias, distancias + tam, POS_INF);
        distancias[x] = 0;
        fila.push({ 0, x });
        bool* visitadas = new bool[tam] {false};

        while (!fila.empty()) {
            int y = fila.top().second;
            fila.pop();
            if (visitadas[y]) {
                continue;
            }
            else {
                visitadas[y] = true;
                for (auto u : arestas[y]) {
                    int b = u.first;
                    int w = u.second;
                    if (distancias[y] + w < distancias[b]) {
                        distancias[b] = distancias[y] + w;
                        fila.push({ -distancias[b], b});
                    }
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