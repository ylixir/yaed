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

#include "utility.h"

//this function checks to see if the directory exists
bool yaedUtilityDirectoryExists(const char* path)
{
  return g_file_test(path, G_FILE_TEST_IS_DIR)?true:false;
}

//this function checks to see if the location exists
bool yaedUtilityLocationExists(const char* path)
{
  return g_file_test(path, G_FILE_TEST_EXISTS)?true:false;
}

//this function checks to see if this is a valid path
bool yaedUtilityLocationHasValidPath(const char* path)
{
  //self explanatory variables used
  gchar* directory_name;
  bool result;

  //get the directory component of the path we are given
  directory_name = g_path_get_dirname(path);
  //does it exist?
  result = g_file_test(directory_name, G_FILE_TEST_IS_DIR)?true:false;
  //cleanup
  g_free(directory_name);

  //done
  return result;
}
