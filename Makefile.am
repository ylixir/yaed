AM_CFLAGS = @PACKAGE_CFLAGS@ --pedantic -Wall -Wextra -Werror -Wno-unused-parameter
bin_PROGRAMS = yaed
yaed_SOURCES =  main.c \
                location-bar.c location-bar.h \
                source-model.c source-model.h \
                source-view.c source-view.h \
                spider.c spider.h \
                tab-contents.c tab-contents.h \
                tab-label.c tab-label.h \
                utility.c utility.h
yaed_LDADD = @PACKAGE_LIBS@