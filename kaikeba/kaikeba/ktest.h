/*************************************************************************
	> File Name: ktest.h
	> Author: huguang
	> Mail: hug@haizeix.com
	> Created Time: 
 ************************************************************************/

#ifndef _KTEST_H
#define _KTEST_H

#define COLOR(msg, code) "\033[0;1;" #code "m" msg "\033[0m"
#define RED(msg)    COLOR(msg, 31) 
#define GREEN(msg)  COLOR(msg, 32) 
#define YELLOW(msg) COLOR(msg, 33)  
#define BLUE(msg)   COLOR(msg, 34) 

#define EXPECT(a, comp, b) { \
    __typeof(a) __a = (a), __b = (b); \
    if (!((__a) comp (__b))) { \
        printf(YELLOW("  %s:%d: Failure\n"), __FILE__, __LINE__); \
        printf(YELLOW("    Expected: (%s) %s (%s), actual: %d vs %d\n"), \
        #a, #comp, #b, __a, __b); \
    } \
}
#define EXPECT_EQ(a, b) EXPECT(a, ==, b)
#define EXPECT_NE(a, b) EXPECT(a, !=, b)
#define EXPECT_LT(a, b) EXPECT(a, <, b)
#define EXPECT_LE(a, b) EXPECT(a, <=, b)
#define EXPECT_GT(a, b) EXPECT(a, >, b)
#define EXPECT_GE(a, b) EXPECT(a, >=, b)

#define TEST(a, b)  \
void kaikeba_##a##_##b(); \
__attribute__((constructor)) \
void reg_##a##_##b() { \
    add_test_func(kaikeba_##a##_##b, #a "." #b); \
} \
void kaikeba_##a##_##b()

struct {
    void (*func)();
    const char *func_name;
} func_arr[100];
int func_cnt = 0;

void add_test_func(void (*func)(), const char *str) {
    func_arr[func_cnt].func = func;
    func_arr[func_cnt].func_name = str;
    func_cnt++;
    return ;
}

int RUN_ALL_TESTS() {
    for (int i = 0; i < func_cnt; i++) {
        printf(GREEN("[  RUN    ]") " %s\n", func_arr[i].func_name);
        func_arr[i].func();
    }
    return 0;
}

#endif
