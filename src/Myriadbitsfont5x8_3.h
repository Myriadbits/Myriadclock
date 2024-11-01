#include "FontsBase.h"

const uint8_t JochemFont58Bitmaps[] PROGMEM = {
	0xFA, 0xB4, 0x00, 0x00, 0x00, 0x5F, 0x55, 0xF5, 
	0x06, 0xF8, 0xF1, 0xF6, 0x00, 0x92, 0x49, 0x00, 
	0x04, 0xA4, 0xBA, 0x50, 0xC0, 0x6A, 0xA4, 0x95, 
	0x58, 0x02, 0xAE, 0xA8, 0x01, 0x2E, 0x90, 0x00, 
	0x06, 0x00, 0x00, 0xF0, 0x00, 0x02, 0x00, 0x12, 
	0x48, 0x00, 0x69, 0x99, 0x99, 0x60, 0xD5, 0x54, 
	0x69, 0x12, 0x48, 0xF0, 0x69, 0x21, 0x19, 0x60, 
	0x35, 0x9F, 0x11, 0x10, 0xF8, 0xE1, 0x19, 0x60, 
	0x78, 0xE9, 0x99, 0x60, 0xF1, 0x24, 0x88, 0x80, 
	0x69, 0x69, 0x99, 0x60, 0x69, 0x99, 0x71, 0xE0, 
	0x24, 0x04, 0x16, 0x12, 0x48, 0x42, 0x10, 0x00, 
	0x71, 0xC0, 0x84, 0x21, 0x24, 0x80, 0x54, 0xA4, 
	0x10, 0x69, 0xBB, 0xB8, 0x60, 0x69, 0xF9, 0x99, 
	0x90, 0xE9, 0xE9, 0x99, 0xE0, 0x69, 0x88, 0x89, 
	0x60, 0xCA, 0x99, 0x9A, 0xC0, 0xF8, 0xE8, 0x88, 
	0xF0, 0xF8, 0xE8, 0x88, 0x80, 0x69, 0x88, 0xB9, 
	0x60, 0x99, 0xF9, 0x99, 0x90, 0xE9, 0x24, 0xB8, 
	0x11, 0x11, 0x99, 0x60, 0x9A, 0xCA, 0x99, 0x90, 
	0x88, 0x88, 0x88, 0xF0, 0x9F, 0xF9, 0x99, 0x90, 
	0x99, 0xDB, 0x99, 0x90, 0x69, 0x99, 0x99, 0x60, 
	0xE9, 0xE8, 0x88, 0x80, 0x69, 0x9B, 0xDB, 0x60, 
	0xE9, 0xEC, 0xA9, 0x90, 0x68, 0x61, 0x19, 0x60, 
	0xE9, 0x24, 0x90, 0x99, 0x99, 0x99, 0x60, 0x99, 
	0x99, 0x66, 0x60, 0x8C, 0x63, 0x5A, 0xD5, 0x40, 
	0x99, 0x66, 0x69, 0x90, 0xB6, 0xD4, 0x90, 0xF1, 
	0x24, 0x88, 0xF0, 0xF2, 0x49, 0x38, 0x00, 0x84, 
	0x21, 0x00, 0xE4, 0x92, 0x78, 0x0A, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x0F, 0xC0, 0x00, 0x61, 0x79, 
	0x70, 0x88, 0xE9, 0x99, 0xE0, 0x00, 0x78, 0x88, 
	0x70, 0x11, 0x79, 0x99, 0x70, 0x00, 0x69, 0xE8, 
	0x70, 0x72, 0x69, 0x20, 0x00, 0x79, 0x97, 0x1E, 
	0x88, 0xE9, 0x99, 0x90, 0xBE, 0x20, 0x92, 0x6A, 
	0x88, 0xAC, 0xCA, 0x90, 0xAA, 0xAC, 0x00, 0xAF, 
	0xD9, 0x90, 0x00, 0xE9, 0x99, 0x90, 0x00, 0x69, 
	0x99, 0x60, 0x00, 0xE9, 0x9E, 0x88, 0x00, 0x79, 
	0x97, 0x11, 0x01, 0xC9, 0x20, 0x00, 0x68, 0x61, 
	0x60, 0x5D, 0x24, 0x88, 0x02, 0xDB, 0x50, 0x00, 
	0x99, 0x66, 0x60, 0x00, 0x15, 0x55, 0xA0, 0x02, 
	0xD5, 0x68, 0x00, 0x99, 0x97, 0x16, 0x00, 0xF2, 
	0x48, 0xF0, 0x34, 0x48, 0x44, 0x30, 0xFE, 0xC2, 
	0x21, 0x22, 0xC0, 0x0A, 0x50, 0x00, 0x00};

