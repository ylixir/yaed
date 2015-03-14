

# Overview #

This module implements the view of the source code. Each view constists of the the tab contents and it's label and the location bar and the actual code view.

# Public Types #

## `YaedSourceViewHandle` ##

An opaque type, visible to other modules that references a [YaedSourceView](SourceViewOverview#YaedSourceView.md).
  * `typedef struct YaedSourceView* YaedSourceViewHandle`

# Private Types #

## `YaedSourceView` ##

  * `YaedTabLabelHandle label`
  * `YaedTabContentsHandle contents`

# Data #

No module specific variables.

# Functions #

## `yaedSourceViewNew` ##

Pretty self explanatory.

  * returns a [YaedSourceViewHandle](SourceViewOverview#YaedSourceViewHandle.md) `view`
  * accepts a `const YaedSourceModelHandle model`
  1. assign `view` to be `NULL`
  1. if `model` is not `NULL` then:
    1. allocate space for the [YaedSourceView](SourceViewOverview#YaedSourceView.md) called `view`.
    1. call [yaedTabLabelNew](TabLabelOverview#yaedTabLabelNew.md) passing in `model` and `view`.
    1. Assign the result of the call to `view->label`.
    1. If `view->label` is `NULL` then do the following:
      1. Free the `view`
      1. assign `NULL` to the view
    1. If `view->label` is not `NULL` then:
      1. call [yaedTabContentsNew](TabContentsOverview#yaedTabContentsNew.md) passing in `model`, and `view`.
      1. Assign the result of the call to `view->contents`
      1. If `view->contents` is `NULL` then do the following:
        1. call [yaedTabLabelDestroy](TabLabelOverview#yaedTabLabelDestroy.md)
        1. free the `view`
        1. assign `NULL` to the view
  1. return `view`

<font color='green'>
<h2><code>yaedSourceViewDestroy</code></h2>
<ul><li>returns nothing<br>
</li><li>accepts <code>YaedSourceViewHandle view</code>
</li></ul><ol><li>call <a href='TabLabelOverview#yaedTabLabelDestroy.md'>yaedTabLabelDestroy</a> and <a href='TabContentsOverview#yaedTabContentsDestroy.md'>yaedTabContentsDestroy</a> with <code>label</code> and <code>contents</code>
</li><li>use <code>g_slice_free</code> to free the memory<br>
</font></li></ol>

## `yaedSourceViewLabelWidget` ##

  * returns a `GtkWidget* widget`
  * accepts a `const YaedSourceViewHandle view`
  1. assign `NULL` to `widget`
  1. if `view` is not `NULL` then:
    1. call [yaedTabLabelWidget](TabLabelOverview#yaedTabLabelWidget.md) passing in `view->label`
    1. assign the result to `widget`
  1. return `widget`

## `yaedSourceViewContentsWidget` ##

  * returns a `GtkWidget* widget`
  * accepts a `const YaedSourceViewHandle view`
  1. assign `NULL` to `widget`
  1. if `view` is not `NULL` then:
    1. call [yaedTabContentsWidget](TabContentsOverview#yaedTabContentsWidget.md) passing in `view->contents`
    1. assign the result to `widget`
  1. return `widget`

## `yaedSourceViewModelUpdate` ##

  * returns a `bool` indicating success or failure
  * accepts a `view`
  * accepts a `model`
  1. call [yaedTabLabelModelUpdate](TabLabelOverview#yaedTabLabelModelUpdate.md)
  1. call [yaedTabContentsModelUpdate](TabContentsOverview#yaedTabContentsModelUpdate.md)
  1. return `true`