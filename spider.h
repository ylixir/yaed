/*******************************************************************************
Copyright (c) 2010 Jon "ylixir" Allen <ylixir@gmail.com>. All rights reserved.

This file is part of yAEd.

yAEd is free software: you can redistribute it and/or modify
it under the terms of the Artistic License 2.0 as published by
the Perl Foundation.

yAEd is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
Artistic License 2.0 for more details.

You should have received a copy of the Artistic License 2.0
along with yAEd.  If not, see <http://www.perlfoundation.org>.

*******************************************************************************/

#ifndef YAED_SPIDER_H
#define YAED_SPIDER_H

#include <stdbool.h>
#include <gtk/gtk.h>

#include "source-view.h"

//set up initial window(s) tab(s) etc.
bool yaedSpiderInit();

//called by any component that wishes a view to close
bool yaedSpiderRequestViewClose(YaedSourceViewHandle view);

//called when the user requests a location be (re)loaded
bool yaedSpiderLoadLocation(YaedSourceViewHandle view, const GString* location);

#endif
