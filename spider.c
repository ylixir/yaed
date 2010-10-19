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
  GtkNotebook* tab_strip;
  struct YaedWindowListElement* next;
} *window_list;

//a list that links the source-models to the windows and source-views
//that they are tied to
struct YaedViewListElement
{
  YaedSourceViewHandle view;
  YaedSourceModelHandle model;
  struct YaedWindowListElement* window_element;
  struct YaedViewListElement* next;
} *view_list;

/*
 * private functions
 */

//create the "new" tab
void yaedSpiderMakeNewTab(GtkNotebook* tab_strip)
{
  GtkImage* child;
  GtkImage* label;
  
  //allocate
  child = (GtkImage*)gtk_image_new_from_stock(GTK_STOCK_NEW,GTK_ICON_SIZE_MENU);
  label = (GtkImage*)gtk_image_new_from_stock(GTK_STOCK_NEW,GTK_ICON_SIZE_MENU);
  //show them
  gtk_widget_show((GtkWidget*)child);
  gtk_widget_show((GtkWidget*)label);
  //add it
  gtk_notebook_append_page(tab_strip, (GtkWidget*)child, (GtkWidget*)label);
}

//create an empty view
struct YaedViewListElement* yaedSpiderCreateEmptyView()
{
  GString* tmp_string;
  struct YaedViewListElement* new_element;
  
  //allocate
  new_element = g_slice_new0(struct YaedViewListElement);
  tmp_string = g_string_new(NULL);
  
  //create the model
  new_element->model = yaedSourceModelNew(tmp_string);
  yaedSourceModelIncrementReferenceCount(new_element->model);
  //create the view
  new_element->view = yaedSourceViewNew(new_element->model);

  //free
  g_string_free(tmp_string, TRUE);
  
  return new_element;
}

//event handler for "new" tab clicks
void yaedSpiderTabSwitched( GtkNotebook* tab_strip,
                            void* page,
                            gint page_num,
                            struct YaedWindowListElement* window_element)
{
  //screw you warnings
  page = NULL;
  
  if(gtk_notebook_get_n_pages(tab_strip) == page_num+1)
  {
    struct YaedViewListElement* new_view_element;
    new_view_element = yaedSpiderCreateEmptyView();
    new_view_element->window_element = window_element;
    new_view_element->next = view_list;
    view_list = new_view_element;

    gtk_notebook_append_page(tab_strip,
      yaedSourceViewContentsWidget(new_view_element->view),
      yaedSourceViewLabelWidget(new_view_element->view));

    yaedSpiderMakeNewTab(tab_strip);
    gtk_notebook_remove_page(tab_strip, page_num);
  }
}

//clean up when a window is closed
gboolean yaedSpiderWindowDeleteEvent(
    GtkWidget* window,
    GdkEvent* event,
    struct YaedWindowListElement* window_element)
{
  struct YaedViewListElement** view_pointer;
  struct YaedWindowListElement** window_pointer;
  
  //keep the compiler happy
  window = NULL;
  event = NULL;
  
  //delete the "new" tab
  gtk_notebook_remove_page(window_element->tab_strip, -1);
  //so i deleted the "new" tab, but sometimes it's still activated?
  //working with gtk is like trying to get a retarded monkey
  //to play chess with me
  gtk_notebook_set_current_page(window_element->tab_strip, 0);
  
  view_pointer = &view_list;
  while(NULL != *view_pointer)
  {
    //convenience, readability
    struct YaedViewListElement* view_element;
    view_element = *view_pointer;
    
    if(view_element->window_element == window_element)
    {
      //kill the tab
      gint page_num =
        gtk_notebook_page_num(window_element->tab_strip,
                              yaedSourceViewContentsWidget(view_element->view));
      gtk_notebook_remove_page(window_element->tab_strip, page_num);
      
      //clean up the view and model data
      yaedSourceViewDestroy(view_element->view);
      if(0 == yaedSourceModelDecrementReferenceCount(view_element->model))
        yaedSourceModelDestroy(view_element->model);
        
      //take the element out of the linked list
      *view_pointer = view_element->next;
      g_slice_free(struct YaedViewListElement, view_element);
      //and continue the loop, now that view_pointer is aiming at the next item
    }
    else  //keep looking
      view_pointer = &((*view_pointer)->next);
  }
  //take the window_element out of the window_list
  for(window_pointer = &window_list;
      *window_pointer != window_element;
      window_pointer=&((*window_pointer)->next)){}
  *window_pointer = (*window_pointer)->next;
  //free it
  g_slice_free(struct YaedWindowListElement, window_element);
  
