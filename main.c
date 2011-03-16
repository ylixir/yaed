/*******************************************************************************
Copyright (c) 2010 Jon "ylixir" Allen <ylixir@gmail.com>. All rights reserved.

This file is part of yAEd.

yAEd is free software: you can redistribute it and/or modify
it under the terms of the Artistic License 2.0 as published by
the Perl Foundation.

yAEd is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
Artistic License 2.0 for more details.

You should have received a copy of the Artistic License 2.0
along with yAEd.  If not, see <http://www.perlfoundation.org>.

*******************************************************************************/

#include <stdbool.h>
#include <gtk/gtk.h>

#include "spider.h"

int main(int argc, char *argv[])
{
  //make threads work
  g_thread_init(NULL);
  gdk_threads_init();
  //make gtk work
  gtk_init(&argc, &argv);
  //lock the global mutex
  gdk_threads_enter();
  
  //go yaed! be free, fly like the wind!
  if(true == yaedSpiderInit())
    gtk_main();
    
  //release the global mutex
  gdk_threads_leave();
  
  return 0;
}