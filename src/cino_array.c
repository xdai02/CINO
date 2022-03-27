#include "cino_array.h"

/****************************************
 *               Iterator
 ****************************************/

typedef struct iterator_t {
    void *iter;      // points to the current element
    int iter_index;  // generic array use only
} iterator_t;

/****************************************
 *               array_t
 ****************************************/

typedef struct array_t {
    void *arr;  // primitive array
    T *t_arr;   // cino-array
    str_t data_type;
    size_t size;
    size_t capacity;
    iterator_t *iterator;
} array_t;

/**
 * @brief   Validate data type
 * @param data_type data type
 *                  valid data type includes:
 *                      1. int
 *                      2. double
 *                      3. T (generic)
 * @return  Returns `true` if the data type is supported, otherwise returns `false`.
 */
static bool is_valid_data_type(const str_t data_type) {
    return_value_if_fail(data_type != NULL, false);

    const str_t data_types[] = {
        "int",
        "double",
        "T",  // generic
    };

    int data_types_len = array_len(data_types);
    for (int i = 0; i < data_types_len; i++) {
        if (str_equal(data_types[i], data_type)) {
            return true;
        }
    }
    return false;
}

/**
 * @brief   Create cino-array.
 * @param data_type data type of each element
 *                  valid data type includes:
 *                      1. int
 *                      2. double
 *                      3. T (for generic)
 * @return  Returns the pointer to cino-array. Returns NULL if creation failed.
 */
array_t *array_create(const str_t data_type) {
    return_value_if_fail(is_valid_data_type(data_type), NULL);

    array_t *array = (array_t *)cino_alloc(sizeof(array_t));
    return_value_if_fail(array != NULL, NULL);

    array->iterator = (iterator_t *)cino_alloc(sizeof(iterator_t));
    call_and_return_value_if_fail(array->iterator != NULL, array_destroy(array), NULL);

    array->data_type = (str_t)cino_alloc(sizeof(char) * (str_length(data_type) + 1));
    call_and_return_value_if_fail(array->data_type != NULL, array_destroy(array), NULL);
    str_copy(array->data_type, data_type);

    array->arr = NULL;
    array->t_arr = NULL;
    array->size = 0;
    array->capacity = 0;
    array->iterator->iter = NULL;
    array->iterator->iter_index = -1;

    return array;
}

/**
 * @brief   Destroy cino-array.
 * @note    It is caller's responsibility to free all the elements before calling
 *          this function, if it is a generic cino-array.
 * @param array cino-array
 */
void array_destroy(array_t *array) {
    return_if_fail(array != NULL);

    array->size = 0;
    array->capacity = 0;

    if (array->data_type) {
        free(array->data_type);
        array->data_type = NULL;
    }

    if (array->arr) {
        free(array->arr);
        array->arr = NULL;
    }

    if (array->t_arr) {
        free(array->t_arr);
        array->t_arr = NULL;
    }

    if (array->iterator) {
        array->iterator->iter = NULL;
        array->iterator->iter_index = -1;
        free(array->iterator);
        array->iterator = NULL;
    }

    if (array) {
        free(array);
        array = NULL;
    }
}

/**
 * @brief   Determine if the cino-array is empty.
 * @param array cino-array
 * @return  Returns true if the cino-array is empty, otherwise returns false.
 */
bool array_is_empty(const array_t *array) {
    return !array || array->size == 0;
}

/**
 * @brief   Get the number of elements in the cino-array.
 * @param array cino-array
 * @return  Returns the number of elements in the cino-array.
 */
size_t array_size(const array_t *array) {
    return_value_if_fail(array != NULL, 0);
    return array->size;
}

/**
 * @brief   Clear all the elments in the cino-array.
 * @note    It is caller's responsibility to free all the elements before calling
 *          this function, if it is a generic cino-array.
 * @param array cino-array
 * @return  Returns the modified cino-array.
 */
array_t *array_clear(array_t *array) {
    return_value_if_fail(array != NULL, NULL);

    if (array->arr) {
        free(array->arr);
        array->arr = NULL;
    }

    if (array->t_arr) {
        free(array->t_arr);
        array->t_arr = NULL;
    }

    array->size = 0;
    array->capacity = 0;
    array->iterator->iter = NULL;
    array->iterator->iter_index = -1;

    return array;
}