  //FALSE means the window destruction continues
  return FALSE;
}

//make sure the program quits when the last window is closed
void yaedSpiderWindowDestroy(GtkWidget* window, gpointer user_data)
{
  window = NULL;
  user_data = NULL;
  if(NULL == window_list)
    gtk_main_quit();
}
/*
 * public functions
 */

//set up initial window(s) tab(s) etc.
bool yaedSpiderInit()
{
  //the new0 thingie zeros out the values of our structs for us
  //so we don't have to initialize anything.  yay.
  window_list = g_slice_new0(struct YaedWindowListElement);

  //create the main window
  window_list->window = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);

  //create the tab strip
  window_list->tab_strip = (GtkNotebook*)gtk_notebook_new();
  gtk_container_add((GtkContainer*)window_list->window,
                    (GtkWidget*)window_list->tab_strip);
  
  //show the strip
  gtk_widget_show((GtkWidget*)window_list->tab_strip);

  //create an initial tab
  view_list = yaedSpiderCreateEmptyView();
  view_list->window_element = window_list;

  //and attach the empty view to our new window
  gtk_notebook_prepend_page( window_list->tab_strip,
                            yaedSourceViewContentsWidget(view_list->view),
                            yaedSourceViewLabelWidget(view_list->view) );

  //create the "new" tab
  yaedSpiderMakeNewTab(window_list->tab_strip);
  
  //wire up events
  g_signal_connect( window_list->tab_strip,"switch-page",
                    (GCallback)yaedSpiderTabSwitched,window_list);
  g_signal_connect( window_list->window,"destroy",
                    (GCallback)yaedSpiderWindowDestroy,NULL);
  g_signal_connect( window_list->window,"delete-event",
                    (GCallback)yaedSpiderWindowDeleteEvent,window_list);

  //show the window and we are done
  gtk_widget_show((GtkWidget*)window_list->window);

  return true;
}

//called by any component that wishes a view to close
bool yaedSpiderRequestViewClose(YaedSourceViewHandle view)
{
  struct YaedViewListElement** view_pointer;
  struct YaedViewListElement* view_element;
  struct YaedWindowListElement* window_element;
  gint page_num;
  
  //find the view element
  for(view_pointer = &view_list;
      (*view_pointer)->view != view;
      view_pointer=&((*view_pointer)->next)){}
  view_element = *view_pointer;
  window_element = view_element->window_element;
  
  //take the element out of the list
  *view_pointer = view_element->next;
  //get rid of the "new" tab if this is the last tab in the window
  if(2 == gtk_notebook_get_n_pages(window_element->tab_strip))
    gtk_notebook_remove_page(window_element->tab_strip, -1);
  
  //get our page number and our window widget
  page_num =
    gtk_notebook_page_num(window_element->tab_strip,
                          yaedSourceViewContentsWidget(view_element->view));
  
  //this is mostly to prevent switching to the "new" tab and hanging the program
  if(gtk_notebook_current_page(window_element->tab_strip) == page_num)
    gtk_notebook_prev_page(window_element->tab_strip);
    
  //ditch the view
  gtk_notebook_remove_page(window_element->tab_strip, page_num);
  yaedSourceViewDestroy(view_element->view);
  //destroy the model if we don't have anymore views using it
  if(0 == yaedSourceModelDecrementReferenceCount(view_element->model))
    yaedSourceModelDestroy(view_element->model);
    
  //free the view element
  g_slice_free(struct YaedViewListElement, view_element);
  
  //kill the window if necessary
  if(0 == gtk_notebook_get_n_pages(window_element->tab_strip))
  {
    struct YaedWindowListElement** window_pointer;
    GtkWindow* window;
    
    //take the window_element out of the window_list
    for(window_pointer = &window_list;
        *window_pointer != window_element;
        window_pointer=&((*window_pointer)->next)){}
    *window_pointer = (*window_pointer)->next;
    
    //save our window
    window = window_element->window;
    //free the memory
    g_slice_free(struct YaedWindowListElement, view_element->window_element);
    //kill the window
    gtk_widget_destroy((GtkWidget*)window);
  }
  return true;
}

