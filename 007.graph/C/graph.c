#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "heap.h"
#include "list.h"
#include "main.h"
#include "queue.h"
#include "stack.h"

/* Private prototypes */
void    graph_destroy_vertex          (void ** data);
int32_t graph_compare_vertex          (void * data_a, void * data_b);
void    graph_print_vertex            (void * data);

int32_t graph_update_vertex_distance  (void * to_update, void * new_data);
int32_t graph_compare_vertex_distance (void * data_a, void * data_b);
void    graph_print_vertex_distance   (void * data);

void    graph_destroy_edge            (void ** data);
int32_t graph_compare_edge            (void * data_a, void * data_b);
void    graph_print_edge              (void * data);

graph_t * graph_init(print_ft print) {
    graph_t * graph = calloc(1, sizeof(*graph));

    if (graph == NULL) {
        errno = GRAPH_FAIL_MALLOC;
        return NULL;
    }

    graph->vertexs = list_init(graph_compare_vertex, graph_print_vertex);

    graph->api.print = print;

    if (graph->vertexs == NULL) {
        memset(graph, 0x00, sizeof(*graph));
        free(graph);
        errno = GRAPH_FAIL_MALLOC;
        return NULL;
    }

    errno = GRAPH_SUCCESS;
    return graph;
}

void graph_destroy(graph_t ** graph) {
    if (*graph == NULL) {
        errno = GRAPH_ARGS_NULL;
        return;
    }

    graph_vertex_t * vertex = NULL;

    while ((vertex = list_lookup_next((*graph)->vertexs, vertex)) != NULL) {
        list_destroy(&(vertex->edges), graph_destroy_edge);
    }

    list_destroy(&((*graph)->vertexs), graph_destroy_vertex);

    memset(*graph, 0x00, sizeof(**graph));
    free(*graph);
    *graph = NULL;
}

int8_t graph_ins_vert(graph_t * graph, void * data) {
    if ((graph == NULL) || (data == NULL))
        return GRAPH_ARGS_NULL;

    int8_t retval;
    graph_vertex_t * new_vertex = calloc(1, sizeof(*new_vertex));
    new_vertex->v = data;

    /* We do not allow repeated elements */
    if (list_lookup(graph->vertexs, new_vertex) != NULL) {
        memset(new_vertex, 0x00, sizeof(*new_vertex));
        free(new_vertex);
        return GRAPH_VERTEX_EXISTS;
    }

    if (new_vertex == NULL)
        return GRAPH_FAIL_MALLOC;

    new_vertex->edges = list_init(graph_compare_edge, graph_print_edge);

    if (new_vertex->edges == NULL) {
        memset(new_vertex, 0x00, sizeof(*new_vertex));
        free(new_vertex);
        return GRAPH_FAIL_MALLOC;
    }

    retval = list_ins_next(graph->vertexs, NULL, new_vertex);

    if (retval != LST_SUCCESS) {
        list_destroy(&(new_vertex->edges), NULL);
        memset(new_vertex, 0x00, sizeof(*new_vertex));
        free(new_vertex);
        return GRAPH_FAIL_MALLOC;
    }

    ++graph->vcnt;

    printf("Add [vert] ");
    graph->api.print(data);
    printf("\n");

    return GRAPH_SUCCESS;
}

