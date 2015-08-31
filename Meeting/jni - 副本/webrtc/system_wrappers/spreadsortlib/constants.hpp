/*Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.*/
#ifndef BOOST_SPREADSORT_CONSTANTS
#define BOOST_SPREADSORT_CONSTANTS
namespace boost {
namespace detail {
//Tuning constants
//Sets the minimum number of items per bin.
static const unsigned LOG_MEAN_BIN_SIZE = 2;
//This should be tuned to your processor cache; if you go too large you get cache misses on bins
//The smaller this number, the less worst-case memory usage.  If too small, too many recursions slow down spreadsort
static const unsigned MAX_SPLITS = 10;
//Used to force a comparison-based sorting for small bins, if it's faster.  Minimum value 0
static const unsigned LOG_MIN_SPLIT_COUNT = 5;
//There is a minimum size below which it is not worth using spreadsort
static const long MIN_SORT_SIZE = 1000;
//This is the constant on the log base n of m calculation; make this larger the faster std::sort is relative to spreadsort
static const unsigned LOG_CONST = 2;
}
}
#endif