/**
 * @brief   Get the element of the indexed component in the cino-array.
 * @param array cino-array
 * @param index index
 * @return  For primitive type cino-array, this function returns a wrapper type of
 *          the primitive. It is caller's responsibility to unwrap to get the primitive.
 */
T array_get(const array_t *array, int index) {
    return_value_if_fail(array != NULL && index >= 0 && index < array->size, NULL);

    if (str_equal(array->data_type, "int")) {
        int *arr = (int *)array->arr;
        return (T)wrap_int(arr[index]);
    } else if (str_equal(array->data_type, "double")) {
        double *arr = (double *)array->arr;
        return (T)wrap_double(arr[index]);
    } else if (str_equal(array->data_type, "T")) {
        return array->t_arr[index];
    }

    return NULL;
}

/**
 * @brief   Update the element of the indexed component in the cino-array.
 * @note    1. For primitive type data, a wrapper type of that primitive is needed.
 *          This function will not unwrap or free the wrapper. It is caller's
 *          responsibility to unwrap.
 *          2. For generic type cino-array, it is caller's responsibility to free
 *          the previous data before overwriting it.
 * @param array cino-array
 * @param index index
 * @param data  new element in wrapper
 */
void array_set(array_t *array, int index, T data) {
    return_if_fail(array != NULL && index >= 0 && index < array->size && data != NULL);

    if (str_equal(array->data_type, "int")) {
        wrapper_int_t *wrapper_int = (wrapper_int_t *)data;
        int *arr = (int *)array->arr;
        arr[index] = wrapper_int->data;
    } else if (str_equal(array->data_type, "double")) {
        wrapper_double_t *wrapper_double = (wrapper_double_t *)data;
        double *arr = (double *)array->arr;
        arr[index] = wrapper_double->data;
    } else if (str_equal(array->data_type, "T")) {
        array->t_arr[index] = data;
    }
}

/**
 * @brief   Expand or shrink the cino-array according to the size and capacity.
 * @param array cino-array
 * @return  Returns the modified cino-array.
 */
static array_t *array_resize(array_t *array) {
    return_value_if_fail(array != NULL, NULL);

    void *p = NULL;

    // first allocation
    if (array->capacity == 0) {
        array->capacity = 1;

        if (str_equal(array->data_type, "int")) {
            array->arr = cino_alloc(sizeof(int) * array->capacity);
            p = array->arr;
        } else if (str_equal(array->data_type, "double")) {
            array->arr = cino_alloc(sizeof(double) * array->capacity);
            p = array->arr;
        } else if (str_equal(array->data_type, "T")) {
            array->t_arr = (T *)cino_alloc(sizeof(T) * array->capacity);
            p = array->t_arr;
        }
        call_and_return_value_if_fail(p != NULL, array_destroy(array), NULL);

        return array;
    }

    const int EXPANSION = 2;  // coefficient of expansion

    // expand
    if (array->size >= array->capacity) {
        if (str_equal(array->data_type, "int")) {
            array->arr = cino_realloc(array->arr, sizeof(int) * array->capacity, sizeof(int) * array->capacity * 2);
            p = array->arr;
        } else if (str_equal(array->data_type, "double")) {
            array->arr = cino_realloc(array->arr, sizeof(double) * array->capacity, sizeof(double) * array->capacity * 2);
            p = array->arr;
        } else if (str_equal(array->data_type, "T")) {
            array->t_arr = (T *)cino_realloc(array->t_arr, sizeof(T) * array->capacity, sizeof(T) * array->capacity * 2);
            p = array->t_arr;
        }
        call_and_return_value_if_fail(p != NULL, array_destroy(array), NULL);
        array->capacity *= EXPANSION;
    }
    // shrink
    else if (array->size <= array->capacity / EXPANSION) {
        size_t capacity = array->capacity / EXPANSION;
        if (capacity > 0) {
            if (str_equal(array->data_type, "int")) {
                array->arr = cino_realloc(array->arr, sizeof(int) * array->capacity, sizeof(int) * capacity);
            } else if (str_equal(array->data_type, "double")) {
                array->arr = cino_realloc(array->arr, sizeof(double) * array->capacity, sizeof(double) * capacity);
            } else if (str_equal(array->data_type, "T")) {
                array->t_arr = (T *)cino_realloc(array->t_arr, sizeof(T) * array->capacity, sizeof(T) * capacity);
            }
            array->capacity = capacity;
        }
    }

    return array;
}

