///////////////////////////////////////////////////////////////////////////////
//
// Definition of the myriadclock display layout
//
// COPYRIGHT NOTICE                                                           
// ================
// Copyright (c) 2024, Jochem Bakker <Jochem@myriadbits.com>
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
// Clocklayout 3 includes full weekdays + month-names
//
// Latest characters:
//   0123456789012345
// ----------------
// 0|VIJFTIENKWARTBT•
// 1|WFXVOOROVERYHALF
// 2|VIERZESZEVENEGEN
// 3|VIJFDRIELFTWAALF
// 4|ACHTIENTWEENZUUR
// 5|MAANDAGDONDERDAG
// 6|WOENSDAGZATERDAG
// 7|DINSDAGVRIJDAGZN
// 8|ZONDAGZMYRIADCLO
// 9|3101130Δ1415161C
// 0|212232425262728K
// 1|099QAPRILMEIJUNI
// 2|SEPTEMBERJANUARI
// 3|MAARTOKTOBERJULI
// 4|AUGUSTUSFEBRUARI
// 5|NOVEMBERDECEMBER
//


// Word definitions, {-1,-1} or WEND terminated (led numbers start at 0)

static const ledpos_t NLV3_minute_1  [] = { WEND};
static const ledpos_t NLV3_minute_2  [] = { WEND};
static const ledpos_t NLV3_minute_3  [] = { WEND};
static const ledpos_t NLV3_minute_4  [] = { WEND};
static const ledpos_t NLV3_minute_5  [] = { {0,0}, {1,0}, {2,0}, {3,0}, WEND};
static const ledpos_t NLV3_minute_10 [] = { {4,0}, {5,0}, {6,0}, {7,0}, WEND};
static const ledpos_t NLV3_quarter   [] = { {8,0}, {9,0}, {10,0}, {11,0}, {12,0}, WEND};

static const ledpos_t NLV3_pulse     [] = { {15,0}, WEND};

static const ledpos_t NLV3_past      [] = { {7,1}, {8,1}, {9,1}, {10,1}, WEND};
static const ledpos_t NLV3_to        [] = { {3,1}, {4,1}, {5,1}, {6,1}, WEND};
static const ledpos_t NLV3_half1     [] = { {12,1}, {13,1}, {14,1}, {15,1}, WEND};
static const ledpos_t NLV3_half2     [] = { {12,1}, {13,1}, {14,1}, {15,1}, WEND};

static const ledpos_t NLV3_hour_1    [] = { {9,4}, {10,4}, {11,4}, WEND};
static const ledpos_t NLV3_hour_2    [] = { {7,4}, {8,4}, {9,4}, {10,4}, WEND};
static const ledpos_t NLV3_hour_3    [] = { {4,3}, {5,3}, {6,3}, {7,3}, WEND};
static const ledpos_t NLV3_hour_4    [] = { {0,2}, {1,2}, {2,2}, {3,2}, WEND};
static const ledpos_t NLV3_hour_5    [] = { {0,3}, {1,3}, {2,3}, {3,3}, WEND};
static const ledpos_t NLV3_hour_6    [] = { {4,2}, {5,2}, {6,2}, WEND};
static const ledpos_t NLV3_hour_7    [] = { {7,2}, {8,2}, {9,2}, {10,2}, {11,2}, WEND};
static const ledpos_t NLV3_hour_8    [] = { {0,4}, {1,4}, {2,4}, {3,4}, WEND};
static const ledpos_t NLV3_hour_9    [] = { {11,2}, {12,2}, {13,2}, {14,2}, {15,2}, WEND};
static const ledpos_t NLV3_hour_10   [] = { {3,4}, {4,4}, {5,4}, {6,4}, WEND};
static const ledpos_t NLV3_hour_11   [] = { {7,3}, {8,3}, {9,3}, WEND};
static const ledpos_t NLV3_hour_12   [] = { {10,3}, {11,3}, {12,3}, {13,3}, {14,3}, {15,3}, WEND};
static const ledpos_t NLV3_hour_full [] = { {13,4}, {14,4}, {15,4}, WEND};


