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

#ifndef YAED_UTILITY_H
#define YAED_UTILITY_H

#include <stdbool.h>
#include <gtk/gtk.h>

//this function checks to see if the directory exists
bool yaedUtilityDirectoryExists(const char* path);

//this function checks to see if the location exists
bool yaedUtilityLocationExists(const char* path);

//this function checks to see if this is a valid path
bool yaedUtilityLocationHasValidPath(const char* path);

#endif
