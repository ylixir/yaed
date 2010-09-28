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

#include "source-view.h"
#include "tab-label.h"
#include "tab-contents.h"
/*
 * private types
 */

struct YaedSourceView
{
  YaedTabLabelHandle label;
  YaedTabContentsHandle contents;
};

/*
 * public funtions
 */

//get a new view for the given model
YaedSourceViewHandle yaedSourceViewNew(const YaedSourceModelHandle model)
{
  //the return value
  YaedSourceViewHandle view = NULL;

  //extensive sanity checks
  if(NULL != model)
  {
    //allocate the view, and through a tab label in it
    view = g_slice_new(struct YaedSourceView);
    view->label = yaedTabLabelNew(model, view);

    //if we are broken somehow, cleanup a little
    if(NULL == view->label)
    {
      g_slice_free(struct YaedSourceView, view);
      view = NULL;
    }
    //still good
    else
    {
      view->contents = yaedTabContentsNew(model, view);
      //more sanity checking
      if(NULL == view->contents)
      {
        yaedTabLabelDestroy(view->label);
        g_slice_free(struct YaedSourceView, view);
        view = NULL;
      }
    }
  }
  return view;
}

//get the gtk widget that implements the tab label for the view
GtkWidget* yaedSourceViewLabelWidget(const YaedSourceViewHandle view)
{
  //if we got a bogus view, return null, otherwise pass out the label widget
  GtkWidget* widget = NULL;
  if(NULL != view)
  {
    widget = yaedTabLabelWidget(view->label);
  }
  return widget;
}

//get the gtk widget that implement the tab contents for the view
GtkWidget* yaedSourceViewContentsWidget(const YaedSourceViewHandle view)
{
  //if we got a bogus view, return null, otherwise pass out the contents widget
  GtkWidget* widget = NULL;
  if(NULL != view)
  {
    widget = yaedTabContentsWidget(view->contents);
  }
  return widget;
}

//associate the view with a new model
bool yaedSourceViewModelUpdate(YaedSourceViewHandle view,
                            const YaedSourceModelHandle model)
{
  yaedTabLabelModelUpdate(view->label, model);
  yaedTabContentsModelUpdate(view->contents, model);

  return true;
}
