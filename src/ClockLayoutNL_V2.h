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

//
// Latest characters:
//   0123456789012345
// ----------------
// 0|TIENKWARTPARTYW•
// 1|VIJF-VOORLHALFA•
// 2|NOYESOVERZHALFK•
// 3|WIFIBT•PASSCODE•
// 4|VIERTWEEENZESIJM
// 5|VIJFDRIELFBLNQUY
// 6|TIENACHTWAALF ️PR
// 7|ZEVENEGENVUURGFI
// 8|GOEDEMORGENHSTXA
// 9|FEESTVERJAARDAGD
// 0|MADIWODOVRZAZOZC
// 1|101121314151617L
// 2|181922023242526O
// 3|27282930@JANFEBC
// 4|MAAAPRMEIJUNJULK
// 5|AUGSEPOKTNOVDEC•
//


// Word definitions, {-1,-1} or WEND terminated (led numbers start at 0)

static const ledpos_t NLV2_minute_1  [] = { {15,0}, WEND};
static const ledpos_t NLV2_minute_2  [] = { {15,0}, {15,1}, WEND};
static const ledpos_t NLV2_minute_3  [] = { {15,0}, {15,1}, {15,2}, WEND};
static const ledpos_t NLV2_minute_4  [] = { {15,0}, {15,1}, {15,2}, {15,3}, WEND};
static const ledpos_t NLV2_minute_5  [] = { {0,1}, {1,1}, {2,1}, {3,1}, WEND};
static const ledpos_t NLV2_minute_10 [] = { {0,0}, {1,0}, {2,0}, {3,0}, WEND};
static const ledpos_t NLV2_quarter   [] = { {4,0}, {5,0}, {6,0}, {7,0}, {8,0}, WEND};

static const ledpos_t NLV2_pulse     [] = { {15,15}, WEND};

static const ledpos_t NLV2_past      [] = { {5,2}, {6,2}, {7,2}, {8,2}, WEND};
static const ledpos_t NLV2_to        [] = { {5,1}, {6,1}, {7,1}, {8,1}, WEND};
static const ledpos_t NLV2_half1     [] = { {10,1}, {11,1}, {12,1}, {13,1}, WEND};
static const ledpos_t NLV2_half2     [] = { {10,2}, {11,2}, {12,2}, {13,2}, WEND};

static const ledpos_t NLV2_hour_1    [] = { {7,4}, {8,4}, {9,4}, WEND};
static const ledpos_t NLV2_hour_2    [] = { {4,4}, {5,4}, {6,4}, {7,4}, WEND};
static const ledpos_t NLV2_hour_3    [] = { {4,5}, {5,5}, {6,5}, {7,5}, WEND};
static const ledpos_t NLV2_hour_4    [] = { {0,4}, {1,4}, {2,4}, {3,4}, WEND};
static const ledpos_t NLV2_hour_5    [] = { {0,5}, {1,5}, {2,5}, {3,5}, WEND};
static const ledpos_t NLV2_hour_6    [] = { {10,4}, {11,4}, {12,4}, WEND};
static const ledpos_t NLV2_hour_7    [] = { {0,7}, {1,7}, {2,7}, {3,7}, {4,7}, WEND};
static const ledpos_t NLV2_hour_8    [] = { {4,6}, {5,6}, {6,6}, {7,6}, WEND};
static const ledpos_t NLV2_hour_9    [] = { {4,7}, {5,7}, {6,7}, {7,7}, {8,7}, WEND};
static const ledpos_t NLV2_hour_10   [] = { {0,6}, {1,6}, {2,6}, {3,6}, WEND};
static const ledpos_t NLV2_hour_11   [] = { {7,5}, {8,5}, {9,5}, WEND};
static const ledpos_t NLV2_hour_12   [] = { {7,6}, {8,6}, {9,6}, {10,6}, {11,6}, {12,6}, WEND};
static const ledpos_t NLV2_hour_full [] = { {10,7}, {11,7}, {12,7}, WEND};


static const ledpos_t NLV2_monday    [] = { {0,10}, {1,10}, WEND};
static const ledpos_t NLV2_tuesday   [] = { {2,10}, {3,10}, WEND};
static const ledpos_t NLV2_wednesday [] = { {4,10}, {5,10}, WEND};
static const ledpos_t NLV2_thursday  [] = { {6,10}, {7,10}, WEND};
static const ledpos_t NLV2_friday    [] = { {8,10}, {9,10}, WEND};
static const ledpos_t NLV2_saturday  [] = { {10,10}, {11,10}, WEND};
static const ledpos_t NLV2_sunday    [] = { {12,10}, {13,10}, WEND};