int8_t graph_ins_edge(graph_t * graph, void * data_a, void *
        data_b, int32_t cost, int8_t dual) {
    if ((graph == NULL) || (data_a == NULL) || (data_b == NULL))
        return GRAPH_ARGS_NULL;

    graph_vertex_t * element_tmp = NULL;
    graph_vertex_t * element_a = NULL;
    graph_vertex_t * element_b = NULL;
    graph_edge_t   * edge_a = calloc(1, sizeof(*edge_a));
    graph_edge_t   * edge_b = NULL;

    if (dual == GRAPH_EDGE_DUAL)
        edge_b = calloc(1, sizeof(*edge_b));

    element_tmp = calloc(1, sizeof(*element_tmp));
    element_tmp->v = data_a;

    element_a = list_lookup(graph->vertexs, element_tmp) ;

    if (element_a == NULL) {
        memset(edge_a, 0x00, sizeof(*edge_a));
        free(edge_a);

        if (edge_b != NULL) {
            memset(edge_b, 0x00, sizeof(*edge_b));
            free(edge_b);
        }

        memset(element_tmp, 0x00, sizeof(*element_tmp));
        free(element_tmp);
        return GRAPH_VERTEX_NOT_FOUND;
    }

    element_tmp->v = data_b;

    element_b = list_lookup(graph->vertexs, element_tmp);

    memset(element_tmp, 0x00, sizeof(*element_tmp));
    free(element_tmp);

    if (element_b == NULL) {
        memset(edge_a, 0x00, sizeof(*edge_a));
        free(edge_a);
        if (edge_b != NULL) {
            memset(edge_b, 0x00, sizeof(*edge_b));
            free(edge_b);
        }
        return GRAPH_VERTEX_NOT_FOUND;
    }

    printf("Add [edge] ");
    graph_print_vertex(element_a);

    edge_a->vertex = element_a;
    edge_a->incident = element_b;
    edge_a->cost = cost;
    ++element_a->info.degree;
    list_ins_next(element_a->edges, NULL, edge_a);

    if (edge_b != NULL) {
        edge_b->vertex = element_b;
        edge_b->incident = element_a;
        edge_b->cost = cost;
        list_ins_next(element_b->edges, NULL, edge_b);
        printf("<-> ");
    } else {
        printf("-> ");
    }

    ++element_b->info.degree;
    graph_print_vertex(element_b);
    printf("| cost %d\n", edge_a->cost);

    ++graph->ecnt;

    return GRAPH_SUCCESS;
}

int8_t graph_bfs(graph_t * graph, void * start_point) {
    if ((graph == NULL) || (start_point == NULL))
        return GRAPH_ARGS_NULL;

    printf("\n---> Running BFS <---\n");

    queue_t * queue = NULL;

    graph_vertex_t * vertex = NULL;
    graph_vertex_t * reset = NULL;
    graph_edge_t * edge = NULL;

    graph_vertex_t * vertex_tmp = calloc(1, sizeof(*vertex_tmp));

    int32_t longestpath = 0;

    if (vertex_tmp == NULL)
        return GRAPH_FAIL_MALLOC;

    vertex_tmp->v = start_point;

    vertex = list_lookup(graph->vertexs, vertex_tmp);

    memset(vertex_tmp, 0x00, sizeof(*vertex_tmp));
    free(vertex_tmp);

    if (vertex == NULL)
        return GRAPH_VERTEX_NOT_FOUND;

    while ((reset = list_lookup_next(graph->vertexs, reset)) != NULL) {
        reset->info.parent = NULL;
        reset->info.status = GRAPH_VERTEX_NVISITED;
        reset->info.distance = 0;
    }

    queue = queue_init();

    if (queue == NULL)
        return GRAPH_FAIL_MALLOC;

    vertex->info.status = GRAPH_VERTEX_VISTED;
    queue_add(queue, vertex);

    printf("------------------------------------------\n");
    printf("Paths from: ");
    graph_print_vertex(vertex);
    printf("\n------------------------------------------\n");

    while ((vertex = queue_get(queue)) != NULL) {
        edge = NULL;
        while ((edge = list_lookup_next(vertex->edges, edge)) != NULL) {
            if (edge->incident->info.status == GRAPH_VERTEX_NVISITED) {
                graph_print_vertex(edge->incident);
                edge->incident->info.parent = vertex;
                edge->incident->info.distance = vertex->info.distance + 1;
                edge->incident->info.status = GRAPH_VERTEX_VISTED;
                queue_add(queue, edge->incident);

                if (longestpath < edge->incident->info.distance)
                    longestpath = edge->incident->info.distance;

                printf(" = distance: %d", edge->incident->info.distance);
                printf("\n");
            }
        }
    }

    queue_destroy(&queue, NULL);
    printf("------------------------------------------\n");
    return longestpath;
}

