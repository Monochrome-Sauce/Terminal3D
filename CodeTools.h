#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#pragma once



/* Data is passed to the called function. The data may or may not change. */
#define _pIn_

/* The caller only provides space for the called function to write to. The called function writes data into that space. */
#define _pOut_

#define VAR_NAME(x) #x
#define BOOL_TO_STR(x) ((x) ? "true" : "false")
