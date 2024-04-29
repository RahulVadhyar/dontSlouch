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

#include "chronos.hpp"
namespace Chronos {
GLFWwindow* Manager::Manager::getWindow()
{
    return engine.window;
}
Manager::Manager::Manager(Chronos::Manager::Initializer initializer)
{   

    //check whether the bg color is between 0 and 1
    if (initializer.BackgroundColor[0] < 0 || initializer.BackgroundColor[0] > 1 || initializer.BackgroundColor[1] < 0 || initializer.BackgroundColor[1] > 1 || initializer.BackgroundColor[2] < 0 || initializer.BackgroundColor[2] > 1) {
        throw std::runtime_error("Invalid background color");
    }
    engine.bgColor[0] = initializer.BackgroundColor[0];
    engine.bgColor[1] = initializer.BackgroundColor[1];
    engine.bgColor[2] = initializer.BackgroundColor[2];

    if (initializer.WindowWidth <= 0 || initializer.WindowHeight <= 0) {
        throw std::runtime_error("Invalid window size");
    }
    engine.width = initializer.WindowWidth;
    engine.height = initializer.WindowHeight;

    #ifdef ENABLE_EDITOR
    engine.setEditorAddElementsCallback(initializer.editorAddElements);
    #endif
}


void Manager::Manager::drawFrame()
{
    engine.drawFrame();
}
int Manager::Manager::changeBackgroundColor(float r, float g, float b)
{
    if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1) {
        throw std::runtime_error("Invalid background color");
    }
    engine.bgColor[0] = r;
    engine.bgColor[1] = g;
    engine.bgColor[2] = b;
    return 0;
}
};
