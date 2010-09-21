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

#ifndef YAED_TAB_CONTENTS_H
#define YAED_TAB_CONTENTS_H

#include <gtk/gtk.h>

#include "source-model.h"
#include "source-view.h"

/*
 * public types
 */

//opaque type that instantiates this module
typedef struct YaedTabContents* YaedTabContentsHandle;

/*
 * public functions
 */

//make a new content pane for a tab
YaedTabContentsHandle yaedTabContentsNew( YaedSourceModelHandle model,
                                          YaedSourceViewHandle view);

//get the gtk widget
GtkWidget* yaedTabContentsWidget(YaedTabContentsHandle contents);

//destroy the thing
void yaedTabContentsDestroy(YaedTabContentsHandle contents);

#endif