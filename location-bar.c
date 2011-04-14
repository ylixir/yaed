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

/*
 * private functions
 */

//called when (before) the user inserts text into the location bar
void yaedLocationBarInsertText(
  GtkEntry* entry,
  gchar* incoming_text,
  gint incoming_size,  //size in bytes, maybe -1 if NULL term
  gint* incoming_position,     //position in characters for insertion
  YaedLocationBarHandle bar)
{
  //make our compiler happy
  bar=NULL;

  //our variables, wooo
  const gchar* old_text = gtk_entry_get_text(entry);
  gchar* new_text;
  size_t incoming_length, old_length;
  size_t old_size, new_size;
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
    
  //check our result
  printf("%s\t%s\n",old_text, new_text);

  //cleanup
  g_slice_free1(new_size+1, new_text);
}

//called when the user deletes text from the location bar
void yaedLocationBarDeleteText(
  GtkEntry* entry,
  gint start_position,  //position in characters
  gint end_position,    //in characters, neg means delete to end
  YaedLocationBarHandle bar)
{
  //make our compiler happy
  bar=NULL;

  printf("%s\t%d\t%d\n", gtk_entry_get_text(entry),start_position,end_position);
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
    
    //allocate everything
    bar = g_slice_new(struct YaedLocationBar);
    bar->model = model;
    bar->box = (GtkHBox*)gtk_hbox_new(FALSE, 0);
    bar->entry = (GtkEntry*)gtk_entry_new();
    menu_button = (GtkButton*)gtk_button_new();
    menu_image = (GtkImage*)gtk_image_new_from_stock(
      GTK_STOCK_PREFERENCES, GTK_ICON_SIZE_MENU);

    //set up and pack everything
    gtk_container_add((GtkContainer*)menu_button, (GtkWidget*)menu_image);
    gtk_entry_set_text(bar->entry, location->str);
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
  }

  return bar;
}

//update the location bar from the given model
bool yaedLocationBarModelUpdate(
  YaedLocationBarHandle bar,
  const YaedSourceModelHandle model)
{
  gtk_entry_set_text(bar->entry, yaedSourceModelGetLocation(model)->str);
  bar->model = model;
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
