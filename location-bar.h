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

#ifndef YAED_LOCATION_BAR_H
#define YAED_LOCATION_BAR_H

#include <gtk/gtk.h>
#include <stdbool.h>

#include "source-model.h"
#include "source-view.h"

/*
 * public types
 */

//opaque type that instantiates this module
typedef struct YaedLocationBar* YaedLocationBarHandle;

/*
 * public functions
 */

//create a new location bar
YaedLocationBarHandle yaedLocationBarNew(const YaedSourceViewHandle view, const YaedSourceModelHandle model);

//update the location bar from the given model
bool yaedLocationBarModelUpdate(YaedLocationBarHandle bar, const YaedSourceModelHandle model);

//get the location bar's widget
GtkWidget* yaedLocationBarWidget(const YaedLocationBarHandle bar);

//destroy the bar
void yaedLocationBarDestroy(YaedLocationBarHandle bar);

#endif
