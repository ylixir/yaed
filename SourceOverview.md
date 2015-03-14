

# Modules #

The following is a hierarchal listing of the modules
  * [main](MainOverview.md)
  * [spider](SpiderOverview.md)
  * [source-model](SourceModelOverview.md)
  * [source-view](SourceViewOverview.md)
    * [tab-label](TabLabelOverview.md)
    * [tab-contents](TabContentsOverview.md)
      * [location-bar](LocationBarOverview.md)
  * [utility](UtilityOverview.md)

# Overview #

Each instance of the application will have one or more windows. Each window will have one or more tabs. Each tab will be one [source-view](SourceViewOverview.md). Each [source-view](SourceViewOverview.md) will contain a [location-bar](LocationBarOverview.md) and the source code. The contents of the [source-view](SourceViewOverview.md) will reflect the [source-model](SourceModelOverview.md). Each [source-model](SourceModelOverview.md) will be represented by one or more [source-view's](SourceViewOverview.md). The [source-model](SourceModelOverview.md) contains the text buffer as well as the location. The [source-view](SourceViewOverview.md) will only be able to access the model information by making requests to [spider](SpiderOverview.md). [spider](SpiderOverview.md) will handle all the saving/loading, as well as providing state information to the [source-view](SourceViewOverview.md).

# Data #

The following contains the data each module will need to handle.

## `source-model` [here](SourceModelOverview.md) ##

  * The text buffer. Implemented as a `GtkSourceBuffer`.
  * The location of the text data, only files for now. Implemented as a `GString`.

## `source-view` [here](SourceViewOverview.md) ##

  * The [tab-label](TabLabelOverview.md).
  * The [tab-contents](TabContentsOverview.md). This will contain the location bar and the source view. These will be implemented with `GtkEntry` and `GtkSourceView` respectively.

## `tab-label` [here](TabLabelOverview.md) ##

This will contain a text object and an image object. The text object will be a shortened form of the location. The image object will be a close icon.
<a href='Hidden comment:   * a reference to the [SourceViewOverview source-view] parent'></a>
  * a `GtkHBox` that holds the text and image objects.
  * a `GtkLabel` that contains the text
<a href='Hidden comment:   * a GtkImage that contains the close icon'></a>

## `tab-contents` [here](TabContentsOverview.md) ##

<a href='Hidden comment:   * a reference to the [SourceViewOverview source-view] parent'></a>
  * a `GtkVBox` that holds the source and location bar
  * a [location-bar](LocationBarOverview.md)
  * a `GtkSourceView`

## `location-bar` [here](LocationBarOverview.md) ##

  * <font color='green'><del>a reference to the <a href='TabContentsOverview.md'>tab-contents</a></del></font>
  * <font color='green'>a <code>GtkHBox</code></font>
  * a `GtkEntry`
  * <font color='green'>a <code>YaedSourceModelHandle</code></font>
  * <font color='green'> a <code>YaedSourceViewHandle</code></font>
## `spider` [here](SpiderOverview#Data.md) ##

  * [YaedWindowList](SpiderOverview#YaedWindowList.md)a list of windows. Each window will contain a tab strip. The window will be implemented with `GtkWindow`, and the tab strip with `GtkNotebook`.
  * [YaedViewList](SpiderOverview#YaedViewList.md) a list of [source-views](SourceViewOverview.md) along with the window and [source-model](SourceModelOverview.md) they are tied to.

# Actions #

## `main`->`spider` ##

  * [yaedSpiderInit](SpiderOverview#yaedSpiderInit.md) Request initialization of the app

## `source-view`->`spider` ##

<a href='Hidden comment: 
* Request that the [SpiderOverview spider] load the given location into the [SourceViewOverview source-view]
* Request that the [SpiderOverview spider] remove the [SourceViewOverview source-view]
'></a>

## `source-model`->`spider` ##
<font color='green'>
<ul><li><a href='SpiderOverview#yaedSpiderRequestViewUpdateForModel.md'>yaedSpiderRequestViewUpdateForModel</a> Notify the <code>spider</code> that the model has changed somehow and the views need to update accordingly<br>
</font></li></ul>

<font color='green'>
<h2><code>source-model</code>-><code>utility</code></h2>
<ul><li><a href='UtilityOverview#yaedUtilityUtf8GetSize.md'>yaedUtilityUtf8GetSize</a> find dimensions of a string<br>
</font>
<h2><code>spider</code>-><code>source-view</code></h2></li></ul>

  * [yaedSourceViewNew](SourceViewOverview#yaedSourceViewNew.md) Request a new view with the given location and source buffer
  * [yaedSourceViewSetModel](SourceViewOverview#yaedSourceViewSetModel.md) Request the view update to the given model
  * [yaedSourceViewLabelWidget](SourceViewOverview#yaedSourceViewLabelWidget.md) Request the tab-label gtk widget
  * [yaedSourceViewContentWidget](SourceViewOverview#yaedSourceViewContentWidget.md) Request the tab-contents gtk widget
  * Tell the view to destroy itself

## `spider`->`source-model` ##

  * [yaedSourceModelNew](SourceModelOverview#yaedSourceModelNew.md) Request an empty model (blank location, blank source)
  * [yaedSourceModelSetLocation](SourceModelOverview#yaedSourceModelSetLocation.md) Give the model a new location
  * [yaedSourceModelSetBufferContents](SourceModelOverview#yaedSourceModelSetBufferContents.md) Give the model some new contents
  * [yaedSourceModelGetLocation](SourceModelOverview#yaedSourceModelGetLocation.md) Request the model's location
  * [yaedSourceModelGetBuffer](SourceModelOverview#yaedSourceModelGetBuffer.md) Request the model's source buffer
  * [yaedSourceModelDestroy](SourceModelOverview#yaedSourceModelDestroy.md) Request that the model destroy itself
  * <font color='green'><a href='SourceModelOverview#yaedSourceModelSetModified.md'>yaedSourceModelSetModified</a> let the model know that it's content hasn't been modified from disk</font>

<font color='green'>
<h2><code>spider</code>-><code>utility</code></h2>

<ul><li><a href='UtilityOverview#yaedUtilityExpandPath.md'>yaedUtilityExpandPath</a> expand "~/" style paths to "/home/name/" style paths<br>
</li><li><font color='green'><a href='UtilityOverview#yaedUtilityUtf8GetSize.md'>yaedUtilityUtf8GetSize</a> find dimensions of a string</font>
</font></li></ul>

## `source-view`->`tab-label` ##

  * [yaedTabLabelNew](TabLabelOverview#yaedTabLabelNew.md) Request a new `tab-label` with the given model
  * [yaedTabLabelModelUpdate](TabLabelOverview#yaedTabLabelModelUpdate.md) Request that the `tab-label` be updated to the given model
  * [yaedTabLabelDestroy](TabLabelOverview#yaedTabLabelDestroy.md) Request the `tab-label` to destroy itself
  * [yaedTabLabelWidget](TabLabelOverview#yaedTabLabelWidget.md) Request the gtk container widget

## `source-view`->`tab-contents` ##

  * [yaedTabContentsNew](TabContentsOverview#yaedTabContentsNew.md) Request a new `tab-contents` with the given model
  * [yaedTabContentsModelUpdate](TabContentsOverview#yaedTabContentsModelUpdate.md) Request that the `tab-contents` be updated to the given model
  * [yaedTabContentsDestroy](TabContentsOverview#yaedTabContentsDestroy.md) Tell the `tab-contents` to destroy itself
  * [yaedTabContentsWidget](TabContentsOverview#yaedTabContentsWidget.md) Request the gtk container widget


## `tab-contents`->`location-bar` ##

  * [yaedLocationBarNew](LocationBarOverview#yaedLocationBarNew.md) Request a new `location-bar` with the given location
  * [yaedLocationBarModelUpdate](LocationBarOverview#yaedLocationBarModelUpdate.md) Request that the `location-bar` be updated to the given model
  * [yaedLocationBarDestroy](LocationBarOverview#yaedLocationBarDestroy.md) Tell the `location-bar` to destroy itself
  * [yaedLocationBarWidget](LocationBarOverview#yaedLocationBarWidget.md) Request the gtk container widget

## `tab-label`->`spider` ##

  * [yaedSpiderRequestViewClose](SpiderOverview#yaedSpiderRequestViewClose.md) Notify the `spider` that the user requested the view to close

## `tab-label`->`source-model` ##
  * <font color='green'><a href='SourceModelOverview#yaedSourceModelGetModified.md'>yaedSourceModelGetModified</a> find out if the model has been modified from disk</font>
<font color='green'>
<h2><code>tab-label</code>-><code>utility</code></h2>
<ul><li><a href='UtilityOverview#yaedUtilityUtf8GetSize.md'>yaedUtilityUtf8GetSize</a> find dimensions of a string<br>
</font></li></ul>

## `location-bar`->`spider` ##

  * [yaedSpiderLoadLocation](SpiderOverview#yaedSpiderLoadLocation.md) Notify the `spider` that the user requested the given location be loaded
  * [yaedSpiderStoreLocation](SpiderOverview#yaedSpiderStoreLocation.md) Notify the `spider` that the user requested the given location be stored

## `location-bar`->`source-model` ##
  * <font color='green'><a href='SourceModelOverview#yaedSourceModelGetModified.md'>yaedSourceModelGetModified</a> find out if the model has been modified from disk</font>
  * <font color='green'><a href='SourceModelOverview#yaedSourceModelGetLocation.md'>yaedSourceModelGetLocation</a> find out the on disk location of the model</font>

<font color='green'>
<h2><code>location-bar</code>-><code>utility</code></h2>
<ul><li><a href='UtilityOverview#yaedUtilitDirectoryExists.md'>yaedUtilitDirectoryExists</a> find out if a path is an existing directory<br>
</li><li><a href='UtilityOverview#yaedUtilityLocationHasValidPath.md'>yaedUtilityLocationHasValidPath</a> find out if a path is in an existing directory<br>
</li><li><a href='UtilityOverview#yaedUtilityLocationExists.md'>yaedUtilityLocationExists</a> find out if a path exists<br>
</li><li><font color='green'><a href='UtilityOverview#yaedUtilityExpandPath.md'>yaedUtilityExpandPath</a> expand "~/" style paths to "/home/name/" style paths</font>
</font>