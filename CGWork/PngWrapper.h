#pragma once
#include <stdio.h>
#include "png.h"//should reside where you can see it
/*
This class was derived from libpng example.c file
it is intended to suply simple read and write 
rutines into the png library.

It only support a small subset of png options 
which means that it will be able to read most of the 
png files available but will only write the files 
in 24bpp format with default compression and no interlace 


please send bugs to schein@cs.technion.ac.il


*/
#define SET_RGB(r,g,b) ((r)<<24|(g)<<16|(b)<<8|0)
#define SET_RGBA(r,g,b,a) ((r)<<24|(g)<<16|(b)<<8|a)
#define GET_R(x) (((x)&0xff000000)>>24)
#define GET_G(x) (((x)&0x00ff0000)>>16)
#define GET_B(x) (((x)&0x0000ff00)>>8)
#define GET_A(x) ((x)&0x000000ff)
class PngWrapper
{
public:
    PngWrapper(const char * name,int width =0,int height =0);
    PngWrapper();
    ~PngWrapper();
    void SetFileName(const char * name);
    void SetWidth(int width);
    void SetHeight(int height);
    int GetWidth();
    int GetHeight();
    void SetValue(unsigned int x,unsigned int y,unsigned int value);
    int GetValue(unsigned int x,unsigned int y);
    int GetNumChannels();
    int GetBPP();
    bool ReadPng();
    bool InitWritePng();
    bool WritePng();
private:
    bool IsPngFile();
    
private:
    int m_width;
    int m_height;
    char * m_fileName;
     FILE *m_fp;
     png_structp m_png_ptr;
     png_infop m_info_ptr;
};