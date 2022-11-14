#include <iostream>
#include <assert.h>
#include "png.h"
#include "PngWrapper.h"
//local defs
#ifndef png_jmpbuf
#  define png_jmpbuf(png_ptr) ((png_ptr)->jmpbuf)
#endif
//check that the file is png
#define PNG_BYTES_TO_CHECK 4
bool PngWrapper::IsPngFile()
{
   char buf[PNG_BYTES_TO_CHECK];
   /* Read in some of the signature bytes */
   if (fread(buf, 1, PNG_BYTES_TO_CHECK, m_fp) != PNG_BYTES_TO_CHECK)
      return false;

   /* Compare the first PNG_BYTES_TO_CHECK bytes of the signature.
      Return nonzero (true) if they match */

   return (!png_sig_cmp((png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK));
}


//ctor
// you give the name of the file and if it is for
//writing new files you may set here the 
// file size in pixels
PngWrapper::PngWrapper(const char * name,int width,int height):
m_width(width),
m_height(height),
m_png_ptr(NULL),
m_info_ptr(NULL),
m_fileName(NULL),
m_fp(NULL)
{
    SetFileName(name);  
}
//this is usfull ctor for 
//creating an array of files for animation purposes

PngWrapper::PngWrapper():
m_width(0),
m_height(0),
m_png_ptr(NULL),
m_info_ptr(NULL),
m_fileName(NULL),
m_fp(NULL)
{
}
//dtor - will close the file ahandle 
//and release allocated memory
PngWrapper::~PngWrapper()
{
     // clean up after the read, and free any memory allocated 
   if(m_png_ptr && m_info_ptr)
    png_destroy_read_struct(&m_png_ptr, &m_info_ptr, png_infopp_NULL);
   if(m_fileName)
       delete [] m_fileName;
   if(m_fp)
       fclose(m_fp);
}


void PngWrapper::SetFileName(const char * name)
{
    if(name == NULL)
        assert(0);
    int l = strlen(name);
    if(m_fileName)
        delete [] m_fileName;
    m_fileName = new char[strlen(name)+1];
    strcpy(m_fileName,name);
}
/*
Read the png file which name is defined in 
*/
bool PngWrapper::ReadPng()
{

 //  unsigned int sig_read = 0;
 //  png_uint_32 width, height;
//   int bit_depth, color_type, interlace_type;
    /* Open the file PNG file. */
   if ((m_fp = fopen(m_fileName, "rb")) == NULL)
      return false; 
   /*is png file ?*/
   if(IsPngFile() == false){
       fclose(m_fp);
       m_fp=NULL;
       return false;
   }
   //create the read struct
   m_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
      NULL, NULL, NULL);

   if (m_png_ptr == NULL)
   {
      fclose(m_fp);
      m_fp=NULL;
      return false;
   }
   // Allocate/initialize the memory for image information.  
   m_info_ptr = png_create_info_struct(m_png_ptr);
   if (m_info_ptr == NULL)
   {
      png_destroy_read_struct(&m_png_ptr, png_infopp_NULL, png_infopp_NULL);     
      fclose(m_fp);
      m_fp=NULL;
      
      return false;
   }
   // this will set the jamp location in case of an error
   if (setjmp(png_jmpbuf(m_png_ptr)))
   {
      /* Free all of the memory associated with the png_ptr and info_ptr */
      png_destroy_read_struct(&m_png_ptr, &m_info_ptr, png_infopp_NULL);
      fclose(m_fp);
      m_fp=NULL;
      /* If we get here, we had a problem reading the file */
      return false;
   }
  //io init - we will read the whole image to the memory 
   png_init_io(m_png_ptr, m_fp);
   //we read the sig bytes and we want to skip the rest
   png_set_sig_bytes(m_png_ptr, PNG_BYTES_TO_CHECK);
   //read to the image
   png_read_png(m_png_ptr, m_info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
   //make sure that we have 24 bpp color image or an 8 bpp gray image
	// update width
   
   m_width = m_info_ptr->width;
   m_height = m_info_ptr->height;
   //close the file
   fclose(m_fp);
   m_fp=NULL;
    return true;
}


void PngWrapper::SetWidth(int width)
{
    m_width = width;

}
void PngWrapper::SetHeight(int height)
{
    m_height = height;
}
int PngWrapper::GetWidth()
{
    return m_width;
}
int PngWrapper::GetHeight()
{
    return m_height;
}
// these access function will assert if you try to 
// set value to non existant png 
// or outside its domain
void PngWrapper::SetValue(unsigned int x,unsigned int y,unsigned int value)
{
    if(y >=  m_info_ptr->height ||
        x >= m_info_ptr->width){
            std::cout<<"bad offset into file "<<__FILE__<<" "<<__LINE__<<std::endl;
            return;
        }
        if (m_info_ptr->channels == 1)
            m_info_ptr->row_pointers[y][m_info_ptr->channels*x] = value & 0x000000ff;
        else if(m_info_ptr->channels == 3){
            m_info_ptr->row_pointers[y][m_info_ptr->channels*x] = GET_R(value),
            m_info_ptr->row_pointers[y][m_info_ptr->channels*x+1] = GET_G(value),
            m_info_ptr->row_pointers[y][m_info_ptr->channels*x+2] = GET_B(value);
        }
        else if(m_info_ptr->channels == 4){
            m_info_ptr->row_pointers[y][m_info_ptr->channels*x] = GET_R(value),
            m_info_ptr->row_pointers[y][m_info_ptr->channels*x+1] = GET_G(value),
            m_info_ptr->row_pointers[y][m_info_ptr->channels*x+2] = GET_B(value),
            m_info_ptr->row_pointers[y][m_info_ptr->channels*x+2] = GET_A(value);
        }
            
}
// the function will assert if a png was not read
int PngWrapper::GetValue(unsigned int x,unsigned int y)
{
    if( y >=  m_info_ptr->height ||
        x >= m_info_ptr->width){
            std::cout<<"bad offset into file "<<__FILE__<<" "<<__LINE__<<std::endl;
            return 0;
        }
        if (m_info_ptr->channels == 1)
            return m_info_ptr->row_pointers[y][m_info_ptr->channels*x];
        else if(m_info_ptr->channels == 3)
            return SET_RGB(m_info_ptr->row_pointers[y][m_info_ptr->channels*x],
                             m_info_ptr->row_pointers[y][m_info_ptr->channels*x+1],
                             m_info_ptr->row_pointers[y][m_info_ptr->channels*x+2]);
        else if(m_info_ptr->channels == 4)
            return SET_RGBA(m_info_ptr->row_pointers[y][m_info_ptr->channels*x],
                             m_info_ptr->row_pointers[y][m_info_ptr->channels*x+1],
                             m_info_ptr->row_pointers[y][m_info_ptr->channels*x+2],
                             m_info_ptr->row_pointers[y][m_info_ptr->channels*x+3]);
        
    //should not reach here - it means that the iamge 
    //format is not supported by the wrapper
    assert(0);
    return 0;
}

bool PngWrapper::InitWritePng()
{
    if(m_width <= 0 || m_height <= 0)
        return false;
    if(m_fileName == NULL)
        return false;
    if(m_fp != NULL)
        return false;
    //we have unclosed streams - no online reuse is allowed for simplicity
    if(m_png_ptr || m_png_ptr)
        return false;
    
    //init the file
     /* open the file */
   m_fp = fopen(m_fileName, "wb");
   if (m_fp == NULL)
      return false;
   //init the png ptr struct
   m_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL, NULL, NULL);
   if (m_png_ptr == NULL)
   {
      fclose(m_fp);
      m_fp = NULL;  
      return false;
   }
   //init the info ptr
   m_info_ptr = png_create_info_struct(m_png_ptr);
   if (m_info_ptr == NULL)
   {
      fclose(m_fp);
      m_fp = NULL;
      png_destroy_write_struct(&m_png_ptr,  png_infopp_NULL);
      return false;
   }
   //
   if (setjmp(png_jmpbuf(m_png_ptr)))
   {
      /* If we get here, we had a problem reading the file */
      fclose(m_fp);
      m_fp = NULL;
      png_destroy_write_struct(&m_png_ptr, &m_info_ptr);
      return false;
   }
   // set application data for the file
    //m_png_ptr setting
   png_set_IHDR(m_png_ptr, m_info_ptr, m_width, m_height,
       8/*bits per channel*/, 
       PNG_COLOR_TYPE_RGB, 
       PNG_INTERLACE_NONE,
       PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
//allocate memory
   png_voidp ppp = png_malloc(m_png_ptr,
      m_height*sizeof(png_bytep));
   png_bytepp row_pointers = (png_bytepp)ppp;
   for (int i=0; i<m_height; i++)
      row_pointers[i]=(png_bytep)png_malloc(m_png_ptr,
      m_width*(m_info_ptr->pixel_depth/m_info_ptr->bit_depth));
   png_set_rows(m_png_ptr, m_info_ptr, row_pointers);
  

   //init io - bind the struct to the stream
   png_init_io(m_png_ptr, m_fp);
   return true;
}

//close the file and write the result - the app will write the 
//data using the SetValue interface
bool PngWrapper::WritePng()
{
    if(!m_png_ptr || !m_info_ptr)
        return false;
    if (setjmp(png_jmpbuf(m_png_ptr)))
   {
      /* If we get here, we had a problem reading the file */
      fclose(m_fp);
      m_fp = NULL;
      png_destroy_write_struct(&m_png_ptr, &m_info_ptr);
      return false;
   }
    png_write_png(m_png_ptr, m_info_ptr, 0, png_voidp_NULL);
    png_destroy_write_struct(&m_png_ptr, &m_info_ptr);
    fclose(m_fp);
    m_fp = NULL;
    return true;
}


int PngWrapper::GetNumChannels()
{
    if(!m_info_ptr)
        return 0;
    return m_info_ptr->channels;
}
int PngWrapper::GetBPP()
{
    if(!m_info_ptr)
        return 0;
    return m_info_ptr->pixel_depth;
}