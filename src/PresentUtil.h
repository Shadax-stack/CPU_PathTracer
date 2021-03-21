#pragma once

#include "Texture.h"
#include "Color.h"

void InitOpenGL(void);

void InitRendererResources(Texture<ColorRGB_32F>* Tex, unsigned int* ScrTex);
void FreeRendererResources(Texture<ColorRGB_32F>* Tex, unsigned int* ScrTex);

void PresentImage(Texture<ColorRGB_32F>* Tex, unsigned int ScrTex);