//called when the user requests a location be (re)loaded
bool yaedSpiderLoadLocation(YaedSourceViewHandle view, const GString* location)
{
  struct YaedViewListElement* view_element = NULL;
  struct YaedViewListElement* iterator = NULL;
  YaedSourceModelHandle new_model = NULL;
  YaedSourceModelHandle old_model = NULL;
  unsigned int new_model_count, old_model_count;
  GString contents;
  bool success = true;
  
  new_model_count = old_model_count = 0;
  //find all the basic values we need for the routine
  for(iterator = view_list; NULL != iterator; iterator = iterator->next)
  {
    if(iterator->view == view)
      view_element = iterator;
    if(TRUE
        == g_string_equal(location,yaedSourceModelGetLocation(iterator->model)))
      new_model = iterator->model;
  }
  old_model = view_element->model;
  //err, special case, empty locations don't share models
  if(0 == location->len)
    new_model = NULL;
  
  //make sure we have some kind of new model to work with so the logic works
  if(NULL == new_model)
    new_model = yaedSourceModelNew(location);
  
  //no matter what we need to increment the new models reference count
  //this is okay since we will decrement old model later, so the net is
  //zero if the two models are the same
  new_model_count = yaedSourceModelIncrementReferenceCount(new_model);
  
  //load/reload
  if(1 == new_model_count || new_model == old_model)
  {if(TRUE==g_file_get_contents(location->str,&contents.str,&contents.len,NULL))
  {
    //yay, we could lod the file, set the contents of the buffer and stuff
    yaedSourceModelSetBufferContents(new_model, &contents);
    yaedSourceModelUpdateHighlighting(new_model, &contents);
    g_free(contents.str);
  }
  else if(new_model != old_model)
  {
    //oh noes, we couldn't load the file! abort!
    new_model_count = yaedSourceModelDecrementReferenceCount(new_model);
    old_model_count = yaedSourceModelIncrementReferenceCount(old_model);
    yaedSourceModelDestroy(new_model);
    new_model=old_model;
    success = false;
  }}
  
  //tie in the new and improved model to the views tuple
  //this some of these operations will essentially be a noop, but the
  //above logic should keep everything consistent
  view_element->model = new_model;
  yaedSourceViewModelUpdate(view, new_model);
  old_model_count = yaedSourceModelDecrementReferenceCount(old_model);
  
  //and if we are done with the old model, toss it
  if(0 == old_model_count)
    yaedSourceModelDestroy(old_model);

  return success;
}

//called when the user requests a location be stored
bool yaedSpiderStoreLocation(YaedSourceViewHandle view, const GString* location)
{
  struct YaedViewListElement* view_element = NULL;
  struct YaedViewListElement* iterator = NULL;
  YaedSourceModelHandle old_model = NULL;
  YaedSourceModelHandle new_model = NULL;
  gchar* contents = NULL;
  GString* string_contents;
  GtkTextIter start;
  GtkTextIter end;
  GtkSourceBuffer* buffer;
  
  //find the stuff, man
  for(iterator = view_list; NULL != iterator; iterator = iterator->next)
  {
    if(iterator->view == view)
      view_element = iterator;
    if(TRUE
        == g_string_equal(location,yaedSourceModelGetLocation(iterator->model)))
      new_model = iterator->model;
  }
  old_model = view_element->model;
  //err, special case, empty locations don't share models
  if(0 == location->len)
    new_model = old_model;
  if(NULL == new_model) //oh noes, we don't have a new model!
    new_model = yaedSourceModelNew(location);
  //get the contents of the buffer
  buffer = yaedSourceModelGetBuffer(old_model);
  gtk_text_buffer_get_bounds((GtkTextBuffer*)buffer, &start, &end);
  contents=gtk_text_buffer_get_text((GtkTextBuffer*)buffer,&start,&end,TRUE);
  string_contents = g_string_new(contents);
  g_free(contents);
  //copy the contents over
  if(new_model != old_model)
    yaedSourceModelSetBufferContents(new_model, string_contents);
  
  //tie it all together and clean up
  yaedSourceModelIncrementReferenceCount(new_model);
  view_element->model = new_model;
  if(0 == yaedSourceModelDecrementReferenceCount(old_model))
    yaedSourceModelDestroy(old_model);
  g_file_set_contents(location->str,
                      string_contents->str,
                      string_contents->len,
                      NULL);
  yaedSourceViewModelUpdate(view, new_model);
  yaedSourceModelUpdateHighlighting(new_model, string_contents);
  g_string_free(string_contents, TRUE);
  
  return true;
}

