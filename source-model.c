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

#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcelanguagemanager.h>

#include "source-model.h"

/*
 * private types
 */

struct YaedSourceModel
{
  GtkSourceBuffer* buffer;
  GString* location;
  unsigned int references;
};

/*
 * private functions
 */

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
    gtk_source_buffer_set_highlight_syntax(model->buffer, TRUE);
    /*g_signal_connect( model->buffer,
                      "changed",
                      (GCallback)yaedSourceModelChangedEvent,
                      model);*/

    if(0 == location->len || NULL == location->str)
    {
      model->location = g_string_new("");
    }
    else
    {
      model->location = g_string_new_len(location->str, location->len);
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

//update the syntax highlighting scheme
bool yaedSourceModelUpdateHighlighting( YaedSourceModelHandle model,
                                        const GString* sample)
{
  GtkSourceLanguage* language;
  GtkSourceLanguageManager* manager;
  gboolean result;
  gchar* contentType;
  gchar* fileName;
  
  //do we have a valid file name?
  fileName = 0 != model->location->len ? model->location->str : NULL;
  //create the manager
  manager = gtk_source_language_manager_get_default();
  
  //try to get the content type
  contentType = g_content_type_guess( fileName,
                                      (guchar*)sample->str,
                                      sample->len,
                                      &result);
  if(FALSE == result)
  {
    g_free(contentType);
    contentType = NULL;
  }
  if(NULL != contentType || NULL != fileName)
  {
    language = gtk_source_language_manager_guess_language(manager,
                                                          fileName,
                                                          contentType);
    gtk_source_buffer_set_language(model->buffer, language);
  }
  
  //clean up
  if(NULL != contentType)
    g_free(contentType);
  
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

//reference counting these references are related to ui,
//not the usual memory management, although at the end
//of the day the effect is essentially the same
unsigned int yaedSourceModelIncrementReferenceCount(YaedSourceModelHandle model)
{
  model->references++;
  return model->references;
}
unsigned int yaedSourceModelDecrementReferenceCount(YaedSourceModelHandle model)
{
  model->references--;
  return model->references;
}
