GraphBellmanFordAlg* GraphBellmanFordAlgExecute(Graph* g,
                                                unsigned int startVertex) {
  assert(g != NULL);
  assert(startVertex < GraphGetNumVertices(g));
  assert(GraphIsWeighted(g) == 1);

  GraphBellmanFordAlg* result =
      (GraphBellmanFordAlg*)malloc(sizeof(struct _GraphBellmanFordAlg));
  assert(result != NULL);

  unsigned int numVertices = GraphGetNumVertices(g);

  // Allocate memory for marked, distance, and predecessor arrays
  result->marked = (unsigned int*)calloc(numVertices, sizeof(unsigned int));
  result->distance = (int*)malloc(numVertices * sizeof(int));
  result->predecessor = (int*)malloc(numVertices * sizeof(int));

  assert(result->marked != NULL);
  assert(result->distance != NULL);
  assert(result->predecessor != NULL);

  // Initialize all distances to "infinity" (INT_MAX) and predecessors to -1
  for (unsigned int i = 0; i < numVertices; i++) {
    result->distance[i] = INT_MAX;
    result->predecessor[i] = -1;
  }

  // Set the start vertex distance to 0
  result->distance[startVertex] = 0;

  // Perform relaxation |V| - 1 times
  for (unsigned int i = 1; i < numVertices; i++) {
    for (unsigned int u = 0; u < numVertices; u++) {
      unsigned int* adj = GraphGetAdjacentsTo(g, u);
      double* weights = GraphGetDistancesToAdjacents(g, u);

      for (unsigned int j = 0; adj[j] != -1; j++) {
        unsigned int v = adj[j];
        double weight = weights[j];

        if (result->distance[u] != INT_MAX &&
            result->distance[u] + weight < result->distance[v]) {
          result->distance[v] = result->distance[u] + weight;
          result->predecessor[v] = u;
        }
      }

      free(adj);
      free(weights);
    }
  }

  // Check for negative weight cycles
  for (unsigned int u = 0; u < numVertices; u++) {
    unsigned int* adj = GraphGetAdjacentsTo(g, u);
    double* weights = GraphGetDistancesToAdjacents(g, u);

    for (unsigned int j = 0; adj[j] != -1; j++) {
      unsigned int v = adj[j];
      double weight = weights[j];

      if (result->distance[u] != INT_MAX &&
          result->distance[u] + weight < result->distance[v]) {
        printf("Graph contains a negative weight cycle.\n");
        free(adj);
        free(weights);
        GraphBellmanFordAlgDestroy(&result);
        return NULL;
      }
    }

    free(adj);
    free(weights);
  }

  return result;
}
