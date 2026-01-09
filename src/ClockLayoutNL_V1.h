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

#include "ClockLayoutBase.h"

// How many leds are connected?
#define NUM_LEDS    256
#define NUM_ROWS    16
#define NUM_COLS    16

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
static const ledpos_t NLV1_itis      [] = { {0,0}, {1,0}, {2,0}, {4,0}, {5,0}, WEND };
static const ledpos_t NLV1_minute_1  [] = { {4,1}, {5,1}, {6,1}, WEND };
static const ledpos_t NLV1_minute_2  [] = { {2,1}, {3,1}, {4,1}, {5,1}, WEND };
static const ledpos_t NLV1_minute_3  [] = { {4,3}, {5,3}, {6,3}, {7,3}, WEND };
static const ledpos_t NLV1_minute_4  [] = { {2,2}, {3,2}, {4,2}, {5,2}, WEND };
static const ledpos_t NLV1_minute_5  [] = { {2,4}, {3,4}, {4, 4}, {5, 4}, WEND };
static const ledpos_t NLV1_minute_6  [] = { {6,2}, {7,2}, {8,2}, WEND};
static const ledpos_t NLV1_minute_7  [] = { {7,1}, {8,1}, {9,1}, {10,1}, {11,1}, WEND};
static const ledpos_t NLV1_minute_8  [] = { {7,4}, {8,4}, {9,4}, {10,4}, WEND};
static const ledpos_t NLV1_minute_9  [] = { {11,1}, {12,1}, {13,1}, {14,1}, {15,1}, WEND};
static const ledpos_t NLV1_minute_10 [] = { {12,2}, {13,2}, {14,2}, {15,2}, WEND};
static const ledpos_t NLV1_minute_11 [] = { {1,3}, {2,3}, {3,3}, WEND};
static const ledpos_t NLV1_minute_12 [] = { {10,4}, {11,4}, {12,4}, {13,4}, {14,4}, {15,4}, WEND};
static const ledpos_t NLV1_minute_13 [] = { {9,2}, {10,2}, {11,2}, {12,2}, {13,2}, {14,2}, {15,2}, WEND};
static const ledpos_t NLV1_minute_14 [] = { {8,3}, {9,3}, {10,3}, {11,3}, {12,3}, {13,3}, {14,3}, {15,3}, WEND};

static const ledpos_t NLV1_quarter   [] = { {7,0}, {8,0}, {9,0}, {10,0}, {11,0}, WEND};

static const ledpos_t NLV1_to        [] = { {5,5}, {6,5}, {7,5}, {8,5}, WEND};
static const ledpos_t NLV1_past      [] = { {1,5}, {2,5}, {3,5}, {4,5}, WEND};

static const ledpos_t NLV1_half      [] = { {10,5}, {11,5}, {12,5}, {13,5}, WEND};
static const ledpos_t NLV1_hour_1    [] = { {3,6}, {4,6}, {5,6}, WEND};
static const ledpos_t NLV1_hour_2    [] = { {1,6}, {2,6}, {3,6}, {4,6}, WEND};
static const ledpos_t NLV1_hour_3    [] = { {10,6}, {11,6}, {12,6}, {13,6}, WEND};
static const ledpos_t NLV1_hour_4    [] = { {6,6}, {7,6}, {8,6}, {9,6}, WEND};
static const ledpos_t NLV1_hour_5    [] = { {2,7}, {3,7}, {4,7}, {5,7}, WEND};
static const ledpos_t NLV1_hour_6    [] = { {6,7}, {7,7}, {8,7}, WEND};
static const ledpos_t NLV1_hour_7    [] = { {1,8}, {2,8}, {3,8}, {4,8}, {5,8}, WEND};
static const ledpos_t NLV1_hour_8    [] = { {9,7}, {10,7}, {11,7}, {12,7}, WEND};
static const ledpos_t NLV1_hour_9    [] = { {5,8}, {6,8}, {7,8}, {8,8}, {9,8}, WEND};
static const ledpos_t NLV1_hour_10   [] = { {12,7}, {13,7}, {14,7}, {15,7}, WEND};
static const ledpos_t NLV1_hour_11   [] = { {13,6}, {14,6}, {15,6}, WEND};
static const ledpos_t NLV1_hour_12   [] = { {10,8}, {11,8}, {12,8}, {13,8}, {14,8}, {15,8}, WEND};
static const ledpos_t NLV1_hour_full [] = { {13,9}, {14,9}, {15,9}, WEND};