int8_t graph_mst_prism(graph_t * graph, void * start_point) {
    if ((graph == NULL) || (start_point == NULL))
        return GRAPH_ARGS_NULL;

    printf("\n---> Running Prim MST <---\n");

    graph_vertex_t * reset  = NULL;
    graph_vertex_t * vertex = NULL;
    graph_vertex_t * vertex_tmp = calloc(1, sizeof(*vertex_tmp));
    graph_edge_t   * edge = NULL;
    int32_t new_distance;

    if (vertex_tmp == NULL)
        return GRAPH_FAIL_MALLOC;

    vertex_tmp->v = start_point;

    vertex = list_lookup(graph->vertexs, vertex_tmp);

    memset(vertex_tmp, 0x00, sizeof(*vertex_tmp));
    free(vertex_tmp);

    if (vertex == NULL)
        return GRAPH_VERTEX_NOT_FOUND;

    heap_t * hmst = hp_init(graph->vcnt, graph_compare_vertex_distance,
            graph_print_vertex_distance);

    if (hmst == NULL)
        return GRAPH_FAIL_MALLOC;

    while ((reset = list_lookup_next(graph->vertexs, reset)) != NULL) {
        reset->info.parent = NULL;
        reset->info.status = GRAPH_VERTEX_NVISITED;

        if (reset == vertex)
            reset->info.distance = 0;
        else
            reset->info.distance = GRAPH_INFINIT;

        hp_insert(hmst, reset);
    }

    while ((vertex = hp_extract(hmst)) != NULL) {
        vertex->info.status = GRAPH_VERTEX_VISTED;
        if (vertex->info.distance != GRAPH_INFINIT) {
            edge = NULL;
            while ((edge = list_lookup_next(vertex->edges, edge)) != NULL) {
                if (edge->incident->info.status == GRAPH_VERTEX_NVISITED) {
                    new_distance = edge->cost;
                    if (edge->incident->info.distance > new_distance) {
                        hp_update(hmst, edge->incident, &new_distance,
                                graph_update_vertex_distance);
                        edge->incident->info.parent = edge->vertex;
                        graph_print_edge(edge);
                        printf("\n");
                    }
                }
            }
        }
    }

    //hp_destroy(hmst, NULL);

    return GRAPH_SUCCESS;
}

int8_t graph_dijkstra(graph_t * graph, void * start_point) {
    if ((graph == NULL) || (start_point == NULL))
        return GRAPH_ARGS_NULL;

    printf("\n---> Running Dijkstra <---\n");

    graph_vertex_t * reset  = NULL;
    graph_vertex_t * vertex_tmp = NULL;
    graph_vertex_t * vertex = NULL;
    graph_edge_t   * edge = NULL;
    graph_vertex_t ** prev = NULL;
    stack_t * shortest_path = NULL;
    int32_t new_distance;
    int32_t idx = 0;
    int32_t dist[graph->vcnt];

    vertex_tmp = calloc(1, sizeof(*vertex_tmp));

    if (vertex_tmp == NULL)
        return GRAPH_FAIL_MALLOC;

    prev = calloc(graph->ecnt, sizeof(*prev));

    if (prev == NULL) {
        memset(vertex_tmp, 0x00, sizeof(*vertex_tmp));
        free(vertex_tmp);
        return GRAPH_FAIL_MALLOC;
    }

    shortest_path = stack_init(NULL);

    vertex_tmp->v = start_point;

    vertex = list_lookup(graph->vertexs, vertex_tmp);

    memset(vertex_tmp, 0x00, sizeof(*vertex_tmp));
    free(vertex_tmp);

    if (vertex == NULL) {
        free(prev);
        return GRAPH_VERTEX_NOT_FOUND;
    }

    heap_t * hmst = hp_init(graph->vcnt, graph_compare_vertex_distance,
            graph_print_vertex_distance);

    if (hmst == NULL) {
        free(prev);
        return GRAPH_FAIL_MALLOC;
    }

    idx = 0;
    while ((reset = list_lookup_next(graph->vertexs, reset)) != NULL) {
        reset->info.parent = NULL;
        reset->info.status = GRAPH_VERTEX_NVISITED;

        if (reset == vertex) {
            reset->info.distance = 0;
            dist[idx] = 0;
        } else {
            reset->info.distance = GRAPH_INFINIT;
            dist[idx] = GRAPH_INFINIT;
        }

        ++idx;

        hp_insert(hmst, reset);
    }

    idx = 0;
    while ((vertex = hp_extract(hmst)) != NULL) {
        if (vertex->info.distance != GRAPH_INFINIT) {
            edge = NULL;
            while ((edge = list_lookup_next(vertex->edges, edge)) != NULL) {
                new_distance = vertex->info.distance + edge->cost;
                if (new_distance < edge->incident->info.distance) {
                    hp_update(hmst, edge->incident, &new_distance,
                            graph_update_vertex_distance);
                    edge->incident->info.parent = edge->vertex;
                    prev[idx] = vertex;
                    dist[idx] = new_distance;
                    ++idx;
                }
            }
        }
    }

    idx = 0;
    while (prev[idx] != NULL) {
        stack_push(shortest_path, prev[idx]);
        graph_print_vertex(prev[idx]);
        printf(" | %d \n", dist[idx]);

        ++idx;
    }

    free(prev);

    while((vertex = stack_pop(shortest_path)) != NULL) {
            }

    //stack_destroy(&shortest_path, NULL);
    //hp_destroy(hmst, NULL);

    return GRAPH_SUCCESS;
}