static const ledpos_t NLV3_monday    [] = { {0,5}, {1,5}, {2,5}, {3,5}, {4,5}, {5,5}, {6,5}, WEND};
static const ledpos_t NLV3_tuesday   [] = { {0,7}, {1,7}, {2,7}, {3,7}, {4,7}, {5,7}, {6,7},  WEND};
static const ledpos_t NLV3_wednesday [] = { {0,6}, {1,6}, {2,6}, {3,6}, {4,6}, {5,6}, {6,6}, {7,6}, WEND};
static const ledpos_t NLV3_thursday  [] = { {7,5}, {8,5}, {9,5}, {10,5}, {11,5}, {12,5}, {13,5}, {14,5}, {15,5},WEND};
static const ledpos_t NLV3_friday    [] = { {7,7}, {8,7}, {9,7}, {10,7}, {11,7}, {12,7}, {13,7}, WEND};
static const ledpos_t NLV3_saturday  [] = { {8,6}, {9,6}, {10,6}, {11,6}, {12,6}, {13,6}, {14,6}, {15,6}, WEND};
static const ledpos_t NLV3_sunday    [] = { {0,8}, {1,8}, {2,8}, {3,8}, {4,8}, {5,8}, WEND};

static const ledpos_t NLV3_day0      [] = { {2,9}, WEND};
static const ledpos_t NLV3_day1      [] = { {1,9}, WEND};
static const ledpos_t NLV3_day2      [] = { {2,10}, WEND};
static const ledpos_t NLV3_day3      [] = { {4,10}, WEND};
static const ledpos_t NLV3_day4      [] = { {6,10}, WEND};
static const ledpos_t NLV3_day5      [] = { {8,10}, WEND};
static const ledpos_t NLV3_day6      [] = { {10,10}, WEND};
static const ledpos_t NLV3_day7      [] = { {12,10}, WEND};
static const ledpos_t NLV3_day8      [] = { {14,10}, WEND};
static const ledpos_t NLV3_day9      [] = { {1,11}, WEND};
static const ledpos_t NLV3_day10     [] = { {1,9}, {2,9}, WEND};
static const ledpos_t NLV3_day11     [] = { {3,9}, {4,9}, WEND};
static const ledpos_t NLV3_day12     [] = { {1,10}, {2,10}, WEND};
static const ledpos_t NLV3_day13     [] = { {4,9}, {5,9}, WEND};
static const ledpos_t NLV3_day14     [] = { {8,9}, {9,9}, WEND};
static const ledpos_t NLV3_day15     [] = { {10,9}, {11,9}, WEND};
static const ledpos_t NLV3_day16     [] = { {12,9}, {13,9}, WEND};
static const ledpos_t NLV3_day17     [] = { {12,9}, {12,10}, WEND};// Vert
static const ledpos_t NLV3_day18     [] = { {14,9}, {14,10}, WEND};// Vert
static const ledpos_t NLV3_day19     [] = { {1,10}, {1,11}, WEND};// Vert
static const ledpos_t NLV3_day20     [] = { {0,10}, {0,11}, WEND};// Vert
static const ledpos_t NLV3_day21     [] = { {0,10}, {1,10}, WEND};
static const ledpos_t NLV3_day22     [] = { {2,10}, {3,10}, WEND};
static const ledpos_t NLV3_day23     [] = { {3,10}, {4,10}, WEND};
static const ledpos_t NLV3_day24     [] = { {5,10}, {6,10}, WEND};
static const ledpos_t NLV3_day25     [] = { {7,10}, {8,10}, WEND};
static const ledpos_t NLV3_day26     [] = { {9,10}, {10,10}, WEND};
static const ledpos_t NLV3_day27     [] = { {11,10}, {12,10}, WEND};
static const ledpos_t NLV3_day28     [] = { {13,10}, {14,10}, WEND}; 
static const ledpos_t NLV3_day29     [] = { {2,10}, {2,11}, WEND};// Vert
static const ledpos_t NLV3_day30     [] = { {5,9}, {6,9}, WEND};
static const ledpos_t NLV3_day31     [] = { {0,9}, {1,9}, WEND};

