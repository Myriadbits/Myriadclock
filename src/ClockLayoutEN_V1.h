///////////////////////////////////////////////////////////////////////////////
//
// Definition of the myriadclock display layout for the English plate
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
// 0|TWENTY-FIVE•TOW•
// 1|TENHALFNPASTOFA•
// 2|QUARTEROPASTOGK•
// 3|GOODBT•PASSCODE•
// 4|ONETHREEEIGHTQJM
// 5|TWOFOURWTWELVEUY
// 6|SIXFIVEISEVENWPR
// 7|TENNINEFELEVENXI
// 8|MORNINGIO'CLOCKA
// 9|BIRTHDAYHOLIDAYD
// 0|MOTUWETHFRSASOZC
// 1|101121314151617L
// 2|181922023242526O
// 3|27282930@JANFEBC
// 4|MARAPRMAYJUNJULK
// 5|AUGSEPOCTNOVDEC•
//


// Word definitions, {-1,-1} or WEND terminated (led numbers start at 0)

static const ledpos_t ENV1_minute_1  [] = { {15,0}, WEND};
static const ledpos_t ENV1_minute_2  [] = { {15,0}, {15,1}, WEND};
static const ledpos_t ENV1_minute_3  [] = { {15,0}, {15,1}, {15,2}, WEND};
static const ledpos_t ENV1_minute_4  [] = { {15,0}, {15,1}, {15,2}, {15,3}, WEND};
static const ledpos_t ENV1_minute_5  [] = { {7,0}, {8,0}, {9,0}, {10,0}, WEND};
static const ledpos_t ENV1_minute_10 [] = { {0,1}, {1,1}, {2,1}, WEND};
static const ledpos_t ENV1_quarter   [] = { {0,2}, {1,2}, {2,2}, {3,2}, {4,2}, {5,2}, {6,2}, WEND};
static const ledpos_t ENV1_minute_20 [] = { {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, WEND};
static const ledpos_t ENV1_minute_25 [] = { {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0}, {9,0}, {10,0}, WEND};

static const ledpos_t ENV1_pulse     [] = { {15,15}, WEND};

static const ledpos_t ENV1_past1     [] = { {8,1}, {9,1}, {10,1}, {11,1}, WEND};
static const ledpos_t ENV1_past2     [] = { {8,2}, {9,2}, {10,2}, {11,2}, WEND};
static const ledpos_t ENV1_to0       [] = { {12,0}, {13,0}, WEND}; // Line 0
static const ledpos_t ENV1_to1       [] = { {11,1}, {12,1}, WEND}; // Line 1
static const ledpos_t ENV1_to2       [] = { {11,2}, {12,2}, WEND}; // Line 2

static const ledpos_t ENV1_half      [] = { {3,1}, {4,1}, {5,1}, {6,1}, {8,1}, {9,1}, {10,1}, {11,1}, WEND}; // It is always "half past"

static const ledpos_t ENV1_hour_1    [] = { {0,4}, {1,4}, {2,4}, WEND};
static const ledpos_t ENV1_hour_2    [] = { {0,5}, {1,5}, {2,5}, WEND};
static const ledpos_t ENV1_hour_3    [] = { {3,4}, {4,4}, {5,4}, {6,4}, {7,4}, WEND};
static const ledpos_t ENV1_hour_4    [] = { {3,5}, {4,5}, {5,5}, {6,5}, WEND};
static const ledpos_t ENV1_hour_5    [] = { {3,6}, {4,6}, {5,6}, {6,6}, WEND};
static const ledpos_t ENV1_hour_6    [] = { {0,6}, {1,6}, {2,6}, WEND};
static const ledpos_t ENV1_hour_7    [] = { {8,6}, {9,6}, {10,6}, {11,6}, {12,6}, WEND};
static const ledpos_t ENV1_hour_8    [] = { {8,4}, {9,4}, {10,4}, {11,4}, {12,4}, WEND};
static const ledpos_t ENV1_hour_9    [] = { {3,7}, {4,7}, {5,7}, {6,7}, WEND};
static const ledpos_t ENV1_hour_10   [] = { {0,7}, {1,7}, {2,7}, WEND};
static const ledpos_t ENV1_hour_11   [] = { {8,7}, {9,7}, {10,7}, {11,7}, {12,7}, {13,7}, WEND};
static const ledpos_t ENV1_hour_12   [] = { {8,5}, {9,5}, {10,5}, {11,5}, {12,5}, {13,5}, WEND};
static const ledpos_t ENV1_hour_full [] = { {8,8}, {9,8}, {10,8}, {11,8}, {12,8}, {13,8}, {14,8}, WEND};


static const ledpos_t ENV1_monday    [] = { {0,10}, {1,10}, WEND};
static const ledpos_t ENV1_tuesday   [] = { {2,10}, {3,10}, WEND};
static const ledpos_t ENV1_wednesday [] = { {4,10}, {5,10}, WEND};
static const ledpos_t ENV1_thursday  [] = { {6,10}, {7,10}, WEND};
static const ledpos_t ENV1_friday    [] = { {8,10}, {9,10}, WEND};
static const ledpos_t ENV1_saturday  [] = { {10,10}, {11,10}, WEND};
static const ledpos_t ENV1_sunday    [] = { {12,10}, {13,10}, WEND};

static const ledpos_t ENV1_day0      [] = { {1,11}, WEND};
static const ledpos_t ENV1_day1      [] = { {2,11}, WEND};
static const ledpos_t ENV1_day2      [] = { {4,11}, WEND};
static const ledpos_t ENV1_day3      [] = { {6,11}, WEND};
static const ledpos_t ENV1_day4      [] = { {8,11}, WEND};
static const ledpos_t ENV1_day5      [] = { {10,11}, WEND};
static const ledpos_t ENV1_day6      [] = { {12,11}, WEND};
static const ledpos_t ENV1_day7      [] = { {14,11}, WEND};
static const ledpos_t ENV1_day8      [] = { {1,12}, WEND};
static const ledpos_t ENV1_day9      [] = { {3,12}, WEND};
static const ledpos_t ENV1_day10     [] = { {0,11}, {1,11}, WEND};
static const ledpos_t ENV1_day11     [] = { {2,11}, {3,11}, WEND};
static const ledpos_t ENV1_day12     [] = { {3,11}, {4,11}, WEND};
static const ledpos_t ENV1_day13     [] = { {5,11}, {6,11}, WEND};
static const ledpos_t ENV1_day14     [] = { {7,11}, {8,11}, WEND};
static const ledpos_t ENV1_day15     [] = { {9,11}, {10,11}, WEND};
static const ledpos_t ENV1_day16     [] = { {11,11}, {12,11}, WEND};
static const ledpos_t ENV1_day17     [] = { {13,11}, {14,11}, WEND};
static const ledpos_t ENV1_day18     [] = { {0,12}, {1,12}, WEND};
static const ledpos_t ENV1_day19     [] = { {2,12}, {3,12}, WEND};
static const ledpos_t ENV1_day20     [] = { {5,12}, {6,12}, WEND};
static const ledpos_t ENV1_day21     [] = { {4,11}, {5,11}, WEND};
static const ledpos_t ENV1_day22     [] = { {4,12}, {5,12}, WEND};
static const ledpos_t ENV1_day23     [] = { {7,12}, {8,12}, WEND};
static const ledpos_t ENV1_day24     [] = { {9,12}, {10,12}, WEND};
static const ledpos_t ENV1_day25     [] = { {11,12}, {12,12}, WEND};
static const ledpos_t ENV1_day26     [] = { {13,12}, {14,12}, WEND};
static const ledpos_t ENV1_day27     [] = { {0,13}, {1,13}, WEND};
static const ledpos_t ENV1_day28     [] = { {2,13}, {3,13}, WEND};
static const ledpos_t ENV1_day29     [] = { {4,13}, {5,13}, WEND};
static const ledpos_t ENV1_day30     [] = { {6,13}, {7,13}, WEND};
static const ledpos_t ENV1_day31     [] = { {6,11}, {7,11}, WEND};

static const ledpos_t ENV1_januari  [] = { {9,13}, {10,13}, {11,13}, WEND};
static const ledpos_t ENV1_februari [] = { {12,13}, {13,13}, {14,13}, WEND};
static const ledpos_t ENV1_march    [] = { {0,14}, {1,14}, {2,14}, WEND};
static const ledpos_t ENV1_april    [] = { {3,14}, {4,14}, {5,14}, WEND};
static const ledpos_t ENV1_may      [] = { {6,14}, {7,14}, {8,14}, WEND};
static const ledpos_t ENV1_june     [] = { {9,14}, {10,14}, {11,14}, WEND};
static const ledpos_t ENV1_july     [] = { {12,14}, {13,14}, {14,14}, WEND};
static const ledpos_t ENV1_august   [] = { {0,15}, {1,15}, {2,15}, WEND};
static const ledpos_t ENV1_september[] = { {3,15}, {4,15}, {5,15}, WEND};
static const ledpos_t ENV1_october  [] = { {6,15}, {7,15}, {8,15}, WEND};
static const ledpos_t ENV1_november [] = { {9,15}, {10,15}, {11,15}, WEND};
static const ledpos_t ENV1_december [] = { {12,15}, {13,15}, {14,15}, WEND};

static const ledpos_t ENV1_myriadclock  [] = { {15,4}, {15,5}, {15,6}, {15,7}, {15,8}, {15,9}, {15,10}, {15,11}, {15,12}, {15,13}, {15,14}, WEND};
static const ledpos_t ENV1_bt [] = { {4,3}, {5,3}, WEND};
static const ledpos_t ENV1_passcode [] = { {7,3}, {8,3}, {9,3}, {10,3}, {11,3}, {12,3}, {13,3}, {14,3}, WEND};
static const ledpos_t ENV1_no  [] = { {7,1}, {7,2}, WEND};
static const ledpos_t ENV1_yes  [] = { {0,3}, {1,3}, {2,3}, {3,3}, WEND}; // Actually = GOOD
static const ledpos_t ENV1_wifi  [] = { {7,5}, {7,6}, {7,7}, {7,8}, WEND};

static const ledpos_t ENV1_birthday [] = { {0,9}, {1,9}, {2,9}, {3,9}, {4,9}, {5,9}, {6,9}, {7,9}, WEND};
static const ledpos_t ENV1_holiday [] = { {8,9}, {10,9}, {11,9}, {12,9}, {13,9}, {14,9}, WEND};
static const ledpos_t ENV1_party [] = { {12,9}, {13,9}, {14,9}, WEND}; // Actually not available, will display 'day'
static const ledpos_t ENV1_wakeup [] = { {14,0}, {14,1}, {14,2}, {14,3}, {14,5}, {14,6}, WEND};
static const ledpos_t ENV1_goodmorning [] = { {0,3}, {1,3}, {2,3}, {3,3}, {0,8}, {1,8}, {2,8}, {3,8}, {4,8}, {5,8}, {6,8}, WEND};

//
// EN clock layout V1
//
static const ledclocklayout_t s_layoutEN_V1 = 
{
    16,
    16,
    256,
    O_NORMAL,
    false,

    // Time display method
    TF_EN_5MIN,

    // Time parts
    {
        ENV1_pulse, EMPTY, ENV1_minute_1, ENV1_minute_2, ENV1_minute_3, ENV1_minute_4, ENV1_minute_5, EMPTY, EMPTY, EMPTY, EMPTY, ENV1_minute_10,
        EMPTY, EMPTY, EMPTY, EMPTY, ENV1_quarter, ENV1_minute_20, ENV1_minute_25, 
        ENV1_past2, ENV1_to0, ENV1_past1, ENV1_to1, ENV1_past2, ENV1_to2, ENV1_past1, ENV1_to0, ENV1_past2, ENV1_to0, 
        ENV1_half, ENV1_half, ENV1_hour_full
    },

    // 12x Hours
    {
        ENV1_hour_12, ENV1_hour_1, ENV1_hour_2, ENV1_hour_3, ENV1_hour_4, ENV1_hour_5, ENV1_hour_6, ENV1_hour_7, ENV1_hour_8, ENV1_hour_9, ENV1_hour_10, ENV1_hour_11         
    },

    // 7x Day of the week
    {
        ENV1_sunday, ENV1_monday, ENV1_tuesday, ENV1_wednesday, ENV1_thursday, ENV1_friday, ENV1_saturday
    },

    // 31x Day number
    {
        ENV1_day1, ENV1_day2, ENV1_day3, ENV1_day4, ENV1_day5, ENV1_day6, ENV1_day7, ENV1_day8, ENV1_day9, ENV1_day10, 
        ENV1_day11, ENV1_day12, ENV1_day13, ENV1_day14, ENV1_day15, ENV1_day16, ENV1_day17, ENV1_day18, ENV1_day19, ENV1_day20, 
        ENV1_day21, ENV1_day22, ENV1_day23, ENV1_day24, ENV1_day25, ENV1_day26, ENV1_day27, ENV1_day28, ENV1_day29, ENV1_day30, 
        ENV1_day31
    },

    // 12x Month
    {
        ENV1_januari, ENV1_februari, ENV1_march, ENV1_april, ENV1_may, ENV1_june, ENV1_july, ENV1_august, ENV1_september, ENV1_october, ENV1_november, ENV1_december
    },

    // Extra parts
    {
        ENV1_no,
        ENV1_yes,
        ENV1_wifi,
        ENV1_myriadclock,
        EMPTY,
        EMPTY,
        ENV1_bt,
        ENV1_passcode,
        ENV1_goodmorning,
        ENV1_birthday,
        ENV1_holiday,
        ENV1_party,
        ENV1_wakeup,
        EMPTY,
        EMPTY,
        EMPTY,
        EMPTY,
    },

    // 10x number
    {
        ENV1_day0, ENV1_day1, ENV1_day2, ENV1_day3, ENV1_day4, ENV1_day5, ENV1_day6, ENV1_day7, ENV1_day8, ENV1_day9
    }
};

