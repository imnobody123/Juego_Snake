/******************************************************************
*                                                                 *
*    Name: Libreria Multimedia                                    * 
*    Author: Xcorpia & Marce82                                    *
*    Description: Libreria Basadada en las Librerias PNG & MP3    *
*    Web-Site: www.marce82projects.es                             *
*                                                                 *  
******************************************************************/

#ifndef MULTIMEDIA_H
#define MULTIMEDIA_H

#define IniciarSonido pspAudioInit

#include <psptypes.h>
#include <png.h>
#include <mad.h>

extern u32* g_vram_base;

#define PSP_LINE_SIZE 512
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

typedef u32 Color;

#define A(color) ((u8)(color >> 24 & 0xFF))
#define B(color) ((u8)(color >> 16 & 0xFF))
#define G(color) ((u8)(color >> 8 & 0xFF))
#define R(color) ((u8)(color & 0xFF))

#define RGB(r, g, b) (0xFF000000 | ((b)<<16) | ((g)<<8) | (r))

typedef struct
{
   int	textureWidth; 
   int	textureHeight;  
   int	imageWidth;  
   int	imageHeight;
   
   Color* data;
} 

Variable;

typedef struct
{
   const unsigned char *buffer;
   png_uint_32 bufsize;
   png_uint_32 current_pos;
} 

ESTADO_DE_LA_LECTURA_DE_MEMORIA;

extern void IniciarGraficos();

extern Variable* CargarImagen(const char* filename);

extern Variable* CargarImagenInterna(const void *buffer, int bufsize);

extern void PosicionImagen(int sx, int sy, int width, int height, Variable* source, int dx, int dy);

extern void MostrarImagen();

extern void LimpiarVariable(Variable* image);

extern void LimpiarPantalla(Color color);

extern void ImprimirTexto(int x, int y, const char* text, u32 color);

extern void DesactivarGraficos();

extern void Graficos();

#ifdef __multimedia

extern "C" 
{
  #endif

     void IniciarMp3(int channel);
     void PararMp3();
     void FinalizarMp3();
  
     int CargarMp3(char *filename);
     int ReproducirMp3();
     int RepetirMp3();
     int DetenerMp3();
    
  #ifdef __multimedia
}

#endif
#endif
