

# Overview #

Initially this will be little more than a GtkEntry, but over time, much more.

# Public Types #

## `YaedLocationBarHandle` ##

Opaque type, references [YaedLocationBar](LocationBarOverview#YaedLocationBar.md)
  * `typedef struct YaedLocationBar* YaedLocationBarHandle`

# Private Types #

## `YaedLocationBar` ##

  * <font color='green'><code>GtkHBox* box</code></font>
  * `GtkEntry* entry`
  * <font color='green'><code>YaedSourceModelHandle model</code></font>
  * <font color='green'><code>YaedSourceViewHandle view</code></font>

<font color='green'>
<h2>Anonymous</h2>
<ul><li><code>enum</code>
<ul><li>COMPLETION_PATH_TEXT<br>
</li><li>COMPLETION_N_COLUMNS<br>
</font></li></ul></li></ul>

# Public Functions #

## `yaedLocationBarNew` ##

  * returns a `YaedLocationBarHandle bar`
  * accepts a `const YaedSourceViewHandle view`
  * accepts a `const YaedSourceModelHandle model`
  1. set bar to `NULL`
  1. declare a `location` <font color='green'> <code>char*</code> </font> initially set to `NULL`
  1. if `model` is not `NULL` get the location string
  1. if `location` is not `NULL` then:
    1. we'll need the following variables
      1. <font color='green'><code>GtkImage* menu_image</code> to pop into the menu button.</font>
      1. <font color='green'><code>GtkButton* menu_button</code> for the menu button.</font>
      1. <font color='green'><code>gchar* location_directory</code> for the directory component of the location</font>
    1. allocate `bar`
    1. <font color='green'>set <code>bar-&gt;model</code> to <code>model</code></font>
    1. <font color='green'>set <code>bar-&gt;view</code> to <code>view</code></font>
    1. <font color='green'>set <code>bar-&gt;box</code> to the result of <code>gtk_hbox_new(FALSE, 0)</code></font>
    1. set `bar->entry` to the result of `gtk_entry_new`
    1. <font color='green'>Give the entry a completion object with <code>gtk_entry_completion_new</code> and <code>gtk_entry_set_completion</code></font>
    1. <font color='green'>give the entry completion a list store</font>
    1. <font color='green'>set the text column with <code>gtk_entry_completion_set_text_column</code> passing in <code>COMPLETION_PATH_TEXT</code></font>
    1. <font color='green'>set <code>menu_button</code> to the result of <code>gtk_button_new</code></font>
    1. <font color='green'>set <code>menu_image</code> to the result of <code>gtk_image_new_from_stock(GTK_STOCK_PREFERENCES, GTK_ICON_SIZE_MENU)</code></font>
    1. <font color='green'>put the <code>menu_image</code> in the <code>menu_button</code> with <code>gtk_container_add</code></font>
    1. use `gtk_entry_set_text` to make the value of `bar->entry` the same as `location->str`
    1. <font color='green'>use <code>g_path_get_dirname</code> to get <code>location_directory</code> from <code>location-&gt;str</code></font>
    1. <font color='green'>call <a href='LocationBarOverview#yaedLocationBarRebuildCompletion.md'>yaedLocationBarRebuildCompletion</a> with <code>location_directory</code></font>
    1. <font color='green'>call <a href='LocationBarOverview#yaedLocationBarUpdateIconActions.md'>yaedLocationBarUpdateIconActions</a> with <code>location-&gt;str</code> and <code>bar</code></font>
    1. <font color='green'>use <code>gtk_box_pack_start</code> with <code>expand</code> and <code>fill</code> set to <code>TRUE</code> to pack <code>bar-&gt;entry</code> into <code>bar-&gt;box</code> </font>
    1. <font color='green'>use <code>gtk_box_pack_start</code> with <code>expand</code> and <code>fill</code> set to <code>FALSE</code> to pack <code>menu_button</code> into <code>bar-&gt;box</code></font>
    1. use `g_signal_connect(bar->entry, "icon-press", yaedLocationBarIconPress, view)`
    1. <font color='green'>use <code>g_signal_connect</code> to connect <code>bar-&gt;entry</code> with <code>insert-text</code> and <code>yaedLocationBarInsertText</code> passing in <code>bar</code> for user data.</font>
    1. <font color='green'>use <code>g_signal_connect</code> to connect <code>bar-&gt;entry</code> with <code>delete-text</code> and <code>yaedLocationBarDeleteText</code> passing in <code>bar</code> for user data.</font>
    1. <font color='green'>use <code>g_signal_connect</code> to connect <code>bar-&gt;entry</code> with <code>activate</code> and <a href='LocationBarOverview#yaedLocationBarIconPress.md'>yaedLocationBarIconPress</a> passing in <code>bar</code> for user data.</font>
    1. <font color='green'>show <code>menu_image</code> with <code>gtk_widget_show</code></font>
    1. <font color='green'>show <code>menu_button</code> with <code>gtk_widget_show</code></font>
    1. show `bar->entry` with `gtk_widget_show`
    1. <font color='green'>show <code>bar-&gt;box</code> with <code>gtk_widget_show</code></font>
    1. <font color='green'>use <code>g_object_ref</code> on <code>bar-&gt;box</code> to make sure gtk doesn't clean up before we like</font>
    1. <font color='green'>use <code>g_free</code> to free <code>location_directory</code></font>
  1. return `bar`

## `yaedLocationBarModelUpdate` ##
  * returns bool
  * accepts `bar`
  * accepts `model`
  1. variables:
    1. <font color='green'><code>gchar* location_directory</code> the directory of the location</font>
  1. set the location with `gtk_entry_set_text`
  1. <font color='green'>set <code>bar-&gt;model</code> to <code>model</code></font>
  1. <font color='green'>call <a href='LocationBarOverview#yaedLocationBarUpdateIconActions.md'>yaedLocationBarUpdateIconActions</a> with <code>gtk_entry_get_text</code> and <code>bar</code></font>
  1. <font color='green'>get the <code>location_directory</code> with <code>g_path_get_dirname</code></font>
  1. <font color='green'>call <a href='LocationBarOverview#yaedLocationBarRebuildCompletion.md'>yaedLocationBarRebuildCompletion</a> with <code>location_directory</code></font>
  1. <font color='green'>cleanup with <code>g_free</code> on <code>location_directory</code></font>
  1. return true

## `yaedLocationBarWidget` ##

  * returns a `GtkWidget*`
  * accepts a `const YaedLocationBarHandle`
  1. if handle is `NULL` return `NULL`
  1. <font color='green'>if handle is not <code>NULL</code> then return <code>handle-&gt;box</code></font>

## `yaedLocationBarDestroy` ##

  * returns nothing
  * accepts a `YaedLocationBarHandle`
  1. if the handle is not `NULL` then:
    1. if `handle->entry` is not `NULL` then:
      1. <font color='green'>use <code>g_object_unref</code> on <code>bar-&gt;box</code></font>
    1. free `handle`
  1. return

# Private Functions #

<font color='green'>
<h2><code>yaedLocationBarActivate</code></h2>
this is called when the user presses "enter" in the location bar<br>
<ul><li>accepts <code>GtkEntry* entry</code>
</li><li>accepts <code>YaedLocationBarHandle bar</code>
</li><li>returns <code>void</code>
</li></ul><ol><li>variables:<br>
<ol><li><code>char* new_text</code> the string in the location bar<br>
</li></ol></li><li>get <code>new_text</code> with <code>gtk_entry_get_text</code>
</li><li>check to ensure that <a href='UtilityOverview#yaedUtilityDirectoryExists.md'>yaedUtilityDirectoryExists</a> returns <code>false</code> for <code>new_text</code> and that <a href='UtilityOverview#yaedUtilityLocationHasValidPath.md'>yaedUtilityLocationHasValidPath</a> returns <code>true</code>
<ol><li>if <a href='UtilityOverview#yaedUtilityLocationExists.md'>yaedUtilityLocationExists</a> is <code>false</code> then<br>
<ol><li>if <a href='SourceModelOverview#yaedSourceModelGetModified.md'>yaedSourceModelGetModified</a> is <code>true</code> or <code>g_utf8_collate</code> doesn't return <code>0</code> with <a href='SourceModelOverview#yaedSourceModelGetLocation.md'>yaedSourceModelGetLocation</a> and <code>""</code>
<ol><li>save the file to <code>new_text</code> with <a href='SpiderOverview#yaedSpiderStoreLocation.md'>yaedSpiderStoreLocation</a>
</li></ol></li></ol></li><li>if <a href='UtilityOverview#yaedUtilityLocationExists.md'>yaedUtilityLocationExists</a> is <code>true</code> and <a href='SourceModelOverview#yaedSourceModelGetModified.md'>yaedSourceModelGetModified</a> is <code>false</code> then<br>
<ol><li>if <code>g_utf8_collate</code> doesn't return <code>0</code> with <code>new_text</code> and <a href='SourceModelOverview#yaedSourceModelGetLocation.md'>yaedSourceModelGetLocation</a> then<br>
<ol><li>open <code>new_text</code>
</li><li>open <code>new_text</code> with <a href='SpiderOverview#yaedSpiderLoadLocation.md'>yaedSpiderLoadLocation</a>
</font></li></ol></li></ol></li></ol></li></ol>

## `yaedLocationBarIconPress` ##

called when the user presses an icon
  * returns `void`
  * accepts `GtkEntry* entry`
  * accepts `GtkEntryIconPosition position`
  * accepts `GdkEvent* event`
  * accepts `YaedSourceViewHandle view`
  1. declare a <font color='green'> <code>const char* location</code> </font>
  1. <font color='green'><del>assign <code>g_string_new_len</code> to <code>location</code> using <code>gtk_entry_get_text</code> and <code>gtk_entry_get_text_length</code></del></font>
  1. <font color='green'> assign <code>location</code> using <code>gtk_entry_get_text</code> </font>
  1. if position is GTK\_ENTRY\_ICON\_SECONDARY then:
    1. call `yaedSpiderLoadLocation` with `view` and `location`
  1. if position is GTK\_ENTRY\_ICON\_PRIMARY then:
    1. call `yaedSpiderStoreLocation` with `view` and `location`
  1. return

<font color='green'>
<h2><code>yaedLocationBarInsertText</code></h2>

called when (before) the user inserts text into the location bar<br>
<ul><li>accepts <code>GtkEntry* entry</code>
</li><li>accepts <code>gchar* incoming_text</code>
</li><li>accepts <code>gint incoming_size</code> in bytes, <code>-1</code> if null terminated<br>
</li><li>accepts <code>gint* incoming_position</code> the position at which the text will be inserted in characters<br>
</li><li>accepts <code>YaedLocationBarHandle bar</code>
</li></ul><ol><li>variables we will need<br>
<ol><li><code>const gchar* old_text=gtk_entry_get_text(entry)</code> the string before the insertion<br>
</li><li><code>gchar* new_text</code> the string after the insertion<br>
</li><li><font color='green'><code>gchar* new_directory</code> the new directory</font>
</li><li><font color='green'><code>gchar* old_directory</code> the old directory</font>
</li><li><code>size_t incoming_length</code> the character length of the incoming text<br>
</li><li><code>ptrdiff_t new_size</code> the number of bytes in the new text (minus the null terminator)<br>
</li><li><code>size_t old_length</code> the length (in characters) of the old text<br>
</li><li><code>ptrdiff_t old_size</code> the size (in bytes) of the old text<br>
</li><li><code>gchar* built_ptr</code> this will be the pointer to keep track of the what we've built<br>
</li></ol></li><li>i think it's possible for a utf8 string to have null bytes, so regular C functions won't work, we'll use the glib utf8 functions<br>
</li><li><code>incoming_size</code> may not be an actual size, which is not useful. we need to get the actual size by the roundabout length->size method.<br>
<ol><li>get <code>incoming_length</code> with <code>g_utf8_strlen</code> passing <code>incoming_size</code> for the <code>max</code> parameter<br>
</li><li>get the new <code>incoming_size</code> with <code>g_utf8_offset_to_pointer</code>, passing in <code>incoming_length</code> and then doing the obvious pointer math<br>
</li></ol></li><li>we need the length of the old text in bytes to calculate the new text size, but no direct way to do that<br>
<ol><li>get the <code>old_length</code> with <code>g_utf8_strlen</code> using <code>-1</code> for the <code>max</code> parameter<br>
</li><li>get the <code>old_size</code> using <code>g_utf8_offset_to_pointer</code> using <code>old_length</code> for the offset and doing the appropriate pointer math<br>
</li></ol></li><li>just for readability, get the <code>new_size</code>
<ol><li><code>new_size</code> is <code>old_size+incoming_size</code>
</li></ol></li><li>we'll need to allocate the <code>new_text</code>
<ol><li>use <code>g_slice_alloc</code> with a size of <code>new_size+1</code>
</li></ol></li><li>now we just need to build the new string<br>
<ol><li>use <code>g_utf8_strncpy</code> to copy the first <code>incoming_position</code> characters of <code>old_text</code> to <code>new_text</code>
</li><li>use <code>g_utf8_offset_to_pointer</code> to get <code>built_ptr</code> passing in <code>incoming_position</code> for the offset<br>
</li><li>use <code>g_utf8_strncpy</code> to copy <code>incoming_text</code> to <code>built_ptr</code>
</li><li>use <code>g_utf8_strncpy</code> to copy <code>old_text+(built_ptr-new_text)</code> to <code>built_ptr+incoming_size</code> passing in <code>old_length-incoming_position</code> as the <code>n</code> parameter<br>
</li></ol></li><li><font color='green'>get the <code>old_directory</code> from <code>old_text</code> with <code>g_path_get_dirname</code></font>
</li><li><font color='green'>get the <code>new_directory</code> from <code>new_text</code> with <code>g_path_get_dirname</code></font>
</li><li><font color='green'>check if the result of <code>g_utf8_collate</code> with <code>new_directory</code> and <code>old_directory</code> is not zero</font>
<ol><li><font color='green'>if so then call <a href='LocationBarOverview#yaedLocationBarRebuildCompletion.md'>yaedLocationBarRebuildCompletion</a> with <code>new_directory</code></font>
</li></ol></li><li><font color='green'>call <a href='LocationBarOverview#yaedLocationBarUpdateIconActions.md'>yaedLocationBarUpdateIconActions</a> with <code>new_text</code> and <code>bar</code></font>
</li><li><font color='green'>use <code>g_free</code> on <code>new_directory</code> and <code>old_directory</code></font>
</li><li>free the <code>new_text</code> with <code>g_slice_free1</code> with a size of <code>new_size+1</code></li></ol>

<h2><code>yaedLocationBarDeleteText</code></h2>

called when the user deletes text from the location bar<br>
<ul><li>accepts <code>GtkEntry* entry</code>
</li><li>accepts <code>gint start_position</code> in characters<br>
</li><li>accepts <code>gint end_position</code> in characters, if neg, then delete to end<br>
</li><li>accepts <code>YaedLocationBarHandle bar</code>
</li></ul><ol><li>variables that we'll need<br>
<ol><li><code>const gchar* old_text=gtk_entry_get_text(entry)</code> the string before the deletion<br>
</li><li><code>gchar* new_text</code> the string after the deletion<br>
</li><li><code>gchar* end_text</code> the part of the string after the part to be deleted<br>
</li><li><font color='green'><code>gchar* new_directory</code> the new directory</font>
</li><li><font color='green'><code>gchar* old_directory</code> the old directory</font>
</li><li><code>size_t old_length</code> the length of the old text<br>
</li><li><code>ptrdiff_t new_size</code> the size of the new text<br>
</li><li><code>ptrdiff_t start_size</code> the size of the pre-delete chunk<br>
</li></ol></li><li>of course we'll need the length of the old string<br>
<ol><li>get <code>old_length</code> from <code>g_utf8_strlen</code> passing in a <code>max</code> of <code>-1</code>
</li></ol></li><li>if the end position is negative we'll want to delete to the end<br>
<ol><li>if <code>end_position</code> is less than zero make <code>end_position</code> the same as <code>old_length</code>
</li></ol></li><li>can <code>end_position</code> be less then <code>start_position</code>? if so than do the following<br>
<ol><li><code>end_position ^= start_position</code>
</li><li><code>start_position ^= end_position</code>
</li><li><code>end_position ^= start_position</code>
</li></ol></li><li>now get the location of the part after deletion<br>
<ol><li>set <code>end_text</code> with the result of <code>g_utf8_offset_to_pointer</code>, passing in <code>old_text</code> and <code>end_position</code>
</li></ol></li><li>and we'll need the size of the new string (<code>new_size</code>)<br>
<ol><li>use <code>g_utf8_offset_to_pointer</code> with <code>start_position</code> to get the <code>start_size</code>
</li><li>use <code>g_utf8_offset_to_pointer</code> with <code>old_length-end_position</code> and add it to <code>start_size</code> to get the <code>new_size</code>
</li></ol></li><li>build the new string<br>
<ol><li>allocate the space with <code>g_slice_alloc(new_size+1)</code>
</li><li>copy the first half with <code>g_utf8_strncpy</code>
</li><li>copy the second half with <code>g_utf8_strncpy</code>
</li></ol></li><li><font color='green'>get the <code>old_directory</code> from <code>old_text</code> with <code>g_path_get_dirname</code></font>
</li><li><font color='green'>get the <code>new_directory</code> from <code>new_text</code> with <code>g_path_get_dirname</code></font>
</li><li><font color='green'>check if the result of <code>g_utf8_collate</code> with <code>new_directory</code> and <code>old_directory</code> is not zero</font>
<ol><li><font color='green'>if so then call <a href='LocationBarOverview#yaedLocationBarRebuildCompletion.md'>yaedLocationBarRebuildCompletion</a> with <code>new_directory</code></font>
</li></ol></li><li><font color='green'>call <a href='LocationBarOverview#yaedLocationBarUpdateIconActions.md'>yaedLocationBarUpdateIconActions</a> with <code>new_text</code> and <code>bar</code></font>
</li><li><font color='green'>use <code>g_free</code> on <code>new_directory</code> and <code>old_directory</code></font>
</li><li>clean up with <code>g_slice_free1(new_size+1, new_text)</code></li></ol>

<h2><code>yaedLocationBarRebuildCompletion</code></h2>
This will create a new liststore model for the location bar's completion based on the directory passed in.<br>
<ul><li>accepts a <code>YaedLocationBarHandle bar</code> the bar<br>
</li><li>accepts a <code>gchar* directory</code> utf8 directory to build from<br>
</li></ul><ol><li>variables:<br>
<ol><li><code>GtkListStore* list</code> to store the results<br>
</li></ol></li><li>get the <code>list</code> from the <code>bar</code>
</li><li>clear the list with <code>gtk_list_store_clear</code><font color='green'>
</li><li><a href='LocationBarOverview#yaedLocationBarAppendDirectoryContentsToList.md'>yaedLocationBarAppendDirectoryContentsToList</a> with <code>directory, list</code> to append the contents<br>
</font>
<h2><code>yaedLocationBarAppendDirectoryContentsToList</code></h2>
This is a convenience function, used by <a href='LocationBarOverview#yaedLocationBarRebuildCompletion.md'>yaedLocationBarRebuildCompletion</a> to fill append the contents of a directory to a list store<br>
</li></ol><ul><li>accepts the following:<br>
<ul><li><code>const gchar* directory</code>
</li><li><code>GtkListStore* list</code>
</li></ul></li><li>returns nothing (void)<br>
</li></ul><ol><li>variables used:<br>
<ol><li><font color='green'><code>char* expanded_path</code></font>
</li><li><code>GDir* current_directory</code>
</li><li><code>GtkTreeIter iter</code> the iterator for adding items to the tree<br>
</li></ol></li><li>use <a href='UtilityOverview#yaedUtilityExpandPath.md'>yaedUtilityExpandPath</a> to get <code>expanded_path</code> from <code>directory</code>
</li><li><font color='green'>use <code>current_directory = g_dir_open(expanded_path, 0, NULL)</code> to see if the path is a directory.</font>
</li><li>if <code>NULL != current_directory</code> then<br>
<ol><li>need <code>gchar* contents</code>
</li><li>iterate <code>contents</code> with <code>g_dir_read_name</code> until <code>NULL</code> is returned<br>
<ol><li>need <code>gchar* contents_path</code>
</li><li>build <code>contents_path</code> with <code>g_build_filename</code> and <code>directory</code> and <code>contents</code>
</li><li>append a new entry with <code>gtk_list_store_append</code> using <code>list</code> and <code>&amp;iter</code>
</li><li>use <code>gtk_list_store_set</code> to append <code>contents_path</code> in the <code>COMPLETION_PATH_TEXT</code> column<br>
</li><li><code>g_free</code> the <code>contents_path</code>
</li></ol></li><li>close <code>current_directory</code> with <code>g_dir_close</code>
</li></ol></li><li><font color='green'>free <code>expanded_path</code> with <code>g_free</code></font></li></ol>

<h2><code>yaedLocationBarUpdateIconActions</code></h2>
<ul><li>returns nothing (<code>void</code>)<br>
</li><li>accepts <code>YaedLocationBarHandle bar</code>
</li><li>accepts <code>const char* bar_location</code> the location that the location bar is/will point to<br>
</li></ul><ol><li>variables:<br>
<ol><li><code>char* model_location</code> the location that the model is pointing to<br>
</li><li><code>bool model_modified</code> whether the contents of the model have been modified since they were saved/opened<br>
</li><li><code>bool location_modified</code> whether the <code>model_location</code> and the <code>bar_location</code> are the same<br>
</li><li><code>const gchar* primary_icon</code> what will be put in the "save" location<br>
</li><li><code>const gchar* secondary_icon</code> what will be put in the "open" location<br>
</li></ol></li><li>set both <code>primary_icon</code> and <code>secondary_icon</code> to <code>NULL</code>
</li><li>assign the results of <a href='SourceModelOverview#yaedSourceModelGetModified.md'>yaedSourceModelGetModified</a> to <code>model_modified</code>
</li><li>assign the results of <a href='SourceModelOverview#yaedSourceModelGetLocation.md'>yaedSourceModelGetLocation</a> to <code>model_location</code>
</li><li>use <code>g_utf8_collate</code> to compare the <code>model_location</code> and <code>bar_location</code> and set <code>location_modified</code> appropriately<br>
</li><li>if <code>location_modified</code> is <code>true</code> then:<br>
<ol><li>if <a href='UtilityOverview#yaedUtilityDirectoryExists.md'>yaedUtilityDirectoryExists</a> is <code>false</code> and <a href='UtilityOverview#yaedUtilityLocationHasValidPath.md'>yaedUtilityLocationHasValidPath</a> is <code>true</code> of <code>bar_location</code> and <code>g_utf8_collate</code> says <code>bar_location</code> is not <code>""</code>
<ol><li>set <code>primary_icon</code> to <code>GTK_STOCK_SAVE_AS</code>
</li></ol></li><li>if <a href='UtilityOverview#yaedUtilityDirectoryExists.md'>yaedUtiliytDirectoryExists</a> is <code>false</code> and <a href='UtilityOverview#yaedUtilityLocationExists.md'>yaedUtilityLocationExists</a> is <code>true</code> of <code>bar_location</code>
<ol><li>set <code>secondary_icon</code> to <code>GTK_STOCK_OPEN</code>
</li></ol></li></ol></li><li>if <code>location_modified</code> is <code>false</code> then:<br>
<ol><li>if <code>g_utf8_collate</code> says that <code>model_location</code> is not the same as <code>""</code> then<br>
<ol><li>if <a href='SourceModelOverview#yaedSourceModelGetModified.md'>yaedSourceModelGetModified</a> is <code>true</code>
<ol><li>set <code>primary_icon</code> to <code>GTK_STOCK_SAVE</code>
</li></ol></li><li>set <code>secondary_icon</code> to <code>GTK_STOCK_REFRESH</code>
</li></ol></li></ol></li><li>set the primary icon with <code>gtk_entry_set_icon_from_stock(bar-&gt;entry, GTK_ENTRY_ICON_PRIMARY, primary_icon)</code>
</li><li>set the secondary icon with <code>gtk_entry_set_icon_from_stock(bar-&gt;entry, GTK_ENTRY_ICON_SECONDARY, secondary_icon)</code>
</font>