/**
 * @brief   Appends the specified element to the end of the cino-array.
 * @note    1. For primitive type data, a wrapper type of that primitive is needed.
 *          This function will not unwrap or free the wrapper. It is caller's
 *          responsibility to unwrap.
 *          2. For generic type cino-array, it is caller's responsibility to make
 *          sure that the inserted element is on the heap.
 * @param array cino-array
 * @param data  new element in wrapper
 * @return  Returns the modified cino-array.
 */
array_t *array_append(array_t *array, T data) {
    return_value_if_fail(data != NULL, array);

    array_resize(array);
    return_value_if_fail(array != NULL, NULL);

    if (str_equal(array->data_type, "int")) {
        wrapper_int_t *wrapper_int = (wrapper_int_t *)data;
        int *arr = (int *)array->arr;
        arr[array->size++] = wrapper_int->data;
    } else if (str_equal(array->data_type, "double")) {
        wrapper_double_t *wrapper_double = (wrapper_double_t *)data;
        double *arr = (double *)array->arr;
        arr[array->size++] = wrapper_double->data;
    } else if (str_equal(array->data_type, "T")) {
        array->t_arr[array->size++] = data;
    }

    return array;
}

/**
 * @brief   Inserts the specified element at the specified position in the cino-array.
 * @note    1. For primitive type data, a wrapper type of that primitive is needed.
 *          This function will not unwrap or free the wrapper. It is caller's
 *          responsibility to unwrap.
 *          2. For generic type cino-array, it is caller's responsibility to make
 *          sure that the inserted element is on the heap.
 * @param array cino-array
 * @param index index
 * @param data  new element in wrapper
 * @return  Returns the modified cino-array.
 */
array_t *array_insert(array_t *array, int index, T data) {
    return_value_if_fail(index >= 0 && index <= array->size && data != NULL, array);

    array_resize(array);
    return_value_if_fail(array != NULL, NULL);

    for (int i = array->size - 1; i >= index; i--) {
        if (str_equal(array->data_type, "int")) {
            int *arr = (int *)array->arr;
            arr[i + 1] = arr[i];
        } else if (str_equal(array->data_type, "double")) {
            double *arr = (double *)array->arr;
            arr[i + 1] = arr[i];
        } else if (str_equal(array->data_type, "T")) {
            array->t_arr[i + 1] = array->t_arr[i];
        }
    }

    if (str_equal(array->data_type, "int")) {
        wrapper_int_t *wrapper_int = (wrapper_int_t *)data;
        int *arr = (int *)array->arr;
        arr[index] = wrapper_int->data;
    } else if (str_equal(array->data_type, "double")) {
        wrapper_double_t *wrapper_double = (wrapper_double_t *)data;
        double *arr = (double *)array->arr;
        arr[index] = wrapper_double->data;
    } else if (str_equal(array->data_type, "T")) {
        array->t_arr[index] = data;
    }

    array->size++;
    return array;
}

/**
 * @brief   Removes the element at the specified position in the cino-array.
 * @note    This function just removes the element from the cino-array. It is caller's
 *          responsibility to free the removed element, if it is a generic cino-array.
 * @param array cino-array
 * @param index index
 * @return  Returns the modified cino-array.
 */
array_t *array_remove(array_t *array, int index) {
    return_value_if_fail(array != NULL && index >= 0 && index < array->size, array);

    for (int i = index + 1; i < array->size; i++) {
        if (str_equal(array->data_type, "int")) {
            int *arr = (int *)array->arr;
            arr[i - 1] = arr[i];
        } else if (str_equal(array->data_type, "double")) {
            double *arr = (double *)array->arr;
            arr[i - 1] = arr[i];
        } else if (str_equal(array->data_type, "T")) {
            array->t_arr[i - 1] = array->t_arr[i];
        }
    }
    array->size--;

    array_resize(array);
    return array;
}

/**
 * @brief   Get the minimum value in the cino-array.
 * @param array     cino-array
 * @param compare   User-defined callback function for comparison, only for T (generic)
 *                  cino-array. Set to `NULL` if the elements in the cino-array are
 *                  primitive.
 * @return  Returns the minimum value in the cino-array, or `NULL` if conditions failed.
 *          For primitive data, a wrapper type of that primitive is returned. It is
 *          caller's responsibility to unwrap.
 */
