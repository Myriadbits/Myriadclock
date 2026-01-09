///////////////////////////////////////////////////////////////////////////////
//
// Definition of the myriadclock display layout
//
// COPYRIGHT NOTICE                                                           
// ================
// Copyright (c) 2026, Jochem Bakker <Jochem@myriadbits.com>
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
//   0123456789012345
// ----------------
// 0|TIENKWARTPARTYG•
// 1|VIJF-VOORLHALFO•
// 2|NOYESOVERZHALFE•
// 3|WIFIBTONTBIJTED•
// 4|VIERTWEEENZESIEC
// 5|VIJFDRIELFCODEMW
// 6|TIENACHTWAALFLOO
// 7|ZEVENEGENVUURARR
// 8|FEESTVERJAARDAGD
// 9|LUNCHETENSTIJDEC
// 0|MADIWODOVRZAZONL
// 1|101121314151617O
// 2|181922023242526X
// 3|27282930@JANFEBE
// 4|MAAAPRMEIJUNJULL
// 5|AUGSEPOKTNOVDEC•
//


// Word definitions, {-1,-1} or WEND terminated (led numbers start at 0)

static const ledpos_t NLV3_minute_1  [] = { {15,0}, WEND};
static const ledpos_t NLV3_minute_2  [] = { {15,0}, {15,1}, WEND};
static const ledpos_t NLV3_minute_3  [] = { {15,0}, {15,1}, {15,2}, WEND};
static const ledpos_t NLV3_minute_4  [] = { {15,0}, {15,1}, {15,2}, {15,3}, WEND};
static const ledpos_t NLV3_minute_5  [] = { {0,1}, {1,1}, {2,1}, {3,1}, WEND};
static const ledpos_t NLV3_minute_10 [] = { {0,0}, {1,0}, {2,0}, {3,0}, WEND};
static const ledpos_t NLV3_quarter   [] = { {4,0}, {5,0}, {6,0}, {7,0}, {8,0}, WEND};

static const ledpos_t NLV3_pulse     [] = { {15,15}, WEND};

static const ledpos_t NLV3_past      [] = { {5,2}, {6,2}, {7,2}, {8,2}, WEND};
static const ledpos_t NLV3_to        [] = { {5,1}, {6,1}, {7,1}, {8,1}, WEND};
static const ledpos_t NLV3_half1     [] = { {10,1}, {11,1}, {12,1}, {13,1}, WEND};
static const ledpos_t NLV3_half2     [] = { {10,2}, {11,2}, {12,2}, {13,2}, WEND};

static const ledpos_t NLV3_hour_1    [] = { {7,4}, {8,4}, {9,4}, WEND};
static const ledpos_t NLV3_hour_2    [] = { {4,4}, {5,4}, {6,4}, {7,4}, WEND};
static const ledpos_t NLV3_hour_3    [] = { {4,5}, {5,5}, {6,5}, {7,5}, WEND};
static const ledpos_t NLV3_hour_4    [] = { {0,4}, {1,4}, {2,4}, {3,4}, WEND};
static const ledpos_t NLV3_hour_5    [] = { {0,5}, {1,5}, {2,5}, {3,5}, WEND};
static const ledpos_t NLV3_hour_6    [] = { {10,4}, {11,4}, {12,4}, WEND};
static const ledpos_t NLV3_hour_7    [] = { {0,7}, {1,7}, {2,7}, {3,7}, {4,7}, WEND};
static const ledpos_t NLV3_hour_8    [] = { {4,6}, {5,6}, {6,6}, {7,6}, WEND};
static const ledpos_t NLV3_hour_9    [] = { {4,7}, {5,7}, {6,7}, {7,7}, {8,7}, WEND};
static const ledpos_t NLV3_hour_10   [] = { {0,6}, {1,6}, {2,6}, {3,6}, WEND};
static const ledpos_t NLV3_hour_11   [] = { {7,5}, {8,5}, {9,5}, WEND};
static const ledpos_t NLV3_hour_12   [] = { {7,6}, {8,6}, {9,6}, {10,6}, {11,6}, {12,6}, WEND};
static const ledpos_t NLV3_hour_full [] = { {10,7}, {11,7}, {12,7}, WEND};


