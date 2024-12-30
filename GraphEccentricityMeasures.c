//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphEccentricityMeasures
//

// Student Name : Eduardo Romano
// Student Number : 118736
// Student Name : Daniel Martins
// Student Number : 115868

/*** COMPLETE THE GraphEccentricityMeasuresCompute FUNCTION ***/
/*** COMPLETE THE GraphGetCentralVertices FUNCTION ***/
/*** COMPLETE THE GraphEccentricityMeasuresPrint FUNCTION ***/

#include "GraphEccentricityMeasures.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphAllPairsShortestDistances.h"

struct _GraphEccentricityMeasures {
  unsigned int*
      centralVertices;  // centralVertices[0] = number of central vertices
                        // array size is (number of central vertices + 1)
  int* eccentricity;    // the eccentricity value of each vertex
  Graph* graph;         // the graph
  int graphRadius;      // the graph radius
  int graphDiameter;    // the graph diameter
};

// Allocate memory
// Compute the vertex eccentricity values
// Compute graph radius and graph diameter
// Compute the set of central vertices
GraphEccentricityMeasures* GraphEccentricityMeasuresCompute(Graph* g) {
  assert(g != NULL);

  // COMPLETE THE CODE
  // CREATE AUXILIARY (static) FUNCTIONS, IF USEFUL
  // Graph radius --- the smallest vertex eccentricity value
  // Graph diameter --- the largest vertex eccentricity value
  // Do not forget that -1 represents an IDEFINITE value

  // Computing the set of central vertices
  // Allocate the central vertices array : number of central vertices + 1
  // Fill in the central vertices array

  assert(GraphIsDigraph(g));
  assert(GraphIsWeighted(g) == 0);

  GraphEccentricityMeasures* result = (GraphEccentricityMeasures*)malloc(sizeof(GraphEccentricityMeasures));
  result->graph = g;

  //Determina-se a matriz de distancias
  GraphAllPairsShortestDistances* distance_matrix_graph = GraphAllPairsShortestDistancesExecute(g);
  
  //Inicializa-se os valores
  unsigned int numVertices = GraphGetNumVertices(g);
  result->eccentricity = (int*)malloc(sizeof(int)*numVertices);
  int radius = 0;
  int diameter = 0;
  int currEccentricity = 0;

  //Determina-se a excentricidade de um vertice v, calculando a sua distancia entre cada vertice w
  for(unsigned int v = 0; v<numVertices; v++){
    for(unsigned int w = 0; w<numVertices; w++){
      if (v == w) continue; //Igora-se a si proprio

      int distanceVW = GraphGetDistanceVW(distance_matrix_graph, v, w);
      if (distanceVW == -1){ //Se a distancia for -1 (unreachable), a excentricidade vai ser imediatamente -1 (terminando o loop entre w)
        currEccentricity = -1;
        break;
      }

      //Se w for alcançavel
      if (currEccentricity < distanceVW){ //Se a distancia for maior do que a excentricidade atual, atualiza-a
        currEccentricity = distanceVW;
      }
    }

    result->eccentricity[v] = currEccentricity; //Atribui a excentricidade ao vertice v
    if (currEccentricity == -1){
      diameter = -1; //Se for unreachable, o diametro também o será
    }
    if (diameter != -1 && diameter < currEccentricity) diameter = currEccentricity; //Se o diametro não for unreachable e for menor do que a excentricidade de v, atualiza
    if (currEccentricity != -1 && (radius == 0 || radius > currEccentricity)) { //Se a excentricidade não for unreachable e (se o raio ainda não foi atualizado ou é maior do que a excentricidade) atualiza-o
      radius = currEccentricity; 
    } 
    currEccentricity = 0; //Reset da excentricidade
  }

  //atribui o raio e diametro
  result->graphDiameter = diameter;
  result->graphRadius = (radius == 0) ? -1 : radius; //Se o raio nunca foi atualizado (ainda é zero), todos os vertices têm excentridicade -1, fazendo o raio ser -1 também

 //Liberta as estruturas auxiliares
  GraphAllPairsShortestDistancesDestroy(&distance_matrix_graph);

  //Determinação dos vértices centrais
  unsigned int count = 0;
  //Conta o numero de vezes que a excentricidade de um vertice é igual ao raio
  for (unsigned int v = 0; v<numVertices; v++){
    if (result->eccentricity[v] == result->graphRadius){
      count++;
    }
  }

  //Alloca a memoria para o array
  unsigned int* centralVertices = (unsigned int*)malloc(sizeof(unsigned int)*count+1); //numero de vertices centrais + 1 (elemento de indice 0 com o numero de vertices)
  centralVertices[0] = count;//atribui o valor inicial ao array
  int i = 1;
  for (unsigned int v = 0; v<numVertices; v++){
    if (result->eccentricity[v] == result->graphRadius){
      centralVertices[i++] = v; //coloca o vertice central no array
    }
  }
  result->centralVertices = centralVertices;

  return result;
}

void GraphEccentricityMeasuresDestroy(GraphEccentricityMeasures** p) {
  assert(*p != NULL);

  GraphEccentricityMeasures* aux = *p;

  free(aux->centralVertices);
  free(aux->eccentricity);

  free(*p);
  *p = NULL;
}

// Getting the computed measures

int GraphGetRadius(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphRadius;
}

int GraphGetDiameter(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphDiameter;
}

int GraphGetVertexEccentricity(const GraphEccentricityMeasures* p,
                               unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(p->eccentricity != NULL);

  return p->eccentricity[v];
}

// Getting a copy of the set of central vertices
// centralVertices[0] = number of central vertices in the set
unsigned int* GraphGetCentralVertices(const GraphEccentricityMeasures* p) {
  assert(p != NULL);
  assert(p->centralVertices != NULL);

  // COMPLETE THE CODE
  unsigned int numVertices = p->centralVertices[0];
  unsigned int* centralVertices = (unsigned int*)malloc(sizeof(unsigned int)*numVertices);
  for(unsigned int v=0; v<numVertices; v++){
    centralVertices[v] = p->centralVertices[v+1];
  }

  return centralVertices;
}

// Print the graph radius and diameter
// Print the vertex eccentricity values
// Print the set of central vertices
void GraphEccentricityMeasuresPrint(const GraphEccentricityMeasures* p) {
  // COMPLETE THE CODE

  printf("Radius: %d\nDiameter: %d\n", GraphGetRadius(p), GraphGetDiameter(p));

  unsigned int numVertices = GraphGetNumVertices(p->graph);
  printf("Vertex Eccentricity:\n");
  for (unsigned int v = 0; v<numVertices; v++){
    printf("\t%u: %d\n", v, GraphGetVertexEccentricity(p, v));
  }
  
  unsigned int numCentVertices = p->centralVertices[0];
  printf("Central Vertices: [ ");
  for (unsigned int cv = 1; cv<numCentVertices; cv++){
    printf("%u, ", p->centralVertices[cv]);
  }
  printf("%u ]\n", p->centralVertices[numCentVertices]);

}
