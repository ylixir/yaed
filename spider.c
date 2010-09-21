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

#include "spider.h"

//a list of all the windows that belong to the application
struct YaedWindowList
{
  GtkWindow* window;
  GtkNotebook* tabStrip;
  struct YaedWindowList* next;
} *windows;

//a list that links the source-models to the windows and source-views
//that they are tied to
struct YaedViewList
{
  YaedSourceViewHandle view;
  YaedSourceModelHandle model;
  GtkWindow* window;
} *views;

//set up initial window(s) tab(s) etc.
bool yaedSpiderInit()
{
  GString* tmpString; //a string we will use to initialize our empty model

  //the new0 thingie zeros out the values of our structs for us
  //so we don't have to initialize anything.  yay.
  windows = g_slice_new0(struct YaedWindowList);
  views = g_slice_new0(struct YaedViewList);

  //create the main window
  windows->window = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
  views->window = windows->window;

  //create the tab strip
  windows->tabStrip = (GtkNotebook*)gtk_notebook_new();
  gtk_container_add((GtkContainer*)windows->window,
                    (GtkWidget*)windows->tabStrip);
  gtk_widget_show((GtkWidget*)windows->tabStrip);

  //create a blank model
  tmpString = g_string_new(NULL);
  views->model = yaedSourceModelNew(tmpString);
  g_string_free(tmpString, TRUE);

  //create the empty view
  views->view = yaedSourceViewNew(views->model);

  //and attach the empty view to our new window
  gtk_notebook_append_page( windows->tabStrip,
                            yaedSourceViewContentsWidget(views->view),
                            yaedSourceViewLabelWidget(views->view) );

  //show the window and we are done
  gtk_widget_show((GtkWidget*)windows->window);

  return true;
}

//called by any component that wishes a view to close
bool yaedSpiderRequestViewClose(YaedSourceViewHandle view)
{
  //TODO: implement this
  YaedSourceViewHandle foo = view;
  foo = NULL;
  return false;
}
