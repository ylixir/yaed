

# Target audience #

Power users. Users that need a dedicated text editor should be relatively computer savvy. They won't need hand holding. They will know common keyboard shortcuts. They will understand how file systems work.

Users are not stupid. Do not ask them if they are sure every time they perform a dangerous operation.

Users are not infallible. If they make a mistake, like overwriting a file on accident, provide a way to recover.


# Design #

The following guidelines should guide the design of the editor:
  * Non-blocking user interface. No modal dialogs, etc.
  * Very minimal user interface. No menu bars, minimal tool bars, etc.
  * Tabbed interface.
  * Very web browser like. Location bar for loading/saving. Back/forward for navigating.
  * Tab on top like chrome
  * Multiple panes, windows, etc, contain a complete copy of the user interface.
  * Snappy, everything should be instant, including startup. Remember:
    1. The users eyes are quick, he will notice little graphics glitches, etc.
    1. The users fingers are slow.

# Implementation Choices #

Highly cross platform. `*`nix, Windows, Mac OS X all supported.

Toolkit used will be GTK. Source code view implemented by GTKSourceView.

Language used will be c99.

Licensed under Artistic 2.0

# Documentation legend #
  * specced but unimplemented documentation is in <font color='red'> red </font>
  * specced behavior with implementation in progress is in <font color='orange'> orange </font>
  * implemented behavior that hasn't made it into a release yet is in <font color='green'> green </font>
  * behavior that is in the current release is in black

# Mock up #
![![](http://wiki.yaed.googlecode.com/hg/images/mockup.png)](http://wiki.yaed.googlecode.com/hg/images/mockup.svg)

This is what the app will look like when it starts up, minus the two tabs. It will contain a fresh tab with a blank source and a blank location bar.

<font color='green'>On startup and when switching tabs, the location bar will NOT have the focus by default</font>

## Tab bar behavior ##
  * <font color='green'>Closing the last tab (non-new) exits the program.<br>
<ul><li>Closing the last (non-new) tab in a window closes the window.</font>
</li><li>Separate tabs referencing the same location will be synchronized.  undos/redos, and content changes will occur in both tabs.<br>
</li><li><font color='red'>Separate tabs referencing the same location will not have synchronized navigation data.</font>
</li><li>A tab containing a new, unsaved file will be labelled <code>[New]</code>.<br>
</li><li><font color='green'>A tab containing a file that has been modified and needs to be saved will be prefixed with a <code>*</code>. eg <code>*main.c</code>.</font>
</li><li>If a modified file is opened in a different tab, the modifications will automatically show up in the new tab.<br>
</li><li>If a file that is open in two tabs is reloaded, then the file will be reloaded in both tabs.<br>
</li><li><font color='red'> Tabs may be rearranged by dragging them.<br>
</li><li>You may drag tabs between windows.<br>
</li><li>Dragging a tab off a window creates a new window containing that tab.<br>
</li><li><font color='green'> There is a special "new" tab in every window<br>
<ul><li>Clicking the new tab opens a new tab. </font>
</li><li>The tab create a new tab anywhere, or a new window by dragging it to the appropriate location.<br>
</li><li><font color='green'> This tab may never be active. </font> </font></li></ul></li></ul>

<font color='green'>
<h2>Location bar behavior</h2>
Contents of the location bar for source panes:<br>
<ul><li><font color='orange'>Text field. Empty if the file is new, and has not been saved. Otherwise, the location of the file.</font>
</li><li>Save button. Visible only when the source has been modified and the text field contains the same location. Not visible while the file is still being opened, or when the source has never been saved anywhere.<br>
</li><li>Save As button. Visible when the text field has been modified to a different valid location (valid path, file need not already exist). Not visible while the file is still being opened. <font color='orange'>Not visible if the source has not been modified and the original location is "" (a new file).</font>
</li><li>Open button. Visible only when the text field has been modified from the source view's contents to a different but valid location.<br>
</li><li>Reload button. Where the open button usually is. Visible only when the text field has not been modified and contains a valid location.<br>
</li><li>Menu Button. Always visible<font color='red'>
<ul><li>Find. Use the location bar to input the search text<br>
<ul><li>Toggle visibility option: highlight or don't highlight the found text<br>
</li><li>Next: jump to the next place where the search text has been found and highlight it.<br>
</li><li>Previous: the obvious counterpart to the next option.</font>
</font>
<font color='orange'>
Behavior when the text field is being edited<br>
</li></ul></li></ul></li><li><font color='green'>a completion list containing valid filenames/directories</font>
<ul><li><font color='green'>make sure <code>"~"</code> directories are handled properly</font>
</li><li>tab completes as far as possible. <code>"c"</code> has matches for <code>"code"</code> and <code>"color"</code> then tab completes to <code>"co"</code>
</li><li>completions that are directories should automatically append a directory separator<br>
</li><li>files that begin with a "." should not be shown unless the "." is specified<br>
</li></ul></li><li><font color='red'>options for open,save,overwrite, find in file</font>
</font></li></ul>

Behavior when text field is edited and `return` or `enter` is pressed:
<font color='green'>
<ul><li>Nothing if the location is not valid or is a directory.<br>
</li><li>If the location does not exist, save the source here, provided the contents of the editor isn't an unmodified new file.<br>
</li><li>If the location exists and the source has not been modified, open, provided the location is different from the original location<br>
</li><li>If the location exists and the source has been modified, do nothing, make them press the open or save as (overwrite) button manually <font color='red'> or select it from the completion list. </font>
</li><li><font color='orange'> when they push enter, the focus should jump to the text edit pane automatically </font>
</font></li></ul>

<font color='green'>
<h2>Editor Window Behavior</h2>

These are defaults, the behavior will eventually be more tune-able with a preferences window.<br>
<ul><li>Monospace font<br>
</li><li>spaces replace tabs<br>
</li><li>line numbers showing<br>
</li><li>tab width/indent width is 2 spaces<br>
</li><li>auto-indenting enabled<br>
</li><li>tab indents highlighted text<br>
</li><li>highlight the current line<br>
</li><li>show the right margin at 80 characters<br>
</li><li>do our best to automatically and intelligently syntax highlight<br>
<ul><li>no matter what, check syntax highlighting whenever the file name changes (via open or save)<br>
</li></ul></li><li>the windows default size should be 600x400<br>
</font></li></ul>

# Design Pattern #
Model-View-Controller design. Models will initially contain only a `source-model`, but eventually also preferences model, state model, etc. View again, initially only a `source-view` but later more. The controller is called `spider`.