/**
 * @file libs/base/include/fatal.h
 * @brief Defines fatal error response
 * @copyright Copyright (c) 2016-2018 Silex Insight. All Rights reserved
 */

#ifndef FATAL_H
#define FATAL_H

/**
 * Response to a fatal error.
 * Triggers tamper and goes into infinite loop.
 * @param tamper_src tamper to be triggered
 */
void fatal_error(uint32_t tamper_src);

#endif /* FATAL_H */
