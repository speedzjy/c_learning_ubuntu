/*************************************************************************
	> File Name: quick_sort_test.cpp
	> Author: huguang
	> Mail: hug@haizeix.com
	> Created Time: 四  8/13 20:53:53 2020
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include <vector>
#include "quick_sort.h"
using namespace std;

#define MAX_N 10000000

#define COLOR(msg, code) "\033[1;" #code "m" msg "\033[0m" 
#define RED(msg) COLOR(msg, 31)
#define GREEN(msg) COLOR(msg, 32)
#define YELLOW(msg) COLOR(msg, 33)
#define BLUE(msg) COLOR(msg, 34)

#define TEST(func, arr, l, r) { \
    int *temp = (int *)malloc(sizeof(int) * (r - l + 1)); \
    for (int i = l; i <= r; i++) temp[i - l] = arr[i]; \
    long long b = clock(); \
    func(temp, 0, r - l); \
    long long e = clock(); \
    if (check(temp, 0, r - l)) { \
        printf(GREEN("[    OK    ]")" %s ", #func); \
    } else { \
        printf(RED("[  FAILED  ]")" %s ", #func); \
    } \
    printf("(%lld ms)\n", 1000 * (e - b) / CLOCKS_PER_SEC); \
}

int *getRandData(int n) {
    int *arr = (int *)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % n;
    }
    return arr;
}

int check(int *arr, int l, int r) {
    for (int i = l + 1; i <= r; i++) {
        if (arr[i] < arr[i - 1]) return 0;
    }
    return 1;
}

int main() {
    srand(time(0));
    int *arr = getRandData(MAX_N);
    TEST(quick_sort_v1, arr, 0, MAX_N - 1);
    return 0;
}
