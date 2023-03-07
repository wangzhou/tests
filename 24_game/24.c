//==============================================================================
//  Copyright (C) 2019 王小康. All rights reserved.
//
//  作者: 王小康
//  描述: 24点游戏求解程序
//  日期: 2019-04-15
//
//==============================================================================
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int   integer;
    int   remainder;
    int   denominator;
    char *string;
} VALUE_T;

static int level_3(VALUE_T *a, VALUE_T *b){
    int va, vb, integer, remainder, denominator = a->denominator * b->denominator;
    int num = 0;
    // +
    va = (a->integer * denominator + a->remainder * b->denominator);
    vb = (b->integer * denominator + b->remainder * a->denominator);
    integer = (va + vb) / denominator;
    remainder = (va + vb) % denominator;
    if((integer == 24) && (remainder == 0)){
        printf("%s+%s = 24\n", a->string, b->string);
        num++;
    }
    
    // -
    va = (a->integer * denominator + a->remainder * b->denominator);
    vb = (b->integer * denominator + b->remainder * a->denominator);
    if(va >= vb){
        integer = (va - vb) / denominator;
        remainder = (va - vb) % denominator;
        if((integer == 24) && (remainder == 0)){
            printf("%s-%s = 24\n", a->string, b->string);
            num++;
        }
    }
    else{
        integer = (vb - va) / denominator;
        remainder = (vb - va) % denominator;
        if((integer == 24) && (remainder == 0)){
            printf("%s-%s = 24\n", b->string, a->string);
            num++;
        }
    }
    
    // *
    va = (a->integer * a->denominator + a->remainder);
    vb = (b->integer * b->denominator + b->remainder);
    integer = (va * vb) / denominator;
    remainder = (va * vb) % denominator;
    if((integer == 24) && (remainder == 0)){
        printf("%sx%s = 24\n", a->string, b->string);
        num++;
    }
    
    // /
    va = (a->integer * a->denominator + a->remainder) * b->denominator;
    vb = (b->integer * b->denominator + b->remainder) * a->denominator;
    if(vb){
        integer = va / vb;
        remainder = va % vb;
        if((integer == 24) && (remainder == 0)){
            printf("%s/%s = 24\n", a->string, b->string);
            num++;
        }
    }
    if(va){
        integer = vb / va;
        remainder = vb % va;
        if((integer == 24) && (remainder == 0)){
            printf("%s/%s = 24\n", b->string, a->string);
            num++;
        }
    }
    return num;
}

static int level_2(VALUE_T *a, VALUE_T *b, VALUE_T *c){
    char stringBuffer[32];
    VALUE_T value;
    int va, vb, denominator = a->denominator * b->denominator;
    int num = 0;
    
    // +
    va = (a->integer * denominator + a->remainder * b->denominator);
    vb = (b->integer * denominator + b->remainder * a->denominator);
    value.integer = (va + vb) / denominator;
    value.remainder = (va + vb) % denominator;
    value.denominator = denominator;
    sprintf(stringBuffer, "(%s+%s)", a->string, b->string);
    value.string = stringBuffer;
    num += level_3(&value, c);

    // -
    va = (a->integer * denominator + a->remainder * b->denominator);
    vb = (b->integer * denominator + b->remainder * a->denominator);
    if(va >= vb){
        value.integer = (va - vb) / denominator;
        value.remainder = (va - vb) % denominator;
        sprintf(stringBuffer, "(%s-%s)", a->string, b->string);
    }
    else{
        value.integer = (vb - va) / denominator;
        value.remainder = (vb - va) % denominator;
        sprintf(stringBuffer, "(%s-%s)", b->string, a->string);
    }
    value.denominator = denominator;
    value.string = stringBuffer;
    num += level_3(&value, c);
    
    // *
    va = (a->integer * a->denominator + a->remainder);
    vb = (b->integer * b->denominator + b->remainder);
    value.integer = (va * vb) / denominator;
    value.remainder = (va * vb) % denominator;
    value.denominator = denominator;
    sprintf(stringBuffer, "(%sx%s)", a->string, b->string);
    value.string = stringBuffer;
    num += level_3(&value, c);
    
    // /
    va = (a->integer * a->denominator + a->remainder) * b->denominator;
    vb = (b->integer * b->denominator + b->remainder) * a->denominator;
    if(vb){
        value.integer = va / vb;
        value.remainder = va % vb;
        value.denominator = vb;
        sprintf(stringBuffer, "(%s/%s)", a->string, b->string);
        value.string = stringBuffer;
        num += level_3(&value, c);
    }
    if(va){
        value.integer = vb / va;
        value.remainder = vb % va;
        value.denominator = va;
        sprintf(stringBuffer, "(%s/%s)", b->string, a->string);
        value.string = stringBuffer;
        num += level_3(&value, c);
    }
    return num;
}

