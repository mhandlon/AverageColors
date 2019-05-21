//
//  main.cpp
//  AvarageColors
//
//  Created by Michael Handlon on 5/21/19.
//  Copyright © 2019 Michael Handlon. All rights reserved.
//

#include <iostream>
#include <Magick++.h>

using namespace Magick;

int main(int argc, const char **argv) {
    
    InitializeMagick(*argv);
    
    Image image;
    try {
        // Read a file into image object
        image.read( "girl.gif" );
        
        // Crop the image to specified size (width, height, xOffset, yOffset)
        image.crop( Geometry(100,100, 100, 100) );
        
        // Write the image to a file
        image.write( "x.gif" );
    }
    catch( Exception &error_ )
    {
        cout << "Caught exception: " << error_.what() << endl;
        return 1;
    }
    
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