static const ledpos_t NLV1_pulse1    [] = { {15,0}, WEND};
static const ledpos_t NLV1_pulse2    [] = { {0,15}, WEND};



static const ledpos_t NLV1_monday    [] = { {1,10}, {2,10}, WEND};
static const ledpos_t NLV1_tuesday   [] = { {3,10}, {4,10}, WEND};
static const ledpos_t NLV1_wednesday [] = { {5,10}, {6,10}, WEND};
static const ledpos_t NLV1_thursday  [] = { {7,10}, {8,10}, WEND};
static const ledpos_t NLV1_friday    [] = { {9,10}, {10,10}, WEND};
static const ledpos_t NLV1_saturday  [] = { {11,10}, {12,10}, WEND};
static const ledpos_t NLV1_sunday    [] = { {13,10}, {14,10}, WEND};

static const ledpos_t NLV1_day0      [] = { {2,11}, WEND};
static const ledpos_t NLV1_day1      [] = { {3,11}, WEND};
static const ledpos_t NLV1_day2      [] = { {5,11}, WEND};
static const ledpos_t NLV1_day3      [] = { {7,11}, WEND};
static const ledpos_t NLV1_day4      [] = { {9,11}, WEND};
static const ledpos_t NLV1_day5      [] = { {11,11}, WEND};
static const ledpos_t NLV1_day6      [] = { {13,11}, WEND};
static const ledpos_t NLV1_day7      [] = { {15,11}, WEND};
static const ledpos_t NLV1_day8      [] = { {2,12}, WEND};
static const ledpos_t NLV1_day9      [] = { {4,12}, WEND};
static const ledpos_t NLV1_day10     [] = { {1,11}, {2,11}, WEND};
static const ledpos_t NLV1_day11     [] = { {3,11}, {4,11}, WEND};
static const ledpos_t NLV1_day12     [] = { {4,11}, {5,11}, WEND};
static const ledpos_t NLV1_day13     [] = { {6,11}, {7,11}, WEND};
static const ledpos_t NLV1_day14     [] = { {8,11}, {9,11}, WEND};
static const ledpos_t NLV1_day15     [] = { {10,11}, {11,11}, WEND};
static const ledpos_t NLV1_day16     [] = { {12,11}, {13,11}, WEND};
static const ledpos_t NLV1_day17     [] = { {14,11}, {15,11}, WEND};
static const ledpos_t NLV1_day18     [] = { {1,12}, {2,12}, WEND};
static const ledpos_t NLV1_day19     [] = { {3,12}, {4,12}, WEND};
static const ledpos_t NLV1_day20     [] = { {6,12}, {7,12}, WEND};
static const ledpos_t NLV1_day21     [] = { {5,11}, {6,11}, WEND};
static const ledpos_t NLV1_day22     [] = { {5,12}, {6,12}, WEND};
static const ledpos_t NLV1_day23     [] = { {8,12}, {9,12}, WEND};
static const ledpos_t NLV1_day24     [] = { {10,12}, {11,12}, WEND};
static const ledpos_t NLV1_day25     [] = { {12,12}, {13,12}, WEND};
static const ledpos_t NLV1_day26     [] = { {14,12}, {15,12}, WEND};
static const ledpos_t NLV1_day27     [] = { {1,13}, {2,13}, WEND};
static const ledpos_t NLV1_day28     [] = { {3,13}, {4,13}, WEND};
static const ledpos_t NLV1_day29     [] = { {5,13}, {6,13}, WEND};
static const ledpos_t NLV1_day30     [] = { {7,13}, {8,13}, WEND};
static const ledpos_t NLV1_day31     [] = { {7,11}, {8,11}, WEND};

