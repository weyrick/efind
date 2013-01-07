
SET(TIME_SRC_LIST astro.c
             dow.c
             parse_date.c
             parse_tz.c
             timelib.c
             tm2unixtime.c
             unixtime2tm.c
             parse_iso_intervals.c
             interval.c)

#add_definitions(-DDEBUG_PARSER)

add_library(timelib ${TIME_SRC_LIST})
