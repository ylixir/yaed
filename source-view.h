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

#ifndef YAED_SOURCE_VIEW_H
#define YAED_SOURCE_VIEW_H

#include <stdbool.h>

#include "source-model.h"

//the public type that provides instances of this module
typedef struct YaedSourceView *YaedSourceViewHandle;

/* public functions */

//get a new view for the given model
YaedSourceViewHandle yaedSourceViewNew(const YaedSourceModelHandle model);

//get the gtk widgets that implement the tab for the view
GtkWidget* yaedSourceViewLabelWidget(const YaedSourceViewHandle view);
GtkWidget* yaedSourceViewContentsWidget(const YaedSourceViewHandle view);

//associate the view with a new model
bool yaedSourceViewSetModel(YaedSourceViewHandle view, const YaedSourceModelHandle model);

#endif
