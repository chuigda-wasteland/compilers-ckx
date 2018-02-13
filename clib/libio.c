#include <stdio.h>
#include <stdint.h>

void print_D_N(double number) {
    printf("%g\n", number);
}

void print_F_N(float number) {
    printf("%g\n", number);
}

void print_l_N(int64_t number) {
    printf("%ld\n", number);
}

void print_i_N(int32_t number) {
    printf("%d\n", number);
}

void print_s_N(int16_t number) {
    printf("%hd\n", number);
}

void print_b_N(int8_t number) {
    printf("%d\n", (int)number);
}

void print_L_N(uint64_t number) {
    printf("%lu\n", number);
}

void input_Pl_N(int64_t *p) {
    scanf("%ld", p);
}

void input_Pi_N(int32_t *p) {
    scanf("%d", p);
}

void input_Ps_N(int16_t *p) {
    scanf("%hd", p);
}

void input_Pb_N(int8_t *p) {
    int temp;
    scanf("%d", &temp);
    *p = temp;
}

void input_PD_N(double *p) {
    scanf("%lf", p);
}

void input_PF_N(float *p) {
    scanf("%f", p);
}

