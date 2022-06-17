#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#pragma once


#define VAR_NAME(x) #x
#define BOOL_TO_STR(x) ((x) ? "true" : "false")
#define IN_RANGE(low, val, high) ((low)<(val) && (val)<(high))
#define INT_SWAP(a, b) do { a ^= b; b ^= a; a ^= b; } while(0)