static const ledpos_t NLV3_monday    [] = { {0,10}, {1,10}, WEND};
static const ledpos_t NLV3_tuesday   [] = { {2,10}, {3,10}, WEND};
static const ledpos_t NLV3_wednesday [] = { {4,10}, {5,10}, WEND};
static const ledpos_t NLV3_thursday  [] = { {6,10}, {7,10}, WEND};
static const ledpos_t NLV3_friday    [] = { {8,10}, {9,10}, WEND};
static const ledpos_t NLV3_saturday  [] = { {10,10}, {11,10}, WEND};
static const ledpos_t NLV3_sunday    [] = { {12,10}, {13,10}, {14,10}, WEND};

static const ledpos_t NLV3_day0      [] = { {1,11}, WEND};
static const ledpos_t NLV3_day1      [] = { {2,11}, WEND};
static const ledpos_t NLV3_day2      [] = { {4,11}, WEND};
static const ledpos_t NLV3_day3      [] = { {6,11}, WEND};
static const ledpos_t NLV3_day4      [] = { {8,11}, WEND};
static const ledpos_t NLV3_day5      [] = { {10,11}, WEND};
static const ledpos_t NLV3_day6      [] = { {12,11}, WEND};
static const ledpos_t NLV3_day7      [] = { {14,11}, WEND};
static const ledpos_t NLV3_day8      [] = { {1,12}, WEND};
static const ledpos_t NLV3_day9      [] = { {3,12}, WEND};
static const ledpos_t NLV3_day10     [] = { {0,11}, {1,11}, WEND};
static const ledpos_t NLV3_day11     [] = { {2,11}, {3,11}, WEND};
static const ledpos_t NLV3_day12     [] = { {3,11}, {4,11}, WEND};
static const ledpos_t NLV3_day13     [] = { {5,11}, {6,11}, WEND};
static const ledpos_t NLV3_day14     [] = { {7,11}, {8,11}, WEND};
static const ledpos_t NLV3_day15     [] = { {9,11}, {10,11}, WEND};
static const ledpos_t NLV3_day16     [] = { {11,11}, {12,11}, WEND};
static const ledpos_t NLV3_day17     [] = { {13,11}, {14,11}, WEND};
static const ledpos_t NLV3_day18     [] = { {0,12}, {1,12}, WEND};
static const ledpos_t NLV3_day19     [] = { {2,12}, {3,12}, WEND};
static const ledpos_t NLV3_day20     [] = { {5,12}, {6,12}, WEND};
static const ledpos_t NLV3_day21     [] = { {4,11}, {5,11}, WEND};
static const ledpos_t NLV3_day22     [] = { {4,12}, {5,12}, WEND};
static const ledpos_t NLV3_day23     [] = { {7,12}, {8,12}, WEND};
static const ledpos_t NLV3_day24     [] = { {9,12}, {10,12}, WEND};
static const ledpos_t NLV3_day25     [] = { {11,12}, {12,12}, WEND};
static const ledpos_t NLV3_day26     [] = { {13,12}, {14,12}, WEND};
static const ledpos_t NLV3_day27     [] = { {0,13}, {1,13}, WEND};
static const ledpos_t NLV3_day28     [] = { {2,13}, {3,13}, WEND};
static const ledpos_t NLV3_day29     [] = { {4,13}, {5,13}, WEND};
static const ledpos_t NLV3_day30     [] = { {6,13}, {7,13}, WEND};
static const ledpos_t NLV3_day31     [] = { {6,11}, {7,11}, WEND};

