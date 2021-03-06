#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "list.h"

#define KEY_TEST_A 10

static int setup(void **state) {
    list_t * list = list_init(compare, print);
    *state = list;
    return 0;
}

static int teardown(void **state) {
    list_t *list = *state;
    list_destroy(&list, destroy);
    assert_null(list);
    return 0;
}

static void list_test_create_a_list(void **state) {
    list_t *list = *state;
    assert_non_null(list);
}

static void list_test_insert_a_node(void **state) {
    list_t *list = *state;
    int8_t ret;
    data_t * node = malloc(sizeof(*node));
    assert_non_null(node);
    node->key = KEY_TEST_A;
    ret = list_ins_in_order(list, node);
    assert_int_equal(ret, LST_SUCCESS);
}

static void list_test_lookup_for_a_node(void **state) {
    list_test_create_a_list(state);
    list_test_insert_a_node(state);
    list_t *list = *state;
    data_t * node_to_find = malloc(sizeof(*node_to_find));
    assert_non_null(node_to_find);
    node_to_find->key = KEY_TEST_A;
    data_t * node = list_lookup(list, node_to_find);
    assert_non_null(node);
    assert_int_equal(node->key, KEY_TEST_A);
    free(node_to_find);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(list_test_create_a_list, setup, teardown),
        cmocka_unit_test_setup_teardown(list_test_insert_a_node, setup, teardown),
        cmocka_unit_test_setup_teardown(list_test_lookup_for_a_node, setup, teardown),
    };
    return cmocka_run_group_tests_name("success_test", tests, NULL, NULL);
}

int32_t compare(void * key1, void * key2) {
    data_t * a = (data_t *)key1;
    data_t * b = (data_t *)key2;

    return a->key - b->key;
}

void destroy(void **data) {
    data_t *tmp = (data_t *) *data;
    memset(tmp, 0x00, sizeof(*tmp));
    free(tmp);
    *data = NULL;
}

void print(void * data) {
    data_t * tmp = (data_t *) data;
    printf("%d ", tmp->key);
}