T array_min(const array_t *array, compare_t compare) {
    return_value_if_fail(array != NULL && array->size > 0, NULL);

    if (str_equal(array->data_type, "int")) {
        int *arr = (int *)array->arr;
        int min = arr[0];
        for (int i = 1; i < array->size; i++) {
            if (arr[i] < min) {
                min = arr[i];
            }
        }
        return wrap_int(min);
    } else if (str_equal(array->data_type, "double")) {
        double *arr = (double *)array->arr;
        double min = arr[0];
        for (int i = 1; i < array->size; i++) {
            if (arr[i] < min) {
                min = arr[i];
            }
        }
        return wrap_double(min);
    } else if (str_equal(array->data_type, "T")) {
        return_value_if_fail(compare != NULL, NULL);
        T min = array->t_arr[0];
        for (int i = 1; i < array->size; i++) {
            if (compare(array->t_arr[i], min) < 0) {
                min = array->t_arr[i];
            }
        }
        return min;
    }

    return NULL;
}

/**
 * @brief   Get the maximum value in the cino-array.
 * @param array     cino-array
 * @param compare   User-defined callback function for comparison, only for T (generic)
 *                  cino-array. Set to `NULL` if the elements in the cino-array are
 *                  primitive.
 * @return  Returns the maximum value in the cino-array, or `NULL` if conditions failed.
 *          For primitive data, a wrapper type of that primitive is returned. It is
 *          caller's responsibility to unwrap.
 */
T array_max(const array_t *array, compare_t compare) {
    return_value_if_fail(array != NULL && array->size > 0, NULL);

    if (str_equal(array->data_type, "int")) {
        int *arr = (int *)array->arr;
        int max = arr[0];
        for (int i = 1; i < array->size; i++) {
            if (arr[i] > max) {
                max = arr[i];
            }
        }
        return wrap_int(max);
    } else if (str_equal(array->data_type, "double")) {
        double *arr = (double *)array->arr;
        double max = arr[0];
        for (int i = 1; i < array->size; i++) {
            if (arr[i] > max) {
                max = arr[i];
            }
        }
        return wrap_double(max);
    } else if (str_equal(array->data_type, "T")) {
        return_value_if_fail(compare != NULL, NULL);
        T max = array->t_arr[0];
        for (int i = 1; i < array->size; i++) {
            if (compare(array->t_arr[i], max) > 0) {
                max = array->t_arr[i];
            }
        }
        return max;
    }

    return NULL;
}

/**
 * @brief   Find the index of the first occurrence of the specified element in the
 *          cino-array.
 * @param array     cino-array
 * @param context   - For primitive type cino-array, a wrapper type of the searching data
 *                    should be passed. It is caller's responsibility to unwrap.
 *                  - For T (generic) cino-array, a match_t callback function should be passed
 *                    as the matching rule.
 * @return  Returns the index of the first occurrence of the specified element in the
 *          cino-array, or `-1` if not found.
 */
int array_index_of(const array_t *array, void *context) {
    return_value_if_fail(array != NULL && context != NULL, -1);

    for (int i = 0; i < array->size; i++) {
        if (str_equal(array->data_type, "int")) {
            wrapper_int_t *wrapper_int = (wrapper_int_t *)context;
            int *arr = (int *)array->arr;
            if (arr[i] == wrapper_int->data) {
                return i;
            }
        } else if (str_equal(array->data_type, "double")) {
            wrapper_double_t *wrapper_double = (wrapper_double_t *)context;
            double *arr = (double *)array->arr;
            if (equal_double(arr[i], wrapper_double->data)) {
                return i;
            }
        } else if (str_equal(array->data_type, "T")) {
            match_t match = (match_t)context;
            if (match(array->t_arr[i])) {
                return i;
            }
        }
    }

    return -1;
}

/**
 * @brief   Find the index of the last occurrence of the specified element in the
 *          cino-array.
 * @param array     cino-array
 * @param context   - For primitive type cino-array, a wrapper type of the searching data
 *                    should be passed. It is caller's responsibility to unwrap.
 *                  - For T (generic) cino-array, a match_t callback function should be passed
 *                    as the matching rule.
 * @return  Returns the index of the last occurrence of the specified element in the
 *          cino-array, or `-1` if not found.
 */
