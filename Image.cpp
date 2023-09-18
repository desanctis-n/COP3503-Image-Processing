#include "Image.h"
#include <fstream>

using namespace std;

//--------------------- CONSTRUCTION AND DESTRUCTION ---------------------//

Image::Image() {
    _pixels = nullptr;
}
Image::Image(Header &imageHeader) {
    _imageHeader = imageHeader;
    _pixels = nullptr;
}
Image::~Image() {
    delete[] _pixels;
}

//---------------------------- HEADER CONSTRUCTION ----------------------------//

Image::Header::Header() = default;
Image::Header::Header(char &idLength, char &colorMapType, char &dataTypeCode, short &colorMapOrigin, short &colorMapLength,
               char &colorMapDepth, short &xOrigin, short &yOrigin, short &width, short &height, char &bitsPerPixel,
               char &imageDescriptor) {
    _idLength = idLength;
    _colorMapType = colorMapType;
    _dataTypeCode = dataTypeCode;
    _colorMapOrigin = colorMapOrigin;
    _colorMapLength = colorMapLength;
    _colorMapDepth = colorMapDepth;
    _xOrigin = xOrigin;
    _yOrigin = yOrigin;
    _width = width;
    _height = height;
    _bitsPerPixel = bitsPerPixel;
    _imageDescriptor = imageDescriptor;
}
Image::Header::Header(const Header &originalHeader) {
    _idLength = originalHeader._idLength;
    _colorMapType = originalHeader._colorMapType;
    _dataTypeCode = originalHeader._dataTypeCode;
    _colorMapOrigin = originalHeader._colorMapOrigin;
    _colorMapLength = originalHeader._colorMapLength;
    _colorMapDepth = originalHeader._colorMapDepth;
    _xOrigin = originalHeader._xOrigin;
    _yOrigin = originalHeader._yOrigin;
    _width = originalHeader._width;
    _height = originalHeader._height;
    _bitsPerPixel = originalHeader._bitsPerPixel;
    _imageDescriptor = originalHeader._imageDescriptor;
}
void Image::Header::operator=(const Header &rhs) {
    _idLength = rhs._idLength;
    _colorMapType = rhs._colorMapType;
    _dataTypeCode = rhs._dataTypeCode;
    _colorMapOrigin = rhs._colorMapOrigin;
    _colorMapLength = rhs._colorMapLength;
    _colorMapDepth = rhs._colorMapDepth;
    _xOrigin = rhs._xOrigin;
    _yOrigin = rhs._yOrigin;
    _width = rhs._width;
    _height = rhs._height;
    _bitsPerPixel = rhs._bitsPerPixel;
    _imageDescriptor = rhs._imageDescriptor;
}

//---------------------------- PIXEL CONSTRUCTION ----------------------------//

Image::Pixel::Pixel() {
    _blue = '0';
    _green = '0';
    _red = '0';
}
Image::Pixel::Pixel(unsigned char &blue, unsigned char &green, unsigned char &red) {
    _blue = blue;
    _green = green;
    _red = red;
}

//---------------------------- READING IMAGE ----------------------------//

