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

#include "location-bar.h"
#include "spider.h"
#include "utility.h"

/*
 * private types
 */

struct YaedLocationBar
{
  GtkHBox* box;
  GtkEntry* entry;
  YaedSourceModelHandle model;
  YaedSourceViewHandle view;
};

enum
{
  COMPLETION_PATH_TEXT,
  COMPLETION_N_COLUMNS
};
/*
 * private functions
 */

//convenience function for updating the action icons in the bar
void yaedLocationBarUpdateIconActions(
  YaedLocationBarHandle bar,
  const char* bar_location)
{
  //the variables we will need
  const char* model_location; //where the model is currently pointing
  bool location_modified; //state variables
  const gchar* primary_icon = NULL; //the "save" icon
  const gchar* secondary_icon = NULL; //the "open" icon

  //set up the variables
  model_location = yaedSourceModelGetLocation(bar->model);
  location_modified =
    (0 == g_utf8_collate(model_location, bar_location)) ? false : true;

  //if the using has typed a new location into the location bar
  if(true == location_modified)
  {
    //make sure we aren't pointing at a directory
    if(false == yaedUtilityDirectoryExists(bar_location))
    {
      //can't save to an invalid path
      if(true == yaedUtilityLocationHasValidPath(bar_location)
        && 0 != g_utf8_collate("", bar_location))
      {
        primary_icon = GTK_STOCK_SAVE_AS;
      }
      //can't open a file that doesn't exist
      if(true == yaedUtilityLocationExists(bar_location))
        secondary_icon = GTK_STOCK_OPEN;
    }
  }
  //if we aren't pointing to nothing, and we aren't typing in a new location
  else if(0 != g_utf8_collate("", model_location))
  {
    //save only makes sense if we've modified something
    if(true == yaedSourceModelGetModified(bar->model))
      primary_icon = GTK_STOCK_SAVE;
    secondary_icon = GTK_STOCK_REFRESH; //changes on disk can happen outside
  }

  //naturally we have to actually set the icons
  gtk_entry_set_icon_from_stock(
    bar->entry,
    GTK_ENTRY_ICON_PRIMARY,
    primary_icon);
  gtk_entry_set_icon_from_stock(
    bar->entry,
    GTK_ENTRY_ICON_SECONDARY,
    secondary_icon);
}

//convenience func for filling out the completion list with directory contents
void yaedLocationBarAppendDirectoryContentsToList(
  const gchar* directory,
  GtkListStore* list)
{
  //some room for the expanded directory
  char* expanded_path;
  //the variable used to track the open directory
  GDir* current_directory;
  //the list iterator
  GtkTreeIter iter;
  
  //get the expanded version of the path
  expanded_path = yaedUtilityExpandPath(directory);
  
  //open the directory
  current_directory = g_dir_open(expanded_path, 0, NULL);
  if(NULL != current_directory)
  {
    //iterate over the contents
    const gchar* contents;
    while(NULL != (contents = g_dir_read_name(current_directory)))
    {
      gchar* contents_path;
      contents_path = g_build_filename(directory, contents, NULL);
      
      //placeholders
      gtk_list_store_append(list, &iter);
      gtk_list_store_set(list, &iter, COMPLETION_PATH_TEXT, contents_path, -1);

      //cleanup
      g_free(contents_path);
    }

    //close it up
    g_dir_close(current_directory);
  }
  
  //cleanup
  g_free(expanded_path);
}

//called to rebuild the completion model
void yaedLocationBarRebuildCompletion(
  YaedLocationBarHandle bar,
  gchar* directory)
{
  //the list we are building
  GtkListStore* list;
  list = (GtkListStore*)gtk_entry_completion_get_model(
    gtk_entry_get_completion(bar->entry));
    
  //empty the list
  gtk_list_store_clear(list);
  
  //rebuild the list
  yaedLocationBarAppendDirectoryContentsToList(directory, list);
}

void yaedLocationBarActivate(GtkEntry* entry, YaedLocationBarHandle bar)
{
  const char* new_text; //the text in the action bar
  
  //get the text that the action is being requested for
  new_text = gtk_entry_get_text(entry);
  
  //make sure that we don't have a directory, but we do have a valid path
  if(false == yaedUtilityDirectoryExists(new_text)
    && true == yaedUtilityLocationHasValidPath(new_text))
  {
    //are we pointing to an existing file?
    if(false == yaedUtilityLocationExists(new_text))
    {
      //nope, save if it makes sense
      if(true == yaedSourceModelGetModified(bar->model)
        || 0 != g_utf8_collate("",yaedSourceModelGetLocation(bar->model)))
      {
        yaedSpiderStoreLocation(bar->view, new_text);
      }
    }
    //have we saved our work?
    else if(false == yaedSourceModelGetModified(bar->model))
    {
      //yep, saved our work, open if it makes sense
      if(0 !=
        g_utf8_collate(new_text, yaedSourceModelGetLocation(bar->model)))
      {
        yaedSpiderLoadLocation(bar->view, new_text);
      }
    }
  }
}

