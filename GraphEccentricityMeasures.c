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
  assert(!GraphIsWeighted(g));

  GraphEccentricityMeasures* result = (GraphEccentricityMeasures*)malloc(sizeof(GraphEccentricityMeasures));
  result->graph = g;

  GraphAllPairsShortestDistances* distance_matrix_graph = GraphAllPairsShortestDistancesExecute(g);
  
  unsigned int numVertices = GraphGetNumVertices(g);
  result->eccentricity = (int*)malloc(sizeof(int)*numVertices);
  int radius = 0;
  int diameter = 0;
  int currEccentricity = 0;
  for(unsigned int v = 0; v<numVertices; v++){
    for(unsigned int w = 0; w<numVertices; w++){
      if (v == w) continue;
      int distanceVW = GraphGetDistanceVW(distance_matrix_graph, v, w);
      if (distanceVW == -1){
        currEccentricity = -1;
        break;
      }
      if (currEccentricity < distanceVW){
        currEccentricity = distanceVW;
      }
    }
    result->eccentricity[v] = currEccentricity;
    if (currEccentricity == -1){
      diameter = -1; //Diameter is undifined
    }
    if (diameter != -1 && diameter < currEccentricity) diameter = currEccentricity; //If diameter is already indefinite skip
    if (currEccentricity != -1 && (radius > currEccentricity || radius == 0)) { //If Eccentricity is indefinite skip, else, if radius is still 0 or greater than Eccentricity, update it
      radius = currEccentricity; 
    } 
    currEccentricity = 0;
  }
  result->graphDiameter = diameter;
  result->graphRadius = (radius == 0) ? -1 : radius; //If eccentricity is allways indefinite, the radius will still be zero

  GraphAllPairsShortestDistancesDestroy(&distance_matrix_graph);

  unsigned int count = 0;
  for (unsigned int v = 0; v<numVertices; v++){
    if (result->eccentricity[v] == result->graphRadius){
      count++;
    }
  }

  unsigned int* centralVertices = (unsigned int*)malloc(sizeof(unsigned int)*count+1);
  centralVertices[0] = count;
  int i = 1;
  for (unsigned int v = 0; v<numVertices; v++){
    if (result->eccentricity[v] == result->graphRadius){
      centralVertices[i++] = v;
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
