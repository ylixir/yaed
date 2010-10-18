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

#ifndef YAED_SOURCE_MODEL_H
#define YAED_SOURCE_MODEL_H

#include <gtk/gtk.h>
#include <gtksourceview/gtksourceview.h>
#include <stdbool.h>

//the public type that provides instances of this module
typedef struct YaedSourceModel *YaedSourceModelHandle;

/*
 * public functions
 */

//create a new model from the given location
YaedSourceModelHandle yaedSourceModelNew(const GString* location);

//get the location string, or the buffer object for the model
GString* yaedSourceModelGetLocation(const YaedSourceModelHandle model);
GtkSourceBuffer* yaedSourceModelGetBuffer(const YaedSourceModelHandle model);

//update the contents of the model
bool yaedSourceModelSetBufferContents(YaedSourceModelHandle model, const GString* content);
//update the location the model references
bool yaedSourceModelSetLocation(YaedSourceModelHandle model, const GString* location);
//update the syntax highlighting scheme
bool yaedSourceModelUpdateHighlighting(YaedSourceModelHandle model, const GString* sample);
//destroy the model
void yaedSourceModelDestroy(YaedSourceModelHandle model);
//reference counting these references are related to ui,
//not the usual memory management, although at the end
//of the day the effect is essentially the same
unsigned int yaedSourceModelIncrementReferenceCount(YaedSourceModelHandle model);
unsigned int yaedSourceModelDecrementReferenceCount(YaedSourceModelHandle model);

#endif