static const ledpos_t NLV2_day0      [] = { {1,11}, WEND};
static const ledpos_t NLV2_day1      [] = { {2,11}, WEND};
static const ledpos_t NLV2_day2      [] = { {4,11}, WEND};
static const ledpos_t NLV2_day3      [] = { {6,11}, WEND};
static const ledpos_t NLV2_day4      [] = { {8,11}, WEND};
static const ledpos_t NLV2_day5      [] = { {10,11}, WEND};
static const ledpos_t NLV2_day6      [] = { {12,11}, WEND};
static const ledpos_t NLV2_day7      [] = { {14,11}, WEND};
static const ledpos_t NLV2_day8      [] = { {1,12}, WEND};
static const ledpos_t NLV2_day9      [] = { {3,12}, WEND};
static const ledpos_t NLV2_day10     [] = { {0,11}, {1,11}, WEND};
static const ledpos_t NLV2_day11     [] = { {2,11}, {3,11}, WEND};
static const ledpos_t NLV2_day12     [] = { {3,11}, {4,11}, WEND};
static const ledpos_t NLV2_day13     [] = { {5,11}, {6,11}, WEND};
static const ledpos_t NLV2_day14     [] = { {7,11}, {8,11}, WEND};
static const ledpos_t NLV2_day15     [] = { {9,11}, {10,11}, WEND};
static const ledpos_t NLV2_day16     [] = { {11,11}, {12,11}, WEND};
static const ledpos_t NLV2_day17     [] = { {13,11}, {14,11}, WEND};
static const ledpos_t NLV2_day18     [] = { {0,12}, {1,12}, WEND};
static const ledpos_t NLV2_day19     [] = { {2,12}, {3,12}, WEND};
static const ledpos_t NLV2_day20     [] = { {5,12}, {6,12}, WEND};
static const ledpos_t NLV2_day21     [] = { {4,11}, {5,11}, WEND};
static const ledpos_t NLV2_day22     [] = { {4,12}, {5,12}, WEND};
static const ledpos_t NLV2_day23     [] = { {7,12}, {8,12}, WEND};
static const ledpos_t NLV2_day24     [] = { {9,12}, {10,12}, WEND};
static const ledpos_t NLV2_day25     [] = { {11,12}, {12,12}, WEND};
static const ledpos_t NLV2_day26     [] = { {13,12}, {14,12}, WEND};
static const ledpos_t NLV2_day27     [] = { {0,13}, {1,13}, WEND};
static const ledpos_t NLV2_day28     [] = { {2,13}, {3,13}, WEND};
static const ledpos_t NLV2_day29     [] = { {4,13}, {5,13}, WEND};
static const ledpos_t NLV2_day30     [] = { {6,13}, {7,13}, WEND};
static const ledpos_t NLV2_day31     [] = { {6,11}, {7,11}, WEND};

static const ledpos_t NLV2_januari  [] = { {9,13}, {10,13}, {11,13}, WEND};
static const ledpos_t NLV2_februari [] = { {12,13}, {13,13}, {14,13}, WEND};
static const ledpos_t NLV2_march    [] = { {0,14}, {1,14}, {2,14}, WEND};
static const ledpos_t NLV2_april    [] = { {3,14}, {4,14}, {5,14}, WEND};
static const ledpos_t NLV2_may      [] = { {6,14}, {7,14}, {8,14}, WEND};
static const ledpos_t NLV2_june     [] = { {9,14}, {10,14}, {11,14}, WEND};
static const ledpos_t NLV2_july     [] = { {12,14}, {13,14}, {14,14}, WEND};
static const ledpos_t NLV2_august   [] = { {0,15}, {1,15}, {2,15}, WEND};
static const ledpos_t NLV2_september[] = { {3,15}, {4,15}, {5,15}, WEND};
static const ledpos_t NLV2_october  [] = { {6,15}, {7,15}, {8,15}, WEND};
static const ledpos_t NLV2_november [] = { {9,15}, {10,15}, {11,15}, WEND};
static const ledpos_t NLV2_december [] = { {12,15}, {13,15}, {14,15}, WEND};



