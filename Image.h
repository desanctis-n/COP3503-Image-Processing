#pragma once
#include<vector>
#include<fstream>

using namespace std;

class Image
{
    struct Pixel
    {
        unsigned char _blue = '0';
        unsigned char _green = '0';
        unsigned char _red = '0';
        
        Pixel();
        Pixel(unsigned char &blue, unsigned char &green, unsigned char &red);
    };
    struct Header
    {
        char _idLength = '0';
        char _colorMapType = '0';
        char _dataTypeCode = '0';
        short _colorMapOrigin = 0;
        short _colorMapLength = 0;
        char _colorMapDepth = '0';
        short _xOrigin = 0;
        short _yOrigin = 0;
        short _width = 0;
        short _height = 0;
        char _bitsPerPixel = '0';
        char _imageDescriptor = '0';

        Header();
        Header(char &idLength, char &colorMapType, char& dataTypeCode,
               short &colorMapOrigin, short &colorMapLength, char& colorMapDepth,
               short &xOrigin, short& yOrigin, short& width, short& height,
               char &bitsPerPixel, char &imageDescriptor);
        Header(const Header &originalHeader);
        void operator=(const Header &rhs);
    };

    Pixel *_pixels;
   Header _imageHeader;

    // PRIVATE FUNCTIONS

    void ReadHeader(ifstream &file);
    void ReadPixels(ifstream &file);

    void WriteHeader(ofstream &file);
    void WritePixels(int range, ofstream &file);

    void WriteBlue(int range, ofstream &file);
    void WriteGreen(int range, ofstream &file);
    void WriteRed(int range, ofstream &file);

public:

    // CONSTRUCTION AND DESTRUCTION
    Image();
    Image(Header &imageHeader);
    ~Image();

    // FILE READING/WRITING
    void ReadImage(const string &filename);
    void ReadFromChannels(const string &bluefilename, const string &greenfilename, const string &redfilename);
    void WriteImage(const string &filename);
    void WriteToSplitChannels(const string &bluefilename, const string &greenfilename, const string &redfilename);


    // IMAGE MANIPULATION
    Image Multiply(const Image &rhs);
    Image Subtract(const Image &rhs);
    Image Screen(const Image &rhs);
    Image Overlay(const Image &foreground);
    Image AddColors(int blue, int green, int red);
    Image ScaleColors(int blueSF, int greenSF, int redSF);
    Image Flip();
};