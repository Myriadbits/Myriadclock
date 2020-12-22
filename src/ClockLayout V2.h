///////////////////////////////////////////////////////////////////////////////
//
// Definition of the myriadclock display layout
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

// How many leds are connected?
#define NUM_LEDS    256
#define NUM_ROWS    16
#define NUM_COLS    16

typedef struct {
    int8_t   x; // X-Pos or Column
    int8_t   y; // Y-Pos or Row
} ledpos_t;


typedef struct {
    const ledpos_t  *leds; // Indices of leds to light up
    const char      *text; // Description
} ledtexts_t;

#define WEND  {-1,-1}

//
// Latest characters:
//   0123456789012345
// ----------------
// 0|KWARTIEN0VIJFPC•
// 1|OVERVOOR1HALFAO•
// 2|DRIETWEE2EEN3SD•
// 3|VIJFVIER4ZES5SE•
// 4|TIENACHT6ELF789M
// 5|ZEVENEGENTWAALFY
// 6|TROUWDAGBUURENAR
// 7|VERJAARDAGABCDEI
// 8|FEESTDAG←→FGHIJA
// 9|KLMNOPQRSTUVWXYD
// 0|MADIWODOVRZAZOZC
// 1|101121314151617L
// 2|181922023242526O
// 3|27282930©JANFEBC
// 4|MAAAPRMEIJUNJULK
// 5|AUGSEPOKTNOVDEC•
//

// Word definitions, {-1,-1} or WEND terminated (led numbers start at 0)


static const ledpos_t minute_1  [] = { {15,0}, WEND};
static const ledpos_t minute_2  [] = { {15,0}, {15,1}, WEND};
static const ledpos_t minute_3  [] = { {15,0}, {15,1}, {15,2}, WEND};
static const ledpos_t minute_4  [] = { {15,0}, {15,1}, {15,2}, {15,3}, WEND};