const GFXglyph JochemFont58Glyphs[] PROGMEM = {
	{ 0, 0, 8, 1, 0, -7 },  // 0x20 ' '
	{ 0, 1, 8, 2, 0, -7 },  // 0x21 '!'
	{ 1, 3, 8, 4, 0, -7 },  // 0x22 '"'
	{ 4, 4, 8, 5, 0, -7 },  // 0x23 '#'
	{ 8, 4, 8, 5, 0, -7 },  // 0x24 '$'
	{ 12, 4, 8, 5, 0, -7 },  // 0x25 '%'
	{ 16, 4, 8, 5, 0, -7 },  // 0x26 '&'
	{ 20, 1, 8, 2, 0, -7 },  // 0x27 '''
	{ 21, 2, 8, 3, 0, -7 },  // 0x28 '('
	{ 23, 2, 8, 3, 0, -7 },  // 0x29 ')'
	{ 25, 3, 8, 4, 0, -7 },  // 0x2A '*'
	{ 28, 3, 8, 4, 0, -7 },  // 0x2B '+'
	{ 31, 2, 8, 3, 0, -7 },  // 0x2C ','
	{ 33, 4, 8, 5, 0, -7 },  // 0x2D '-'
	{ 37, 1, 8, 2, 0, -7 },  // 0x2E '.'
	{ 38, 4, 8, 5, 0, -7 },  // 0x2F '/'
	{ 42, 4, 8, 5, 0, -7 },  // 0x30 '0'
	{ 46, 2, 8, 3, 0, -7 },  // 0x31 '1'
	{ 48, 4, 8, 5, 0, -7 },  // 0x32 '2'
	{ 52, 4, 8, 5, 0, -7 },  // 0x33 '3'
	{ 56, 4, 8, 5, 0, -7 },  // 0x34 '4'
	{ 60, 4, 8, 5, 0, -7 },  // 0x35 '5'
	{ 64, 4, 8, 5, 0, -7 },  // 0x36 '6'
	{ 68, 4, 8, 5, 0, -7 },  // 0x37 '7'
	{ 72, 4, 8, 5, 0, -7 },  // 0x38 '8'
	{ 76, 4, 8, 5, 0, -7 },  // 0x39 '9'
	{ 80, 1, 8, 2, 0, -7 },  // 0x3A ':'
	{ 81, 2, 8, 3, 0, -7 },  // 0x3B ';'
	{ 83, 4, 8, 5, 0, -7 },  // 0x3C '<'
	{ 87, 3, 8, 4, 0, -7 },  // 0x3D '='
	{ 90, 4, 8, 5, 0, -7 },  // 0x3E '>'
	{ 94, 3, 8, 4, 0, -7 },  // 0x3F '?'
	{ 97, 4, 8, 5, 0, -7 },  // 0x40 '@'
	{ 101, 4, 8, 5, 0, -7 },  // 0x41 'A'
	{ 105, 4, 8, 5, 0, -7 },  // 0x42 'B'
	{ 109, 4, 8, 5, 0, -7 },  // 0x43 'C'
	{ 113, 4, 8, 5, 0, -7 },  // 0x44 'D'
	{ 117, 4, 8, 5, 0, -7 },  // 0x45 'E'
	{ 121, 4, 8, 5, 0, -7 },  // 0x46 'F'
	{ 125, 4, 8, 5, 0, -7 },  // 0x47 'G'
	{ 129, 4, 8, 5, 0, -7 },  // 0x48 'H'
	{ 133, 3, 8, 4, 0, -7 },  // 0x49 'I'
	{ 136, 4, 8, 5, 0, -7 },  // 0x4A 'J'
	{ 140, 4, 8, 5, 0, -7 },  // 0x4B 'K'
	{ 144, 4, 8, 5, 0, -7 },  // 0x4C 'L'
	{ 148, 4, 8, 5, 0, -7 },  // 0x4D 'M'
	{ 152, 4, 8, 5, 0, -7 },  // 0x4E 'N'
	{ 156, 4, 8, 5, 0, -7 },  // 0x4F 'O'
	{ 160, 4, 8, 5, 0, -7 },  // 0x50 'P'
	{ 164, 4, 8, 5, 0, -7 },  // 0x51 'Q'
	{ 168, 4, 8, 5, 0, -7 },  // 0x52 'R'
	{ 172, 4, 8, 5, 0, -7 },  // 0x53 'S'
	{ 176, 3, 8, 4, 0, -7 },  // 0x54 'T'
	{ 179, 4, 8, 5, 0, -7 },  // 0x55 'U'
	{ 183, 4, 8, 5, 0, -7 },  // 0x56 'V'
	{ 187, 5, 8, 6, 0, -7 },  // 0x57 'W'
	{ 192, 4, 8, 5, 0, -7 },  // 0x58 'X'
	{ 196, 3, 8, 4, 0, -7 },  // 0x59 'Y'
	{ 199, 4, 8, 5, 0, -7 },  // 0x5A 'Z'
	{ 203, 3, 8, 4, 0, -7 },  // 0x5B '['
	{ 206, 4, 8, 5, 0, -7 },  // 0x5C '\'
	{ 210, 3, 8, 4, 0, -7 },  // 0x5D ']'
	{ 213, 3, 8, 4, 0, -7 },  // 0x5E '^'
	{ 216, 4, 8, 5, 0, -7 },  // 0x5F '_'
	{ 220, 1, 8, 2, 0, -7 },  // 0x60 '`'
	{ 221, 4, 8, 5, 0, -7 },  // 0x61 'a'
	{ 225, 4, 8, 5, 0, -7 },  // 0x62 'b'
	{ 229, 4, 8, 5, 0, -7 },  // 0x63 'c'
	{ 233, 4, 8, 5, 0, -7 },  // 0x64 'd'
	{ 237, 4, 8, 5, 0, -7 },  // 0x65 'e'
	{ 241, 3, 8, 4, 0, -7 },  // 0x66 'f'
	{ 244, 4, 8, 5, 0, -7 },  // 0x67 'g'
	{ 248, 4, 8, 5, 0, -7 },  // 0x68 'h'
	{ 252, 1, 8, 2, 0, -7 },  // 0x69 'i'
	{ 253, 3, 8, 4, 0, -7 },  // 0x6A 'j'
	{ 256, 4, 8, 5, 0, -7 },  // 0x6B 'k'
	{ 260, 2, 8, 3, 0, -7 },  // 0x6C 'l'
	{ 262, 4, 8, 5, 0, -7 },  // 0x6D 'm'
	{ 266, 4, 8, 5, 0, -7 },  // 0x6E 'n'
	{ 270, 4, 8, 5, 0, -7 },  // 0x6F 'o'
	{ 274, 4, 8, 5, 0, -7 },  // 0x70 'p'
	{ 278, 4, 8, 5, 0, -7 },  // 0x71 'q'
	{ 282, 3, 8, 4, 0, -7 },  // 0x72 'r'
	{ 285, 4, 8, 5, 0, -7 },  // 0x73 's'
	{ 289, 3, 8, 4, 0, -7 },  // 0x74 't'
	{ 292, 3, 8, 4, 0, -7 },  // 0x75 'u'
	{ 295, 4, 8, 5, 0, -7 },  // 0x76 'v'
	{ 299, 4, 8, 5, 0, -7 },  // 0x77 'w'
	{ 303, 3, 8, 4, 0, -7 },  // 0x78 'x'
	{ 306, 4, 8, 5, 0, -7 },  // 0x79 'y'
	{ 310, 4, 8, 5, 0, -7 },  // 0x7A 'z'
	{ 314, 4, 8, 5, 0, -7 },  // 0x7B '{'
	{ 318, 1, 8, 2, 0, -7 },  // 0x7C '|'
	{ 319, 4, 8, 5, 0, -7 },  // 0x7D '}'
	{ 323, 4, 8, 5, 0, -7 }};  // 0x7E '~'

const GFXfont JochemFont58 PROGMEM = {
	(uint8_t*)JochemFont58Bitmaps,
	(GFXglyph*)JochemFont58Glyphs,
	0x20, 0x7E, 8 };