static const ledpos_t NLV3_januari  [] = { {9,13}, {10,13}, {11,13}, WEND};
static const ledpos_t NLV3_februari [] = { {12,13}, {13,13}, {14,13}, WEND};
static const ledpos_t NLV3_march    [] = { {0,14}, {1,14}, {2,14}, WEND};
static const ledpos_t NLV3_april    [] = { {3,14}, {4,14}, {5,14}, WEND};
static const ledpos_t NLV3_may      [] = { {6,14}, {7,14}, {8,14}, WEND};
static const ledpos_t NLV3_june     [] = { {9,14}, {10,14}, {11,14}, WEND};
static const ledpos_t NLV3_july     [] = { {12,14}, {13,14}, {14,14}, WEND};
static const ledpos_t NLV3_august   [] = { {0,15}, {1,15}, {2,15}, WEND};
static const ledpos_t NLV3_september[] = { {3,15}, {4,15}, {5,15}, WEND};
static const ledpos_t NLV3_october  [] = { {6,15}, {7,15}, {8,15}, WEND};
static const ledpos_t NLV3_november [] = { {9,15}, {10,15}, {11,15}, WEND};
static const ledpos_t NLV3_december [] = { {12,15}, {13,15}, {14,15}, WEND};

static const ledpos_t NLV3_cloxel  [] = { {15,9}, {15,10}, {15,11}, {15,12}, {15,13}, {15,14}, WEND};
static const ledpos_t NLV3_word  [] = { {15,5}, {15,6}, {15,7}, {15,8}, WEND};
static const ledpos_t NLV3_bt [] = { {4,3}, {5,3}, WEND};
static const ledpos_t NLV3_passcode [] = { {10,5}, {11,5}, {12,5}, {13,5}, {14,5}, WEND};
static const ledpos_t NLV3_no  [] = { {0,2}, {1,2}, WEND};
static const ledpos_t NLV3_yes  [] = { {2,2}, {3,2}, {4,2}, WEND};
static const ledpos_t NLV3_wifi  [] = { {0,3}, {1,3}, {2,3}, {3,3}, WEND};

static const ledpos_t NLV3_birthday [] = { {5,8}, {6,8}, {7,8}, {8,8}, {9,8}, {10,8}, {11,8}, {12,8}, {13,8}, {14,8}, WEND};
static const ledpos_t NLV3_holiday [] = { {0,8}, {1,8}, {2,8}, {3,8}, {4,8}, {12,8}, {13,8}, {14,8}, WEND};
static const ledpos_t NLV3_party [] = { {9,0}, {10,0}, {11,0}, {12,0}, {13,0}, WEND};
static const ledpos_t NLV3_wakeup [] = { {14,0}, {14,1}, {14,2}, {14,3}, {14,5}, {14,6}, WEND};
static const ledpos_t NLV3_goodmorning [] = { {14,0}, {14,1}, {14,2}, {14,3}, {14,4}, {14,5}, {14,6}, {14,7}, {14,8}, {14,9}, {14,10}, WEND};

static const ledpos_t NLV3_breakfast [] = { {6,3}, {7,3}, {8,3}, {9,3}, {10,3}, {11,3}, {12,3}, WEND};
static const ledpos_t NLV3_lunch [] = { {0,9}, {1,9}, {2,9}, {3,9}, {4,9}, {10,9}, {11,9}, {12,9}, {13,9}, WEND};
static const ledpos_t NLV3_dinner [] = { {5,9}, {6,9}, {7,9}, {8,9}, {9,9}, {10,9}, {11,9}, {12,9}, {13,9}, WEND};

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
        NLV3_cloxel,
        NLV3_word,
        EMPTY,
        NLV3_bt,
        NLV3_passcode,
        NLV3_goodmorning,
        NLV3_birthday,
        NLV3_holiday,
        NLV3_party,
        NLV3_wakeup,
        NLV3_breakfast,
        NLV3_lunch,
        NLV3_dinner,
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

