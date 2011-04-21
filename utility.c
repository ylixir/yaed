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
  bool result;
  char* expanded_path = yaedUtilityExpandPath(path);
  
  result = g_file_test(expanded_path, G_FILE_TEST_IS_DIR)?true:false;
  
  g_free(expanded_path);
  
  return result;
}

//this function checks to see if the location exists
bool yaedUtilityLocationExists(const char* path)
{
  bool result;
  char* expanded_path = yaedUtilityExpandPath(path);
  
  result = g_file_test(expanded_path, G_FILE_TEST_EXISTS)?true:false;
  
  g_free(expanded_path);
  
  return result;
}

//this function checks to see if this is a valid path
bool yaedUtilityLocationHasValidPath(const char* path)
{
  //self explanatory variables used
  char* directory_name;
  char* expanded_name;
  bool result;

  //get the directory component of the path we are given
  directory_name = g_path_get_dirname(path);
  expanded_name = yaedUtilityExpandPath(directory_name);
  //does it exist?
  result = g_file_test(expanded_name, G_FILE_TEST_IS_DIR)?true:false;
  //cleanup
  g_free(directory_name);
  g_free(expanded_name);

  //done
  return result;
}

//these functions get the dimensions of a null terminated utf8 string
//they get both length and size at the same time so are better than glib funcs
//the size includes the null terminator
size_t yaedUtilityUtf8GetSize(const char* str, size_t* out_length)
{
  //declare and initialize...
  size_t length, size;
  length = 0;
  size = 1; //assume we have at least a null terminator, if this becomes zero
            //we have big trouble
            
  //count it up
  for(char* ptr = (char*)str; '\0' != *ptr; ptr++)
  {
    length++;
    
    //check first for the "normal" case, 7bit ascii
    if(0 == (128 & *ptr)) //128 = 0x80 = 0b1000 0000
      size++;
    //check for valid utf8 headers
    else if(252 == (254 & *ptr)) //252 = 1111 1100, 254 = 1111 1110
    {
      size += 6;
      ptr += 5;
    }
    else if(248 == (252 & *ptr)) //248 = 1111 1000
    {
      size += 5;
      ptr += 4;
    }
    else if(240 == (248 & *ptr)) //240 = 1111 0000
    {
      size += 4;
      ptr += 3;
    }
    else if(224 == (240 & *ptr)) //224 = 1110 0000
    {
      size += 3;
      ptr += 2;
    }
    else if(192 == (224 & *ptr)) //192 = 1100 0000
    {
      size += 2;
      ptr += 1;
    }
    else  //if we got here, we are in trouble, make everything zero and bail
    {
      length = 0;
      size = 0;
      break;
    }
  }
  
  //done, return our results
  if(NULL != out_length)
    *out_length = length;
  return size;
}
//just a wrapper around the getsize function
size_t yaedUtilityUtf8GetLength(const char* str, size_t* out_size)
{
  //get the results
  size_t length, size;
  size = yaedUtilityUtf8GetSize(str, &length);
  //return the results
  if(NULL != out_size)
    *out_size = size;
  return length;
}

//this function expands "~/" style home paths to proper paths
//the return value needs to be freed manually
char* yaedUtilityExpandPath(const char* path)
{
  //set the result to null initially to make later logic a little easier
  char* result = NULL;
  
  //do we start with "~/"?
  if('~' == path[0])
    if('\\' == path[1] || '/' == path[1] || '\0' == path[1])
    {
      //build the path
      const char* home_dir = g_get_home_dir();
      result = g_build_filename(home_dir, path+1, NULL);
    }
  //nothing to but, but we have to return a freeable string :-/
  if(NULL == result)
  {
    //get our dimensions
    size_t size, length;
    size = yaedUtilityUtf8GetSize(path, &length);
    
    //make some roome to copy the string and then copy it
    result = g_malloc(size);
    g_utf8_strncpy(result, path, length);
    //make sure it's terminated
    result[size-1] = '\0';
  }
  
  //done
  return result;
}

