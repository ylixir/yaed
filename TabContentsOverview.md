

# Overview #

This modules provides the interface to the tab content view of the source, including the text area and the location bar

# Types #

## `YaedTabContentsHandle` ##

An opaque type, visible to other modules that references a [YaedTabContents](TabContentsOverview#YaedTabContents.md)
  * `typedef struct YaedTabContents* YaedTabContentsHandle`

## `YaedTabContents` ##

  * `GtkVBox* box`
  * `GtkSourceView* text`
  * `YaedLocationBarHandle location_bar`

# Public Functions #

## `yaedTabContentsNew` ##
  * returns a `YaedTabContentsHandle contents`
  * accepts `const YaedSourceModelHandle model`
  * accepts `const YaedSourceViewHandle view`
  1. we will need a `GtkSourceBuffer* buffer`
  1. set contents to `NULL`
  1. set `buffer` to `NULL`
  1. if neither the `view` or the `model` is `NULL` then:
    1. get `buffer` with `yaedSourceModelGetBuffer`
  1. if `buffer` is not `NULL` then:
    1. we will need a `GtkScrolledWindow* scroll_area`
    1. we will need a `GtkWidget* location_widget`
    1. <font color='green'>we will need a <code>GList* focus_chain</code></font>
    1. <font color='green'>set <code>focus_chain</code> to <code>NULL</code></font>
    1. allocate `contents`
    1. assign the result of `gtk_vbox_new(FALSE, 0)` to `contents->box
    1. assign the results of [yaedLocationBarNew](LocationBarOverview#yaedLocationBarNew.md) to `contents->location_bar`
    1. assign the results of `gtk_source_view_new_with_buffer(buffer)` to `contents->text`
    1. assign the results of `gtk_scrolled_window_new(NULL, NULL)` to `scroll_area`
    1. <font color='green'>use <code>g_list_prepend</code> to add <code>contents-&gt;text</code> to <code>focus_chain</code>, remember to assign <code>focus_chain</code> to the results of the call</font>
    1. <font color='green'>use <code>gtk_conainer_set_focus_chain</code> to set <code>contents-&gt;box</code> to <code>focus_chain</code></font>
    1. use `gtk_container_add` to add the `contents->text` to the `scroll_area`
    1. get the `location_widget` with [yaedLocationBarWidget](LocationBarOverview#yaedLocationBarWidget.md)
    1. use `gtk_box_pack_start` to pack the `scroll_area` and the `location_widget` in the `contents->box`
    1. <font color='green'> use <code>pango_font_description_from_string</code> to load a <code>"monospace 10"</code> font<br>
<ol><li>use <code>gtk_widget_modify_font</code> to set the font<br>
</li><li>use <code>pango_font_description_free</code> to free the font<br>
</li><li>use the three <code>gtk_text_view_set_pixels</code> functions to give us breathing room.<br>
</li><li>use <code>gtk_source_view_set_insert_space_instead_of_tabs</code> to enable replacing spaces with tabs<br>
</li><li><code>gtk_source_view_set_show_line_numbers</code> to show line numbers<br>
</li><li><code>gtk_source_view_set_indent_width</code> and <code>gtk_source_view_set_tab_width</code> to set indent width to 2<br>
</li><li><code>gtk_source_view_set_auto_indent</code> to enable auto indenting<br>
</li><li><code>gtk_source_view_set_indent_on_tab</code> to enable indenting highlighted text<br>
</li><li><code>gtk_source_view_set_highlight_current_line</code> to highlight the current line<br>
</li><li><code>gtk_source_view_set_show_right_margin</code> and <code>gtk_source_view_show_right_margin</code> to show a margin at 80 characters </font>
</li><li>show the source view with <code>gtk_widget_show(contents-&gt;text)</code>
</li><li>show the <code>scroll_area</code> with <code>gtk_widget_show</code>
</li><li>use <code>gtk_widget_show</code> to show the <code>contents-&gt;box</code>
</li><li><font color='green'> add a reference to <code>contents-&gt;box</code> with <code>g_object_ref</code> </font>
</li><li><font color='green'>use <code>g_list_free</code> to free <code>focus_chain</code></font>
</li></ol>  1. return contents

## `yaedTabContentsModelUpdate` ##
  * returns `bool`
  * accepts `contents`
  * accepts `model`
  1. get the model buffer with `yaedSourceModelGetBuffer`
  1. get the view's buffer with `gtk_text_view_get_buffer`
  1. if the model buffer and the view buffer are different set the new buffer with `gtk_text_view_set_buffer`
  1. update the locationbar with [yaedLocationBarModelUpdate](LocationBarOverview#yaedLocationBarModelUpdate.md)
  1. return true

## `yaedTabContentsWidget` ##

  * returns a `GtkWidget*`
  * accepts a `const YaedTabContentsHandle`
  1. if handle is `NULL` then return `NULL`
  1. if handle is not `NULL` then return `handle->box`

## `yaedTabContentsDestroy` ##

  * returns nothing
  * accepts a `YaedTabContentsHandle`
  1. if handle is not `NULL` then:
    1. if `handle->location_bar` is not `NULL` then:
      1. call [yaedLocationBarDestroy](LocationBarOverview#yaedLocationBarDestroy.md) to destroy the `handle->location_bar`
    1. if `handle->box` is not `NULL` then:
      1. <font color='green'> <del>call <code>gtk_widget_destroy(handle-&gt;box)</code> to destroy everything else</del>
<ol><li>call <code>g_object_unref</code> on <code>handle-&gt;box</code> </font>
</li></ol>    1. free the handle
  1. return