void Image::ReadHeader(ifstream &file) {
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;

    if (file.is_open()) {
        file.read(reinterpret_cast<char *>(&idLength), sizeof(idLength));
        file.read(reinterpret_cast<char *>(&colorMapType), sizeof(colorMapType));
        file.read(reinterpret_cast<char *>(&dataTypeCode), sizeof(dataTypeCode));
        file.read(reinterpret_cast<char *>(&colorMapOrigin), sizeof(colorMapOrigin));
        file.read(reinterpret_cast<char *>(&colorMapLength), sizeof(colorMapLength));
        file.read(reinterpret_cast<char *>(&colorMapDepth), sizeof(colorMapDepth));
        file.read(reinterpret_cast<char *>(&xOrigin), sizeof(xOrigin));
        file.read(reinterpret_cast<char *>(&yOrigin), sizeof(yOrigin));
        file.read(reinterpret_cast<char *>(&width), sizeof(width));
        file.read(reinterpret_cast<char *>(&height), sizeof(height));
        file.read(reinterpret_cast<char *>(&bitsPerPixel), sizeof(bitsPerPixel));
        file.read(reinterpret_cast<char *>(&imageDescriptor), sizeof(imageDescriptor));
        _imageHeader = Header(idLength, colorMapType, dataTypeCode, colorMapOrigin, colorMapLength, colorMapDepth,
                              xOrigin, yOrigin, width, height, bitsPerPixel, imageDescriptor);
    } else
        throw runtime_error("No file loaded.");
}
void Image::ReadPixels(ifstream &file) {
    if (file.is_open()) {
        _pixels = new Pixel[_imageHeader._width * _imageHeader._height];
        for (int i = 0; i < static_cast<int>(_imageHeader._width) * static_cast<int>(_imageHeader._height); i++) {
            file.read(reinterpret_cast<char *>(&_pixels[i]._blue), sizeof(_pixels[i]._blue));
            file.read(reinterpret_cast<char *>(&_pixels[i]._green), sizeof(_pixels[i]._green));
            file.read(reinterpret_cast<char *>(&_pixels[i]._red), sizeof(_pixels[i]._red));
        }
    }
    else
        throw runtime_error("No file loaded.");
}
void Image::ReadImage(const string &filename) {
    ifstream file(filename, ios_base::binary);
    ReadHeader(file);
    ReadPixels(file);
}
void Image::ReadFromChannels(const string &bluefilename, const string &greenfilename, const string &redfilename) {

    Image junkImage;

    ifstream blueFile(bluefilename, ios_base::binary);
    ifstream greenFile(greenfilename, ios_base::binary);
    ifstream redFile(redfilename, ios_base::binary);
    ReadHeader(blueFile);

    junkImage.ReadHeader(greenFile);
    junkImage.ReadHeader(redFile);

    _pixels = new Pixel[_imageHeader._width * _imageHeader._height];
    junkImage._pixels = new Pixel[_imageHeader._width * _imageHeader._height];

    for (int i = 0; i < static_cast<int>(_imageHeader._width) * static_cast<int>(_imageHeader._height); i++) {

        blueFile.read(reinterpret_cast<char *>(&_pixels[i]._blue), sizeof(_pixels[i]._blue));
        blueFile.read(reinterpret_cast<char *>(&junkImage._pixels[i]._blue), sizeof(junkImage._pixels[i]._blue));
        blueFile.read(reinterpret_cast<char *>(&junkImage._pixels[i]._green), sizeof(junkImage._pixels[i]._green));

        greenFile.read(reinterpret_cast<char *>(&junkImage._pixels[i]._red), sizeof(junkImage._pixels[i]._red));
        greenFile.read(reinterpret_cast<char *>(&_pixels[i]._green), sizeof(_pixels[i]._green));
        greenFile.read(reinterpret_cast<char *>(&junkImage._pixels[i]._blue), sizeof(junkImage._pixels[i]._blue));

        redFile.read(reinterpret_cast<char *>(&junkImage._pixels[i]._green), sizeof(junkImage._pixels[i]._green));
        redFile.read(reinterpret_cast<char *>(&junkImage._pixels[i]._red), sizeof(junkImage._pixels[i]._red));
        redFile.read(reinterpret_cast<char *>(&_pixels[i]._red), sizeof(_pixels[i]._red));
    }
}

//---------------------------- WRITING IMAGE ----------------------------//

