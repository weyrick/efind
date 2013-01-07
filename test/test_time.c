/*

Copyright (c) 2012-2013, Shannon Weyrick <weyrick@mozek.us>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "timelib.h"
#include "time.h"
#include "assert.h"

signed long test_strtotime(char *str) {
    int   error1, error2;
    struct timelib_error_container *error;
    long  ts;

    timelib_time *t, *now;
    timelib_tzinfo *tzi;

    tzi = timelib_parse_tzfile("UTC", timelib_builtin_db());
    assert(tzi);

    now = timelib_time_ctor();
    now->tz_info = tzi;
    now->zone_type = TIMELIB_ZONETYPE_ID;
    timelib_unixtime2local(now, (timelib_sll) time(NULL));

    t = timelib_strtotime(str,
                         strlen(str),
                         &error,
                         timelib_builtin_db(),
                         timelib_parse_tzfile);
    error1 = error->error_count;
    timelib_error_container_dtor(error);
    timelib_fill_holes(t, now, TIMELIB_NO_CLONE);
    timelib_update_ts(t, tzi);
    ts = timelib_date_to_int(t, &error2);

    timelib_time_dtor(now);
    timelib_time_dtor(t);

    if (error1 || error2) {
        return -1;
    } else {
        return ts;
    }
}

int do_test_time(void) {

    signed long t, t2;

    printf("** timelib test\n");
    t = test_strtotime("today");
    printf("[today]: %ld, ", t);
    t2 = test_strtotime("today + 1 day");
    printf("[today + 1 day] %ld: ", t2);

    if (t2 == t + 3600*24) {
        printf("OK\n");
        return 0;
    }
    else {
        printf("FAIL\n");
        return 1;
    }
}

