A text editor for programmers primarily.

Next milestone will have feature parity with your basic notepad clones.

Current features:
  * tabs
  * syntax highlighting for many languages
  * filename completion instead of open/close dialogs
  * status of a files saved/modified state

Things we are currently working on for the next release:
  * find
  * command line opening of files
  * better keyboard control when opening/saving files
  * better status (save/overwrite, open/reload, etc)

Currently it is capable of opening, editing and saving files. That's it. It is very close to being about as useful as notepad. Basic features missing are find and command line opening of files. Other than that it's about as useful as notepad clone, but with syntax highlighting.

The code is available under the Artistic 2.0 license.

Goals include:
  * keep out of the way, nothing blocks the ui--no dialogs, pervasive multithreading
  * persistence, close the app, reopen, everything is still there, including unsaved changes
  * vi (and emacs) keybindings
  * robust plugin system