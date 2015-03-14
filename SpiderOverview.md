

# Overview #

This module implements the "controller". It handles all the core application logic and passing data and action requests back and forth between the models and the views.

# Private Types #

## <font color='green'> <code>YaedWindowListElement</code> </font> ##

  * `GtkWindow *window`
  * `GtkNotebook *tab_strip`
  * <font color='green'> <code>struct YaedWindowListElement *next</code> </font>

## <font color='green'> <code>YaedViewListElement</code> </font> ##

  * `YaedSourceViewHandle view`
  * `YaedSourceModelHandle model`
  * <font color='green'> <code>struct YaedWindowListElement *window_element</code> </font>
  * <font color='green'> <code>struct YaedViewListElement *next</code> </font>

# Data #
<font color='green'>
<ul><li><code>struct YaedWindowList *window_list</code>
</li><li><code>struct YaedViewList *view_list</code>
</font></li></ul>

# Public Functions #

## `yaedSpiderInit` ##

Initialize the window and view lists, and create a single window with a single empty tab

  * returns a `bool`

  1. <font color='green'> Allocate a new <code>YaedWindowListElement</code> and assign it to <code>window_list</code>.</font>
  1. set <font color='green'> <code>window_list-&gt;next</code> </font> to `NULL`
  1. create a main window
    1. call `gtk_window_new` with `GTK_WINDOW_TOPLEVEL`
    1. <font color='green'>use <code>gtk_window_set_default_size</code> to make the default size 600x400</font>
    1. assign the result of the above call to <font color='green'> <code>window_list-&gt;window</code>
