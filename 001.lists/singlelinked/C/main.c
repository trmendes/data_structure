#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "list.h"

int32_t main() {
    list_t * list = list_init(compare, print);
    data_t * data = NULL;
    data_t * dataprev = calloc(1, sizeof(*dataprev));
    data_t * remove = NULL;

    data = calloc(1, sizeof(*data));
    data->key = 10;
    //list_ins_next(list, NULL, data);
    list_ins_in_order(list, data);

    dataprev->key = 1;
    data = calloc(1, sizeof(*data));
    data->key = 2;
    //list_ins_next(list, NULL, data);
    list_ins_in_order(list, data);

    dataprev->key = 2;
    data = calloc(1, sizeof(*data));
    data->key = 5;
    //list_ins_next(list, NULL, data);
    list_ins_in_order(list, data);

    dataprev->key = 3;
    data = calloc(1, sizeof(*data));
    data->key = 4;
    //list_ins_next(list, NULL, data);
    list_ins_in_order(list, data);

    dataprev->key = 4;
    data = calloc(1, sizeof(*data));
    data->key = 3;
    //list_ins_next(list, NULL, data);
    list_ins_in_order(list, data);

    dataprev->key = 4;
    data = calloc(1, sizeof(*data));
    data->key = 1;
    //list_ins_next(list, NULL, data);
    list_ins_in_order(list, data);

    data = calloc(1, sizeof(*data));
    data->key = 20;
    //list_ins_next(list, NULL, data);
    list_ins_in_order(list, data);

    list_print_elements(list);

    list_rem_next(list, NULL, (void **) &remove);
    if (remove != NULL) {
        printf("Removed: %d\n", remove->key);
        free(remove);
        remove = NULL;
    }

    list_print_elements(list);

    dataprev->key = 4;
    list_rem_next(list, dataprev, (void **) &remove);
    if (remove != NULL) {
        printf("Removed: %d\n", remove->key);
        free(remove);
        remove = NULL;
    }

    list_print_elements(list);

    dataprev->key = 3;
    list_rem_next(list, dataprev, (void **) &remove);
    if (remove != NULL) {
        printf("Removed: %d\n", remove->key);
        free(remove);
        remove = NULL;
    }

    dataprev->key = 7;
    data = list_lookup(list, dataprev);
    if (data != NULL) {
        printf("%d is on the list [%d]\n", data->key, dataprev->key);
    } else {
        printf("%d isn't on the list\n", dataprev->key);
    }

    data = NULL;
    while ((data = list_lookup_next(list, data)) != NULL) {
        printf(" %d ", data->key);
    }
    printf("\n");

    list_print_elements(list);

    free(dataprev);
    list_destroy(&list, destroy);

    return EXIT_SUCCESS;
}

int32_t compare(void * key1, void * key2) {
    data_t * a = (data_t *)key1;
    data_t * b = (data_t *)key2;

    return a->key - b->key;
}

void destroy(void **data) {
    data_t *tmp = (data_t *) *data;
    printf("Removed: %d [destroy]\n", tmp->key);
    memset(tmp, 0x00, sizeof(*tmp));
    free(tmp);
    *data = NULL;
}

void print(void * data) {
    data_t * tmp = (data_t *) data;
    printf("%d ", tmp->key);
}