int array_last_index_of(const array_t *array, void *context) {
    return_value_if_fail(array != NULL && context != NULL, -1);

    for (int i = array->size - 1; i >= 0; i--) {
        if (str_equal(array->data_type, "int")) {
            wrapper_int_t *wrapper_int = (wrapper_int_t *)context;
            int *arr = (int *)array->arr;
            if (arr[i] == wrapper_int->data) {
                return i;
            }
        } else if (str_equal(array->data_type, "double")) {
            wrapper_double_t *wrapper_double = (wrapper_double_t *)context;
            double *arr = (double *)array->arr;
            if (equal_double(arr[i], wrapper_double->data)) {
                return i;
            }
        } else if (str_equal(array->data_type, "T")) {
            match_t match = (match_t)context;
            if (match(array->t_arr[i])) {
                return i;
            }
        }
    }

    return -1;
}

/**
 * @brief   Count the occurrences of the element matched.
 * @param array     cino-array
 * @param context   - For primitive type cino-array, a wrapper type of the counting data
 *                    should be passed. It is caller's responsibility to unwrap.
 *                  - For T (generic) cino-array, a match_t callback function should be passed
 *                    as the matching rule.
 * @return  Returns occurrences of the element matched.
 */
int array_count(const array_t *array, void *context) {
    return_value_if_fail(array != NULL && context != NULL, 0);

    int cnt = 0;
    for (int i = 0; i < array->size; i++) {
        if (str_equal(array->data_type, "int")) {
            wrapper_int_t *wrapper_int = (wrapper_int_t *)context;
            int *arr = (int *)array->arr;
            if (arr[i] == wrapper_int->data) {
                cnt++;
            }
        } else if (str_equal(array->data_type, "double")) {
            wrapper_double_t *wrapper_double = (wrapper_double_t *)context;
            double *arr = (double *)array->arr;
            if (equal_double(arr[i], wrapper_double->data)) {
                cnt++;
            }
        } else if (str_equal(array->data_type, "T")) {
            match_t match = (match_t)context;
            if (match(array->t_arr[i])) {
                cnt++;
            }
        }
    }

    return cnt;
}

/**
 * @brief   Reverses the order of all elements in the cino-array.
 * @param array cino-array
 * @return  Returns the modified cino-array.
 */
array_t *array_reverse(array_t *array) {
    return_value_if_fail(array != NULL, NULL);

    int i = 0;
    int j = array->size - 1;

    while (i < j) {
        if (str_equal(array->data_type, "int")) {
            int *arr = (int *)array->arr;
            swap(arr[i], arr[j], int);
        } else if (str_equal(array->data_type, "double")) {
            double *arr = (double *)array->arr;
            swap(arr[i], arr[j], double);
        } else if (str_equal(array->data_type, "T")) {
            swap(array->t_arr[i], array->t_arr[j], T);
        }
        i++;
        j--;
    }

    return array;
}

/**
 * @brief   Swap two elements at specified indices in the cino-array.
 * @param array     cino-array
 * @param index1    index 1
 * @param index2    index 2
 * @return  Returns the modified cino-array.
 */
array_t *array_swap(array_t *array, int index1, int index2) {
    return_value_if_fail(array != NULL && index1 >= 0 && index1 < array->size && index2 >= 0 && index2 < array->size && index1 != index2, array);

    if (str_equal(array->data_type, "int")) {
        int *arr = (int *)array->arr;
        swap(arr[index1], arr[index2], int);
    } else if (str_equal(array->data_type, "double")) {
        double *arr = (double *)array->arr;
        swap(arr[index1], arr[index2], double);
    } else if (str_equal(array->data_type, "T")) {
        swap(array->t_arr[index1], array->t_arr[index2], T);
    }

    return array;
}

// /**
//  * @brief   Specify the rules for comparing two int values in ascending order.
//  * @param data1 pointer to the first value
//  * @param data2 pointer to the second value
//  * @return  Returns
//  *              - 0 if two values are equal
//  *              - positive if the first value is greater than the second value
//  *              - negative if the first value is less than the second value
//  */
// static int cmp_int_less(const void *data1, const void *data2) {
//     int *p1 = (int *)data1;
//     int *p2 = (int *)data2;
//     return *p1 - *p2;
// }

