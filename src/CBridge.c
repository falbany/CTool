/**
 * @file CBridge.c
 * @brief Aggregator for implementation files.
 */

#include "CBridge.h"

const struct CBridgeNamespace CBridge = {.str = &cbridge_string, .vector = &cbridge_vector, .file = &cbridge_file, .math = &cbridge_math, .net = &cbridge_net};