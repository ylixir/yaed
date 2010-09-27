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

#include "tab-label.h"
#include "spider.h"

/*
 * private data types
 */

struct YaedTabLabel
{
  GtkHBox* box;
  GtkLabel* text;
};

/*
 * private functions
 */

//event handler for the close icon on the tabs
gboolean yaedTabLabelClosePress(GtkWidget* box,
                                GdkEventButton* event,
                                YaedSourceViewHandle view)
{
  //suppress compiler warnings
  box = box;
  event = event;

  //request that the view be closed
  yaedSpiderRequestViewClose(view);
  //return true so that the event doesn't continue propagating
  return TRUE;
}

/*
 * public functions
 */

//make a new label for the view from the model
YaedTabLabelHandle yaedTabLabelNew( const YaedSourceModelHandle model,
                                    const YaedSourceViewHandle view)
{
  //the return value
  YaedTabLabelHandle label = NULL;
  GString* location = NULL;

  //get the location if we can
  if(NULL != model)
    location = yaedSourceModelGetLocation(model);

  //sanity checking...
  if(NULL != location && NULL != view)
  {
    //we will have to wrap the image in an event box
    //this will allow us to catch clicks on it
    GtkEventBox* image_box;
    //the close image
    GtkImage* image;
    //the file name minus all the path info
    gchar* file_name;
    file_name = g_path_get_basename(location->str);

    //create the label
    label = g_slice_new(struct YaedTabLabel);
    label->box = (GtkHBox*)gtk_hbox_new(FALSE, 0);
    label->text = location->len ? (GtkLabel*)gtk_label_new(file_name)
                                : (GtkLabel*)gtk_label_new("[New]");

    //cleanup before we forget
    g_free(file_name);

    //nothing more to do for the text component of the label
    gtk_widget_show((GtkWidget*)label->text);

    //set up the image and it's event catcher wrapper
    image_box = (GtkEventBox*)gtk_event_box_new();
    image = (GtkImage*)gtk_image_new_from_stock(GTK_STOCK_CLOSE,
                                                GTK_ICON_SIZE_MENU);
    gtk_container_add((GtkContainer*)image_box, (GtkWidget*)image);
    g_signal_connect( image_box,
                      "button_press_event",
                      (GCallback)yaedTabLabelClosePress,
                      view);

    //done setting up the image half of the label
    gtk_widget_show((GtkWidget*)image);
    gtk_widget_show((GtkWidget*)image_box);

    //pack the label, and close icon, and show the whole works
    gtk_box_pack_start((GtkBox*)label->box, (GtkWidget*)label->text, TRUE, TRUE, 0);
    gtk_box_pack_start((GtkBox*)label->box, (GtkWidget*)image_box, FALSE, TRUE, 0);
    gtk_widget_show((GtkWidget*)label->box);
  }

  return label;
}

//get the GtkWidget for the label
GtkWidget* yaedTabLabelWidget(const YaedTabLabelHandle label)
{
  return NULL == label ? NULL : (GtkWidget*)label->box;
}

//destroy the label
void yaedTabLabelDestroy(YaedTabLabelHandle label)
{
  if(NULL != label)
  {
    //destroy the widgets
    if(NULL != label->box)
      gtk_widget_destroy((GtkWidget*)label->box); //destroys the text, and icon too

    //free the memory
    g_slice_free(struct YaedTabLabel, label);
  }

  return;
}
