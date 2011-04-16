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

/*
 * private types
 */

struct YaedLocationBar
{
  GtkHBox* box;
  GtkEntry* entry;
  YaedSourceModelHandle model;
};

enum
{
  COMPLETION_PATH_TEXT,
  COMPLETION_N_COLUMNS
};
/*
 * private functions
 */

//convenience func for filling out the completion list with directory contents
void yaedLocationBarAppendDirectoryContentsToList(
  const gchar* directory,
  GtkListStore* list)
{
  //the variable used to track the open directory
  GDir* current_directory;
  //the list iterator
  GtkTreeIter iter;
  
  //open the directory
  current_directory = g_dir_open(directory, 0, NULL);
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
}
//called to rebuild the completion model
void yaedLocationBarRebuildCompletion(YaedLocationBarHandle bar, gchar* directory)
{
  //track root status
  bool is_root;
  
  //the list we are building
  GtkListStore* list;
  list = gtk_list_store_new(COMPLETION_N_COLUMNS, G_TYPE_STRING);
  
  //just in case there were no path components, get rid of the dot
  if(0 == g_utf8_collate(".", directory))
    directory++;

  is_root = false;
  if(TRUE == g_path_is_absolute(directory))
    is_root = true;
  else if(TRUE == g_unichar_isalpha(directory[0]))
    if(':' == directory[1])
      is_root = true;
      
  //is the path absolute?
  if(true == is_root)
  {
    yaedLocationBarAppendDirectoryContentsToList(directory, list);
  }
  else
  {
    //these will be used for building directory paths
    gchar* built_path;
    const gchar* home_path;

    //make sure we aren't directly pointing to the home directory
    if('~' == directory[0])
    {
      directory++;
      if('/' == directory[0] || '\\' == directory[0])
        directory++;
    }
    else //we need to build paths for root directories, and the cwd
    {
      //we'll need the cwd
      gchar* working_path;
      //we in windows?
      if(TRUE == g_file_test("c:\\", G_FILE_TEST_IS_DIR))
      {
        //check all roots from c:-z:
        built_path = g_build_filename("c:\\", directory, NULL);
        for(;'z' >= *built_path; built_path[0]++)
          yaedLocationBarAppendDirectoryContentsToList(built_path, list);
        //cleanup
        g_free(built_path);
      }
      else //we aren't in windows
      {
        //do the / root
        built_path = g_build_filename("/", directory, NULL);
        yaedLocationBarAppendDirectoryContentsToList(built_path, list);
        g_free(built_path);
      }

      //do the cwd check
      working_path =
        g_path_get_dirname(yaedSourceModelGetLocation(bar->model)->str);
      if(0 != g_utf8_collate(".", working_path))
      {
        //fill out the cwd contents
        built_path = g_build_filename(working_path, directory, NULL);
        yaedLocationBarAppendDirectoryContentsToList(built_path, list);
        g_free(built_path);
      }
      g_free(working_path);
    }

    //get the home path
    home_path = g_get_home_dir();
    built_path = g_build_filename(home_path, directory, NULL);
    yaedLocationBarAppendDirectoryContentsToList(built_path, list);
    g_free(built_path);
  }
  
  //set the list
  gtk_entry_completion_set_model(
    gtk_entry_get_completion(bar->entry),
    (GtkTreeModel*)list);
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
  GString* location;
  //dummy code
  event = event;
  position = position;
  //get the location string from the location bar
  location = g_string_new_len(gtk_entry_get_text(entry),
                              gtk_entry_get_text_length(entry));
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
  GString* location = NULL;
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
    bar->box = (GtkHBox*)gtk_hbox_new(FALSE, 0);
    bar->entry = (GtkEntry*)gtk_entry_new();
    gtk_entry_set_completion(bar->entry, gtk_entry_completion_new());
    gtk_entry_completion_set_text_column(
      gtk_entry_get_completion(bar->entry),
      COMPLETION_PATH_TEXT);
    menu_button = (GtkButton*)gtk_button_new();
    menu_image = (GtkImage*)gtk_image_new_from_stock(
      GTK_STOCK_PREFERENCES, GTK_ICON_SIZE_MENU);

    //set up and pack everything
    gtk_container_add((GtkContainer*)menu_button, (GtkWidget*)menu_image);
    gtk_entry_set_text(bar->entry, location->str);
    location_directory = g_path_get_dirname(location->str);
    yaedLocationBarRebuildCompletion(bar, location_directory);
    gtk_entry_set_icon_from_stock(bar->entry,
                                  GTK_ENTRY_ICON_PRIMARY,
                                  GTK_STOCK_SAVE);
    gtk_entry_set_icon_from_stock(bar->entry,
                                  GTK_ENTRY_ICON_SECONDARY,
                                  GTK_STOCK_OPEN);
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
  gtk_entry_set_text(bar->entry, yaedSourceModelGetLocation(model)->str);

  //rebuild the completion model
  location_directory =
    g_path_get_dirname(yaedSourceModelGetLocation(model)->str);
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
