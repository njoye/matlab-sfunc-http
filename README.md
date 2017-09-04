# matlab-sfunc-http
Example of making HTTP Requests with C++ MatLab S-Function Code



## File Explanation
#### _sfunc.cpp_ 
contains C++ Code that gets a random numer from https://www.random.org/integers/?num=1&min=1&max=100&col=5&base=10&format=plain&rnd=new. The number gets returned as plain text document, making it easy to parse and use in the Code, that's why I chose this website.

#### _simulink_model.slx_ 
contains the SimuLink Model that is already set up for the function. You should be able to open and run it directly.


#### _sfunc.mexmaci64_ 
is the compiled version of _sfunc.cpp_. See **Compiling with _mex_** for more information on that.


## Usage
1. Open the files in your Matlab Environment (mine is R2017a v9.2.0.556344).
2. Start Simulink and open the simulink model.
3. Compile _sfunc.cpp_ with **mex**
4. Click the "Play" / "Run" Button (easy, huh?:) )


## Compiling with _mex_
I've used [cURL](https://curl.haxx.se/libcurl/) in this code, simply because it's easy. You'll find many examples of how to install libcurl on your computer, so I won't explain that here as well. After that, you'll have to find the cURL library on your computer in order to tell _mex_ where ```#include <curl/curl.h>``` is at, so that everything compiles properly. 

For me, the compilation command looked like this: ```mex sfunc.cpp -L/usr/bin -lcurl```.
Or generalizing it: ```mex filename.cpp -L/path/to/bin -lbinary```
**IMPORTANT: As with most compilers, no space is allowed after -L or -l !**

After properly compiling the ```.cpp``` file, there will be a new file in your directory. Since I work on a MacBook, it's called ```sfunc.mexmaci64```. If you're working with a Mac as well, there's a chance that that file works for you already.