static int level_1(VALUE_T *a, VALUE_T *b, VALUE_T *c, VALUE_T *d){
    char stringBuffer[32];
    VALUE_T value;
    int va, vb, denominator = a->denominator * b->denominator;
    int num = 0;
    
    // +
    va = (a->integer * denominator + a->remainder * b->denominator);
    vb = (b->integer * denominator + b->remainder * a->denominator);
    value.integer = (va + vb) / denominator;
    value.remainder = (va + vb) % denominator;
    value.denominator = denominator;
    sprintf(stringBuffer, "(%s+%s)", a->string, b->string);
    value.string = stringBuffer;
    num += level_2(&value, c, d);
    num += level_2(&value, d, c);
    num += level_2(c, d, &value);
    
    // -
    va = (a->integer * denominator + a->remainder * b->denominator);
    vb = (b->integer * denominator + b->remainder * a->denominator);
    if(va >= vb){
        value.integer = (va - vb) / denominator;
        value.remainder = (va - vb) % denominator;
        sprintf(stringBuffer, "(%s-%s)", a->string, b->string);
    }
    else{
        value.integer = (vb - va) / denominator;
        value.remainder = (vb - va) % denominator;
        sprintf(stringBuffer, "(%s-%s)", b->string, a->string);
    }
    value.denominator = denominator;
    value.string = stringBuffer;
    num += level_2(&value, c, d);
    num += level_2(&value, d, c);
    num += level_2(c, d, &value);
    
    // *
    va = (a->integer * a->denominator + a->remainder);
    vb = (b->integer * b->denominator + b->remainder);
    value.integer = (va * vb) / denominator;
    value.remainder = (va * vb) % denominator;
    value.denominator = denominator;
    sprintf(stringBuffer, "(%sx%s)", a->string, b->string);
    value.string = stringBuffer;
    num += level_2(&value, c, d);
    num += level_2(&value, d, c);
    num += level_2(c, d, &value);
    
    // /
    va = (a->integer * a->denominator + a->remainder) * b->denominator;
    vb = (b->integer * b->denominator + b->remainder) * a->denominator;
    if(vb){
        value.integer = va / vb;
        value.remainder = va % vb;
        value.denominator = vb;
        sprintf(stringBuffer, "(%s/%s)", a->string, b->string);
        value.string = stringBuffer;
        num += level_2(&value, c, d);
        num += level_2(&value, d, c);
        num += level_2(c, d, &value);
    }
    if(va){
        value.integer = vb / va;
        value.remainder = vb % va;
        value.denominator = va;
        sprintf(stringBuffer, "(%s/%s)", b->string, a->string);
        value.string = stringBuffer;
        num += level_2(&value, c, d);
        num += level_2(&value, d, c);
        num += level_2(c, d, &value);
    }
    return num;
}

static int level_0(int a, int b, int c, int d){
    char buffer[4][4];
    sprintf(buffer[0],"%d", a);
    sprintf(buffer[1],"%d", b);
    sprintf(buffer[2],"%d", c);
    sprintf(buffer[3],"%d", d);
    
    VALUE_T value[4];
    
    value[0].integer = a;
    value[0].remainder = 0;
    value[0].denominator = 1;
    value[0].string = buffer[0];
    
    value[1].integer = b;
    value[1].remainder = 0;
    value[1].denominator = 1;
    value[1].string = buffer[1];
    
    value[2].integer = c;
    value[2].remainder = 0;
    value[2].denominator = 1;
    value[2].string = buffer[2];
    
    value[3].integer = d;
    value[3].remainder = 0;
    value[3].denominator = 1;
    value[3].string = buffer[3];
    
    int num = 0;
    num += level_1(&value[0], &value[1], &value[2], &value[3]);
    num += level_1(&value[0], &value[2], &value[1], &value[3]);
    num += level_1(&value[0], &value[3], &value[1], &value[2]);
    num += level_1(&value[1], &value[2], &value[0], &value[3]);
    num += level_1(&value[1], &value[3], &value[0], &value[2]);
    num += level_1(&value[2], &value[3], &value[0], &value[1]);
    return num;
}

////////////////////////////////////////////////////////////////////////////////

static int parameterCheck(int argc, char* argv[]){
    char *p;
    int value;
    while(*argv){
        p = *argv;
        if((*p < '1') || (*p > '9')) return 1;
        p++;
        while(*p){
            if((*p < '0') || (*p > '9')) return 1;
            p++;
        }
        value = atoi(*argv);
        if((value < 1) || (value > 13)) return 1;
        argv++;
    }
    return 0;
}

static void help(char *name){
    printf("Usage  : %s num1 num2 num3 num4\n", name);
    printf("Example: %s 5 5 5 1\n", name);
}

int main(int argc, char* argv[]){
    if(argc < 5){
        help(argv[0]);
        return 0;
    }
    if(parameterCheck(argc-1, argv+1)){
        help(argv[0]);
        return 0;
    }
    int num = level_0(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
    printf("total = %d\n", num);
    return 0;
}
