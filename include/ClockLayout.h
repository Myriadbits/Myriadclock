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
    const char      *text; // Description
    const int16_t   *leds; // Indices of leds to light up
} intro_t;

#define WEND  {-1,-1}

//
// Latest characters:
//
// Hetmispkwartjas●
// Mntweenzevenegen
// yovierzesdertien
// relfdrieveertien
// ipvijfnachtwaalf
// aovervoorxhalf✖✔
// dtweenvierdrielf
// b⚑vijfzesachtien
// izevenegentwaalf
// tqMyriadclockuur
// sMaDiWoDoVrZaZon
// .101121314151617
// c181922023242526
// o27282930♥JanFeb
// mMaaAprMeiJunJul
// ●AugSepOktNovDec
//

// Word definitions, {-1,-1} or WEND terminated (led numbers start at 0)
static const ledpos_t itis      [] = { {0,0}, {1,0}, {2,0}, {4,0}, {5,0}, WEND };
static const ledpos_t minute_1  [] = { {4,1}, {5,1}, {6,1}, WEND };
static const ledpos_t minute_2  [] = { {2,1}, {3,1}, {4,1}, {5,1}, WEND };
static const ledpos_t minute_3  [] = { {4,3}, {5,3}, {6,3}, {7,3}, WEND };
static const ledpos_t minute_4  [] = { {2,2}, {3,2}, {4,2}, {5,2}, WEND };
static const ledpos_t minute_5  [] = { {2,4}, {3,4}, {4, 4}, {5, 4}, WEND };
static const ledpos_t minute_6  [] = { {6,2}, {7,2}, {8,2}, WEND};
static const ledpos_t minute_7  [] = { {7,1}, {8,1}, {9,1}, {10,1}, {11,1}, WEND};
static const ledpos_t minute_8  [] = { {7,4}, {8,4}, {9,4}, {10,4}, WEND};
static const ledpos_t minute_9  [] = { {11,1}, {12,1}, {13,1}, {14,1}, {15,1}, WEND};
static const ledpos_t minute_10 [] = { {12,2}, {13,2}, {14,2}, {15,2}, WEND};
static const ledpos_t minute_11 [] = { {1,3}, {2,3}, {3,3}, WEND};
static const ledpos_t minute_12 [] = { {10,4}, {11,4}, {12,4}, {13,4}, {14,4}, {15,4}, WEND};
static const ledpos_t minute_13 [] = { {9,2}, {10,2}, {11,2}, {12,2}, {13,2}, {14,2}, {15,2}, WEND};
static const ledpos_t minute_14 [] = { {8,3}, {9,3}, {10,3}, {11,3}, {12,3}, {13,3}, {14,3}, {15,3}, WEND};

static const ledpos_t quarter   [] = { {7,0}, {8,0}, {9,0}, {10,0}, {11,0}, WEND};

static const ledpos_t to        [] = { {5,5}, {6,5}, {7,5}, {8,5}, WEND};
static const ledpos_t past      [] = { {1,5}, {2,5}, {3,5}, {4,5}, WEND};

static const ledpos_t half      [] = { {10,5}, {11,5}, {12,5}, {13,5}, WEND};
static const ledpos_t hour_1    [] = { {3,6}, {4,6}, {5,6}, WEND};
static const ledpos_t hour_2    [] = { {1,6}, {2,6}, {3,6}, {4,6}, WEND};
static const ledpos_t hour_3    [] = { {10,6}, {11,6}, {12,6}, {13,6}, WEND};
static const ledpos_t hour_4    [] = { {6,6}, {7,6}, {8,6}, {9,6}, WEND};
static const ledpos_t hour_5    [] = { {2,7}, {3,7}, {4,7}, {5,7}, WEND};
static const ledpos_t hour_6    [] = { {6,7}, {7,7}, {8,7}, WEND};
static const ledpos_t hour_7    [] = { {1,8}, {2,8}, {3,8}, {4,8}, {5,8}, WEND};
static const ledpos_t hour_8    [] = { {9,7}, {10,7}, {11,7}, {12,7}, WEND};
static const ledpos_t hour_9    [] = { {5,8}, {6,8}, {7,8}, {8,8}, {9,8}, WEND};
static const ledpos_t hour_10   [] = { {12,7}, {13,7}, {14,7}, {15,7}, WEND};
static const ledpos_t hour_11   [] = { {13,6}, {14,6}, {15,6}, WEND};
static const ledpos_t hour_12   [] = { {10,8}, {11,8}, {12,8}, {13,8}, {14,8}, {15,8}, WEND};
static const ledpos_t hour_full [] = { {13,9}, {14,9}, {15,9}, WEND};

static const ledpos_t monday    [] = { {1,10}, {2,10}, WEND};
static const ledpos_t tuesday   [] = { {3,10}, {4,10}, WEND};
static const ledpos_t wednesday [] = { {5,10}, {6,10}, WEND};
static const ledpos_t thursday  [] = { {7,10}, {8,10}, WEND};
static const ledpos_t friday    [] = { {9,10}, {10,10}, WEND};
static const ledpos_t saturday  [] = { {11,10}, {12,10}, WEND};
static const ledpos_t sunday    [] = { {13,10}, {14,10}, WEND};