void Image::WriteHeader(ofstream &file){
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(&_imageHeader._idLength), sizeof(_imageHeader._idLength));
        file.write(reinterpret_cast<char*>(&_imageHeader._colorMapType), sizeof(_imageHeader._colorMapType));
        file.write(reinterpret_cast<char*>(&_imageHeader._dataTypeCode), sizeof(_imageHeader._dataTypeCode));
        file.write(reinterpret_cast<char*>(&_imageHeader._colorMapOrigin), sizeof(_imageHeader._colorMapOrigin));
        file.write(reinterpret_cast<char*>(&_imageHeader._colorMapLength), sizeof(_imageHeader._colorMapLength));
        file.write(reinterpret_cast<char*>(&_imageHeader._colorMapDepth), sizeof(_imageHeader._colorMapDepth));
        file.write(reinterpret_cast<char*>(&_imageHeader._xOrigin), sizeof(_imageHeader._xOrigin));
        file.write(reinterpret_cast<char*>(&_imageHeader._yOrigin), sizeof(_imageHeader._yOrigin));
        file.write(reinterpret_cast<char*>(&_imageHeader._width), sizeof(_imageHeader._width));
        file.write(reinterpret_cast<char*>(&_imageHeader._height), sizeof(_imageHeader._height));
        file.write(reinterpret_cast<char*>(&_imageHeader._bitsPerPixel), sizeof(_imageHeader._bitsPerPixel));
        file.write(reinterpret_cast<char*>(&_imageHeader._imageDescriptor), sizeof(_imageHeader._imageDescriptor));
    }
}
void Image::WritePixels(int range, ofstream &file) {
    if (file.is_open())
        for (int i = 0; i < range; i++) {
            file.write(reinterpret_cast<char*>(&_pixels[i]._blue), sizeof(_pixels[i]._blue));
            file.write(reinterpret_cast<char*>(&_pixels[i]._green), sizeof(_pixels[i]._green));
            file.write(reinterpret_cast<char*>(&_pixels[i]._red), sizeof(_pixels[i]._red));
        }
}
void Image::WriteImage(const string &filename) {
    ofstream file(filename, ios_base::binary);
    WriteHeader(file);
    int range = static_cast<int>(_imageHeader._width) * static_cast<int>(_imageHeader._height);
    WritePixels(range, file);
}
void Image::WriteBlue(int range, ofstream &file) {
    if (file.is_open()) {
        for (int i = 0; i < range; i++) {
            file.write(reinterpret_cast<char*>(&_pixels[i]._blue), sizeof(_pixels[i]._blue));
            file.write(reinterpret_cast<char*>(&_pixels[i]._blue), sizeof(_pixels[i]._blue));
            file.write(reinterpret_cast<char*>(&_pixels[i]._blue), sizeof(_pixels[i]._blue));
        }
    }
}
void Image::WriteGreen(int range, ofstream &file) {
    if (file.is_open()) {
        for (int i = 0; i < range; i++) {
            file.write(reinterpret_cast<char*>(&_pixels[i]._green), sizeof(_pixels[i]._green));
            file.write(reinterpret_cast<char*>(&_pixels[i]._green), sizeof(_pixels[i]._green));
            file.write(reinterpret_cast<char*>(&_pixels[i]._green), sizeof(_pixels[i]._green));
        }
    }
}
void Image::WriteRed(int range, ofstream &file) {
    if (file.is_open()) {
        for (int i = 0; i < range; i++) {
            file.write(reinterpret_cast<char*>(&_pixels[i]._red), sizeof(_pixels[i]._red));
            file.write(reinterpret_cast<char*>(&_pixels[i]._red), sizeof(_pixels[i]._red));
            file.write(reinterpret_cast<char*>(&_pixels[i]._red), sizeof(_pixels[i]._red));
        }
    }
}
void Image::WriteToSplitChannels(const string &bluefilename, const string &greenfilename, const string &redfilename) {

    int range = static_cast<int>(_imageHeader._width) * static_cast<int>(_imageHeader._height);

    ofstream bluefile(bluefilename, ios_base::binary);
    WriteHeader(bluefile);
    WriteBlue(range, bluefile);

    ofstream greenfile(greenfilename, ios_base::binary);
    WriteHeader(greenfile);
    WriteGreen(range, greenfile);

    ofstream redfile(redfilename, ios_base::binary);
    WriteHeader(redfile);
    WriteRed(range, redfile);
}

//----------------------- IMAGE MANIPULATION --------------------//