static const ledpos_t NLV3_januari  [] = { {9,12}, {10,12}, {11,12}, {12,12}, {13,12}, {14,12}, {15,12}, WEND};
static const ledpos_t NLV3_februari [] = { {8,14}, {9,14}, {10,14}, {11,14}, {12,14}, {13,14}, {14,14}, {15,14}, WEND};
static const ledpos_t NLV3_march    [] = { {0,13}, {1,13}, {2,13}, {3,13}, {4,13},WEND};
static const ledpos_t NLV3_april    [] = { {4,11}, {5,11}, {6,11}, {7,11}, {8,11},WEND};
static const ledpos_t NLV3_may      [] = { {9,11}, {10,11}, {11,11}, WEND};
static const ledpos_t NLV3_june     [] = { {12,11}, {13,11}, {14,11}, {15,11},WEND};
static const ledpos_t NLV3_july     [] = { {12,13}, {13,13}, {14,13}, {15,13},WEND};
static const ledpos_t NLV3_august   [] = { {0,14}, {1,14}, {2,14}, {3,14}, {4,14}, {5,14}, {6,14}, {7,14}, WEND};
static const ledpos_t NLV3_september[] = { {0,12}, {1,12}, {2,12}, {3,12}, {4,12}, {5,12}, {6,12}, {7,12}, {8,12}, WEND};
static const ledpos_t NLV3_october  [] = { {5,13}, {6,13}, {7,13}, {8,13}, {9,13}, {10,13}, {11,13}, WEND};
static const ledpos_t NLV3_november [] = { {0,15}, {1,15}, {2,15}, {3,15}, {4,15}, {5,15}, {6,15}, {7,15}, WEND};
static const ledpos_t NLV3_december [] = { {8,15}, {9,15}, {10,15}, {11,15}, {12,15}, {13,15}, {14,15}, {15,15},WEND};


static const ledpos_t NLV3_myriadclock  [] = { {7,8}, {8,8}, {9,8}, {10,8}, {11,8}, {12,8}, {13,8}, {14,8}, {15,8}, {15,9}, {15,10}, WEND};
static const ledpos_t NLV3_bt [] = { {13,0}, {14,0}, WEND};
static const ledpos_t NLV3_passcode [] = { {13,8}, {15,8}, {8,15}, {9,15}, WEND};
static const ledpos_t NLV3_no  [] = { {7,0}, {7,1}, WEND};
static const ledpos_t NLV3_yes  [] = { {9,12}, {10,12}, WEND};
static const ledpos_t NLV3_wifi  [] = { {0,1}, {1,1}, WEND};

static const ledpos_t NLV3_birthday [] = { WEND};
static const ledpos_t NLV3_holiday [] = { WEND};
static const ledpos_t NLV3_party [] = { WEND};
static const ledpos_t NLV3_wakeup [] = { WEND};
static const ledpos_t NLV3_goodmorning [] = { WEND};

//
// New NL clock layout
//
static const ledclocklayout_t s_layoutNL_V3 = 
{
    // Time display method
    TF_NL_5MIN,

    // Time parts
    {
        NLV3_pulse, EMPTY, NLV3_minute_1, NLV3_minute_2, NLV3_minute_3, NLV3_minute_4, NLV3_minute_5, EMPTY, EMPTY, EMPTY, EMPTY, NLV3_minute_10,
        EMPTY, EMPTY, EMPTY, EMPTY, NLV3_quarter, EMPTY, EMPTY, 
        NLV3_past, NLV3_to, NLV3_past, NLV3_to, NLV3_past, NLV3_to, NLV3_past, NLV3_to, NLV3_past, NLV3_to, 
        NLV3_half1, NLV3_half2, NLV3_hour_full
    },

    // 12x Hours
    {
        NLV3_hour_12, NLV3_hour_1, NLV3_hour_2, NLV3_hour_3, NLV3_hour_4, NLV3_hour_5, NLV3_hour_6, NLV3_hour_7, NLV3_hour_8, NLV3_hour_9, NLV3_hour_10, NLV3_hour_11         
    },

    // 7x Day of the week
    {
        NLV3_sunday, NLV3_monday, NLV3_tuesday, NLV3_wednesday, NLV3_thursday, NLV3_friday, NLV3_saturday
    },

    // 31x Day number
    {
        NLV3_day1, NLV3_day2, NLV3_day3, NLV3_day4, NLV3_day5, NLV3_day6, NLV3_day7, NLV3_day8, NLV3_day9, NLV3_day10, 
        NLV3_day11, NLV3_day12, NLV3_day13, NLV3_day14, NLV3_day15, NLV3_day16, NLV3_day17, NLV3_day18, NLV3_day19, NLV3_day20, 
        NLV3_day21, NLV3_day22, NLV3_day23, NLV3_day24, NLV3_day25, NLV3_day26, NLV3_day27, NLV3_day28, NLV3_day29, NLV3_day30, 
        NLV3_day31
    },

    // 12x Month
    {
        NLV3_januari, NLV3_februari, NLV3_march, NLV3_april, NLV3_may, NLV3_june, NLV3_july, NLV3_august, NLV3_september, NLV3_october, NLV3_november, NLV3_december
    },

    // Extra parts
    {
        NLV3_no,
        NLV3_yes,
        NLV3_wifi,
        NLV3_myriadclock,
        EMPTY,
        NLV3_bt,
        NLV3_passcode,
        NLV3_goodmorning,
        NLV3_birthday,
        NLV3_holiday,
        NLV3_party,
        NLV3_wakeup,
        EMPTY,
    },

    // 10x number
    {
        NLV3_day0, NLV3_day1, NLV3_day2, NLV3_day3, NLV3_day4, NLV3_day5, NLV3_day6, NLV3_day7, NLV3_day8, NLV3_day9
    }
};


