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
  GtkEntry* entry;
};

/*
 * private functions
 */

//signal that handles user pressing an icon in the location bar
void yaedLocationBarIconPress(GtkEntry* entry,
                              GtkEntryIconPosition* position,
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
  //ask spider to load it all up
  yaedSpiderLoadLocation(view, location);
  //done
  return;
}

/*
 * public functions
 */

//create a new location bar
YaedLocationBarHandle yaedLocationBarNew(const YaedSourceViewHandle view, const YaedSourceModelHandle model)
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
    //allocate everything
    bar = g_slice_new(struct YaedLocationBar);
    bar->entry = (GtkEntry*)gtk_entry_new();

    //set it up and show it
    gtk_entry_set_text(bar->entry, location->str);
    gtk_entry_set_icon_from_stock(bar->entry,
                                  GTK_ENTRY_ICON_SECONDARY,
                                  GTK_STOCK_OPEN);
    g_signal_connect( bar->entry,
                      "icon-press",
                      (GCallback)yaedLocationBarIconPress,
                      view);
    gtk_widget_show((GtkWidget*)bar->entry);
  }

  return bar;
}

//update the location bar from the given model
bool yaedLocationBarModelUpdate(YaedLocationBarHandle bar, const YaedSourceModelHandle model)
{
  gtk_entry_set_text(bar->entry, yaedSourceModelGetLocation(model)->str);
  return true;
}
//get the location bar's widget
GtkWidget* yaedLocationBarWidget(const YaedLocationBarHandle bar)
{
  return NULL == bar ? NULL : (GtkWidget*)bar->entry;
}

//destroy the bar
void yaedLocationBarDestroy(YaedLocationBarHandle bar)
{
  if(NULL != bar)
  {
    if(NULL != bar->entry)
      gtk_widget_destroy((GtkWidget*)bar->entry);
    g_slice_free(struct YaedLocationBar, bar);
  }
  return;
}