// /**
//  * @brief   Specify the rules for comparing two int values in descending order.
//  * @param data1 pointer to the first value
//  * @param data2 pointer to the second value
//  * @return  Returns
//  *              - 0 if two values are equal
//  *              - positive if the first value is less than the second value
//  *              - negative if the first value is greater than the second value
//  */
// static int cmp_int_greater(const void *data1, const void *data2) {
//     int *p1 = (int *)data1;
//     int *p2 = (int *)data2;
//     return *p2 - *p1;
// }

// /**
//  * @brief   Sort the cino-int-array.
//  * @param array     cino-int-array
//  * @param reverse   true for descending, false for ascending
//  * @return  Returns the modified cino-int-array.
//  */
// array_t *array_int_sort(array_t *array, bool reverse) {
//     return_value_if_fail(array != NULL, NULL);
//     if (reverse) {
//         qsort(array->i_arr, array->size, sizeof(int), cmp_int_greater);
//     } else {
//         qsort(array->i_arr, array->size, sizeof(int), cmp_int_less);
//     }
//     return array;
// }

// /**
//  * @brief   Get the iterator.
//  * @param array cino-int-array
//  * @return  Returns the iterator.
//  */
// iter_t array_int_iter(array_t *array) {
//     return_value_if_fail(array != NULL, NULL);
//     if (array->size > 0) {
//         array->iterator->iter = array->i_arr;
//     } else {
//         array->iterator->iter = NULL;
//     }
//     return array->iterator->iter;
// }

// /**
//  * @brief   Determine if the cino-int-array has the next iterator.
//  * @param array cino-int-array
//  * @return  Returns `true` if next iterator exists, otherwise returns `false`.
//  */
// bool array_int_iter_has_next(const array_t *array) {
//     return_value_if_fail(array != NULL && array->iterator->iter != NULL, false);
//     return (void *)((byte_t *)array->iterator->iter + sizeof(int)) < (void *)(array->i_arr + array->size);
// }

// /**
//  * @brief   Get the next iterator.
//  * @param array cino-int-array
//  * @return  Returns the next iterator.
//  */
// iter_t array_int_iter_next(array_t *array) {
//     return_value_if_fail(array != NULL, NULL);
//     if (array_int_iter_has_next(array)) {
//         byte_t *iter = (byte_t *)array->iterator->iter;
//         iter += sizeof(int);
//         array->iterator->iter = (void *)iter;
//     } else {
//         array->iterator->iter = NULL;
//     }
//     return array->iterator->iter;
// }

// /**
//  * @brief   Specify the rules for comparing two double values in ascending order.
//  * @param data1 pointer to the first value
//  * @param data2 pointer to the second value
//  * @return  Returns
//  *              - 0 if two values are equal
//  *              - positive if the first value is greater than the second value
//  *              - negative if the first value is less than the second value
//  */
// static int cmp_double_less(const void *data1, const void *data2) {
//     double *p1 = (double *)data1;
//     double *p2 = (double *)data2;
//     return *p1 > *p2 ? 1 : -1;
// }

// /**
//  * @brief   Specify the rules for comparing two double values in descending order.
//  * @param data1 pointer to the first value
//  * @param data2 pointer to the second value
//  * @return  Returns
//  *              - 0 if two values are equal
//  *              - positive if the first value is less than the second value
//  *              - negative if the first value is greater than the second value
//  */
// static int cmp_double_greater(const void *data1, const void *data2) {
//     double *p1 = (double *)data1;
//     double *p2 = (double *)data2;
//     return *p2 > *p1 ? 1 : -1;
// }

// /**
//  * @brief   Sort the cino-double-array.
//  * @param array     cino-double-array
//  * @param reverse   true for descending, false for ascending
//  * @return  Returns the modified cino-double-array.
//  */
// array_t *array_double_sort(array_t *array, bool reverse) {
//     return_value_if_fail(array != NULL, NULL);
//     if (reverse) {
//         qsort(array->d_arr, array->size, sizeof(double), cmp_double_greater);
//     } else {
//         qsort(array->d_arr, array->size, sizeof(double), cmp_double_less);
//     }
//     return array;
// }

// /**
//  * @brief   Get the iterator.
//  * @param array cino-double-array
//  * @return  Returns the iterator.
//  */
// iter_t array_double_iter(array_t *array) {
//     return_value_if_fail(array != NULL, NULL);
//     if (array->size > 0) {
//         array->iterator->iter = array->d_arr;
//     } else {
//         array->iterator->iter = NULL;
//     }
//     return array->iterator->iter;
// }