// static const ledpos_t code1     [] = { {0,11}, {1,11}, {2, 11}, WEND};
// static const ledpos_t code2     [] = { {1,11}, {2,11}, {3, 11}, WEND};
// static const ledpos_t code3     [] = { {2,11}, {3,11}, {4, 11}, WEND};
// static const ledpos_t code4     [] = { {3,11}, {4,11}, {5, 11}, WEND};
// static const ledpos_t code5     [] = { {4,11}, {5,11}, {6, 11}, WEND};
// static const ledpos_t code6     [] = { {5,11}, {6,11}, {7, 11}, WEND};
// static const ledpos_t code7     [] = { {6,11}, {7,11}, {8, 11}, WEND};
// static const ledpos_t code8     [] = { {7,11}, {8,11}, {9, 11}, WEND};
// static const ledpos_t code9     [] = { {8,11}, {9,11}, {10, 11}, WEND};
// static const ledpos_t code10    [] = { {9,11}, {10,11}, {11, 11}, WEND};
// static const ledpos_t code11    [] = { {10,11}, {11,11}, {12, 11}, WEND};
// static const ledpos_t code12    [] = { {11,11}, {12,11}, {13, 11}, WEND};
// static const ledpos_t code13    [] = { {12,11}, {13,11}, {14, 11}, WEND};
// static const ledpos_t code14    [] = { {0,12}, {1,12}, {2, 12}, WEND};
// static const ledpos_t code15    [] = { {1,12}, {2,12}, {3, 12}, WEND};
// static const ledpos_t code16    [] = { {2,12}, {3,12}, {4, 12}, WEND};
// static const ledpos_t code17    [] = { {3,12}, {4,12}, {5, 12}, WEND};
// static const ledpos_t code18    [] = { {4,12}, {5,12}, {6, 12}, WEND};
// static const ledpos_t code19    [] = { {5,12}, {6,12}, {7, 12}, WEND};
// static const ledpos_t code20    [] = { {6,12}, {7,12}, {8, 12}, WEND};
// static const ledpos_t code21    [] = { {7,12}, {8,12}, {9, 12}, WEND};
// static const ledpos_t code22    [] = { {8,12}, {9,12}, {10, 12}, WEND};
// static const ledpos_t code23    [] = { {9,12}, {10,12}, {11, 12}, WEND};
// static const ledpos_t code24    [] = { {10,12}, {11,12}, {12, 12}, WEND};
// static const ledpos_t code25    [] = { {11,12}, {12,12}, {13, 12}, WEND};
// static const ledpos_t code26    [] = { {12,12}, {13,12}, {14, 12}, WEND};
// static const ledpos_t code27    [] = { {0,13}, {1,13}, {2, 13}, WEND};
// static const ledpos_t code28    [] = { {1,13}, {2,13}, {3, 13}, WEND};
// static const ledpos_t code29    [] = { {2,13}, {3,13}, {4, 13}, WEND};
// static const ledpos_t code30    [] = { {3,13}, {4,13}, {5, 13}, WEND};
// static const ledpos_t code31    [] = { {4,13}, {5,13}, {6, 13}, WEND};
// static const ledpos_t code32    [] = { {5,13}, {6,13}, {7, 13}, WEND};

// // List all words 

// static const ledtexts_t s_wordCodes[] = {
//     {code1, "101"}, {code2, "011"}, {code3, "112"}, {code4, "121"}, {code5, "213"}, {code6, "131"}, {code7, "314"}, {code8, "141"}, {code9, "415"}, {code10, "151"}, {code11, "516"}, {code12, "161"}, {code13, "617"},
//     {code14, "181"}, {code15, "819"}, {code16, "192"}, {code17, "922"}, {code18, "220"}, {code19, "202"}, {code20, "023"}, {code21, "232"}, {code22, "324"}, {code23, "242"}, {code24, "425"}, {code25, "252"}, {code26, "526"},
//     {code27, "272"}, {code28, "728"}, {code29, "282"}, {code30, "829"}, {code31, "293"}, {code32, "930"},
//     NULL };

