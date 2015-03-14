

# Introduction #

This module just contains some miscellaneous utility functions that might be useful to any part of this (or any) program.

<font color='green'>
<h1>Public Functions</h1>

<h2><code>yaedUtilityDirectoryExists</code></h2>
This function checks to see if the directory string it is passed is an existing directory. Note that if it isn't a directory, that doesn't mean it doesn't exist as a file.<br>
<ul><li>returns a <code>bool</code>
</li><li>accepts a <code>const char* path</code>
<font color='green'>
</li></ul><ol><li>variables<br>
<ol><li><code>bool result</code>
</li><li><code>char* expanded_path</code>
</li></ol></li><li>use <a href='UtilityOverview#yaedUtilityExpandPath.md'>yaedUtilityExpandPath</a> to get <code>expanded_path</code>
</li><li>use <code>g_file_test</code> with <code>G_FILE_TEST_IS_DIR</code> to get <code>result</code>
</li><li>free <code>expanded_path</code>
</li><li>return <code>result</code>
</font>
<h2><code>yaedUtilityLocationExists</code></h2>
This function checks to see if the location string it is passed actually exists. Note that just because it exists doesn't mean it's not a directory.<br>
</li></ol><ul><li>returns a <code>bool</code>
</li><li>accepts a <code>const char* path</code>
<font color='green'>
</li></ul><ol><li>variables<br>
<ol><li><code>bool result</code>
</li><li><code>char* expanded_path</code>
</li></ol></li><li>use <a href='UtilityOverview#yaedUtilityExpandPath.md'>yaedUtilityExpandPath</a> to get <code>expanded_path</code>
</li><li>use <code>g_file_test</code> with <code>G_FILE_TEST_EXISTS</code> to get <code>result</code>
</li><li>free <code>expanded_path</code>
</li><li>return <code>result</code>
</font>
<h2><code>yaedUtilityLocationHasValidPath</code></h2>
This checks to see if the location has a valid path. This doesn't mean the file does or doesn't exist, just that the directories in the path point to an existing place.<br>
</li></ol><ul><li>returns a <code>bool</code>
</li><li>accepts a <code>const char* path</code>
</li></ul><ol><li>we'll need<br>
<ol><li>a <font color='green'><code>char* directory_name</code></font>
</li><li><font color='green'><code>char* expanded_name</code></font>
</li><li>a <code>bool result</code>
</li></ol></li><li>get the <code>directory_name</code> with <code>g_path_get_dirname</code>
</li><li><font color='green'>use <a href='UtilityOverview#yaedUtilityExpandPath.md'>yaedUtilityExpandPath</a> to get <code>expanded_path</code></font>
</li><li>set <code>result</code> based on the results of <code>g_file_test</code> with <font color='green'> <code>expanded_name</code> </font>
</li><li>free <code>directory_name</code> with <code>g_free</code>
</li><li><font color='green'>free <code>expanded_path</code> with <code>g_free</code></font>
</li><li>return the <code>result</code></li></ol>


<h2><code>yaedUtilityExpandPath</code></h2>
currently this will just expand home paths, eg <code>"~/"</code> becomes <code>"/home/user/"</code> but later may be expanded to include environment variables<br>
<ul><li>accepts <code>const char* path</code>
</li><li>returns <code>char*</code> the expanded path.  this will need to be freed with <code>g_free</code>
</li></ul><ol><li>we'll need <code>char* result</code>
</li><li>set <code>result</code> to <code>NULL</code>
</li><li>check to see that <code>'~'</code> is the first character<br>
<ol><li>if it is check to see if the second character is <code>'\'</code> or <code>'/'</code> <font color='green'> or <code>'\0'</code> </font>
<ol><li>replace the <code>'~'</code> using <code>g_build_filename</code> and <code>g_get_home_dir</code> and assign the result to <code>result</code>
</li></ol></li></ol></li><li>if <code>result</code> is <code>NULL</code>
<ol><li>variables <code>size_t size, length</code>
</li><li>get <code>size</code> and <code>length</code> of <code>path</code> with <code>yaedUtilityUtf8GetSize</code>
</li><li>allocate <code>result</code> with <code>g_malloc</code> and <code>size</code>
</li><li>copy <code>path</code> to <code>result</code> with <code>g_utf8_strncpy</code> and <code>length</code>
</li><li>set <code>result[size-1]</code> to <code>'\0'</code> to make sure it's null terminated<br>
</li></ol></li><li>return <code>result</code></li></ol>

