

# Overview #

Implements the tab label. Actually a label and a close image.

# Types #

## `YaedTabLabelHandle` ##

An opaque type, visible to other modules that references a [YaedTabLabel](TabLabelOverview#YaedTabLabel.md).
  * `typedef struct YaedTabLabel* YaedTabLabelHandle`

## `YaedTabLabel` ##

  * `GtkHBox* box`
  * `GtkLabel* text`

# Data #

No module specific data.

# Functions #

## `yaedTabLabelNew` ##

Create a new tab label widget.
  * returns a `YaedTabLabelHangle label`
  * accepts `const YaedSourceModelHandle model`
  * accepts `const YaedSourceViewHandle view`
  1. set `label` to `NULL`
  1. set a <font color='green'> <code>char* location</code> </font> to `NULL`
  1. if `model is not `NULL` then:
    1. set `location` to the value gotten by [yaedSourceModelGetLocation](SourceModelOverview#yaedSourceModelGetLocation.md)
  1. If neither the `view` nor the `location` is `NULL` then:
    1. we will need a `GtkEventBox* image_box` to catch clicks.
    1. we will need a `GtkImage* image` to display the close icon.
    1. <font color='green'><del>assign the result of <code>g_path_get_basename(location-&gt;str)</code> to <code>file_name</code></del></font>
    1. allocate `label`
    1. asign the result of `gtk_hbox_new(FALSE, 0)` to `label->box`.
    1. assign `gtk_label_new` to `label->text`
    1. call [yaedTabLabelModelUpdate](TabLabelOverview#yaedTabLabelModelUpdate.md)
    1. show the label with `gtk_widget_show(label->text)`
    1. set `image_box` to the results of `gtk_event_box_new()`
    1. set `image` to the result of `gtk_image_new_from_stock(GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU)`
    1. put the `image` in the `image_box` with `gtk_container_add(image_box, image)`
    1. add a callback with `g_signal_connect(image_box, "button_press_event", yaedTabLabelClosePress, view)`
    1. show them with `gtk_widget_show(image)` and `gtk_widget_show(image_box)`
    1. pack the widgets with `gtk_box_pack_start(label->box, label->text, TRUE, TRUE, 0)` and `gtk_box_pack_start(label->box, image_box, FALSE, TRUE, 0)`
    1. show the whole thing with `gtk_widget_show(label->box)`
    1. <font color='green'> use <code>g_object_ref</code> on <code>label-&gt;box</code> to make sure gtk doesn't clean up before we like </font>
  1. return `label`

## `yaedTabLabelModelUpdate` ##

  * returns a bool
  * accepts a `label`
  * accepts a `model`
  1. declare `char* file_name` to hold the base name
  1. <font color='green'>declare <code>GString* label_name</code> to hold the label</font>
  1. assign the result of `g_path_get_basename` to `file_name`, pulling from `model` for input
  1. <font color='green'>check if <code>model</code> is modified with <a href='SourceModelOverview#yaedSourceModelGetModified.md'>yaedSourceModelGetModified</a></font>
    1. <font color='green'>if it is modified set <code>label_name</code> to <code>g_string_new("*")</code></font>
    1. <font color='green'>if it isn't modified set <code>label_name</code> to <code>g_string_new("")</code></font>
  1. if `location` is not empty then
    1. <font color='green'> <del><code>gtk_label_set_text(label-&gt;text, file_name)</code> to <code>label-&gt;text</code></del></font>
    1. <font color='green'><code>g_string_append(label_name, file_name)</code></font>
  1. if `location` is empty then
    1. <font color='green'> <del><code>gtk_label_set_text(label-&gt;text, "[New]")</code></del></font>
    1. <font color='green'> <code>g_string_append(label_name, "[New]")</code> </font>
  1. <font color='green'> <code>gtk_label_set_text(label-&gt;text, label_name-&gt;str)</code> </font>
  1. <font color='green'>free the label text with <code>g_string_free(label_name, TRUE)</code>
</font>
  1. free `file_name`
  1. return true

## `yaedTabLabelWidget` ##
  * returns a `GtkWidget*`
  * accepts a `const YaedTabLabelHandle`
  1. if the handle is `NULL` then return `NULL`
  1. if the handle is not `NULL` then return `handle->box`

## `yaedTabLabelDestroy` ##
  * returns nothing
  * accepts a `YaedTabLabelHandle`
  1. if handle is not `NULL` then:
    1. if `handle->box` is not `NULL` then:
      1. <font color='green'> <del>call <code>gtk_widget_destroy(handle-&gt;box)</code>.  This should also destroy the text, and everything in the label.</del>
<ol><li>call <code>g_object_unref</code> on <code>handle-&gt;box</code> </font>
</li></ol>    1. free the handle
    1. shouldn't need extra checks for text item?
  1. return

# Private Functions #

## `yaedTabLabelClosePress` ##
  * returns `gboolean`
  * accepts `GtkWidget* box`
  * accepts `GdkEventButton* event`
  * accepts `YaedSourceViewHandle view`
  1. call [yaedSpiderRequestViewClose](SpiderOverview#yaedSpiderRequestViewClose.md) and pass it `view`
  1. return `TRUE` to stop other handlers from being invoked