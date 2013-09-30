#ifndef _proxy_h_
#define _proxy_h_

#include <utils/String16.h>

using namespace android;

char convert_hex_digit_to_char(unsigned char value, int lowercase);
String16 convert_buffer_to_string16(unsigned char* buffer, int length);
unsigned char convert_char_to_hex_digit(char c);
unsigned char* convert_string16_to_buffer(String16 str16, int* outlen);
unsigned char* convert_string16_to_string8(String16 str16, int* outlen);

#endif