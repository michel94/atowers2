#pragma once

#include "quad.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

class Textbox : public Quad {
public:

	Textbox(vec2 pos, int size, string text);
	~Textbox();

	void draw(float elapsed, ShaderData& shader, mat4* MVP);
  void setText(string t);
private:
	string text;
	FT_Face face;
	
	FT_Library* getFT(){
		static FT_Library ft;
		static bool ftInitialized = false;
		if(!ftInitialized){
			FT_Init_FreeType(&ft);
			ftInitialized = true;
		}
		return &ft;
	}

  vector<GLuint> textures;
};