static const ledpos_t NLV1_januari  [] = { {10,13}, {11,13}, {12,13}, WEND};
static const ledpos_t NLV1_februari [] = { {13,13}, {14,13}, {15,13}, WEND};
static const ledpos_t NLV1_march    [] = { {1,14}, {2,14}, {3,14}, WEND};
static const ledpos_t NLV1_april    [] = { {4,14}, {5,14}, {6,14}, WEND};
static const ledpos_t NLV1_may      [] = { {7,14}, {8,14}, {9,14}, WEND};
static const ledpos_t NLV1_june     [] = { {10,14}, {11,14}, {12,14}, WEND};
static const ledpos_t NLV1_july     [] = { {13,14}, {14,14}, {15,14}, WEND};
static const ledpos_t NLV1_august   [] = { {1,15}, {2,15}, {3,15}, WEND};
static const ledpos_t NLV1_september[] = { {4,15}, {5,15}, {6,15}, WEND};
static const ledpos_t NLV1_october  [] = { {7,15}, {8,15}, {9,15}, WEND};
static const ledpos_t NLV1_november [] = { {10,15}, {11,15}, {12,15}, WEND};
static const ledpos_t NLV1_december [] = { {13,15}, {14,15}, {15,15}, WEND};


static const ledpos_t NLV1_myriadbits  [] = { {0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}, {0,11}, {0,12}, {0,13}, {0,14}, WEND};
static const ledpos_t NLV1_myriadclock  [] = { {2,9}, {3,9}, {4,9}, {5,9}, {6,9}, {7,9}, {8,9}, {9,9}, {10,9}, {11,9}, {12,9}, WEND};
static const ledpos_t NLV1_no [] = { {1,1}, {1,2}, WEND};
static const ledpos_t NLV1_wifi [] = { {3,1}, {3,2}, {3,3}, {3,4}, WEND};

static const ledpos_t NLV1_ok  [] = { {15,5}, WEND};
static const ledpos_t NLV1_nok  [] = { {14,5}, WEND};

static const ledpos_t NLV1_heart [] = { {9,13}, WEND};
static const ledpos_t NLV1_flag [] = { {1,7}, WEND};

//
// Complete clock definition for old NL variant
//
static const ledclocklayout_t s_layoutNL_V1 = 
{
    // Time display method
    TF_NL_EVERYMIN,

    // Time parts
    {
        NLV1_pulse2, NLV1_itis, NLV1_minute_1, NLV1_minute_2, NLV1_minute_3, NLV1_minute_4, NLV1_minute_5, NLV1_minute_6, NLV1_minute_7, NLV1_minute_8, NLV1_minute_9, NLV1_minute_10,
        NLV1_minute_11, NLV1_minute_12, NLV1_minute_13, NLV1_minute_14, NLV1_quarter, EMPTY, EMPTY, 
        NLV1_past, NLV1_to, NLV1_past, NLV1_to, NLV1_past, NLV1_to, NLV1_past, NLV1_to, NLV1_past, NLV1_to, 
        NLV1_half, NLV1_half, NLV1_hour_full
    },

    // 12x Hours
    {
        NLV1_hour_12, NLV1_hour_1, NLV1_hour_2, NLV1_hour_3, NLV1_hour_4, NLV1_hour_5, NLV1_hour_6, NLV1_hour_7, NLV1_hour_8, NLV1_hour_9, NLV1_hour_10, NLV1_hour_11       
    },

    // 7x Day of the week
    {
        NLV1_sunday, NLV1_monday, NLV1_tuesday, NLV1_wednesday, NLV1_thursday, NLV1_friday, NLV1_saturday
    },

    // 31x Day number
    {
        NLV1_day1, NLV1_day2, NLV1_day3, NLV1_day4, NLV1_day5, NLV1_day6, NLV1_day7, NLV1_day8, NLV1_day9, NLV1_day10, 
        NLV1_day11, NLV1_day12, NLV1_day13, NLV1_day14, NLV1_day15, NLV1_day16, NLV1_day17, NLV1_day18, NLV1_day19, NLV1_day20, 
        NLV1_day21, NLV1_day22, NLV1_day23, NLV1_day24, NLV1_day25, NLV1_day26, NLV1_day27, NLV1_day28, NLV1_day29, NLV1_day30, NLV1_day31
    },        

    // 12x Month
    {
        NLV1_januari, NLV1_februari, NLV1_march, NLV1_april, NLV1_may, NLV1_june, NLV1_july, NLV1_august, NLV1_september, NLV1_october, NLV1_november, NLV1_december
    },

    // Extra parts
    {
        NLV1_no,
        NLV1_nok,
        NLV1_wifi,
        NLV1_myriadclock,
        EMPTY,
        NLV1_myriadbits,
        EMPTY,
        EMPTY,
        EMPTY,
        NLV1_flag,
        NLV1_flag,
        NLV1_flag,
        EMPTY,
        EMPTY,
        EMPTY,
        EMPTY,
    },

    // 10x number
    {
        NLV1_day0, NLV1_day1, NLV1_day2, NLV1_day3, NLV1_day4, NLV1_day5, NLV1_day6, NLV1_day7, NLV1_day8, NLV1_day9
    }
};


