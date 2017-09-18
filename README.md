# matlab-sfunc-http
Example of making HTTP Requests with C++ MatLab S-Function Code



## File Explanation
#### ```sfunc.cpp``` 
contains C++ Code that gets a random numer from https://www.random.org/integers/?num=1&min=1&max=100&col=5&base=10&format=plain&rnd=new. The number gets returned as plain text document, making it easy to parse and use in the Code, that's why I chose this website.

#### ```simulink_model.slx```
contains the SimuLink Model that is already set up for the function. You should be able to open and run it directly.


#### ```sfunc.mexmaci64``` 
is the compiled version of _sfunc.cpp_. See **Compiling with _mex_** for more information on that. Yes I use OSX, for those who are interested in it;).


## Usage
1. Open the files in your Matlab Environment (mine is R2017a v9.2.0.556344).
2. Start Simulink and open the simulink model.
3. Compile ```sfunc.cpp``` with ```mex```
4. Click the "Play" / "Run" Button (easy, huh?:) )


## Compiling with ```mex```
#### Unix
I've used [cURL](https://curl.haxx.se/libcurl/) in this code, simply because it's easy. You'll find many examples of how to install libcurl on your computer, so I won't explain that here as well. After that, you'll have to find the cURL library on your computer in order to tell ```mex``` where ```#include <curl/curl.h>``` is at, so that everything compiles properly. 

For me, the compilation command looked like this: ```mex sfunc.cpp -L/usr/bin -lcurl```.
Or generalizing it: ```mex filename.cpp -L/path/to/bin -lbinary```
**IMPORTANT: As with most compilers, no space is allowed after -L or -l !**

After properly compiling the ```.cpp``` file, there will be a new file in your directory. Since I work on a MacBook, it's called ```sfunc.mexmaci64```. If you're working with a Mac as well, there's a chance that that file works for you already.

#### Windows
Since I had to build a similar project like this on Windows as well and ran into some dumb errors, I'll also add an instruction for Windows compilation now.

###### Prerequisites:
Microsoft Visual Studio 2017 (Community) - For .lib compilation
Microsoft Visual Studio 2015 (Community) - For mex compilation inside MatLab
After installing VS 2017 I followed [this](https://docs.lenux.org/libcurl-mit-visual-studio-2017 "libcurl-mit-visual-studio") article. If you don't know german, the following part is for you:

1. Open your CMD and execute

```"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"``` (for 32-Bit Systems)

**OR**

```"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"``` (for 64-bit Systems).

The 64/32-Bit difference between the ```.bat```-files tricked me at first, leading to another error, so watch out that you choose the correct file for your system.

2. After that cd to your cURL-folder (```curl_download_folder\winbuild```). (Which you downloaded inbefore from the [official page](https://curl.haxx.se/download.html). And execute this command: ```nmake /f Makefile.vc mode=static VC=12```

This compiles all the files you need to run cURL inside MatLab and to integrate it into any other program as well.

There should be some folders with files in them inside ```curl_download_folder\builds``` now. Namely:

I. ```libcurl-vc12-x64-release-static-ipv6-ssl```

II. ```libcurl-vc12-x64-release-static-ipv6-ssl-obj-curl```

III. ```libcurl-vc12-x64-release-static-ipv6-ssl-obj-lib```

3. Copy the whole ```\builds``` folder to your MatLab working directory that contains the ```.cpp``` file you want to compile.

3(/2). If you didn't do that so far, set your compiler to ```Microsoft Visual C++ 2015 Professional``` with ```mex -setup cpp``` or ```mex -setup c++```. Then simply choose the compiler.

4. Compile your S-Function (```sfunc.cpp``` in this case) with this command:

```mex sfunc.cpp -lbuilds\libcurl-vc12-x64-release-static-ipv6-sspi-winssl\include\ -Lbuilds\libcurl-vc12-x64-release-static-ipv6-sspi-winssl\lib\ -llibcurl_a.lib -DCURL_STATICLIB```

5. Voilà, your ```.mexw64-file``` should compile correctly. (The -D flag is super important in this case. \builds folders, you need them all!)


** IMPORTANT: ** Since all ```\builds``` folders seem to be referenced from the ```.lib``` file, it's super important to copy the whole build-folder into your project folder. Also: Everywhere in this text, replace ```x64``` with ```x86``` if you're on a 32-bit system.

Also: sorry for the crappy formatting in the Windows-Compilation-Explanation-Part. 
