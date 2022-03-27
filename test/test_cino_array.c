#include "test_cino_array.h"

typedef struct test_t {
    int a;
    char p[16];
} test_t;

void test_array_create() {
    array_t *arr = array_create("int");
    assert(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    array_destroy(arr);

    arr = array_create("double");
    assert(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    array_destroy(arr);

    arr = array_create("T");
    assert(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    array_destroy(arr);
}

void test_array_destroy() {
    array_t *arr = array_create("int");
    assert(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    array_destroy(arr);

    arr = array_create("double");
    assert(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    array_destroy(arr);

    arr = array_create("T");
    assert(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    array_destroy(arr);
}

void test_array_is_empty() {
    array_t *arr = array_create("int");
    assert(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    array_destroy(arr);

    arr = array_create("double");
    assert(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    array_destroy(arr);

    arr = array_create("T");
    assert(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    array_destroy(arr);
}

void test_array_size() {
    array_t *arr = array_create("int");
    assert(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    array_destroy(arr);

    arr = array_create("double");
    assert(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    array_destroy(arr);

    arr = array_create("T");
    assert(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    array_destroy(arr);
}

void test_array_clear() {
    array_t *arr = array_create("int");
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    for (int i = 0; i < 5; i++) {
        wrapper_int_t *wrapper = wrap_int(i);
        array_append(arr, wrapper);
        unwrap_int(wrapper);
    }
    assert(!array_is_empty(arr));
    assert(array_size(arr) == 5);
    array_clear(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    array_destroy(arr);

    arr = array_create("double");
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    for (int i = 0; i < 5; i++) {
        wrapper_double_t *wrapper = wrap_double((double)i);
        array_append(arr, wrapper);
        unwrap_double(wrapper);
    }
    assert(!array_is_empty(arr));
    assert(array_size(arr) == 5);
    array_clear(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    array_destroy(arr);

    arr = array_create("T");
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    test_t *test = (test_t *)cino_alloc(sizeof(test_t) * 5);
    for (int i = 0; i < 5; i++) {
        test[i].a = i;
        char str[8] = {0};
        int_to_str(i, str, sizeof(str));
        str_copy(test[i].p, str);
        array_append(arr, &test[i]);
    }
    assert(!array_is_empty(arr));
    assert(array_size(arr) == 5);
    array_clear(arr);
    assert(array_is_empty(arr));
    assert(array_size(arr) == 0);
    free(test);
    test = NULL;
    array_destroy(arr);
}

void test_array_get() {
    array_t *arr = array_create("int");
    for (int i = 0; i < 5; i++) {
        wrapper_int_t *wrapper = wrap_int(i);
        array_append(arr, wrapper);
        unwrap_int(wrapper);
    }
    for (int i = 0; i < 5; i++) {
        wrapper_int_t *wrapper = (wrapper_int_t *)array_get(arr, i);
        assert(unwrap_int(wrapper) == i);
    }
    array_destroy(arr);

    arr = array_create("double");
    for (int i = 0; i < 5; i++) {
        wrapper_double_t *wrapper = wrap_double(i);
        array_append(arr, wrapper);
        unwrap_double(wrapper);
    }
    for (int i = 0; i < 5; i++) {
        wrapper_double_t *wrapper = (wrapper_double_t *)array_get(arr, i);
        assert(unwrap_double(wrapper) == i);
    }
    array_destroy(arr);

    arr = array_create("T");
    test_t *test = (test_t *)cino_alloc(sizeof(test_t) * 5);
    for (int i = 0; i < 5; i++) {
        test[i].a = i;
        char str[8] = {0};
        int_to_str(i, str, sizeof(str));
        str_copy(test[i].p, str);
        array_append(arr, &test[i]);
    }
    for (int i = 0; i < 5; i++) {
        test_t *t = (test_t *)array_get(arr, i);
        assert(t->a == i);
        char p[8] = {0};
        int_to_str(i, p, sizeof(p));
        assert(str_equal(t->p, p));
    }
    free(test);
    test = NULL;
    array_destroy(arr);
}

void test_array_set() {
    array_t *arr = array_create("int");
    for (int i = 0; i < 3; i++) {
        wrapper_int_t *wrapper = wrap_int(i);
        array_append(arr, wrapper);
        unwrap_int(wrapper);
    }

    wrapper_int_t *wrapper_int = wrap_int(9);
    array_set(arr, 0, wrapper_int);
    unwrap_int(wrapper_int);
    wrapper_int = wrap_int(7);
    array_set(arr, 2, wrapper_int);
    unwrap_int(wrapper_int);

    wrapper_int = (wrapper_int_t *)array_get(arr, 0);
    assert(unwrap_int(wrapper_int) == 9);
    wrapper_int = (wrapper_int_t *)array_get(arr, 1);
    assert(unwrap_int(wrapper_int) == 1);
    wrapper_int = (wrapper_int_t *)array_get(arr, 2);
    assert(unwrap_int(wrapper_int) == 7);
    array_destroy(arr);

    arr = array_create("double");
    for (int i = 0; i < 3; i++) {
        wrapper_double_t *wrapper = wrap_double((double)i);
        array_append(arr, wrapper);
        unwrap_double(wrapper);
    }

    wrapper_double_t *wrapper_double = wrap_double(9.9);
    array_set(arr, 0, wrapper_double);
    unwrap_double(wrapper_double);
    wrapper_double = wrap_double(7.7);
    array_set(arr, 2, wrapper_double);
    unwrap_double(wrapper_double);

    wrapper_double = (wrapper_double_t *)array_get(arr, 0);
    assert(equal_double(unwrap_double(wrapper_double), 9.9));
    wrapper_double = (wrapper_double_t *)array_get(arr, 1);
    assert(equal_double(unwrap_double(wrapper_double), 1.0));
    wrapper_double = (wrapper_double_t *)array_get(arr, 2);
    assert(equal_double(unwrap_double(wrapper_double), 7.7));
    array_destroy(arr);

    arr = array_create("T");
    test_t *test = (test_t *)cino_alloc(sizeof(test_t) * 3);
    for (int i = 0; i < 3; i++) {
        test[i].a = i;
        char str[8] = {0};
        int_to_str(i, str, sizeof(str));
        str_copy(test[i].p, str);
        array_append(arr, &test[i]);
    }

    test[0].a = 9;
    str_copy(test[0].p, "9");
    array_set(arr, 0, &test[0]);
    test[2].a = 7;
    str_copy(test[2].p, "7");
    array_set(arr, 2, &test[2]);

    test_t *t = (test_t *)array_get(arr, 0);
    assert(t->a == 9);
    assert(str_equal(t->p, "9"));
    t = (test_t *)array_get(arr, 1);
    assert(t->a == 1);
    assert(str_equal(t->p, "1"));
    t = (test_t *)array_get(arr, 2);
    assert(t->a == 7);
    assert(str_equal(t->p, "7"));

    free(test);
    test = NULL;
    array_destroy(arr);
}

void test_array_append() {
    array_t *arr = array_create("int");
    for (int i = 0; i < 5; i++) {
        wrapper_int_t *wrapper = wrap_int(i);
        array_append(arr, wrapper);
        unwrap_int(wrapper);
    }
    for (int i = 0; i < 5; i++) {
        wrapper_int_t *wrapper = (wrapper_int_t *)array_get(arr, i);
        assert(unwrap_int(wrapper) == i);
    }
    array_destroy(arr);

    arr = array_create("double");
    for (int i = 0; i < 5; i++) {
        wrapper_double_t *wrapper = wrap_double((double)i);
        array_append(arr, wrapper);
        unwrap_double(wrapper);
    }
    for (int i = 0; i < 5; i++) {
        wrapper_double_t *wrapper = (wrapper_double_t *)array_get(arr, i);
        assert(equal_double(unwrap_double(wrapper), (double)i));
    }
    array_destroy(arr);

    arr = array_create("T");
    test_t *test = (test_t *)cino_alloc(sizeof(test_t) * 5);
    for (int i = 0; i < 5; i++) {
        test[i].a = i;
        char str[8] = {0};
        int_to_str(i, str, sizeof(str));
        str_copy(test[i].p, str);
        array_append(arr, &test[i]);
    }

    for (int i = 0; i < 5; i++) {
        test_t *t = (test_t *)array_get(arr, i);
        assert(t->a == i);
        char p[8] = {0};
        int_to_str(i, p, sizeof(p));
        assert(str_equal(t->p, p));
    }

    free(test);
    test = NULL;
    array_destroy(arr);
}

void test_array_insert() {
    int arr1[] = {5, 1, 9, 7, 4};
    int len1 = array_len(arr1);
    array_t *arr = array_create("int");

    wrapper_int_t *wrapper_int = wrap_int(1);
    array_insert(arr, 0, wrapper_int);
    unwrap_int(wrapper_int);
    wrapper_int = wrap_int(5);
    array_insert(arr, 0, wrapper_int);
    unwrap_int(wrapper_int);
    wrapper_int = wrap_int(7);
    array_insert(arr, 2, wrapper_int);
    unwrap_int(wrapper_int);
    wrapper_int = wrap_int(4);
    array_insert(arr, 3, wrapper_int);
    unwrap_int(wrapper_int);
    wrapper_int = wrap_int(9);
    array_insert(arr, 2, wrapper_int);
    unwrap_int(wrapper_int);

    for (int i = 0; i < len1; i++) {
        wrapper_int_t *wrapper = (wrapper_int_t *)array_get(arr, i);
        assert(unwrap_int(wrapper) == arr1[i]);
    }
    array_destroy(arr);

    double arr2[] = {5.5, 1.1, 9.9, 7.7, 4.4};
    int len2 = array_len(arr2);
    arr = array_create("double");

    wrapper_double_t *wrapper_double = wrap_double(1.1);
    array_insert(arr, 0, wrapper_double);
    unwrap_double(wrapper_double);
    wrapper_double = wrap_double(5.5);
    array_insert(arr, 0, wrapper_double);
    unwrap_double(wrapper_double);
    wrapper_double = wrap_double(7.7);
    array_insert(arr, 2, wrapper_double);
    unwrap_double(wrapper_double);
    wrapper_double = wrap_double(4.4);
    array_insert(arr, 3, wrapper_double);
    unwrap_double(wrapper_double);
    wrapper_double = wrap_double(9.9);
    array_insert(arr, 2, wrapper_double);
    unwrap_double(wrapper_double);

    for (int i = 0; i < len2; i++) {
        wrapper_double_t *wrapper = (wrapper_double_t *)array_get(arr, i);
        assert(equal_double(unwrap_double(wrapper), arr2[i]));
    }
    array_destroy(arr);

    arr = array_create("T");
    test_t *test = (test_t *)cino_alloc(sizeof(test_t) * 5);
    int arr3[] = {1, 4, 3, 2, 0};
    int len3 = array_len(arr3);

    for (int i = 0; i < len3; i++) {
        test[i].a = i;
        char str[8] = {0};
        int_to_str(i, str, sizeof(str));
        str_copy(test[i].p, str);
    }

    array_insert(arr, 0, &test[3]);
    array_insert(arr, 0, &test[1]);
    array_insert(arr, 2, &test[0]);
    array_insert(arr, 1, &test[4]);
    array_insert(arr, 3, &test[2]);

    for (int i = 0; i < len3; i++) {
        test_t *t = (test_t *)array_get(arr, i);
        assert(t->a == arr3[i]);
        char p[8] = {0};
        int_to_str(arr3[i], p, sizeof(p));
        assert(str_equal(t->p, p));
    }

    free(test);
    test = NULL;
    array_destroy(arr);
}

void test_array_remove() {
    array_t *arr = array_create("int");
    for (int i = 0; i < 10; i++) {
        wrapper_int_t *wrapper = wrap_int(i);
        array_append(arr, wrapper);
        unwrap_int(wrapper);
    }
    array_remove(arr, 2);
    array_remove(arr, 1);
    array_remove(arr, 0);
    array_remove(arr, 5);
    array_remove(arr, 5);
    for (int i = 0; i < 5; i++) {
        wrapper_int_t *wrapper = (wrapper_int_t *)array_get(arr, i);
        assert(unwrap_int(wrapper) == i + 3);
    }
    array_destroy(arr);

    arr = array_create("double");
    for (int i = 0; i < 10; i++) {
        wrapper_double_t *wrapper = wrap_double((double)i);
        array_append(arr, wrapper);
        unwrap_double(wrapper);
    }
    array_remove(arr, 2);
    array_remove(arr, 1);
    array_remove(arr, 0);
    array_remove(arr, 5);
    array_remove(arr, 5);
    for (int i = 0; i < 5; i++) {
        wrapper_double_t *wrapper = (wrapper_double_t *)array_get(arr, i);
        assert(equal_double(unwrap_double(wrapper), (double)(i + 3)));
    }
    array_destroy(arr);

    arr = array_create("T");
    test_t *test = (test_t *)cino_alloc(sizeof(test_t) * 10);
    for (int i = 0; i < 10; i++) {
        test[i].a = i;
        char str[8] = {0};
        int_to_str(i, str, sizeof(str));
        str_copy(test[i].p, str);
        array_append(arr, &test[i]);
    }
    array_remove(arr, 2);
    array_remove(arr, 1);
    array_remove(arr, 0);
    array_remove(arr, 5);
    array_remove(arr, 5);
    for (int i = 0; i < 5; i++) {
        test_t *t = (test_t *)array_get(arr, i);
        assert(t->a == i + 3);
        char p[8] = {0};
        int_to_str(i + 3, p, sizeof(p));
        assert(str_equal(t->p, p));
    }
    free(test);
    test = NULL;
    array_destroy(arr);
}

static int cmp_by_int(const void *data1, const void *data2) {
    test_t *t1 = (test_t *)data1;
    test_t *t2 = (test_t *)data2;
    return t1->a - t2->a;
}

static int cmp_by_str(const void *data1, const void *data2) {
    test_t *t1 = (test_t *)data1;
    test_t *t2 = (test_t *)data2;
    return strncmp(t1->p, t2->p, strlen(t1->p));
}

void test_array_min() {
    int arr1[] = {5, 7, 1, 9, 4, 2, 5, 6};
    int len1 = array_len(arr1);
    array_t *arr = array_create("int");
    for (int i = 0; i < len1; i++) {
        wrapper_int_t *wrapper = wrap_int(arr1[i]);
        array_append(arr, wrapper);
        unwrap_int(wrapper);
    }
    wrapper_int_t *wrapper_int = (wrapper_int_t *)array_min(arr, NULL);
    assert(unwrap_int(wrapper_int) == 1);
    array_destroy(arr);

    double arr2[] = {5.5, 7.7, 1.1, 9.9, 4.4, 2.2, 5.5, 6.6};
    int len2 = array_len(arr2);
    arr = array_create("double");
    for (int i = 0; i < len2; i++) {
        wrapper_double_t *wrapper = wrap_double((double)arr2[i]);
        array_append(arr, wrapper);
        unwrap_double(wrapper);
    }
    wrapper_double_t *wrapper_double = (wrapper_double_t *)array_min(arr, NULL);
    assert(equal_double(unwrap_double(wrapper_double), 1.1));
    array_destroy(arr);

    arr = array_create("T");
    test_t *test = (test_t *)cino_alloc(sizeof(test_t) * len1);
    for (int i = 0; i < len1; i++) {
        test[i].a = arr1[i];
        char str[8] = {0};
        int_to_str(arr1[i], str, sizeof(str));
        str_copy(test[i].p, str);
        array_append(arr, &test[i]);
    }
    test_t *t = (test_t *)array_min(arr, cmp_by_int);
    assert(t->a == 1);
    t = (test_t *)array_min(arr, cmp_by_str);
    char p[8] = {0};
    int_to_str(1, p, sizeof(p));
    assert(str_equal(t->p, p));
    free(test);
    test = NULL;
    array_destroy(arr);
}

void test_array_max() {
    int arr1[] = {5, 7, 1, 9, 4, 2, 5, 6};
    int len1 = array_len(arr1);
    array_t *arr = array_create("int");
    for (int i = 0; i < len1; i++) {
        wrapper_int_t *wrapper = wrap_int(arr1[i]);
        array_append(arr, wrapper);
        unwrap_int(wrapper);
    }
    wrapper_int_t *wrapper_int = (wrapper_int_t *)array_max(arr, NULL);
    assert(unwrap_int(wrapper_int) == 9);
    array_destroy(arr);

    double arr2[] = {5.5, 7.7, 1.1, 9.9, 4.4, 2.2, 5.5, 6.6};
    int len2 = array_len(arr2);
    arr = array_create("double");
    for (int i = 0; i < len2; i++) {
        wrapper_double_t *wrapper = wrap_double((double)arr2[i]);
        array_append(arr, wrapper);
        unwrap_double(wrapper);
    }
    wrapper_double_t *wrapper_double = (wrapper_double_t *)array_max(arr, NULL);
    assert(equal_double(unwrap_double(wrapper_double), 9.9));
    array_destroy(arr);

    arr = array_create("T");
    test_t *test = (test_t *)cino_alloc(sizeof(test_t) * len1);
    for (int i = 0; i < len1; i++) {
        test[i].a = arr1[i];
        char str[8] = {0};
        int_to_str(arr1[i], str, sizeof(str));
        str_copy(test[i].p, str);
        array_append(arr, &test[i]);
    }
    test_t *t = (test_t *)array_max(arr, cmp_by_int);
    assert(t->a == 9);
    t = (test_t *)array_max(arr, cmp_by_str);
    char p[8] = {0};
    int_to_str(9, p, sizeof(p));
    assert(str_equal(t->p, p));
    free(test);
    test = NULL;
    array_destroy(arr);
}

static bool match_by_int(const void *data) {
    test_t *t = (test_t *)data;
    return t->a == 4;
}

void test_array_index_of() {
    int arr1[] = {5, 4, 2, 2, 5, 4, 1};
    int len1 = array_len(arr1);

    array_t *arr = array_create("int");
    for (int i = 0; i < len1; i++) {
        wrapper_int_t *wrapper = wrap_int(arr1[i]);
        array_append(arr, wrapper);
        unwrap_int(wrapper);
    }
    wrapper_int_t *wrapper_int = wrap_int(4);
    assert(array_index_of(arr, wrapper_int) == 1);
    unwrap_int(wrapper_int);
    wrapper_int = wrap_int(10);
    assert(array_index_of(arr, wrapper_int) == -1);
    unwrap_int(wrapper_int);
    array_destroy(arr);

    arr = array_create("double");
    for (int i = 0; i < len1; i++) {
        wrapper_double_t *wrapper = wrap_double((double)arr1[i]);
        array_append(arr, wrapper);
        unwrap_double(wrapper);
    }
    wrapper_double_t *wrapper_double = wrap_double(4.0);
    assert(array_index_of(arr, wrapper_double) == 1);
    unwrap_double(wrapper_double);
    wrapper_double = wrap_double(10.0);
    assert(array_index_of(arr, wrapper_double) == -1);
    unwrap_double(wrapper_double);
    array_destroy(arr);

    arr = array_create("T");
    test_t *test = (test_t *)cino_alloc(sizeof(test_t) * len1);
    for (int i = 0; i < len1; i++) {
        test[i].a = arr1[i];
        char str[8] = {0};
        int_to_str(arr1[i], str, sizeof(str));
        str_copy(test[i].p, str);
        array_append(arr, &test[i]);
    }
    assert(array_index_of(arr, match_by_int) == 1);
    free(test);
    test = NULL;
    array_destroy(arr);
}

void test_array_last_index_of() {
    int arr1[] = {5, 4, 2, 2, 5, 4, 1};
    int len1 = array_len(arr1);

    array_t *arr = array_create("int");
    for (int i = 0; i < len1; i++) {
        wrapper_int_t *wrapper = wrap_int(arr1[i]);
        array_append(arr, wrapper);
        unwrap_int(wrapper);
    }
    wrapper_int_t *wrapper_int = wrap_int(4);
    assert(array_last_index_of(arr, wrapper_int) == 5);
    unwrap_int(wrapper_int);
    wrapper_int = wrap_int(10);
    assert(array_last_index_of(arr, wrapper_int) == -1);
    unwrap_int(wrapper_int);
    array_destroy(arr);

    arr = array_create("double");
    for (int i = 0; i < len1; i++) {
        wrapper_double_t *wrapper = wrap_double((double)arr1[i]);
        array_append(arr, wrapper);
        unwrap_double(wrapper);
    }
    wrapper_double_t *wrapper_double = wrap_double(4.0);
    assert(array_last_index_of(arr, wrapper_double) == 5);
    unwrap_double(wrapper_double);
    wrapper_double = wrap_double(10.0);
    assert(array_last_index_of(arr, wrapper_double) == -1);
    unwrap_double(wrapper_double);
    array_destroy(arr);

    arr = array_create("T");
    test_t *test = (test_t *)cino_alloc(sizeof(test_t) * len1);
    for (int i = 0; i < len1; i++) {
        test[i].a = arr1[i];
        char str[8] = {0};
        int_to_str(arr1[i], str, sizeof(str));
        str_copy(test[i].p, str);
        array_append(arr, &test[i]);
    }
    assert(array_last_index_of(arr, match_by_int) == 5);
    free(test);
    test = NULL;
    array_destroy(arr);
}

void test_array_count() {
}

void test_array_reverse() {
}

void test_array_swap() {
}

// void test_array_int_sort() {
//     array_t *arr = array_create();
//     for (int i = 0; i < 100; i++) {
//         array_int_append(arr, i);
//     }
//     array_int_sort(arr, false);
//     for (int i = 0; i < 100; i++) {
//         assert(array_int_get(arr, i) == i);
//     }
//     array_destroy(arr);

//     arr = array_create();
//     for (int i = 99; i >= 0; i--) {
//         array_int_append(arr, i);
//     }
//     array_int_sort(arr, false);
//     for (int i = 0; i < 100; i++) {
//         assert(array_int_get(arr, i) == i);
//     }
//     array_destroy(arr);

//     arr = array_create();
//     for (int i = 99; i >= 0; i--) {
//         array_int_append(arr, i);
//     }
//     array_int_sort(arr, true);
//     for (int i = 0; i < 100; i++) {
//         assert(array_int_get(arr, i) == 100 - i - 1);
//     }
//     array_destroy(arr);

//     arr = array_create();
//     int result1[] = {-7, -5, 1, 2, 5, 5, 5, 6, 78, 145};
//     int result1_len = array_len(result1);
//     array_int_append(arr, 5);
//     array_int_append(arr, 5);
//     array_int_append(arr, -5);
//     array_int_append(arr, 1);
//     array_int_append(arr, 2);
//     array_int_append(arr, 5);
//     array_int_append(arr, -7);
//     array_int_append(arr, 145);
//     array_int_append(arr, 78);
//     array_int_append(arr, 6);
//     array_int_sort(arr, false);
//     for (int i = 0; i < result1_len; i++) {
//         assert(array_int_get(arr, i) == result1[i]);
//     }
//     array_destroy(arr);

//     arr = array_create();
//     int result2[] = {145, 78, 6, 5, 5, 5, 2, 1, -5, -7};
//     int result2_len = array_len(result1);
//     array_int_append(arr, 5);
//     array_int_append(arr, 5);
//     array_int_append(arr, -5);
//     array_int_append(arr, 1);
//     array_int_append(arr, 2);
//     array_int_append(arr, 5);
//     array_int_append(arr, -7);
//     array_int_append(arr, 145);
//     array_int_append(arr, 78);
//     array_int_append(arr, 6);
//     array_int_sort(arr, true);
//     for (int i = 0; i < result2_len; i++) {
//         assert(array_int_get(arr, i) == result2[i]);
//     }
//     array_destroy(arr);
// }

// void test_array_int_iter() {
//     array_t *arr = array_create();
//     for (int i = 0; i < 10; i++) {
//         array_int_append(arr, i);
//     }
//     assert(array_int_iter(arr));
//     array_destroy(arr);
// }

// void test_array_int_iter_has_next() {
//     array_t *arr = array_create();
//     for (int i = 0; i < 10; i++) {
//         array_int_append(arr, i);
//     }
//     void *iter = array_int_iter(arr);
//     assert(iter);
//     assert(array_int_iter_has_next(arr));
//     for (int i = 0; i < 10; i++) {
//         array_int_remove(arr, 0);
//     }
//     iter = array_int_iter(arr);
//     assert(!array_int_iter_has_next(arr));
//     array_destroy(arr);
// }

// void test_array_int_iter_next() {
//     array_t *arr = array_create();
//     for (int i = 0; i < 10; i++) {
//         array_int_append(arr, i);
//     }
//     int i = 0;
//     void *iter = array_int_iter(arr);
//     while (iter) {
//         assert(*(int *)iter == i);
//         iter = array_int_iter_next(arr);
//         i++;
//     }
//     array_destroy(arr);
// }

// void test_array_double_swap() {
//     array_t *arr = array_double_create();
//     for (int i = 0; i < 100; i++) {
//         array_double_append(arr, i);
//     }
//     int i = 0;
//     int j = 99;
//     while (i < j) {
//         array_double_swap(arr, i, j);
//         i++;
//         j--;
//     }
//     for (int i = 0; i < 100; i++) {
//         assert(equal_double(array_double_get(arr, i), 100 - i - 1));
//     }
//     array_double_destroy(arr);
// }

// void test_array_double_sort() {
//     array_t *arr = array_double_create();
//     for (int i = 0; i < 100; i++) {
//         array_double_append(arr, i);
//     }
//     array_double_sort(arr, false);
//     for (int i = 0; i < 100; i++) {
//         assert(equal_double(array_double_get(arr, i), i));
//     }
//     array_double_destroy(arr);

//     arr = array_double_create();
//     for (int i = 99; i >= 0; i--) {
//         array_double_append(arr, i);
//     }
//     array_double_sort(arr, false);
//     for (int i = 0; i < 100; i++) {
//         assert(equal_double(array_double_get(arr, i), i));
//     }
//     array_double_destroy(arr);

//     arr = array_double_create();
//     for (int i = 99; i >= 0; i--) {
//         array_double_append(arr, i);
//     }
//     array_double_sort(arr, true);
//     for (int i = 0; i < 100; i++) {
//         assert(equal_double(array_double_get(arr, i), 100 - i - 1));
//     }
//     array_double_destroy(arr);

//     arr = array_double_create();
//     double result1[] = {-7, -5, 1, 2, 5, 5, 5, 6, 78, 145};
//     int result1_len = array_len(result1);
//     array_double_append(arr, 5);
//     array_double_append(arr, 5);
//     array_double_append(arr, -5);
//     array_double_append(arr, 1);
//     array_double_append(arr, 2);
//     array_double_append(arr, 5);
//     array_double_append(arr, -7);
//     array_double_append(arr, 145);
//     array_double_append(arr, 78);
//     array_double_append(arr, 6);
//     array_double_sort(arr, false);
//     for (int i = 0; i < result1_len; i++) {
//         assert(equal_double(array_double_get(arr, i), result1[i]));
//     }
//     array_double_destroy(arr);

//     arr = array_double_create();
//     double result2[] = {145, 78, 6, 5, 5, 5, 2, 1, -5, -7};
//     int result2_len = array_len(result1);
//     array_double_append(arr, 5);
//     array_double_append(arr, 5);
//     array_double_append(arr, -5);
//     array_double_append(arr, 1);
//     array_double_append(arr, 2);
//     array_double_append(arr, 5);
//     array_double_append(arr, -7);
//     array_double_append(arr, 145);
//     array_double_append(arr, 78);
//     array_double_append(arr, 6);
//     array_double_sort(arr, true);
//     for (int i = 0; i < result2_len; i++) {
//         assert(equal_double(array_double_get(arr, i), result2[i]));
//     }
//     array_double_destroy(arr);
// }

// void test_array_double_iter() {
//     array_t *arr = array_double_create();
//     for (int i = 0; i < 10; i++) {
//         array_double_append(arr, i);
//     }
//     assert(array_double_iter(arr));
//     array_double_destroy(arr);
// }

// void test_array_double_iter_has_next() {
//     array_t *arr = array_double_create();
//     for (int i = 0; i < 10; i++) {
//         array_double_append(arr, i);
//     }
//     void *iter = array_double_iter(arr);
//     assert(iter);
//     assert(array_double_iter_has_next(arr));
//     for (int i = 0; i < 10; i++) {
//         array_double_remove(arr, 0);
//     }
//     iter = array_double_iter(arr);
//     assert(!array_double_iter_has_next(arr));
//     array_double_destroy(arr);
// }

// void test_array_double_iter_next() {
//     array_t *arr = array_double_create();
//     for (int i = 0; i < 10; i++) {
//         array_double_append(arr, i);
//     }
//     int i = 0;
//     void *iter = array_double_iter(arr);
//     while (iter) {
//         assert(equal_double(*(double *)iter, i));
//         iter = array_double_iter_next(arr);
//         i++;
//     }
//     array_double_destroy(arr);
// }

// void test_array_swap() {
//     int *nums = (int *)malloc(sizeof(int) * 10);

//     array_t *arr = array_create();
//     for (int i = 0; i < 10; i++) {
//         nums[i] = i;
//         array_append(arr, &nums[i]);
//     }
//     int i = 0;
//     int j = 9;
//     while (i < j) {
//         array_swap(arr, i, j);
//         i++;
//         j--;
//     }
//     for (int i = 0; i < 10; i++) {
//         assert(*(int *)array_get(arr, i) == 10 - i - 1);
//     }

//     array_destroy(arr);
//     free(nums);
// }

// void test_array_sort() {
//     student_t *stu = (student_t *)malloc(sizeof(student_t) * 10);
//     int id[] = {4, 6, 9, 0, 1, 7, 5, 2, 3, 8};

//     array_t *arr = array_create();
//     for (int i = 0; i < 10; i++) {
//         stu[i].id = id[i];
//         snprintf(stu[i].name, sizeof(stu[i].name), "Name-%d", id[i]);
//         array_append(arr, &stu[i]);
//     }

//     array_sort(arr, cmp_by_id);
//     for (int i = 0; i < 10; i++) {
//         student_t *s = (student_t *)array_get(arr, i);
//         char name[32] = {0};
//         snprintf(name, sizeof(name), "Name-%d", i);
//         assert(s->id == i);
//         assert(strncmp(s->name, name, strlen(name)) == 0);
//     }

//     array_sort(arr, cmp_by_name);
//     for (int i = 0; i < 10; i++) {
//         student_t *s = (student_t *)array_get(arr, i);
//         char name[32] = {0};
//         snprintf(name, sizeof(name), "Name-%d", i);
//         assert(s->id == i);
//         assert(strncmp(s->name, name, strlen(name)) == 0);
//     }

//     array_destroy(arr);
//     free(stu);
// }

// void test_array_iter() {
//     student_t *stu = (student_t *)malloc(sizeof(student_t) * 10);

//     array_t *arr = array_create();
//     for (int i = 0; i < 10; i++) {
//         stu[i].id = i;
//         snprintf(stu[i].name, sizeof(stu[i].name), "Name-%d", i);
//         array_append(arr, &stu[i]);
//     }
//     assert(array_iter(arr));

//     array_destroy(arr);
//     free(stu);
// }

// void test_array_iter_has_next() {
//     student_t *stu = (student_t *)malloc(sizeof(student_t) * 10);

//     array_t *arr = array_create();
//     for (int i = 0; i < 10; i++) {
//         stu[i].id = i;
//         snprintf(stu[i].name, sizeof(stu[i].name), "Name-%d", i);
//         array_append(arr, &stu[i]);
//     }
//     void *iter = array_iter(arr);
//     assert(iter);
//     assert(array_iter_has_next(arr));
//     for (int i = 0; i < 10; i++) {
//         array_remove(arr, 0);
//     }
//     iter = array_iter(arr);
//     assert(!array_iter_has_next(arr));

//     array_destroy(arr);
//     free(stu);
// }

// void test_array_iter_next() {
//     student_t *stu = (student_t *)malloc(sizeof(student_t) * 10);

//     array_t *arr = array_create();
//     for (int i = 0; i < 10; i++) {
//         stu[i].id = i;
//         snprintf(stu[i].name, sizeof(stu[i].name), "Name-%d", i);
//         array_append(arr, &stu[i]);
//     }

//     int i = 0;
//     void *iter = array_iter(arr);
//     while (iter) {
//         student_t *s = (student_t *)iter;
//         char name[32] = {0};
//         snprintf(name, sizeof(name), "Name-%d", i);
//         assert(s->id == i);
//         assert(strncmp(s->name, name, strlen(name)) == 0);
//         iter = array_iter_next(arr);
//         i++;
//     }

//     array_destroy(arr);
//     free(stu);
// }