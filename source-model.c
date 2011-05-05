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

#include "spider.h"
#include "source-model.h"
#include "utility.h"

/*
 * private types
 */

struct YaedSourceModel
{
  GtkSourceBuffer* buffer;
  char* location;
  unsigned int references;
};

/*
 * private functions
 */

void yaedSourceModelModifiedChangedEvent(GtkSourceBuffer* buffer,
  YaedSourceModelHandle model)
{
  yaedSpiderRequestViewUpdateForModel(model);
}

/*
 * public functions
 */

//create a new model from the given location
YaedSourceModelHandle yaedSourceModelNew(const char* location)
{
  //this is what will be returned
  YaedSourceModelHandle model = NULL;

  if(NULL != location)
  {
    //variables for the string
    size_t length, size;
    
    //allocat the structure
    model = g_slice_new0(struct YaedSourceModel);
    
    //setup the buffer
    model->buffer = gtk_source_buffer_new(NULL);
    g_object_ref(model->buffer);
    gtk_source_buffer_set_highlight_syntax(model->buffer, TRUE);
    g_signal_connect( model->buffer,
                      "modified-changed",
                      (GCallback)yaedSourceModelModifiedChangedEvent,
                      model);
    
    //get the string dimensions
    size = yaedUtilityUtf8GetSize(location, &length);
    //allocate and copy
    model->location = g_malloc(size);
    g_utf8_strncpy(model->location, location, length);
    //terminate
    model->location[size-1] = '\0';
  }

  return model;
}

//get the location string for the model
char* yaedSourceModelGetLocation(const YaedSourceModelHandle model)
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
                                const char* location)
{
  //the dimensions of the new location
  size_t length, size;
  
  //convenience variables to make this operation as close to atomic as we can
  char* new_tmp;
  char* old_tmp;
  
  //allocate and copy the new string
  size = yaedUtilityUtf8GetSize(location, &length);
  new_tmp = g_malloc(size);
  g_utf8_strncpy(new_tmp, location, length);
  new_tmp[size-1] = '\0';
  
  //save the old string
  old_tmp = model->location;
  //assign the new string
  model->location = new_tmp;
  //cleanup the old string
  g_free(old_tmp);

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
  char* fileName;
  
  //do we have a valid file name?
  fileName =
    0 != yaedUtilityUtf8GetLength(model->location, NULL)
    ? model->location : NULL;
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
  g_free(model->location);
  g_object_unref(model->buffer);
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

//used to manually set the modified bit of a model (opening, saving, etc)
void yaedSourceModelSetModified(YaedSourceModelHandle model, bool modified)
{
  gtk_text_buffer_set_modified((GtkTextBuffer*)model->buffer, modified);
}

//used to get the modified bit of a model (opening, saving, etc)
bool yaedSourceModelGetModified(YaedSourceModelHandle model)
{
  return gtk_text_buffer_get_modified((GtkTextBuffer*)model->buffer)
    ? true:false;
}
