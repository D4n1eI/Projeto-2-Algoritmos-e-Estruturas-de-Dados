//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphTransitiveClosure - Transitive Closure of a directed graph
//

// Student Name : Eduardo Romano
// Student Number : 118736
// Student Name : Daniel Martins
// Student Number : 115868

/*** COMPLETE THE GraphComputeTransitiveClosure FUNCTION ***/

#include "GraphTransitiveClosure.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "instrumentation.h"

// Compute the transitive closure of a directed graph
// Return the computed transitive closure as a directed graph
// Use the Bellman-Ford algorithm
Graph* GraphComputeTransitiveClosure(Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g));
  assert(GraphIsWeighted(g) == 0);

  // COMPLETE THE CODE
  unsigned int numVertices = GraphGetNumVertices(g);
  Graph* fecho_transivito = GraphCreate(numVertices, GraphIsDigraph(g), GraphIsWeighted(g)); //Cria-se um grafo com o mesmo numero de vertices

  for (unsigned int v = 0; v<numVertices; v++){
    //Itera-se sobre cada vertice, usando o algoritmo de BellmanFord para cada um
    GraphBellmanFordAlg* bellman_alg = GraphBellmanFordAlgExecute(g, v);
    
    for (unsigned int bellman_v = 0; bellman_v<numVertices; bellman_v++){
      //Para todos os vertices que nao sao o atual, se foram alcançados, adiciona-se uma aresta
      if (v != bellman_v && GraphBellmanFordAlgReached(bellman_alg, bellman_v)){
        GraphAddEdge(fecho_transivito, v, bellman_v);
      }
    }
    //Liberta-se as estruturas auxiliares
    GraphBellmanFordAlgDestroy(&bellman_alg);
  }
  
  GraphCheckInvariants(fecho_transivito);
  return fecho_transivito;
}