static const ledpos_t day1      [] = { {3,11}, WEND};
static const ledpos_t day2      [] = { {5,11}, WEND};
static const ledpos_t day3      [] = { {7,11}, WEND};
static const ledpos_t day4      [] = { {9,11}, WEND};
static const ledpos_t day5      [] = { {11,11}, WEND};
static const ledpos_t day6      [] = { {13,11}, WEND};
static const ledpos_t day7      [] = { {15,11}, WEND};
static const ledpos_t day8      [] = { {2,12}, WEND};
static const ledpos_t day9      [] = { {4,12}, WEND};
static const ledpos_t day10     [] = { {1,11}, {2,11}, WEND};
static const ledpos_t day11     [] = { {3,11}, {4,11}, WEND};
static const ledpos_t day12     [] = { {4,11}, {5,11}, WEND};
static const ledpos_t day13     [] = { {6,11}, {7,11}, WEND};
static const ledpos_t day14     [] = { {8,11}, {9,11}, WEND};
static const ledpos_t day15     [] = { {10,11}, {11,11}, WEND};
static const ledpos_t day16     [] = { {12,11}, {13,11}, WEND};
static const ledpos_t day17     [] = { {14,11}, {15,11}, WEND};
static const ledpos_t day18     [] = { {1,12}, {2,12}, WEND};
static const ledpos_t day19     [] = { {3,12}, {4,12}, WEND};
static const ledpos_t day20     [] = { {6,12}, {7,12}, WEND};
static const ledpos_t day21     [] = { {5,11}, {6,11}, WEND};
static const ledpos_t day22     [] = { {5,12}, {6,12}, WEND};
static const ledpos_t day23     [] = { {8,12}, {9,12}, WEND};
static const ledpos_t day24     [] = { {10,12}, {11,12}, WEND};
static const ledpos_t day25     [] = { {12,12}, {13,12}, WEND};
static const ledpos_t day26     [] = { {14,12}, {15,12}, WEND};
static const ledpos_t day27     [] = { {1,13}, {2,13}, WEND};
static const ledpos_t day28     [] = { {3,13}, {4,13}, WEND};
static const ledpos_t day29     [] = { {5,13}, {6,13}, WEND};
static const ledpos_t day30     [] = { {7,13}, {8,13}, WEND};
static const ledpos_t day31     [] = { {7,11}, {8,11}, WEND};

static const ledpos_t januari  [] = { {10,13}, {11,13}, {12,13}, WEND};
static const ledpos_t februari [] = { {13,13}, {14,13}, {15,13}, WEND};
static const ledpos_t march    [] = { {1,14}, {2,14}, {3,14}, WEND};
static const ledpos_t april    [] = { {4,14}, {5,14}, {6,14}, WEND};
static const ledpos_t may      [] = { {7,14}, {8,14}, {9,14}, WEND};
static const ledpos_t june     [] = { {10,14}, {11,14}, {12,14}, WEND};
static const ledpos_t july     [] = { {13,14}, {14,14}, {15,14}, WEND};
static const ledpos_t august   [] = { {1,15}, {2,15}, {3,15}, WEND};
static const ledpos_t september[] = { {4,15}, {5,15}, {6,15}, WEND};
static const ledpos_t october  [] = { {7,15}, {8,15}, {9,15}, WEND};
static const ledpos_t november [] = { {10,15}, {11,15}, {12,15}, WEND};
static const ledpos_t december [] = { {13,15}, {14,15}, {15,15}, WEND};

static const ledpos_t myriadbits  [] = { {0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}, {0,11}, {0,12}, {0,13}, {0,14}, WEND};
static const ledpos_t myriadclock  [] = { {2,9}, {3,9}, {4,9}, {5,9}, {6,9}, {7,9}, {8,9}, {9,9}, {10,9}, {11,9}, {12,9}, WEND};
static const ledpos_t nowifi [] = { {1,1}, {1,2}, {3,1}, {3,2}, {3,3}, {3,4}, WEND};

static const ledpos_t ok  [] = { {15,5}, WEND};
static const ledpos_t nok  [] = { {14,5}, WEND};
static const ledpos_t pulse1  [] = { {15,0}, WEND};
static const ledpos_t pulse2  [] = { {0,15}, WEND};
static const ledpos_t heart [] = { {9,13}, WEND};
static const ledpos_t flag [] = { {1,7}, WEND};

// List all words 
static const ledpos_t* const s_wordsTop[] = {
    itis, 
    minute_1, minute_2, minute_3, minute_4, minute_5, minute_6, minute_7, minute_8, minute_9, minute_10, minute_11, minute_12, minute_13, minute_14,
    quarter, to, past, half,
    hour_1, hour_2, hour_3, hour_4, hour_5, hour_6, hour_7, hour_8, hour_9, hour_10, hour_11, hour_12,
    hour_full,        
    NULL };

static const ledpos_t* const s_wordsBottom[] = {   
    monday, tuesday, wednesday, thursday, friday, saturday, sunday,
    day1, day2, day3, day4, day5, day6, day7, day8, day9, day10, day11, day12, day13, day14, day15, day16, day17, day18, day19, day20, 
    day21, day22, day23, day24, day25, day26, day27, day28, day29, day30, day31,
    januari, februari, march, april, may, june, july, august, september, october, november, december,
    myriadbits, myriadclock, nowifi, ok, nok, pulse1, pulse2, heart, flag,
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

