#pragma once

/*!
 * @file z-form.h
 * @brief Low level text formatting
 * @date 2023/04/30
 * @author Ben Harrison, 1997
 */

#include "system/h-basic.h"
#include <string>

uint vstrnfmt(char *buf, uint max, concptr fmt, va_list vp);
uint strnfmt(char *buf, uint max, concptr fmt, ...);
std::string format(concptr fmt, ...);
void plog_fmt(concptr fmt, ...);
void quit_fmt(concptr fmt, ...);
void core_fmt(concptr fmt, ...);
