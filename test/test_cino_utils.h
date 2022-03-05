#ifndef _TEST_CINO_UTILS_H_
#define _TEST_CINO_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>

void test_return_if_fail();
void test_return_value_if_fail();
void test_call_and_return_if_fail();
void test_call_and_return_value_if_fail();

void test_array_len();

void test_string_equal();
void test_equals_ignore_case();
void test_string_clear();
void test_string_length();
void test_string_copy();

void test_string_to_bool();
void test_bool_to_string();
void test_string_to_int();
void test_int_to_string();
void test_string_to_double();
void test_double_to_string();

#endif