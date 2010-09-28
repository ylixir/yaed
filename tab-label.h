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

#ifndef YAED_TAB_LABEL_H
#define YAED_TAB_LABEL_H

#include <gtk/gtk.h>
#include <stdbool.h>

#include "source-model.h"
#include "source-view.h"

/*
 * public types
 */

//opaque "instance" type for this module
typedef struct YaedTabLabel* YaedTabLabelHandle;

/*
 * public functions
 */

//make a new label for the view from the model
YaedTabLabelHandle yaedTabLabelNew( const YaedSourceModelHandle model,
                                    const YaedSourceViewHandle view);

//update the label to reflect the model
bool yaedTabLabelModelUpdate(YaedTabLabelHandle label, const YaedSourceModelHandle model);

//get the GtkWidget for the label
GtkWidget* yaedTabLabelWidget(const YaedTabLabelHandle label);

//destroy the label
void yaedTabLabelDestroy(YaedTabLabelHandle label);

#endif
