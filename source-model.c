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

#include "source-model.h"

/*
 * private types
 */

struct YaedSourceModel
{
  GtkSourceBuffer* buffer;
  GString* location;
};

/*
 * public functions
 */

//create a new model from the given location
YaedSourceModelHandle yaedSourceModelNew(const GString* location)
{
  //this is what will be returned
  YaedSourceModelHandle model = NULL;

  if(NULL != location)
  {
    model = g_slice_new0(struct YaedSourceModel);
    model->buffer = gtk_source_buffer_new(NULL);

    if(0 == location->len || NULL == location->str)
    {
      model->location = g_string_new("");
    }
    else
    {
      //TODO: this needs implementation
      model->location = g_string_new("");
    }
  }

  return model;
}

//get the location string for the model
GString* yaedSourceModelGetLocation(const YaedSourceModelHandle model)
{
  //if we got a bogus model, return null, otherwise pass out the location
  return NULL == model ? NULL : model->location;
}

//get the buffer object string for the model
GtkSourceBuffer* yaedSourceModelGetBuffer(const YaedSourceModelHandle model)
{
  //if we got a bogus model, return null, otherwise pass out the location
  return NULL == model ? NULL : model->buffer;
}

//update the contents of the model
bool yaedSourceModelSetBufferContents(YaedSourceModelHandle model,
                                      const GString* content)
{
  gtk_text_buffer_set_text( (GtkTextBuffer*)model->buffer,
                            content->str,
                            content->len);
  return true;
}

//update the location the model references
bool yaedSourceModelSetLocation(YaedSourceModelHandle model,
                                const GString* location)
{
  g_string_assign(model->location, location->str);
  return true;
}

//destroy the model
void yaedSourceModelDestroy(YaedSourceModelHandle model)
{
  //release the gtk objects
  g_string_free(model->location, TRUE);
  gtk_object_destroy((GtkObject*)model->buffer);
  //free the actual model structure
  g_slice_free(struct YaedSourceModel, model);

  return;
}

