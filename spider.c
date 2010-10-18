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

/*
 * private types
 */

//a list of all the windows that belong to the application
struct YaedWindowListElement
{
  GtkWindow* window;
  GtkNotebook* tabStrip;
  struct YaedWindowListElement* next;
} *windowList;

//a list that links the source-models to the windows and source-views
//that they are tied to
struct YaedViewListElement
{
  YaedSourceViewHandle view;
  YaedSourceModelHandle model;
  struct YaedWindowListElement* windowElement;
  struct YaedViewListElement* next;
} *viewList;

/*
 * private functions
 */

//create an empty view
struct YaedViewListElement* yaedSpiderCreateEmptyView()
{
  GString* tmpString;
  struct YaedViewListElement* newElement;
  
  //allocate
  newElement = g_slice_new0(struct YaedViewListElement);
  tmpString = g_string_new(NULL);
  
  //create the model
  newElement->model = yaedSourceModelNew(tmpString);
  yaedSourceModelIncrementReferenceCount(newElement->model);
  //create the view
  newElement->view = yaedSourceViewNew(newElement->model);

  //free
  g_string_free(tmpString, TRUE);
  
  return newElement;
}

//event handler for "new" tab clicks
void yaedSpiderTabSwitched( GtkNotebook* tabStrip,
                            void* page,
                            gint pageNum,
                            struct YaedWindowListElement* windowElement)
{
  //screw you warnings
  page = NULL;
  
  if(gtk_notebook_get_n_pages(tabStrip) == pageNum+1)
  {
    struct YaedViewListElement* newViewElement;
    newViewElement = yaedSpiderCreateEmptyView();
    newViewElement->windowElement = windowElement;
    newViewElement->next = viewList;
    viewList = newViewElement;

    gtk_notebook_insert_page( tabStrip,
                              yaedSourceViewContentsWidget(newViewElement->view),
                              yaedSourceViewLabelWidget(newViewElement->view),
                              pageNum);
    gtk_notebook_set_current_page(tabStrip, 0);
  }
}
/*
 * public functions
 */

//set up initial window(s) tab(s) etc.
bool yaedSpiderInit()
{
  //the "new" tab contents
  GtkImage* label;
  GtkImage* child;
  
  //the new0 thingie zeros out the values of our structs for us
  //so we don't have to initialize anything.  yay.
  windowList = g_slice_new0(struct YaedWindowListElement);

  //create the main window
  windowList->window = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);

  //create the tab strip
  windowList->tabStrip = (GtkNotebook*)gtk_notebook_new();
  gtk_container_add((GtkContainer*)windowList->window,
                    (GtkWidget*)windowList->tabStrip);
  //create the "new" tab
  label = (GtkImage*)gtk_image_new_from_stock(GTK_STOCK_NEW, GTK_ICON_SIZE_MENU);
  child = (GtkImage*)gtk_image_new_from_stock(GTK_STOCK_NEW, GTK_ICON_SIZE_MENU);
  gtk_notebook_append_page( windowList->tabStrip,
                            (GtkWidget*)child,
                            (GtkWidget*)label);
  gtk_widget_show((GtkWidget*)label);
  gtk_widget_show((GtkWidget*)child);

  
  //show the strip
  gtk_widget_show((GtkWidget*)windowList->tabStrip);

  //create an initial tab
  viewList = yaedSpiderCreateEmptyView();
  viewList->windowElement = windowList;

  //and attach the empty view to our new window
  gtk_notebook_prepend_page( windowList->tabStrip,
                            yaedSourceViewContentsWidget(viewList->view),
                            yaedSourceViewLabelWidget(viewList->view) );

  //wire up events
  g_signal_connect( windowList->tabStrip,"switch-page",
                    (GCallback)yaedSpiderTabSwitched,windowList);

  //show the window and we are done
  gtk_widget_show((GtkWidget*)windowList->window);

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

