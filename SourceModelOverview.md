

# Overview #

This module implements the "model" for the text data. It will interact with spider to provide a meaningful ui through the [source-view](SourceViewOverview.md) module.

# Types #

## `YaedSourceModelHandle` ##

An opaque type, visible to other modules that references a [YaedSourceModel](SourceModelOverview#YaedSourceModel.md).
  * `typedef struct YaedSourceModel* YaedSourceModelHandle`

## `YaedSourceModel` ##

  * `GtkSourceBuffer* buffer`
  * <font color='green'> <code>char* location</code> </font>
  * unsigned int references

# Data #

No module specific variables.

# Public Functions #

## `yaedSourceModelNew` ##

Create a new model based on a location string. The model is empty, save the location

  * returns a [YaedSourceModelHandle](SourceModelOverview#YaedSourceModelHandle.md) `model`
  * accepts a <font color='green'> <code>const char*</code> </font> that references a location
  1. assign `NULL` to `model`
  1. if `location` is not `NULL`
    1. Allocate a new [YaedSourceModel](SourceModelOverview#YaedSourceModel.md). Call it `model`. fill it with zeros.
    1. Allocate a new `GtkSourceBuffer`.
      1. Call `gtk_source_buffer_new` passing in `NULL` for the tag table.
      1. Assign the result to `model->buffer`
      1. <font color='green'> call <code>g_object_ref</code> to add a reference to the buffer </font>
      1. <font color='green'> use <code>g_signal_connect</code> to link <code>modified-changed</code> signal for <code>model-&gt;buffer</code> to <a href='SourceModelOverview#yaedSourceModelModifiedChangedEvent.md'>yaedSourceModelModifiedChangedEvent</a></font>
    1. <font color='green'> need <code>size_t length, size</code> </font>
    1. <font color='green'> get size info with <a href='UtilityOverview#yaedUtilityUtf8GetSize.md'>yaedUtilityUtf8GetSize</a> </font>
    1. <font color='green'> allocation <code>model-&gt;location</code> </font>
    1. <font color='green'> copy <code>location</code> to <code>model-&gt;location</code> with <code>g_utf8_strncpy</code> </font>
    1. <font color='green'> set <code>model-&gt;location[size-1]</code> to <code>'\0'</code> </font>
  1. return a handle pointing to `model`.

## `yaedSourceModelGetLocation` ##

Return the GString pointing to the location. If we are passed a `NULL` handle, simply return `NULL`.

  * returns a <font color='green'> <code>char*</code> </font>
  * accepts a const [YaedSourceModelHandle](SourceModelOverview#YaedSourceModelHandle.md)

  1. If the handle is `NULL` then return `NULL`
  1. If the handle is not `NULL` then return `handle->location`


## `yaedSourceModelGetBuffer` ##

Return the `GtkSourceBuffer` pointing to the data. If we are passed a `NULL` handle, simply return `NULL`.

  * returns a `GtkSourceBuffer*`
  * accepts a const [YaedSourceModelHandle](SourceModelOverview#YaedSourceModelHandle.md)

  1. If the handle is `NULL` then return `NULL`
  1. If the handle is not `NULL` then return `handle->buffer`

## `yaedSourceModelSetBufferContents` ##

  * returns `bool` indicating success
  * accepts `YaedSourceModelHandle model` indicating the model
  * accepts `GString* content`
  1. use gtk\_text\_buffer\_set\_text to update the contents

## `yaedSourceModelSetLocation` ##

  * returns `bool` indicating success
  * accepts `YaedSourceModelHandle model` indicating the model
  * accepts <font color='green'> <code>const char* location</code> </font>
  1. <font color='green'> need <code>size_t length, size</code>, <code>char* old_tmp</code>, <code>char* new_tmp</code> </font>
  1. <font color='green'> get size info with <a href='UtilityOverview#yaedUtilityUtf8GetSize.md'>yaedUtilityUtf8GetSize</a> </font>
  1. <font color='green'> allocate <code>new_tmp</code> </font>
  1. <font color='green'> copy <code>location</code> to <code>new_tmp</code> with <code>g_utf8_strncpy</code> </font>
  1. <font color='green'> set <code>new_tmp[size-1]</code> to <code>'\0'</code> </font>
  1. <font color='green'> set <code>old_tmp</code> to <code>model-&gt;location</code> </font>
  1. <font color='green'> set <code>model-&gt;location</code> to <code>new_tmp</code> </font>
  1. <font color='green'> free <code>old_tmp</code> </font>
  1. return `true`

## `yaedSourceModelUpdateHighlighting` ##

Attempts to correct the models syntax highlighting.

  * returns bool to indicate success
  * accepts `model`
  * accepts `GString* sample` some sample content
  1. we need:
    1. `GtkSourceLanguage* language`
    1. `gboolean result`
    1. `gchar* contentType`
    1. <font color='green'> <code>char* fileName</code> </font>
    1. `GtkSourceLanguageManager* manager`
  1. <font color='green'> if location is empty set fileName to null, otherwise set it to locations string </font>
  1. get the  manager with `get_source_language_manager_get_default()`
  1. set `contentType` to the result of `g_content_type_guess`
  1. if the result is false then:
    1. free the `contentType`
    1. set content\_type to `NULL`
  1. if we have <font color='green'> a fileName or </font> a contentType then:
    1. set language to `gtk_source_language_manager_guess_language`
    1. set the buffer's language with `gtk_source_buffer_set_language`
  1. free contentType if it's not null
  1. return true

## `yaedSourceModelDestroy` ##

  * returns void
  * accepts the model to destroy
  1. use <font color='green'> <code>g_free</code> </font> to free the location
  1. use <font color='green'> gtk_object_unref </font> to destroy the buffer
  1. free the data associated with the handle

## `yaedSourceModelIncrementReferenceCount` ##

  * returns an unsigned int representing the reference count for this model
  * accepts the model whose reference count needs incremented
  1. increment the value of references
  1. return the value of references

## `yaedSourceModelDecrementReferenceCount` ##

  * returns an unsigned int representing the reference count for this model
  * accepts the model whose reference count needs decremented
  1. decrement the value of references
  1. return the value of references

<font color='green'>
<h2><code>yaedSourceModelSetModified</code></h2>
<ul><li>returns void<br>
</li><li>accepts <code>YaedSourceModelHandle model</code>
</li><li>accepts <code>bool modified</code>
</li></ul><ol><li>use <code>gtk_text_buffer_set_modified</code> on <code>model-&gt;buffer</code> to set the modified bit<br>
</font>
<font color='green'>
<h2><code>yaedSourceModelGetModified</code></h2>
</li></ol><ul><li>returns bool<br>
</li><li>accepts <code>YaedSourceModelHandle model</code>
</li></ul><ol><li>use <code>gtk_text_buffer_get_modified</code> on <code>model-&gt;buffer</code> to get the modified bit<br>
</li><li>return the result<br>
</font></li></ol>

# Private Functions #

<font color='green'>
<h2><code>yaedSourceModelModifiedChangedEvent</code></h2>
<ul><li>returns void<br>
</li><li>accepts a <code>GtkSourceBuffer* buffer</code>
</li><li>accepts a <code>YaedSourceModelHandle model</code>
</li></ul><ol><li>call <a href='SpiderOverview#yaedSpiderRequestViewUpdateForModel.md'>yaedSpiderRequestViewUpdateForModel</a>
</font>