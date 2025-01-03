//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphBellmanFord - Bellman-Ford Algorithm
//

// Student Name : Eduardo Romano
// Student Number : 118736
// Student Name : Daniel Martins
// Student Number : 115868

/*** COMPLETE THE GraphBellmanFordAlgExecute FUNCTION ***/

#include "GraphBellmanFordAlg.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IntegersStack.h"
#include "instrumentation.h"

struct _GraphBellmanFordAlg {
  unsigned int* marked;  // To mark vertices when reached for the first time
  int* distance;  // The number of edges on the path from the start vertex
                  // distance[i]=-1, if no path found from the start vertex to i
  int* predecessor;  // The predecessor vertex in the shortest path
                     // predecessor[i]=-1, if no predecessor exists
  Graph* graph;
  unsigned int startVertex;  // The root of the shortest-paths tree
};

GraphBellmanFordAlg* GraphBellmanFordAlgExecute(Graph* g, 
                                                unsigned int startVertex) {
  assert(g != NULL);
  assert(startVertex < GraphGetNumVertices(g));
  assert(GraphIsWeighted(g) == 0);

  GraphBellmanFordAlg* result =
      (GraphBellmanFordAlg*)malloc(sizeof(struct _GraphBellmanFordAlg));
  assert(result != NULL);

  // Given graph and start vertex for the shortest-paths
  result->graph = g;
  result->startVertex = startVertex;

  unsigned int numVertices = GraphGetNumVertices(g);

  //
  // TO BE COMPLETED !!
  //
  // CREATE AND INITIALIZE
  // result->marked
  // result->distance
  // result->predecessor
  //

  // Mark all vertices as not yet visited, i.e., ZERO
  
  // No vertex has (yet) a (valid) predecessor
  
  // No vertex has (yet) a (valid) distance to the start vertex
  
  // THE ALGORTIHM TO BUILD THE SHORTEST-PATHS TREE

  // Aloca-se arrays da estrutura
  result->marked = (unsigned int*)calloc(numVertices, sizeof(unsigned int));
  result->distance = (int*)malloc(numVertices * sizeof(int));
  result->predecessor = (int*)malloc(numVertices * sizeof(int));
  assert(result->marked != NULL && result->distance != NULL && result->predecessor != NULL);

  InstrCount[0] += sizeof(struct _GraphBellmanFordAlg); //Estrutura
  InstrCount[0] += sizeof(unsigned int)*numVertices+1; //Array marked
  InstrCount[0] += 2*sizeof(int)*numVertices; //Array prececessor e distance

  // Inicializa os valores dos arrays
  for (unsigned int i = 0; i < numVertices; i++) {
    result->distance[i] = 9999999; // representa infinidade
    result->predecessor[i] = -1;
    result->marked[i] = 0;
  }
  result->distance[startVertex] = 0; //distancia do inicial é 0

  // Fase de relaxamento (V-1 iteracoes)
  for (unsigned int relaxation = 0; relaxation < numVertices - 1; relaxation++) {
    
    for (unsigned int vertice = 0; vertice < numVertices; vertice++) { //Itera sobre todos os vértices
      unsigned int* adj_vertices = GraphGetAdjacentsTo(g, vertice); //Determina os adjacentes
      for (unsigned int i_adj = 1; i_adj <= adj_vertices[0]; i_adj++) {
        unsigned int adjacente = adj_vertices[i_adj];

        //Se o vertice já tiver sido alcançado (distancia não infinita) e a distancia do vertice + 1 for menor do que a distancia atual do adjacente
        if (result->distance[vertice] != 9999999 &&
          result->distance[vertice] + 1 < result->distance[adjacente]) {

          //Atualiza a distancia atual do ajacente para a do vertice + 1, sendo o ajacente marcado e o seu predecessor será o vertice
          result->distance[adjacente] = result->distance[vertice] + 1;
          result->predecessor[adjacente] = vertice;
          result->marked[adjacente] = 1;
        }
      }
      //Liberta-se as estruturas auxiliares
      free(adj_vertices);
    }
  }

  //InstrCount[0] += sizeof(unsigned int)*(numVertices); //Espaço ocupado temporariamente (adj_vertices) para o no pior caso (valor do tamanho + numVertices-1) 
  InstrCount[0] += sizeof(unsigned int); //Espaço ocupado temporariamente (adj_vetices) para o melhor caso (valor do tamanho)
  
  for(unsigned int i = 0; i< numVertices; i++){ //Todos os vertices que continuam com distancia infinita, ficam com distancia -1
    if (result->distance[i] == 9999999){
      result->distance[i] = -1;
    }
  }


  return result;
}

void GraphBellmanFordAlgDestroy(GraphBellmanFordAlg** p) {
  assert(*p != NULL);

  GraphBellmanFordAlg* aux = *p;

  free(aux->marked);
  free(aux->predecessor);
  free(aux->distance);

  free(*p);
  *p = NULL;
}

// Getting the paths information

int GraphBellmanFordAlgReached(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->marked[v];
}

int GraphBellmanFordAlgDistance(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->distance[v];
}
Stack* GraphBellmanFordAlgPathTo(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = StackCreate(GraphGetNumVertices(p->graph));

  if (p->marked[v] == 0) {
    return s;
  }

  // Store the path
  for (unsigned int current = v; current != p->startVertex;
       current = p->predecessor[current]) {
    StackPush(s, current);
  }

  StackPush(s, p->startVertex);

  return s;
}

// DISPLAYING on the console

void GraphBellmanFordAlgShowPath(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = GraphBellmanFordAlgPathTo(p, v);

  while (StackIsEmpty(s) == 0) {
    printf("%d ", StackPop(s));
  }

  StackDestroy(&s);
}

// Display the Shortest-Paths Tree in DOT format
void GraphBellmanFordAlgDisplayDOT(const GraphBellmanFordAlg* p) {
  assert(p != NULL);

  Graph* original_graph = p->graph;
  unsigned int num_vertices = GraphGetNumVertices(original_graph);

  // The paths tree is a digraph, with no edge weights
  Graph* paths_tree = GraphCreate(num_vertices, 1, 0);

  // Use the predecessors array to add the tree edges
  for (unsigned int w = 0; w < num_vertices; w++) {
    // Vertex w has a predecessor vertex v?
    int v = p->predecessor[w];
    if (v != -1) {
      GraphAddEdge(paths_tree, (unsigned int)v, w);
    }
  }

  // Display the tree in the DOT format
  GraphDisplayDOT(paths_tree);

  // Housekeeping
  GraphDestroy(&paths_tree);
}
