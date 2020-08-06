/*
 * Base64 encoding/decoding (RFC1341)
 * Copyright (c) 2005-2011, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */


#include "base64.h"

namespace base64 {
static const unsigned char base64_table[65] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * base64_encode - Base64 encode
 * @src: Data to be encoded
 * @len: Length of the data to be encoded
 * @out_len: Pointer to output length variable, or %NULL if not used
 * Returns: Allocated buffer of out_len bytes of encoded data,
 * or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer. Returned buffer is
 * nul terminated to make it easier to use as a C string. The nul terminator is
 * not included in out_len.
 */

absl::StatusOr<std::string> base64_encode(std::string str) {
	unsigned char *out, *pos;
	const unsigned char *end, *in;
	std::size_t olen;
	int line_len;

	// adaptation for str
	unsigned char *src = (unsigned char*) str.c_str();
	std::size_t len = str.length();
	if (len == 0) { // handle empty str
		return str;
	}
	// end

	olen = (len +2) / 3 * 4;
	if (olen < len)
		return absl::InvalidArgumentError("Given string is too long to encode and has cause integer overflow."); /* integer overflow */


	// adaptation to return str
	std::string outStr;
    outStr.resize(olen);
    out = (unsigned char*)&outStr[0];
	//

	end = src + len;
	in = src;
	pos = out;
	line_len = 0;
	while (end - in >= 3) {
		*pos++ = base64_table[in[0] >> 2];
		*pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		*pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
		*pos++ = base64_table[in[2] & 0x3f];
		in += 3;
		line_len += 4;
		if (line_len >= 72) {
			*pos++ = '\n';
			line_len = 0;
		}
	}

	if (end - in) {
		*pos++ = base64_table[in[0] >> 2];
		if (end - in == 1) {
			*pos++ = base64_table[(in[0] & 0x03) << 4];
			*pos++ = '=';
		} else {
			*pos++ = base64_table[((in[0] & 0x03) << 4) |
					      (in[1] >> 4)];
			*pos++ = base64_table[(in[1] & 0x0f) << 2];
		}
		*pos++ = '=';
		line_len += 4;
	}

	if (line_len)
		*pos++ = '\n';

	*pos = '\0';
	return outStr;
}


/**
 * base64_decode - Base64 decode
 * @src: Data to be decoded
 * @len: Length of the data to be decoded
 * @out_len: Pointer to output length variable
 * Returns: Allocated buffer of out_len bytes of decoded data,
 * or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer.
 */

absl::StatusOr<std::string> base64_decode(std::string str) {
	unsigned char dtable[256], *out, *pos, block[4], tmp;
	std::size_t i, count, olen;
	int pad = 0;

	// adaptation for str
	unsigned char *src = (unsigned char*) str.c_str();
	std::size_t len = str.length();
	if (len == 0) { // handle empty str
		return str;
	}
	// end


	os_memset(dtable, 0x80, 256);
	for (i = 0; i < sizeof(base64_table) - 1; i++)
		dtable[base64_table[i]] = (unsigned char) i;
	dtable['='] = 0;

	count = 0;
	for (i = 0; i < len; i++) {
		if (dtable[src[i]] != 0x80)
			count++;
	}

	// Temporarily comment out
	// if (count == 0 || count % 4)
	// 	return absl::InvalidArgumentError("Given string is not a valid base64 encoding");

	olen = count / 4 * 3;

	// adaptation to return str
	std::string outStr;
    outStr.resize(olen);
    pos = out = (unsigned char*)&outStr[0];
	//

	count = 0;
	for (i = 0; i < len; i++) {
		tmp = dtable[src[i]];
		if (tmp == 0x80)
			continue;

		if (src[i] == '=')
			pad++;
		block[count] = tmp;
		count++;
		if (count == 4) {
			*pos++ = (block[0] << 2) | (block[1] >> 4);
			*pos++ = (block[1] << 4) | (block[2] >> 2);
			*pos++ = (block[2] << 6) | block[3];
			count = 0;
			if (pad) {
				if (pad == 1)
					pos--;
				else if (pad == 2)
					pos -= 2;
				else {
					/* Invalid padding */
					os_free(out);
					return absl::InvalidArgumentError("Given string has invalid padding");
				}
				break;
			}
		}
	}
	// strip all trailing nulls
	std::size_t nulls_found = outStr.find('\0');
	if (nulls_found!=std::string::npos) {
		return outStr.erase(nulls_found);
	}

	return outStr;
}
} // base64