Image Image::Multiply(const Image &rhs) {
    Image blendedImage(_imageHeader);
    blendedImage._pixels = new Pixel[_imageHeader._width * _imageHeader._height];
    for (int i = 0; i < static_cast<int>(_imageHeader._width) * static_cast<int>(_imageHeader._height); i++) {
        blendedImage._pixels[i]._blue = static_cast<unsigned char>(0.5f + (((int)(this->_pixels[i]._blue) * (int)(rhs._pixels[i]._blue)) / 255.0));
        blendedImage._pixels[i]._green = static_cast<unsigned char>(0.5f + (((int)(this->_pixels[i]._green) * (int)(rhs._pixels[i]._green)) / 255.0));
        blendedImage._pixels[i]._red = static_cast<unsigned char>(0.5f + (((int)(this->_pixels[i]._red) * (int)(rhs._pixels[i]._red)) / 255.0));
    }
    return blendedImage;
}
Image Image::Subtract(const Image &rhs) {
    Image blendedImage(_imageHeader);
    int temp = 0;
    blendedImage._pixels = new Pixel[_imageHeader._width * _imageHeader._height];
    for (int i = 0; i < static_cast<int>(_imageHeader._width) * static_cast<int>(_imageHeader._height); i++) {
        temp = ((int)(this->_pixels[i]._blue) - (int)(rhs._pixels[i]._blue));
        if (temp > 255)
            temp = 255;
        else if (temp < 0)
            temp = 0;
        blendedImage._pixels[i]._blue = static_cast<unsigned char>(temp);

        temp = ((int)(this->_pixels[i]._green) - (int)(rhs._pixels[i]._green));
        if (temp > 255)
            temp = 255;
        else if (temp < 0)
            temp = 0;
        blendedImage._pixels[i]._green = static_cast<unsigned char>(temp);

        temp = ((int)(this->_pixels[i]._red) - (int)(rhs._pixels[i]._red));
        if (temp > 255)
            temp = 255;
        else if (temp < 0)
            temp = 0;
        blendedImage._pixels[i]._red = static_cast<unsigned char>(temp);
    }
    return blendedImage;
}
Image Image::Screen(const Image &rhs) {
    Image blendedImage(_imageHeader);
    int temp = 0;
    blendedImage._pixels = new Pixel[_imageHeader._width * _imageHeader._height];
    for (int i = 0; i < static_cast<int>(_imageHeader._width) * static_cast<int>(_imageHeader._height); i++) {
        temp = 0.5 + 255.0 * (1.0 - ((1.0 - ((int)(this->_pixels[i]._blue)) / 255.0)) * ((1.0 - ((int)(rhs._pixels[i]._blue)) / 255.0)));
        blendedImage._pixels[i]._blue = static_cast<unsigned char>(temp);
        temp = 0.5 + 255.0 * (1.0 - ((1.0 - ((int)(this->_pixels[i]._green)) / 255.0)) * ((1.0 - ((int)(rhs._pixels[i]._green)) / 255.0)));
        blendedImage._pixels[i]._green = static_cast<unsigned char>(temp);
        temp = 0.5 + 255.0 * (1.0 - ((1.0 - ((int)(this->_pixels[i]._red)) / 255.0)) * ((1.0 - ((int)(rhs._pixels[i]._red)) / 255.0)));
        blendedImage._pixels[i]._red = static_cast<unsigned char>(temp);
    }
    return blendedImage;
}
Image Image::Overlay(const Image &foreground) {
    Image blendedImage(_imageHeader);
    int temp = 0;
    blendedImage._pixels = new Pixel[_imageHeader._width * _imageHeader._height];
    for (int i = 0; i < static_cast<int>(_imageHeader._width) * static_cast<int>(_imageHeader._height); i++) {
        if ((int)(this->_pixels[i]._blue) <= 127.5)
            blendedImage._pixels[i]._blue = static_cast<unsigned char>(0.5 + (2.0 * ((((int)(this->_pixels[i]._blue) * (int)(foreground._pixels[i]._blue)) / 255.0))));
        else {
            temp = 0.5 + 255.0 * (1.0 - (2.0 * (1.0 - ((int)(this->_pixels[i]._blue)) / 255.0)) * ((1.0 - ((int)(foreground._pixels[i]._blue)) / 255.0)));
            blendedImage._pixels[i]._blue = static_cast<unsigned char>(temp);
        }
        if ((int)(this->_pixels[i]._green) <= 127.5)
            blendedImage._pixels[i]._green = static_cast<unsigned char>(0.5 + (2.0 * ((((int)(this->_pixels[i]._green) * (int)(foreground._pixels[i]._green)) / 255.0))));
        else {
            temp = 0.5 + 255.0 * (1.0 - (2.0 * (1.0 - ((int)(this->_pixels[i]._green)) / 255.0)) * ((1.0 - ((int)(foreground._pixels[i]._green)) / 255.0)));
            blendedImage._pixels[i]._green = static_cast<unsigned char>(temp);
        }
        if ((int)(this->_pixels[i]._red) <= 127.5)
            blendedImage._pixels[i]._red = static_cast<unsigned char>(0.5 + (2.0 * ((((int)(this->_pixels[i]._red) * (int)(foreground._pixels[i]._red)) / 255.0))));
        else {
            temp = 0.5 + 255.0 * (1.0 - (2.0 * (1.0 - ((int)(this->_pixels[i]._red)) / 255.0)) * ((1.0 - ((int)(foreground._pixels[i]._red)) / 255.0)));
            blendedImage._pixels[i]._red = static_cast<unsigned char>(temp);
        }
    }
    return blendedImage;
}
Image Image::AddColors(int blue, int green, int red) {
    Image blendedImage(_imageHeader);
    blendedImage._pixels = new Pixel[_imageHeader._width * _imageHeader._height];
    int temp;
    for (int i = 0; i < static_cast<int>(_imageHeader._width) * static_cast<int>(_imageHeader._height); i++) {

        temp = ((int)(_pixels[i]._blue)) + blue;
        if (temp > 255)
            temp = 255;
        else if (temp < 0)
            temp = 0;
        temp = static_cast<unsigned char>(temp);
        blendedImage._pixels[i]._blue = temp;

        temp = ((int)(_pixels[i]._green)) + green;
        if (temp > 255)
            temp = 255;
        else if (temp < 0)
            temp = 0;
        temp = static_cast<unsigned char>(temp);
        blendedImage._pixels[i]._green = temp;

        temp = ((int)(_pixels[i]._red)) + red;
        if (temp > 255)
            temp = 255;
        else if (temp < 0)
            temp = 0;
        temp = static_cast<unsigned char>(temp);
        blendedImage._pixels[i]._red = temp;
    }
    return blendedImage;
}
Image Image::ScaleColors(int blueSF, int greenSF, int redSF) {
    Image blendedImage(_imageHeader);
    blendedImage._pixels = new Pixel[_imageHeader._width * _imageHeader._height];
    int temp;
    for (int i = 0; i < static_cast<int>(_imageHeader._width) * static_cast<int>(_imageHeader._height); i++) {

        temp = ((int)(_pixels[i]._blue)) * blueSF;
        if (temp > 255)
            temp = 255;
        else if (temp < 0)
            temp = 0;
        temp = static_cast<unsigned char>(temp);
        blendedImage._pixels[i]._blue = temp;

        temp = ((int)(_pixels[i]._green)) * greenSF;
        if (temp > 255)
            temp = 255;
        else if (temp < 0)
            temp = 0;
        temp = static_cast<unsigned char>(temp);
        blendedImage._pixels[i]._green = temp;

        temp = ((int)(_pixels[i]._red)) * redSF;
        if (temp > 255)
            temp = 255;
        else if (temp < 0)
            temp = 0;
        temp = static_cast<unsigned char>(temp);
        blendedImage._pixels[i]._red = temp;
    }
    return blendedImage;
}
Image Image::Flip() {
    Image flipped(_imageHeader);
    flipped._pixels = new Pixel[_imageHeader._width * _imageHeader._height];
    for (int i = 0; i < _imageHeader._width * _imageHeader._height; i++) {
        flipped._pixels[i] = this->_pixels[_imageHeader._width * _imageHeader._height - 1 - i];
    }
    return flipped;
}