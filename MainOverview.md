
# Overview #
All this module really does is get the ball rolling by initializing gtk, having spider do initial setup, and starting the application loop.
# Types #
This module doesn't have any types.
# Data #
This module doesn't have any data.
# Functions #
## `main` ##
  * returns an `int`
  * accepts `int argc` that defines the number of command line arguments
  * accepts `char **argv` that contains the command line arguments
  1. <font color='green'>initialize threads with <code>g_thread_init</code> and <code>gdk_threads_init</code></font>
  1. Initialize Gtk with `gtk_init`
  1. <font color='green'>initialize the global gtk mutex with <code>gdk_threads_enter</code></font>
  1. Tell spider to set up the application with [yaedSpiderInit](SpiderOverview#yaedSpiderInit.md)
  1. Start the application loop with `gtk_main` if the spider init was successful
  1. <font color='green'>release the global gtk mutex with <code>gdk_threads_leave</code></font>
  1. exit with `return 0`