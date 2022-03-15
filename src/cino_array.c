#include "cino_array.h"

/**
 * @brief cino数组结构
 */
typedef struct array_t {
    void **arr;     // 数组
    int size;       // 元素个数
    int capacity;   // 容量
    int elem_size;  // 每个元素大小
} array_t;

/****************************************
 *            cino数组创建
 ****************************************/

/**
 * @brief   创建cino数组
 * @param elem_size :   每个元素大小
 * @return  返回构造好的cino数组，失败返回NULL。
 */
array_t *array_create(int elem_size) {
    return_value_if_fail(elem_size > 0, NULL);
    array_t *array = (array_t *)cino_alloc(sizeof(array_t));
    return_value_if_fail(array != NULL, NULL);
    array->arr = (void **)cino_alloc(sizeof(void *));
    // TODO: call_and_return
    array->size = 0;
    array->capacity = 1;
    array->elem_size = elem_size;
    return array;
}

/****************************************
 *            cino数组销毁
 ****************************************/

/**
 * @brief   销毁cino数组
 * @param array    :   cino数组
 */
void array_destroy(array_t *array) {
    return_if_fail(array != NULL);

    array->size = 0;
    array->capacity = 0;
    array->elem_size = 0;

    if (array->arr) {
        free(array->arr);
        array->arr = NULL;
    }

    if (array) {
        free(array);
        array = NULL;
    }
}

/****************************************
 *            cino数组操作
 ****************************************/

/**
 * @brief   获取cino数组元素个数
 * @param string    :   cino数组
 * @return  cino数组元素个数
 */
int array_size(const array_t *array) {
    return_value_if_fail(array != NULL && array->arr != NULL, 0);
    return array->size;
}

void *array_get(const array_t *array, int index) {
    return_value_if_fail(array != NULL && index >= 0 && index < array->size, NULL);
    return array->arr[index];
}

void array_append(array_t *array, const void *data) {
    return_if_fail(array != NULL && data != NULL);

    // // 首次分配空间
    // if (!array->arr) {
    //     array->arr = cino_alloc(array->elem_size);
    //     return_if_fail(array->arr != NULL);
    //     array->capacity = 1;
    // }

    // 翻倍扩容
    if (array->size == array->capacity) {
        array->arr = cino_realloc(array->arr, array->capacity * sizeof(void *), array->capacity * sizeof(void *) * 2);
        call_and_return_if_fail(array->arr != NULL, array_destroy(array));
        array->capacity *= 2;
    }

    array->arr[array->size] = (void *)data;
    array->size++;
}