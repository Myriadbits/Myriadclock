///////////////////////////////////////////////////////////////////////////////
//
// Type definitions of the myriadclock display layout
//
// COPYRIGHT NOTICE                                                           
// ================
// Copyright (c) 2020, Jochem Bakker <Jochem@myriadbits.com>
//
// Published under the Internet Systems Consortium (ISC) license:
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
// IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef NULL
#define NULL    (void *)0
#endif

#define WEND  {-1,-1}

typedef struct {
    int8_t   x; // X-Pos or Column
    int8_t   y; // Y-Pos or Row
} ledpos_t;


typedef struct {
    const ledpos_t  *leds; // Indices of leds to light up
    const char      *text; // Description
} ledtexts_t;

enum ETimeFormat
{
    TF_NL_5MIN,      // Dutch, 5, 10, quarter, to, past + 4 separate minute dots. i.e. "vijf voor half tien" with 2 dots indicating actual "3 voor half 10"
    TF_NL_EVERYMIN,  // Dutch, Every minute a single word, one, two, three, etc. i.e. "dertien voor half tien"
    TF_EN_5MIN       // English, 5, 10, quarter, 20, 25 to/past "twenty-five past ten"
};

// Definitions for times text
typedef struct {
    const ETimeFormat timeFormat;
    const ledpos_t *second;
    const ledpos_t *leadtext; 
    const ledpos_t *minute_1;  
    const ledpos_t *minute_2;
    const ledpos_t *minute_3;
    const ledpos_t *minute_4;
    const ledpos_t *minute_5;
    const ledpos_t *minute_6;
    const ledpos_t *minute_7;
    const ledpos_t *minute_8;
    const ledpos_t *minute_9;
    const ledpos_t *minute_10;
    const ledpos_t *minute_11;
    const ledpos_t *minute_12;
    const ledpos_t *minute_13;
    const ledpos_t *minute_14;
    const ledpos_t *quarter;
    const ledpos_t *minute_20;
    const ledpos_t *minute_25;
    const ledpos_t *past;
    const ledpos_t *to;
    const ledpos_t *half;
    const ledpos_t *hour_full;    
    const ledpos_t *hours[12];
} ledtime_t;

// Definitions for day numbers
typedef struct {
    const ledpos_t *weekdays[7];
    const ledpos_t *days[31];
    const ledpos_t *months[12];
} leddate_t;

// Definitions for extra words
typedef struct {
    const ledpos_t *no;
    const ledpos_t *wifi;
    const ledpos_t *myriadclock;
    const ledpos_t *myriadbits;
    const ledpos_t *bluetooth;
    const ledpos_t *passcode;
    const ledpos_t *weddingday;
    const ledpos_t *birthday;
    const ledpos_t *holiday;
    const ledpos_t *party;
} ledextra_t;

// empty led position
static const ledpos_t EMPTY[] = { WEND};