// /**
//  * @brief   Determine if the cino-double-array has the next iterator.
//  * @param array cino-double-array
//  * @return  Returns `true` if next iterator exists, otherwise returns `false`.
//  */
// bool array_double_iter_has_next(const array_t *array) {
//     return_value_if_fail(array != NULL && array->iterator->iter != NULL, false);
//     return (void *)((byte_t *)array->iterator->iter + sizeof(double)) < (void *)(array->d_arr + array->size);
// }

// /**
//  * @brief   Get the next iterator.
//  * @param array cino-double-array
//  * @return  Returns the next iterator.
//  */
// iter_t array_double_iter_next(array_t *array) {
//     return_value_if_fail(array != NULL, NULL);
//     if (array_double_iter_has_next(array)) {
//         byte_t *iter = (byte_t *)array->iterator->iter;
//         iter += sizeof(double);
//         array->iterator->iter = (void *)iter;
//     } else {
//         array->iterator->iter = NULL;
//     }
//     return array->iterator->iter;
// }

// /**
//  * @brief   Parition function for quick sort.
//  * @param arr       an array of pointers
//  * @param start     start index of partition
//  * @param end       end index of partition
//  * @param compare   user-defined callback function for comparison
//  * @return  Returns the index of the pivot.
//  */
// static int quick_sort_partition(void **arr, int start, int end, compare_t compare) {
//     int i = start - 1;
//     void *pivot = arr[end];

//     for (int j = start; j < end; j++) {
//         if (compare(arr[j], pivot) < 0) {
//             i++;
//             swap(arr[i], arr[j], void *);
//         }
//     }

//     swap(arr[i + 1], arr[end], void *);
//     return i + 1;
// }

// /**
//  * @brief   Stack-based quick sort for cino-array.
//  * @param arr       an array of pointers
//  * @param size      number of elements in the array
//  * @param compare   user-defined callback function for comparison
//  */
// static void quick_sort(void **arr, size_t size, compare_t compare) {
//     int stack[size];
//     memset(stack, 0x00, size * sizeof(int));

//     int n = 0;
//     stack[n++] = 0;
//     stack[n++] = size - 1;

//     while (n > 0) {
//         int right = stack[--n];
//         int left = stack[--n];

//         int index = quick_sort_partition(arr, left, right, compare);
//         if (index - 1 > left) {
//             stack[n++] = left;
//             stack[n++] = index - 1;
//         }
//         if (index + 1 < right) {
//             stack[n++] = index + 1;
//             stack[n++] = right;
//         }
//     }
// }

// /**
//  * @brief   Sort the cino-array.
//  * @param array     cino-array
//  * @param compare   user-defined callback function for comparison
//  * @return  Returns the modified cino-array.
//  */
// array_t *array_sort(array_t *array, compare_t compare) {
//     return_value_if_fail(array != NULL, NULL);
//     quick_sort(array->t_arr, array->size, compare);
//     return array;
// }

// /**
//  * @brief   Get the iterator.
//  * @param array cino-array
//  * @return  Returns the iterator.
//  */
// iter_t array_iter(array_t *array) {
//     return_value_if_fail(array != NULL, NULL);
//     if (array->size > 0) {
//         array->iterator->iter = array->t_arr[0];
//         array->iterator->iter_index = 0;
//     } else {
//         array->iterator->iter = NULL;
//         array->iterator->iter_index = -1;
//     }
//     return array->iterator->iter;
// }

// /**
//  * @brief   Determine if the cino-array has the next iterator.
//  * @param array cino-array
//  * @return  Returns `true` if next iterator exists, otherwise returns `false`.
//  */
// bool array_iter_has_next(const array_t *array) {
//     return_value_if_fail(array != NULL && array->iterator->iter != NULL && array->iterator->iter_index >= 0, false);
//     return array->iterator->iter_index + 1 < array->size;
// }

// /**
//  * @brief   Get the next iterator.
//  * @param array cino-array
//  * @return  Returns the next iterator.
//  */
// iter_t array_iter_next(array_t *array) {
//     return_value_if_fail(array != NULL, NULL);
//     if (array_iter_has_next(array)) {
//         array->iterator->iter_index++;
//         array->iterator->iter = array->t_arr[array->iterator->iter_index];
//     } else {
//         array->iterator->iter = NULL;
//         array->iterator->iter_index = -1;
//     }
//     return array->iterator->iter;
// }