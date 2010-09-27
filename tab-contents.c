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

#include "tab-contents.h"
#include "location-bar.h"

/*
 * private types
 */

struct YaedTabContents
{
  GtkVBox* box;
  GtkSourceView* text;
  YaedLocationBarHandle location_bar;
};

/*
 * public functions
 */

//make a new content pane for a tab
YaedTabContentsHandle yaedTabContentsNew( const YaedSourceModelHandle model,
                                          const YaedSourceViewHandle view)
{
  //return value
  YaedTabContentsHandle contents = NULL;
  //the bits from the model that we will need to tie to the view
  GtkSourceBuffer* buffer = NULL;

  if(NULL != view && NULL != model)
    buffer = yaedSourceModelGetBuffer(model);

  if(NULL != buffer)
  {
    //misc variables we need for packing
    GtkScrolledWindow* scroll_area;
    GtkWidget* location_widget;

    //allocate and fill out the contents
    contents = g_slice_new(struct YaedTabContents);
    contents->box = (GtkVBox*)gtk_vbox_new(FALSE, 0);
    contents->location_bar = yaedLocationBarNew(model);
    contents->text = (GtkSourceView*)gtk_source_view_new_with_buffer(buffer);

    //pack it all up
    scroll_area = (GtkScrolledWindow*)gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add((GtkContainer*)scroll_area, (GtkWidget*)contents->text);
    location_widget = yaedLocationBarWidget(contents->location_bar);
    gtk_box_pack_start((GtkBox*)contents->box, location_widget, FALSE, TRUE, 0);
    gtk_box_pack_start((GtkBox*)contents->box, (GtkWidget*)scroll_area, TRUE, TRUE, 0);

    //it's all ready to go
    gtk_widget_show((GtkWidget*)contents->text);
    gtk_widget_show((GtkWidget*)scroll_area);
    gtk_widget_show((GtkWidget*)contents->box);
  }

  return contents;
}

//get the gtk widget
GtkWidget* yaedTabContentsWidget(const YaedTabContentsHandle contents)
{
  return NULL == contents ? NULL : (GtkWidget*)contents->box;
}

//destroy the thing
void yaedTabContentsDestroy(YaedTabContentsHandle contents)
{
  if(NULL != contents)
  {
    if(NULL != contents->location_bar)
      yaedLocationBarDestroy(contents->location_bar);
    if(NULL != contents->box)
      gtk_widget_destroy((GtkWidget*)contents->box);
    g_slice_free(struct YaedTabContents, contents);
  }
  return;
}
