/*
Copyright (c) 2024 Rahul Satish Vadhyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/** \file stlheader.hpp

 \brief Contains all the standard library headers used in the project

 This file is included in all the source files of the project.
 It contains all the standard library headers used in the project.
 This is done to reduce the number of includes in the source files and to make the source files more readable.
 This also makes it easier to add new standard library headers to the project.
 It also makes it simple to know which standard library headers are used in the project.
*/

#pragma once
// glibc has some warnings that are not related to chronos, so lets just disable them only for them.
#pragma GCC diagnostic push 
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
#pragma GCC diagnostic ignored "-Warray-bounds"
#include <algorithm>
#include <numbers>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <unordered_set>
#include <stdexcept>
#include <string>
#include <vector>
#include <type_traits>
 
#pragma GCC diagnostic pop