//called when the user requests a location be (re)loaded
bool yaedSpiderLoadLocation(YaedSourceViewHandle view, const GString* location)
{
  struct YaedViewListElement* viewElement = NULL;
  struct YaedViewListElement* iterator = NULL;
  YaedSourceModelHandle newModel = NULL;
  YaedSourceModelHandle oldModel = NULL;
  unsigned int newModelCount, oldModelCount;
  GString contents;
  bool success = true;
  
  newModelCount = oldModelCount = 0;
  //find all the basic values we need for the routine
  for(iterator = viewList; NULL != iterator; iterator = iterator->next)
  {
    if(iterator->view == view)
      viewElement = iterator;
    if(TRUE
        == g_string_equal(location,yaedSourceModelGetLocation(iterator->model)))
      newModel = iterator->model;
  }
  oldModel = viewElement->model;
  //err, special case, empty locations don't share models
  if(0 == location->len)
    newModel = NULL;
  
  //make sure we have some kind of new model to work with so the logic works
  if(NULL == newModel)
    newModel = yaedSourceModelNew(location);
  
  //no matter what we need to increment the new models reference count
  //this is okay since we will decrement old model later, so the net is
  //zero if the two models are the same
  newModelCount = yaedSourceModelIncrementReferenceCount(newModel);
  
  //load/reload
  if(1 == newModelCount || newModel == oldModel)
  {if(TRUE==g_file_get_contents(location->str,&contents.str,&contents.len,NULL))
  {
    //yay, we could lod the file, set the contents of the buffer and stuff
    yaedSourceModelSetBufferContents(newModel, &contents);
    yaedSourceModelUpdateHighlighting(newModel, &contents);
    g_free(contents.str);
  }
  else if(newModel != oldModel)
  {
    //oh noes, we couldn't load the file! abort!
    newModelCount = yaedSourceModelDecrementReferenceCount(newModel);
    oldModelCount = yaedSourceModelIncrementReferenceCount(oldModel);
    yaedSourceModelDestroy(newModel);
    newModel=oldModel;
    success = false;
  }}
  
  //tie in the new and improved model to the views tuple
  //this some of these operations will essentially be a noop, but the
  //above logic should keep everything consistent
  viewElement->model = newModel;
  yaedSourceViewModelUpdate(view, newModel);
  oldModelCount = yaedSourceModelDecrementReferenceCount(oldModel);
  
  //and if we are done with the old model, toss it
  if(0 == oldModelCount)
    yaedSourceModelDestroy(oldModel);

  return success;
}

//called when the user requests a location be stored
bool yaedSpiderStoreLocation(YaedSourceViewHandle view, const GString* location)
{
  struct YaedViewListElement* viewElement = NULL;
  struct YaedViewListElement* iterator = NULL;
  YaedSourceModelHandle oldModel = NULL;
  YaedSourceModelHandle newModel = NULL;
  gchar* contents = NULL;
  GString* stringContents;
  GtkTextIter start;
  GtkTextIter end;
  GtkSourceBuffer* buffer;
  
  //find the stuff, man
  for(iterator = viewList; NULL != iterator; iterator = iterator->next)
  {
    if(iterator->view == view)
      viewElement = iterator;
    if(TRUE
        == g_string_equal(location,yaedSourceModelGetLocation(iterator->model)))
      newModel = iterator->model;
  }
  oldModel = viewElement->model;
  //err, special case, empty locations don't share models
  if(0 == location->len)
    newModel = oldModel;
  if(NULL == newModel) //oh noes, we don't have a new model!
    newModel = yaedSourceModelNew(location);
  //get the contents of the buffer
  buffer = yaedSourceModelGetBuffer(oldModel);
  gtk_text_buffer_get_bounds((GtkTextBuffer*)buffer, &start, &end);
  contents=gtk_text_buffer_get_text((GtkTextBuffer*)buffer,&start,&end,TRUE);
  stringContents = g_string_new(contents);
  g_free(contents);
  //copy the contents over
  if(newModel != oldModel)
    yaedSourceModelSetBufferContents(newModel, stringContents);
  
  //tie it all together and clean up
  yaedSourceModelIncrementReferenceCount(newModel);
  viewElement->model = newModel;
  if(0 == yaedSourceModelDecrementReferenceCount(oldModel))
    yaedSourceModelDestroy(oldModel);
  g_file_set_contents(location->str, stringContents->str, stringContents->len, NULL);
  yaedSourceViewModelUpdate(view, newModel);
  yaedSourceModelUpdateHighlighting(newModel, stringContents);
  g_string_free(stringContents, TRUE);
  
  return true;
}