//called when (before) the user inserts text into the location bar
void yaedLocationBarInsertText(
  GtkEntry* entry,
  gchar* incoming_text,
  gint incoming_size,  //size in bytes, maybe -1 if NULL term
  gint* incoming_position,     //position in characters for insertion
  YaedLocationBarHandle bar)
{
  //our variables, wooo
  const gchar* old_text = gtk_entry_get_text(entry);
  gchar* new_text;
  gchar* new_directory;
  gchar* old_directory;
  size_t incoming_length, old_length;
  ptrdiff_t old_size, new_size;
  gchar* built_ptr;

  //get the number of characters that are incoming
  incoming_length = g_utf8_strlen(incoming_text, incoming_size);
  //get the size of the incoming text
  incoming_size =
    g_utf8_offset_to_pointer(incoming_text, incoming_length) - incoming_text;

  //get the old_text info
  old_length = g_utf8_strlen(old_text, -1);
  old_size = g_utf8_offset_to_pointer(old_text, old_length) - old_text;

  //for readabilit, the new_size
  new_size = old_size+incoming_size;

  //build the string
  new_text = g_slice_alloc(new_size+1);
  g_utf8_strncpy(new_text, old_text, *incoming_position);
  built_ptr = g_utf8_offset_to_pointer(new_text, *incoming_position);
  g_utf8_strncpy(built_ptr, incoming_text, incoming_length);
  g_utf8_strncpy(
    built_ptr + incoming_size,
    old_text + (built_ptr - new_text),
    old_length - *incoming_position);

  //okies, check to see if the base directory has changed
  old_directory = g_path_get_dirname(old_text);
  new_directory = g_path_get_dirname(new_text);

  if(0 != g_utf8_collate(old_directory, new_directory))
  {
    yaedLocationBarRebuildCompletion(bar, new_directory);
  }

  //update the icons
  yaedLocationBarUpdateIconActions(bar, new_text);
  
  //cleanup
  g_free(new_directory);
  g_free(old_directory);
  g_slice_free1(new_size+1, new_text);
}

//called when the user deletes text from the location bar
void yaedLocationBarDeleteText(
  GtkEntry* entry,
  gint start_position,  //position in characters
  gint end_position,    //in characters, neg means delete to end
  YaedLocationBarHandle bar)
{
  //the variables
  const gchar* old_text = gtk_entry_get_text(entry);
  gchar* new_text;
  gchar* end_text;
  gchar* new_directory;
  gchar* old_directory;
  size_t old_length;
  ptrdiff_t new_size, start_size;

  //get the length of the old string
  old_length = g_utf8_strlen(old_text, -1);

  //fixup any problems with the end position
  if(0 > end_position)
    end_position = old_length;
  if(start_position > end_position) //if the positions are swapped
  { //xor swap
    end_position ^= start_position;
    start_position ^= end_position;
    end_position ^= start_position;
  }

  //get the pointer for the chunk following the deletion
  end_text = g_utf8_offset_to_pointer(old_text, end_position);

  //get the size of the final string
  start_size = g_utf8_offset_to_pointer(old_text, start_position) - old_text;
  new_size = start_size +
    (g_utf8_offset_to_pointer(end_text, old_length-end_position) - end_text);

  //build the string
  new_text = g_slice_alloc(new_size+1);
  g_utf8_strncpy(new_text, old_text, start_position);
  g_utf8_strncpy(new_text+start_size, end_text, old_length - end_position);
  
  //okies, check to see if the base directory has changed
  old_directory = g_path_get_dirname(old_text);
  new_directory = g_path_get_dirname(new_text);
  if(0 != g_utf8_collate(old_directory, new_directory))
  {
    yaedLocationBarRebuildCompletion(bar, new_directory);
  }

  //update the icons
  yaedLocationBarUpdateIconActions(bar, new_text);

  //cleanup
  g_free(new_directory);
  g_free(old_directory);
  g_slice_free1(new_size+1, new_text);
}

//signal that handles user pressing an icon in the location bar
void yaedLocationBarIconPress(GtkEntry* entry,
                              GtkEntryIconPosition position,
                              GdkEvent* event,
                              YaedSourceViewHandle view)
{
  const char* location;
  //dummy code
  event = event;
  position = position;
  //get the location string from the location bar
  /*
  location = g_string_new_len(gtk_entry_get_text(entry),
                              gtk_entry_get_text_length(entry));
  */
  location = gtk_entry_get_text(entry);
  
  switch(position)
  {
  case GTK_ENTRY_ICON_PRIMARY:
    //ask spider to store it away
    yaedSpiderStoreLocation(view, location);
    break;
  case GTK_ENTRY_ICON_SECONDARY:
    //ask spider to load it all up
    yaedSpiderLoadLocation(view, location);
    break;
  default:
    break;
  }
  //done
  return;
}