static const ledpos_t minute_5  [] = { {9,0}, {10,0}, {11,0}, {12,0}, WEND};
static const ledpos_t minute_10 [] = { {4,0}, {5,0}, {6,0}, {7,0}, WEND};
static const ledpos_t minute_15 [] = { {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, WEND};

static const ledpos_t pulse1    [] = { {15,15}, WEND};

static const ledpos_t past      [] = { {0,1}, {1,1}, {2,1}, {3,1}, WEND};
static const ledpos_t to        [] = { {4,1}, {5,1}, {6,1}, {7,1}, WEND};
static const ledpos_t half      [] = { {9,1}, {10,1}, {11,1}, {12,1}, WEND};

static const ledpos_t hour_1    [] = { {9,2}, {10,2}, {11,2}, WEND};
static const ledpos_t hour_2    [] = { {4,2}, {5,2}, {6,2}, {7,2}, WEND};
static const ledpos_t hour_3    [] = { {0,2}, {1,2}, {2,2}, {3,2}, WEND};
static const ledpos_t hour_4    [] = { {4,3}, {5,3}, {6,3}, {7,3}, WEND};
static const ledpos_t hour_5    [] = { {0,3}, {1,3}, {2,3}, {3,3}, WEND};
static const ledpos_t hour_6    [] = { {9,3}, {10,3}, {11,3}, WEND};
static const ledpos_t hour_7    [] = { {0,5}, {1,5}, {2,5}, {3,5}, {4,5}, WEND};
static const ledpos_t hour_8    [] = { {4,4}, {5,4}, {6,4}, {7,4}, WEND};
static const ledpos_t hour_9    [] = { {4,5}, {5,5}, {6,5}, {7,5}, {8,5}, WEND};
static const ledpos_t hour_10   [] = { {0,4}, {1,4}, {2,4}, {3,4}, WEND};
static const ledpos_t hour_11   [] = { {9,4}, {10,4}, {11,4}, WEND};
static const ledpos_t hour_12   [] = { {9,5}, {10,5}, {11,5}, {12,5}, {13,5}, {14,5}, WEND};
static const ledpos_t hour_full [] = { {9,6}, {10,6}, {11,6}, WEND};

static const ledpos_t monday    [] = { {0,10}, {1,10}, WEND};
static const ledpos_t tuesday   [] = { {2,10}, {3,10}, WEND};
static const ledpos_t wednesday [] = { {4,10}, {5,10}, WEND};
static const ledpos_t thursday  [] = { {6,10}, {7,10}, WEND};
static const ledpos_t friday    [] = { {8,10}, {9,10}, WEND};
static const ledpos_t saturday  [] = { {10,10}, {11,10}, WEND};
static const ledpos_t sunday    [] = { {12,10}, {13,10}, WEND};

static const ledpos_t day1      [] = { {2,11}, WEND};
static const ledpos_t day2      [] = { {4,11}, WEND};
static const ledpos_t day3      [] = { {6,11}, WEND};
static const ledpos_t day4      [] = { {8,11}, WEND};
static const ledpos_t day5      [] = { {10,11}, WEND};
static const ledpos_t day6      [] = { {12,11}, WEND};
static const ledpos_t day7      [] = { {14,11}, WEND};
static const ledpos_t day8      [] = { {1,12}, WEND};
static const ledpos_t day9      [] = { {3,12}, WEND};
static const ledpos_t day10     [] = { {0,11}, {1,11}, WEND};
static const ledpos_t day11     [] = { {2,11}, {3,11}, WEND};
static const ledpos_t day12     [] = { {3,11}, {4,11}, WEND};
static const ledpos_t day13     [] = { {5,11}, {6,11}, WEND};
static const ledpos_t day14     [] = { {7,11}, {8,11}, WEND};
static const ledpos_t day15     [] = { {9,11}, {10,11}, WEND};
static const ledpos_t day16     [] = { {11,11}, {12,11}, WEND};
static const ledpos_t day17     [] = { {13,11}, {14,11}, WEND};
static const ledpos_t day18     [] = { {0,12}, {1,12}, WEND};
static const ledpos_t day19     [] = { {2,12}, {3,12}, WEND};
static const ledpos_t day20     [] = { {5,12}, {6,12}, WEND};
static const ledpos_t day21     [] = { {4,11}, {5,11}, WEND};
static const ledpos_t day22     [] = { {4,12}, {5,12}, WEND};
static const ledpos_t day23     [] = { {7,12}, {8,12}, WEND};
static const ledpos_t day24     [] = { {9,12}, {10,12}, WEND};
static const ledpos_t day25     [] = { {11,12}, {12,12}, WEND};
static const ledpos_t day26     [] = { {13,12}, {14,12}, WEND};
static const ledpos_t day27     [] = { {0,13}, {1,13}, WEND};
static const ledpos_t day28     [] = { {2,13}, {3,13}, WEND};
static const ledpos_t day29     [] = { {4,13}, {5,13}, WEND};
static const ledpos_t day30     [] = { {6,13}, {7,13}, WEND};
static const ledpos_t day31     [] = { {6,11}, {7,11}, WEND};

static const ledpos_t januari  [] = { {9,13}, {10,13}, {11,13}, WEND};
static const ledpos_t februari [] = { {12,13}, {13,13}, {14,13}, WEND};
static const ledpos_t march    [] = { {0,14}, {1,14}, {2,14}, WEND};
static const ledpos_t april    [] = { {3,14}, {4,14}, {5,14}, WEND};
static const ledpos_t may      [] = { {6,14}, {7,14}, {8,14}, WEND};
static const ledpos_t june     [] = { {9,14}, {10,14}, {11,14}, WEND};
static const ledpos_t july     [] = { {12,14}, {13,14}, {14,14}, WEND};
static const ledpos_t august   [] = { {0,15}, {1,15}, {2,15}, WEND};
static const ledpos_t september[] = { {3,15}, {4,15}, {5,15}, WEND};
static const ledpos_t october  [] = { {6,15}, {7,15}, {8,15}, WEND};
static const ledpos_t november [] = { {9,15}, {10,15}, {11,15}, WEND};
static const ledpos_t december [] = { {12,15}, {13,15}, {14,15}, WEND};

static const ledpos_t myriadclock  [] = { {15,4}, {14,5}, {15,6}, {15,7}, {15,8}, {15,9}, {15,10}, {15,11}, {15,12}, {15,13}, {15,14}, WEND};
static const ledpos_t passcode [] = { {13,0}, {13,1}, {13,2}, {13,3}, {14,0}, {14,1}, {14,2}, {14,3}, WEND};

static const ledpos_t ok  [] = { {15,5}, WEND};
static const ledpos_t nok  [] = { {14,5}, WEND};


static const ledpos_t code1     [] = { {0,11}, {1,11}, {2, 11}, WEND};
static const ledpos_t code2     [] = { {1,11}, {2,11}, {3, 11}, WEND};
static const ledpos_t code3     [] = { {2,11}, {3,11}, {4, 11}, WEND};
static const ledpos_t code4     [] = { {3,11}, {4,11}, {5, 11}, WEND};
static const ledpos_t code5     [] = { {4,11}, {5,11}, {6, 11}, WEND};
static const ledpos_t code6     [] = { {5,11}, {6,11}, {7, 11}, WEND};
static const ledpos_t code7     [] = { {6,11}, {7,11}, {8, 11}, WEND};
static const ledpos_t code8     [] = { {7,11}, {8,11}, {9, 11}, WEND};
static const ledpos_t code9     [] = { {8,11}, {9,11}, {10, 11}, WEND};
static const ledpos_t code10    [] = { {9,11}, {10,11}, {11, 11}, WEND};
static const ledpos_t code11    [] = { {10,11}, {11,11}, {12, 11}, WEND};
static const ledpos_t code12    [] = { {11,11}, {12,11}, {13, 11}, WEND};
static const ledpos_t code13    [] = { {12,11}, {13,11}, {14, 11}, WEND};
static const ledpos_t code14    [] = { {0,12}, {1,12}, {2, 12}, WEND};
static const ledpos_t code15    [] = { {1,12}, {2,12}, {3, 12}, WEND};
static const ledpos_t code16    [] = { {2,12}, {3,12}, {4, 12}, WEND};
static const ledpos_t code17    [] = { {3,12}, {4,12}, {5, 12}, WEND};
static const ledpos_t code18    [] = { {4,12}, {5,12}, {6, 12}, WEND};
static const ledpos_t code19    [] = { {5,12}, {6,12}, {7, 12}, WEND};
static const ledpos_t code20    [] = { {6,12}, {7,12}, {8, 12}, WEND};
static const ledpos_t code21    [] = { {7,12}, {8,12}, {9, 12}, WEND};
static const ledpos_t code22    [] = { {8,12}, {9,12}, {10, 12}, WEND};
static const ledpos_t code23    [] = { {9,12}, {10,12}, {11, 12}, WEND};
static const ledpos_t code24    [] = { {10,12}, {11,12}, {12, 12}, WEND};
static const ledpos_t code25    [] = { {11,12}, {12,12}, {13, 12}, WEND};
static const ledpos_t code26    [] = { {12,12}, {13,12}, {14, 12}, WEND};
static const ledpos_t code27    [] = { {0,13}, {1,13}, {2, 13}, WEND};
static const ledpos_t code28    [] = { {1,13}, {2,13}, {3, 13}, WEND};
static const ledpos_t code29    [] = { {2,13}, {3,13}, {4, 13}, WEND};
static const ledpos_t code30    [] = { {3,13}, {4,13}, {5, 13}, WEND};
static const ledpos_t code31    [] = { {4,13}, {5,13}, {6, 13}, WEND};
static const ledpos_t code32    [] = { {5,13}, {6,13}, {7, 13}, WEND};

// List all words 
static const ledpos_t* const s_wordsTop[] = {
    minute_1, minute_2, minute_3, minute_4, minute_5, minute_10, minute_15,
    to, past, half,
    hour_1, hour_2, hour_3, hour_4, hour_5, hour_6, hour_7, hour_8, hour_9, hour_10, hour_11, hour_12,
    hour_full,        
    NULL };

static const ledpos_t* const s_wordsBottom[] = {   
    monday, tuesday, wednesday, thursday, friday, saturday, sunday,
    day1, day2, day3, day4, day5, day6, day7, day8, day9, day10, day11, day12, day13, day14, day15, day16, day17, day18, day19, day20, 
    day21, day22, day23, day24, day25, day26, day27, day28, day29, day30, day31,
    januari, februari, march, april, may, june, july, august, september, october, november, december,
    myriadclock, passcode, pulse1,
    NULL };

static const ledpos_t* const s_wordsWeekDays[] = {
    sunday, monday, tuesday, wednesday, thursday, friday, saturday, // Day 0 = sunday
    NULL };

static const ledpos_t* const s_wordsMonthDays[] = {
    day1, day2, day3, day4, day5, day6, day7, day8, day9, day10, day11, day12, day13, day14, day15, day16, day17, day18, day19, day20, 
    day21, day22, day23, day24, day25, day26, day27, day28, day29, day30, day31, 
    NULL };

static const ledpos_t* const s_wordMonths[] = {
    januari, februari, march, april, may, june, july, august, september, october, november, december,
    NULL };

static const ledtexts_t s_wordCodes[] = {
    {code1, "101"}, {code2, "011"}, {code3, "112"}, {code4, "121"}, {code5, "213"}, {code6, "131"}, {code7, "314"}, {code8, "141"}, {code9, "415"}, {code10, "151"}, {code11, "516"}, {code12, "161"}, {code13, "617"},
    {code14, "181"}, {code15, "819"}, {code16, "192"}, {code17, "922"}, {code18, "220"}, {code19, "202"}, {code20, "023"}, {code21, "232"}, {code22, "324"}, {code23, "242"}, {code24, "425"}, {code25, "252"}, {code26, "526"},
    {code27, "272"}, {code28, "728"}, {code29, "282"}, {code30, "829"}, {code31, "293"}, {code32, "930"},
    NULL };