// static const ledpos_t code1     [] = { {1,11}, {2,11}, {3, 11}, WEND};
// static const ledpos_t code2     [] = { {2,11}, {3,11}, {4, 11}, WEND};
// static const ledpos_t code3     [] = { {3,11}, {4,11}, {5, 11}, WEND};
// static const ledpos_t code4     [] = { {4,11}, {5,11}, {6, 11}, WEND};
// static const ledpos_t code5     [] = { {5,11}, {6,11}, {7, 11}, WEND};
// static const ledpos_t code6     [] = { {6,11}, {7,11}, {8, 11}, WEND};
// static const ledpos_t code7     [] = { {7,11}, {8,11}, {9, 11}, WEND};
// static const ledpos_t code8     [] = { {8,11}, {9,11}, {10, 11}, WEND};
// static const ledpos_t code9     [] = { {9,11}, {10,11}, {11, 11}, WEND};
// static const ledpos_t code10    [] = { {10,11}, {11,11}, {12, 11}, WEND};
// static const ledpos_t code11    [] = { {11,11}, {12,11}, {13, 11}, WEND};
// static const ledpos_t code12    [] = { {12,11}, {13,11}, {14, 11}, WEND};
// static const ledpos_t code13    [] = { {13,11}, {14,11}, {15, 11}, WEND};
// static const ledpos_t code14    [] = { {1,12}, {2,12}, {3, 12}, WEND};
// static const ledpos_t code15    [] = { {2,12}, {3,12}, {4, 12}, WEND};
// static const ledpos_t code16    [] = { {3,12}, {4,12}, {5, 12}, WEND};
// static const ledpos_t code17    [] = { {4,12}, {5,12}, {6, 12}, WEND};
// static const ledpos_t code18    [] = { {5,12}, {6,12}, {7, 12}, WEND};
// static const ledpos_t code19    [] = { {6,12}, {7,12}, {8, 12}, WEND};
// static const ledpos_t code20    [] = { {7,12}, {8,12}, {9, 12}, WEND};
// static const ledpos_t code21    [] = { {8,12}, {9,12}, {10, 12}, WEND};
// static const ledpos_t code22    [] = { {9,12}, {10,12}, {11, 12}, WEND};
// static const ledpos_t code23    [] = { {10,12}, {11,12}, {12, 12}, WEND};
// static const ledpos_t code24    [] = { {11,12}, {12,12}, {13, 12}, WEND};
// static const ledpos_t code25    [] = { {12,12}, {13,12}, {14, 12}, WEND};
// static const ledpos_t code26    [] = { {13,12}, {14,12}, {15, 12}, WEND};
// static const ledpos_t code27    [] = { {1,13}, {2,13}, {3, 13}, WEND};
// static const ledpos_t code28    [] = { {2,13}, {3,13}, {4, 13}, WEND};
// static const ledpos_t code29    [] = { {3,13}, {4,13}, {5, 13}, WEND};
// static const ledpos_t code30    [] = { {4,13}, {5,13}, {6, 13}, WEND};
// static const ledpos_t code31    [] = { {5,13}, {6,13}, {7, 13}, WEND};
// static const ledpos_t code32    [] = { {6,13}, {7,13}, {8, 13}, WEND};

// static const ledtexts_t s_wordCodes[] = {
//     {code1, "101"}, {code2, "011"}, {code3, "112"}, {code4, "121"}, {code5, "213"}, {code6, "131"}, {code7, "314"}, {code8, "141"}, {code9, "415"}, {code10, "151"}, {code11, "516"}, {code12, "161"}, {code13, "617"},
//     {code14, "181"}, {code15, "819"}, {code16, "192"}, {code17, "922"}, {code18, "220"}, {code19, "202"}, {code20, "023"}, {code21, "232"}, {code22, "324"}, {code23, "242"}, {code24, "425"}, {code25, "252"}, {code26, "526"},
//     {code27, "272"}, {code28, "728"}, {code29, "282"}, {code30, "829"}, {code31, "293"}, {code32, "930"},
//     NULL };

