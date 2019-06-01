//
//  main.cpp
//  AvarageColors
//
//  Created by Michael Handlon on 5/21/19.
//  Copyright © 2019 Michael Handlon. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <Magick++.h>

//#define MAGICKCORE_QUANTUM_DEPTH 8

using namespace std;
using namespace Magick;

struct RGB
{
    float red;
    float green;
    float blue;
    friend ostream& operator<< (ostream &o, const RGB &i)
    {
        return o << ",c:\"" << i.red << "|" << i.green << "|" << i.blue << "\"";
    }
};

struct circle
{
    int x;
    int y;
    int t;
    int l;
    RGB color;
    friend ostream& operator<< (ostream &o, const circle &i)
    {
        return o << "x:" << i.x << ",y:" << i.y << ",t:" << i.t << ",l:" << i.l << i.color;
    }
};

double distance(double x1, double y1, double x2, double y2)
{
    //cout << x1 << " " << x2 << " " << y1 << " " << y2 << endl;
    double x = abs(x2 - x1); //calculating number to square in next step
    double y = abs(y2 - y1);
    double dist = pow(x, 2) + pow(y, 2); //calculating Euclidean distance
    return sqrt(dist);
}

ColorRGB getPixel(Image img, int x, int y)
{
    ColorRGB c = img.pixelColor(x, y);
    return c;
}

/*
 * Averages the pixels in a given image (img) within a circular
 * region centered at (x, y) with a radius of 'radius' pixels.
 */
RGB getAverageRGBCircle(Image img, int x, int y, int radius)
{
    RGB average;
    
    float r = 0;
    float g = 0;
    float b = 0;
    int num = 0;
    
    size_t w = img.columns();
    size_t h = img.rows();
    
    // Iterate through a bounding box in which the circle lies
    for (int i = x - radius; i < x + radius; i++) {
        for (int j = y - radius; j < y + radius; j++) {
            // If the pixel is outside the canvas, skip it
            if (i < 0 || i >= w || j < 0 || j >= h){
                continue;
            }
            
            /*
            if ( (i+6) >= w || (j+6) >= h){
                continue;
            }
             */
            
            // If the pixel is outside the circle, skip it
            if (distance(x, y, i, j) > radius){
                //std::cout << "[Out of Range] (" << x << ", " << y << ") (" << x << ", " << y << ") " << distance(x, y, i, j) << std::endl;
                continue;
            }
            //std::cout << "[OK] (" << x << ", " << y << " (" << x << ", " << y << ") " << distance(x, y, i, j) << std::endl;
            
            ColorRGB c = img.pixelColor(i, j);
            
            // Get the color from the image, add to a running sum
            r += (c.red()*255) * (c.red()*255);
            g += (c.green()*255) * (c.green()*255);
            b += (c.blue()*255) * (c.green()*255);
            
            ++num;
        }
    }
    
    //cout << "[COUNT] " << num << endl;

    average.red = round(sqrt(r/num));
    average.green = round(sqrt(g/num));
    average.blue = round(sqrt(b/num));
    
    return average;
   }

int main(int argc, const char **argv)
{
    InitializeMagick(*argv);
    Image image;
    
    try {
        image.read( "512-github.jpg" );
        
        size_t width = image.columns();
        size_t height = image.rows();
        
        cout << "Image width: " << width << endl;
        cout << "Image height: " << height << endl;
        cout << "Image channels: " << image.channels() << endl;
        
        /*
        int cnt = 0;
        for ( int row = 0; row <= height; row++ )
        {
            for ( int column = 0; column <= width; column++ )
            {
                ++cnt;
                ColorRGB px = image.pixelColor( column, row );
                std::cout << "(" << row << "," << column << ") R: " << px.red() << " G: " << px.green() << " B: " << px.blue() << std::endl;
            }
        }
        std::cout << "cnt: " << cnt << std::endl;
        */
        //image.modifyImage();
        
        int n_px = 0;
        vector<circle> points;
        for ( int row = 6; row <= height; row += 12)
        {
            for (int column = 6; column <= width; column += 12)
            {
                if (((column + 6) <= height) && (((row + 6) <= width))){
                    ++n_px;
                    
                    //std::cout << "pixel: " << n_px << std::endl;
                    RGB px = getAverageRGBCircle(image, row, column, 5);
                    //std::cout << " 12/12: r: " << px.red() << " g: " << px.green() << " b: " << px.blue() << std::endl;
                    
                    circle mycircle;
                    mycircle.x = row;
                    mycircle.y = column;
                    
                    if (row == 6){
                        mycircle.l = 12;
                    } else {
                        mycircle.l = ((row+6)/12)*12;
                    }
                    
                    if (column == 6){
                        mycircle.t = 12;
                    } else {
                        mycircle.t = ((column+6)/12)*12;
                    }
                
                    mycircle.color = px;
                    //cout << "(x:" << mycircle.x << ", y:" << mycircle.y << ") " << mycircle.color << endl;
                    points.push_back(mycircle);
                }
            }
        }
        
        //cout << "n_px: " << n_px << endl;
        
        /*
        int i = 0;  // counter.
        std::vector<circle>::iterator it;
        for(it = points.begin(); it != points.end(); it++, i++ )
        {
            // found nth element..print and break.
            std::cout<< *it->x << std::endl;  // prints d.
        }
        */
        
        ofstream outFile("me.js");
        
        for( std::vector<circle>::const_iterator i = points.begin(); i != points.end(); ++i){
            cout << "{" << *i << "}," << endl;
            outFile << "{" << *i << "}," << endl;
        }
        
         
    } catch( Magick::Exception & error ) {
        cerr << "Caught Magick++ exception: " << error.what() << endl;
        return 1;
    }
    
    return 0;
}