<h2><code>yaedUtilityUtf8GetSize</code></h2>
gets the size in bytes of the null terminated string (including null terminator) and optionally the length in characters<br>
<ul><li>returns <code>size_t</code> for the size<br>
</li><li>accepts <code>const char* str</code> the string to check<br>
</li><li>accepts <code>size_t* out_length</code> will fill this out for you, can be <code>NULL</code> if you don't need the length<br>
</li></ul><ol><li>variables:<br>
</li><li><code>size_t length, size</code>
</li><li><code>char* ptr</code>
</li><li>set <code>length</code> to <code>0</code>
</li><li>set  <code>size</code> to <code>1</code>
</li><li>set <code>ptr</code> to <code>str</code>
</li><li>loop until <code>*ptr</code> is <code>'\0'</code> incrememting by 1 every time<br>
<ol><li>increment <code>length</code>
</li><li>if <code>*ptr &amp; "1000 0000" == "0000 0000</code> then<br>
<ol><li>increment <code>size</code> by one<br>
</li></ol></li><li>else if <code>*ptr &amp; "1111 1110" == "1111 1100</code> then<br>
<ol><li>increment <code>size</code> by six<br>
</li><li>increment <code>ptr</code> by five<br>
</li></ol></li><li>else if <code>*ptr &amp; "1111 1100" == "1111 1000</code> then<br>
<ol><li>increment <code>size</code> by five<br>
</li><li>increment <code>ptr</code> by four<br>
</li></ol></li><li>else if <code>*ptr &amp; "1111 1000" == "1111 0000</code> then<br>
<ol><li>increment <code>size</code> by four<br>
</li><li>increment <code>ptr</code> by three<br>
</li></ol></li><li>else if <code>*ptr &amp; "1111 0000" == "1110 0000</code> then<br>
<ol><li>increment <code>size</code> by three<br>
</li><li>increment <code>ptr</code> by two<br>
</li></ol></li><li>else if <code>*ptr &amp; "1110 0000" == "1100 0000</code> then<br>
<ol><li>increment <code>size</code> by two<br>
</li><li>increment <code>ptr</code> by one<br>
</li></ol></li><li>else ohmaigawd malformed!<br>
<ol><li>set <code>size</code> and <code>length</code> to <code>0</code>
</li><li><code>break</code> out of the loop<br>
</li></ol></li></ol></li><li>if <code>out_length</code> is not <code>NULL</code> set <code>*out_length</code> to length<br>
</li><li>return <code>size</code></li></ol>

<h2><code>yaedUtilitUtf8GetLength</code></h2>
get the length in characters of the null terminated string and optionally the size in bytes (including null terminator)<br>
<ul><li>returns <code>size_t</code> for the length<br>
</li><li>accepts <code>const char* str</code> the string to check<br>
</li><li>accepts <code>size_t* out_size</code> will fill this out for you, can be <code>NULL</code> if you don't need the length<br>
</li></ul><ol><li>variables:<br>
</li><li><code>size_t length, size</code>
</li><li>assign <a href='UtilityOverview#yaedUtilityUtf8GetSize.md'>yaedUtilityUtf8GetSize</a> to <code>size</code> passing in <code>&amp;length</code>
</li><li>if <code>out_size</code> is not <code>NULL</code> set <code>*out_size</code> to <code>size</code>
</li><li>return <code>length</code>
</font>