static const ledpos_t NLV2_myriadclock  [] = { {15,4}, {15,5}, {15,6}, {15,7}, {15,8}, {15,9}, {15,10}, {15,11}, {15,12}, {15,13}, {15,14}, WEND};
static const ledpos_t NLV2_bt [] = { {4,3}, {5,3}, WEND};
static const ledpos_t NLV2_passcode [] = { {7,3}, {8,3}, {9,3}, {10,3}, {11,3}, {12,3}, {13,3}, {14,3}, WEND};
static const ledpos_t NLV2_no  [] = { {0,2}, {1,2}, WEND};
static const ledpos_t NLV2_yes  [] = { {2,2}, {3,2}, {4,2}, WEND};
static const ledpos_t NLV2_wifi  [] = { {0,3}, {1,3}, {2,3}, {3,3}, WEND};

static const ledpos_t NLV2_birthday [] = { {5,9}, {6,9}, {7,9}, {8,9}, {9,9}, {10,9}, {11,9}, {12,9}, {13,9}, {14,9}, WEND};
static const ledpos_t NLV2_holiday [] = { {0,9}, {1,9}, {2,9}, {3,9}, {4,9}, {12,9}, {13,9}, {14,9}, WEND};
static const ledpos_t NLV2_party [] = { {9,0}, {10,0}, {11,0}, {12,0}, {13,0}, WEND};
static const ledpos_t NLV2_wakeup [] = { {14,0}, {14,1}, {14,2}, {14,3}, {14,5}, {14,6}, WEND};
static const ledpos_t NLV2_goodmorning [] = { {0,8}, {1,8}, {2,8}, {3,8}, {4,8}, {5,8}, {6,8}, {7,8}, {8,8}, {9,8}, {10,8}, WEND};

//
// New NL clock layout
//
static const ledclocklayout_t s_layoutNL_V2 = 
{
    16,
    16,
    256,
    O_NORMAL,
    false,

    // Time display method
    TF_NL_5MIN,

    // Time parts
    {
        NLV2_pulse, EMPTY, NLV2_minute_1, NLV2_minute_2, NLV2_minute_3, NLV2_minute_4, NLV2_minute_5, EMPTY, EMPTY, EMPTY, EMPTY, NLV2_minute_10,
        EMPTY, EMPTY, EMPTY, EMPTY, NLV2_quarter, EMPTY, EMPTY, 
        NLV2_past, NLV2_to, NLV2_past, NLV2_to, NLV2_past, NLV2_to, NLV2_past, NLV2_to, NLV2_past, NLV2_to, 
        NLV2_half1, NLV2_half2, NLV2_hour_full
    },

    // 12x Hours
    {
        NLV2_hour_12, NLV2_hour_1, NLV2_hour_2, NLV2_hour_3, NLV2_hour_4, NLV2_hour_5, NLV2_hour_6, NLV2_hour_7, NLV2_hour_8, NLV2_hour_9, NLV2_hour_10, NLV2_hour_11         
    },

    // 7x Day of the week
    {
        NLV2_sunday, NLV2_monday, NLV2_tuesday, NLV2_wednesday, NLV2_thursday, NLV2_friday, NLV2_saturday
    },

    // 31x Day number
    {
        NLV2_day1, NLV2_day2, NLV2_day3, NLV2_day4, NLV2_day5, NLV2_day6, NLV2_day7, NLV2_day8, NLV2_day9, NLV2_day10, 
        NLV2_day11, NLV2_day12, NLV2_day13, NLV2_day14, NLV2_day15, NLV2_day16, NLV2_day17, NLV2_day18, NLV2_day19, NLV2_day20, 
        NLV2_day21, NLV2_day22, NLV2_day23, NLV2_day24, NLV2_day25, NLV2_day26, NLV2_day27, NLV2_day28, NLV2_day29, NLV2_day30, 
        NLV2_day31
    },

    // 12x Month
    {
        NLV2_januari, NLV2_februari, NLV2_march, NLV2_april, NLV2_may, NLV2_june, NLV2_july, NLV2_august, NLV2_september, NLV2_october, NLV2_november, NLV2_december
    },

    // Extra parts
    {
        NLV2_no,
        NLV2_yes,
        NLV2_wifi,
        NLV2_myriadclock,
        EMPTY,
        EMPTY,
        NLV2_bt,
        NLV2_passcode,
        NLV2_goodmorning,
        NLV2_birthday,
        NLV2_holiday,
        NLV2_party,
        NLV2_wakeup,
        EMPTY,
        EMPTY,
        EMPTY,
        EMPTY,
    },

    // 10x number
    {
        NLV2_day0, NLV2_day1, NLV2_day2, NLV2_day3, NLV2_day4, NLV2_day5, NLV2_day6, NLV2_day7, NLV2_day8, NLV2_day9
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

