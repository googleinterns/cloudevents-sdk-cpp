/*
 * Base64 encoding/decoding (RFC1341)
 * Copyright (c) 2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 *
 * EDITTED to accomodate std::string
 */

#ifndef BASE64_H
#define BASE64_H

#include <cstddef>
#include <cstring>
#include <string>

unsigned char * base64_encode(std::string str);
unsigned char * base64_decode(std::string str);

#endif /* BASE64_H */