void graph_destroy_vertex(void ** data) {
    //FIXME: I don't know which approach would be the best one
    // to have access to the data from the first layer (main)
    graph_vertex_t *vertex = (graph_vertex_t *) *data;
    main_destroy(&(vertex->v));
    memset(vertex, 0x00, sizeof(*vertex));
    free(vertex);
    *data = NULL;
}

int32_t graph_compare_vertex(void * data_a, void * data_b) {
    //FIXME: I don't know which approach would be the best one
    // to have access to the data from the first layer (main)
    graph_vertex_t * vertex_a = (graph_vertex_t *) data_a;
    graph_vertex_t * vertex_b = (graph_vertex_t *) data_b;
    return main_compare(vertex_a->v, vertex_b->v);
}

void graph_print_vertex(void * data) {
    //FIXME: I don't know which approach would be the best one
    // to have access to the data from the first layer (main)
    graph_vertex_t * vertex = (graph_vertex_t *) data;
    main_print(vertex->v);
}


int32_t graph_update_vertex_distance(void * to_update, void * new_data) {
    graph_vertex_t * vertex_to_update = (graph_vertex_t *) to_update;
    int32_t * new_distance = (int32_t *) new_data;
    vertex_to_update->info.distance = *new_distance;
    return GRAPH_SUCCESS;
}

int32_t graph_compare_vertex_distance(void * data_a, void * data_b) {
    graph_vertex_t * vertex_a = (graph_vertex_t *) data_a;
    graph_vertex_t * vertex_b = (graph_vertex_t *) data_b;
    return vertex_b->info.distance - vertex_a->info.distance;
}

void graph_print_vertex_distance(void * data) {
    //FIXME: I don't know which approach would be the best one
    // to have access to the data from the first layer (main)
    graph_vertex_t * vertex = (graph_vertex_t *) data;
    main_print(vertex->v);
    printf(" - Distance: %d\n", vertex->info.distance);
}

void graph_destroy_edge(void ** data) {
    graph_edge_t * edge_a = (graph_edge_t *) *data;
    memset(edge_a, 0x00, sizeof(*edge_a));
    free(edge_a);
    *data = NULL;
}

int32_t graph_compare_edge(void * data_a, void * data_b) {
    return data_a - data_b;
}

void graph_print_edge(void * data) {
    graph_edge_t * edge = (graph_edge_t *) data;
    printf("| ");
    graph_print_vertex(edge->vertex);
    printf("-> ");
    graph_print_vertex(edge->incident);
    printf(": %d ", edge->cost);
}

void graph_print_vertexes(graph_t * graph) {
    if (graph == NULL) {
        errno = GRAPH_ARGS_NULL;
        return;
    }

    printf("\n---> Graph Vertexes Info <----\n");
    graph_vertex_t * vertex = NULL;
    graph_edge_t * edge = NULL;

    while ((vertex = list_lookup_next(graph->vertexs, vertex)) != NULL) {
        printf("V: ");
        graph_print_vertex(vertex);
        printf(" - Degree: %d | Distance %d\n", vertex->info.degree,
                vertex->info.distance);
        while ((edge = list_lookup_next(vertex->edges, edge)) != NULL) {
            graph_print_edge(edge);
        }
        printf("\n\n");
    }
}
