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
#pragma once

#define CHRONOS_LOG_LEVEL 3
// 0 - nothing
// 1 - additional info on errors
// 2 - basic info
// 3 - verbose
// 4 - includes frame fraw
#ifdef CHRONOS_ENABLE_LOGGING
#define LOG(LEVEL, CLASS, MESSAGE) \
    if (LEVEL <= CHRONOS_LOG_LEVEL) { \
        std::cout << "[Chronos log][" << CLASS "]: " << MESSAGE << std::endl; \
    }
#else
#define LOG(LEVEL, CLASS, MESSAGE)
#endif