</li></ul><ol><li>create the tab strip<br>
<ol><li>call <code>gtk_notebook_new</code>
</li><li>assign the result of the above call to <font color='green'> <code>window_list-&gt;tab_strip</code> </font>
</li><li>add the tab strip to the window with <code>gtk_container_add</code>
</li><li>show the tab strip with <code>gtk_widget_show</code>
</li></ol></li><li>call <a href='SpiderOverview#yaedSpiderCreateEmptyView.md'>yaedSpiderCreateEmptyView</a>
</li><li>assign the result of the above to <code>view_list</code>
</li><li>assign <code>window_list</code> to <code>view_list-&gt;windowElement</code> </font>
  1. attach the view to the tab strip
    1. get the tab label gtk widget with [yaedSourceViewLabelWidget](SourceViewOverview#yaedSourceViewLabelWidget.md)
    1. get the tab contents gtk widget with [yaedSourceViewContentWidget](SourceViewOverview#yaedSourceViewContentWidget.md)
    1. use the results of the previous calls to call <font color='green'> <code>gtk_notebook_prepend_page</code> </font>
    1. <font color='green'> create the "new" tab<br>
<ol><li>call <a href='SpiderOverview#yaedSpiderMakeNewTab.md'>yaedSpiderMakeNewTab</a> </font>
</li></ol>  1. <font color='green'> wire up <a href='SpiderOverview#yaedSpiderTabSwitched.md'>yaedSpiderTabSwitched</a> with <code>g_signal_connect</code> </font>
  1. <font color='green'> wire up <a href='SpiderOverview#yaedSpiderWindowDestroy.md'>yaedSpiderWindowDestroy</a> with <code>g_signal_connect</code>
</li><li>wire up <a href='SpiderOverview#yaedSpiderWindowDeleteEvent.md'>yaedSpiderWindowDeleteEvent</a> with <code>g_signal_connect</code> </font>
  1. show the window by calling `gtk_widget_show`
  1. exit with `return true`

<font color='green'>
<h2><code>yaedSpiderRequestViewClose</code></h2>

</li></ol><ul><li>returns a <code>bool</code> indicating whether the view was closed<br>
</li><li>accepts a <code>YaedSourceViewHandle view</code>
</li></ul><ol><li>find a <code>YaedViewListElement view_element</code> in the <code>view_list</code>
</li><li>decouple the <code>view_element</code> from the <code>view_list</code>
</li><li>if <code>gtk_notebook_get_n_pages</code> says there are only <code>2</code> tabs:<br>
<ol><li>use <a href='SpiderOverview#yaedSpiderDeleteNewTab.md'>yaedSpiderDeleteNewTab</a> to remove the "new" tab<br>
</li></ol></li><li>get the page number of the tab to close with <code>gtk_notebook_page_num</code>
</li><li>if the page number is the same as a the current page then:<br>
<ol><li><code>gtk_notebook_prev_page</code>
</li></ol></li><li><code>gtk_notebook_remove_page</code> to remove this view's tab<br>
</li><li>destroy the view with <a href='SourceViewOverview#yaedSourceViewDestroy.md'>yaedSourceViewDestroy</a>
</li><li>decrement the model reference count with <a href='SourceModelOverview#yaedSourceModelDecrementReferenceCount.md'>yaedSourceModelDecrementReferenceCount</a>
</li><li>destroy the model with <a href='SourceModelOverview#yaedSourceModelDestroy.md'>yaedSourceModelDestroy</a> if the reference count is <code>0</code>
</li><li>use <code>g_slice_free</code> to free the view elements memory<br>
</li><li>if <code>gtk_notebook_get_n_pages</code> says there are no tabs:<br>
<ol><li>decouple the <code>window_element</code> from the <code>window_list</code>
</li><li>free it with <code>g_slice_free</code>
</li><li>destroy the window with <code>gtk_widget_destroy</code>
</li></ol></li><li>return <code>true</code>
</font>
## `yaedSpiderLoadLocation` ##
<font color='green'></li></ol>


<ul><li>returns a <code>bool</code> indicating whether the location was loaded<br>
</li><li>accepts a <code>YaedSourceViewHandle view</code> indicating the view that needs the location loaded<br>
</li><li>accepts a <code>const char* location</code> indicating the location that needs to be loaded<br>
</li></ul><ol><li>declare a <code>YaedViewListElement* view_element</code> for the view/model/window tuple<br>
</li><li>declare a <code>YaedViewListElement* iterator</code> to use in tuple search loops<br>
</li><li>declare a <code>YaedSourceModelHandle new_model</code> to point to the new model we are loading<br>
</li><li>declare a <code>YaedSourceModelHandle old_model</code> to point to the old model we are unloading<br>
</li><li>set <code>new_model</code> to <code>NULL</code>
</li><li>find <code>view_element</code> by searching for the tuple that has the same <code>view</code> that we were passed<br>
</li><li>piggyback the above loop to find an element in <code>view_list</code> with a model that has the same <code>location</code> as we are given, and assign to <code>new_model</code> if found<br>
</li><li>set <code>old_model</code> to the model in <code>view_element</code>
</li><li>if <code>location</code> is empty then<br>
<ol><li>set <code>new_model</code> to <code>NULL</code>
</li></ol></li><li>if <code>new_model</code> is <code>NULL</code> we need to create a new model<br>
<ol><li><a href='SourceModelOverview#yaedSourceModelNew.md'>yaedSourceModelNew</a>
</li></ol></li><li>increment <code>new_model</code> reference count with <a href='SourceModelOverview#yaedSourceModelIncrementReferenceCount.md'>yaedSourceModelIncrementReferenceCount</a>
</li><li>if the reference count of <code>new_model</code> is <code>1</code> or <code>new_model</code> is the same as <code>old_model</code> then (re)load the location<br>
<ol><li><font color='green'>need a <code>char* expanded_location</code> from <a href='UtilityOverview#yaedUtilityExpandPath.md'>yaedUtilityExpandPath</a></font>
</li><li>if <code>g_file_get_contents</code> succeeds then<br>
<ol><li><a href='SourceModelOverview#yaedSourceModelSetBufferContents.md'>yaedSourceModelSetBufferContents</a>
</li><li><a href='SourceModelOverview#yaedSourceModelUpdateHighlighting.md'>yaedSourceModelUpdateHighlighting</a>
</li><li><a href='SourceModelOverview#yaedSourceModelSetModified.md'>yaedSourceModelSetModified</a> to <code>false</code>
</li><li>free the file contents<br>
</li></ol></li><li>otherwise if <code>new_model</code> is different from <code>old_model</code>
<ol><li>decrement <code>new_model</code> reference count to <code>0</code> with <a href='SourceModelOverview#yaedSourceModelDecrementReferenceCount.md'>yaedSourceModelDecrementReferenceCount</a>
</li><li>increment <code>old_model</code> reference count with <a href='SourceModelOverview#yaedSourceModelIncrementReferenceCount.md'>yaedSourceModelIncrementReferenceCount</a>
</li><li>destroy <code>new_model</code> with <a href='SourceModelOverview#yaedSourceModelDestroy.md'>yaedSourceModelDestroy</a>
</li><li>set <code>new_model</code> to <code>old_model</code>
</li><li>set <code>success</code> to <code>false</code>
</li></ol></li><li><font color='green'>free the <code>expanded_location</code></font>
</li></ol></li><li>tie the <code>new_model</code> to the <code>view_element</code> tuple<br>
</li><li><a href='SourceViewOverview#yaedSourceViewModelUpdate.md'>yaedSourceViewModelUpdate</a>
</li><li>decrement <code>old_model</code> reference count with <a href='SourceModelOverview#yaedSourceModelDecrementReferenceCount.md'>yaedSourceModelDecrementReferenceCount</a>
</li><li>if <code>old_model</code> reference count is <code>0</code> then<br>
<ol><li>destroy <code>old_model</code> with <a href='SourceModelOverview#yaedSourceModelDestroy.md'>yaedSourceModelDestroy</a>
</font></li></ol>

## `yaedSpiderStoreLocation` ##
<font color='green'>
</li></ol><ul><li>returns <code>bool</code> indicating success<br>
</li><li>accepts a handle to a <code>view</code>
</li><li>accepts a <code>const char* location</code> indicating the location that needs to be saved<br>
</li></ul><ol><li>variables<br>
<ol><li><code>YaedViewListElement</code>: <code>view_element</code>
</li><li><code>YaedSourceModelHandle</code>: <code>old_model</code>, <code>new_model</code>
</li><li><font color='green'> <code>char*</code> </font>: <code>contents</code>
</li><li><font color='green'><code>char*</code>: <code>expanded_location</code> </font>
</li></ol></li><li>initialize the above to <code>NULL</code>
</li><li>find the <code>view_element</code> with the <code>view</code>
</li><li>while finding <code>view_element</code> find a <code>new_model</code> with the <code>location</code>
</li><li>set <code>old_model</code> to <code>view_element</code>'s model<br>
</li><li>if <code>location</code> is empty, then set <code>new_model</code> to <code>old_model</code>
</li><li>if <code>new_model</code> is <code>NULL</code> then create a new model<br>
<ol><li>call <a href='SourceModelOverview#yaedSourceModelNew.md'>yaedSourceModelNew</a> with <code>location</code>
</li><li>assign the result of the above to <code>new_model</code>
</li></ol></li><li>get the <code>contents</code> of <code>old_model</code>
<ol><li><code>gtk_text_buffer_get_bounds</code> and <code>gtk_text_buffer_get_text</code> to get <code>contents</code>
</li></ol></li><li>if <code>new_model</code> is different from <code>old_model</code>
<ol><li>use <a href='SourceModelOverview#yaedSourceModelSetBufferContents.md'>yaedSourceModelSetBufferContents</a> to set <code>new_model</code> to <code>contents</code>
</li></ol></li><li>use <a href='SourceModelOverview#yaedSourceModelIncrementReferenceCount.md'>yaedSourceModelIncrementReferenceCount</a> on <code>new_model</code>
</li><li>associate <code>new_model</code> with the <code>view_element</code>
</li><li>use <a href='SourceModelOverview#yaedSourceModelDecrementReferenceCount.md'>yaedSourceModelDecrementReferenceCount</a> on <code>old_model</code>
</li><li>if <code>old_model</code> reference count is <code>0</code> use <a href='SourceModelOverview#yaedSourceModelDestroy.md'>yaedSourceModelDestroy</a>
</li><li><font color='green'>need a <code>expanded_location</code> from <a href='UtilityOverview#yaedUtilityExpandPath.md'>yaedUtilityExpandPath</a></font>
</li><li>write the <code>contents</code> to the location with <code>g_file_set_contents</code>
</li><li>call <a href='SourceViewOverview#yaedSourceViewModelUpdate.md'>yaedSourceViewModelUpdate</a>
</li><li>call <a href='SourceModelOverview#yaedSourceModelUpdateHighlighting.md'>yaedSourceModelUpdateHighlighting</a>
</li><li><a href='SourceModelOverview#yaedSourceModelSetModified.md'>yaedSourceModelSetModified</a> to <code>false</code>
</li><li>free the <code>contents</code>
</li><li><font color='green'>free the <code>expanded_path</code></font>
</font>

<font color='green'>
<h2><code>yaedSpiderRequestViewUpdateForModel</code></h2>
</li></ol><ul><li>returns <code>void</code>
</li><li>accepts <code>YaedSourceModelHandle model</code>
</li></ul><ol><li>we need a <code>struct YaedViewListElement* views</code>
</li><li>set <code>views</code> to <code>view_list</code>
</li><li>loop through <code>views</code> until it is <code>NULL</code>
<ol><li>if <code>views-&gt;model</code> is the same as <code>model</code> call <a href='SourceViewOverview#yaedSourceViewModelUpdate.md'>yaedSourceViewModelUpdate</a>
</font></li></ol>

# Private Functions #

<font color='green'>

<h2>yaedSpiderCreateEmptyView</h2>
</li></ol><ul><li>returns a newly allocated <code>struct YaedViewListElement*</code> with the <code>view</code> and <code>model</code> populated<br>
</li></ul><ol><li>declare <code>struct YaedViewListElement* new_element</code>
</li><li>allocate <code>new_element</code> with <code>g_slice_new0</code>
</li><li>create a blank model<br>
<ol><li>call <a href='SourceModelOverview#yaedSourceModelNew.md'>yaedSourceModelNew</a> with an empty string to get an empty model.<br>
</li><li>assign the result to <code>new_element-&gt;model</code>
</li><li>call <a href='SourceModelOverview#yaedSourceModelIncrementReferenceCount.md'>yaedSourceModelReferenceCount</a> to increment the model count to 1<br>
</li></ol></li><li>create a new view<br>
<ol><li>call <a href='SourceViewOverview#yaedSourceViewNew.md'>yaedSourceViewNew</a> to get the view handle<br>
</li><li>assign the result to <code>new_element-&gt;view</code>
</li></ol></li><li>return <code>new_element</code></li></ol>

<h2>yaedSpiderMakeNewTab ##
  * returns nothing
  * accepts `struct YaedWindowListElement* window_element`
  1. get a `struct YaedViewListElement* new_view_element` with [yaedSpiderCreateEmptyView](SpiderOverview#yaedSpiderCreateEmptyView.md)
  1. assign `window_element` to `new_view_element->window_element`
  1. wire `new_view_element` into the `view_list`
  1. `GtkImage* label` is created with `gtk_image_new_from_stock(GTK_STOCK_NEW, GTK_ICON_SIZE_MENU)`
  1. append the tab with `gtk_append_page`. `child` parameter is from [yaedSourceViewContentsWidget](SourceViewOverview#yaedSourceViewContentsWidget.md)

## yaedSpiderDeleteNewTab ##
  * returns nothing
  * accepts `struct YaedWindowListElement* window_element`
  1. use `gtk_notebook_get_nth_page` with `page_num=-1` to get the `child`
  1. use `child` and [yaedSourceViewContentsWidget](SourceViewOverview#yaedSourceViewContentsWidget.md) to find `view_element` in `view_list`
  1. decouple the `view_element` from `view_list`
  1. use `gtk_notebook_remove_page` to remove the tab
  1. destroy the view with [yaedSourceViewDestroy](SourceViewOverview#yaedSourceViewDestroy.md)
  1. decrement the model reference count with [yaedSourceModelDecrementReferenceCount](SourceModelOverview#yaedSourceModelDecrementReferenceCount.md)
  1. destroy the model with [yaedSourceModelDestroy](SourceModelOverview#yaedSourceModelDestroy.md)
  1. use `g_slice_free` to free the view elements memory

## yaedSpiderTabSwitched ##

called when the tab is switched

  * returns nothing
  * accepts `GtkNotebook* tab_strip`
  * accepts `GtkNotebookPage* page`
  * accepts `gint page_num`
  * accepts `struct YaedWindowListElement* window_element`
  1. if `gtk_notebook_get_n_pages=page_num+1` then:
    1. use `gtk_notebook_get_nth_page` with `page_num=-1` to get the `child`
    1. use `child` and [yaedSourceViewContentsWidget](SourceViewOverview#yaedSourceViewContentsWidget.md) to find `view_element` in `view_list`
    1. make a new tab with  [yaedSpiderMakeNewTab](SpiderOverview#yaedSpiderMakeNewTab.md)
    1. use `child` and [yaedSourceViewLabelWidget](SourceViewOverview#yaedSourceViewLabelWidget.md) with `gtk_set_tab_label` to set the correct tab widget
</font></li></ul></li></ul>

<font color='green'>
<h2>yaedSpiderWindowDeleteEvent</h2>

called when the user requests a window be closed<br>
<ul><li>returns a <code>gboolean</code>
</li><li>accepts a <code>GtkWidget* window</code>
</li><li>accepts a <code>GdkEvent* event</code>
</li><li>accepts a <code>YaedWindowListElement window_element</code>
</li></ul><ol><li>use <a href='SpiderOverview#yaedSpiderDeleteNewTab.md'>yaedSpiderDeleteNewTab</a> to remove the "new" tab<br>
</li><li>and because we can apparently still hang by activating this non-existent tab...set the current tab to the first tab<br>
</li><li>loop through <code>view_list</code>
<ol><li>if the `view_list` element's `window_element` is the same as our `window_element` then
      1. `gtk_notebook_remove_page`
      1. destroy the view with [yaedSourceViewDestroy](SourceViewOverview#yaedSourceViewDestroy.md)
      1. decrement the model reference count with [yaedSourceModelDecrementReferenceCount](SourceModelOverview#yaedSourceModelDecrementReferenceCount.md)
      1. destroy the model with [yaedSourceModelDestroy](SourceModelOverview#yaedSourceModelDestroy.md) if the reference count is `0`
      1. decouple this element from the `view_list`
      1. use `g_slice_free` to free the elements memory
  1. decouple the `window_element` from the `window_list` and free it with `g_slice_free`
  1. return `FALSE` to allow the window destruction to continue

## yaedSpiderWindowDestroy ##

called when the window is being destroyed

  * returns nothing
  * accepts `GtkWidget* object`
  * accepts `gpointer user_data`
  1. if `window_list` is empty then call `gtk_main_quit`
</font>