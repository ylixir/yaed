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

/*
 * private types
 */

struct YaedLocationBar
{
  GtkEntry* entry;
};

/*
 * public functions
 */

//create a new location bar
YaedLocationBarHandle yaedLocationBarNew(const YaedSourceModelHandle model)
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
    gtk_widget_show((GtkWidget*)bar->entry);
  }

  return bar;
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