/*
 * public functions
 */

//create a new location bar
YaedLocationBarHandle yaedLocationBarNew(
  const YaedSourceViewHandle view,
  const YaedSourceModelHandle model)
{
  //the return value
  YaedLocationBarHandle bar = NULL;

  //get the location from the model
  char* location = NULL;
  if(NULL != model)
    location = yaedSourceModelGetLocation(model);

  //if we could get a location from the model then make the bar
  if(NULL != location)
  {
    //the menu button and the image it displays
    GtkButton* menu_button;
    GtkImage* menu_image;
    gchar* location_directory;
    
    //allocate everything
    bar = g_slice_new(struct YaedLocationBar);
    bar->model = model;
    bar->view = view;
    bar->box = (GtkHBox*)gtk_hbox_new(FALSE, 0);
    bar->entry = (GtkEntry*)gtk_entry_new();
    gtk_entry_set_completion(bar->entry, gtk_entry_completion_new());
    //set the list
    gtk_entry_completion_set_model(
      gtk_entry_get_completion(bar->entry),
      (GtkTreeModel*)gtk_list_store_new(COMPLETION_N_COLUMNS, G_TYPE_STRING));
    gtk_entry_completion_set_text_column(
      gtk_entry_get_completion(bar->entry),
      COMPLETION_PATH_TEXT);

    menu_button = (GtkButton*)gtk_button_new();
    menu_image = (GtkImage*)gtk_image_new_from_stock(
      GTK_STOCK_PREFERENCES, GTK_ICON_SIZE_MENU);

    //set up and pack everything
    gtk_container_add((GtkContainer*)menu_button, (GtkWidget*)menu_image);
    gtk_entry_set_text(bar->entry, location);
    location_directory = g_path_get_dirname(location);
    yaedLocationBarRebuildCompletion(bar, location_directory);
    yaedLocationBarUpdateIconActions(bar, location);
    gtk_box_pack_start((GtkBox*)bar->box,
      (GtkWidget*)bar->entry, TRUE, TRUE, 0);
    gtk_box_pack_start((GtkBox*)bar->box,
      (GtkWidget*)menu_button, FALSE, FALSE, 0);
    
    //connect the signals
    g_signal_connect(
      bar->entry,
      "icon-press",
      (GCallback)yaedLocationBarIconPress,
      view);
    g_signal_connect(
      bar->entry,
      "insert-text",
      (GCallback)yaedLocationBarInsertText,
      bar);
    g_signal_connect(
      bar->entry,
      "delete-text",
      (GCallback)yaedLocationBarDeleteText,
      bar);
    g_signal_connect(
      bar->entry,
      "activate",
      (GCallback)yaedLocationBarActivate,
      bar);
      
    //show everything
    gtk_widget_show((GtkWidget*)menu_image);
    gtk_widget_show((GtkWidget*)menu_button);
    gtk_widget_show((GtkWidget*)bar->entry);
    gtk_widget_show((GtkWidget*)bar->box);
    
    //make sure gtk only cleans up when we want
    g_object_ref(bar->box);

    //free the location directory
    g_free(location_directory);
  }

  return bar;
}

//update the location bar from the given model
bool yaedLocationBarModelUpdate(
  YaedLocationBarHandle bar,
  const YaedSourceModelHandle model)
{
  gchar* location_directory;

  //associate the model, update the location text
  bar->model = model;
  gtk_entry_set_text(bar->entry, yaedSourceModelGetLocation(model));

  //update the icons
  yaedLocationBarUpdateIconActions(bar, gtk_entry_get_text(bar->entry));
  //rebuild the completion model
  location_directory =
    g_path_get_dirname(yaedSourceModelGetLocation(model));
  yaedLocationBarRebuildCompletion(bar, location_directory);
  g_free(location_directory);
  
  return true;
}
//get the location bar's widget
GtkWidget* yaedLocationBarWidget(const YaedLocationBarHandle bar)
{
  return NULL == bar ? NULL : (GtkWidget*)bar->box;
}

//destroy the bar
void yaedLocationBarDestroy(YaedLocationBarHandle bar)
{
  if(NULL != bar)
  {
    if(NULL != bar->entry)
      g_object_unref(bar->box);
    g_slice_free(struct YaedLocationBar, bar);
  }
  return;
}
