#include "textbox.hpp"

Textbox::Textbox(vec2 pos, int size, string text) : Quad(0, pos, vec2(0, size)){
	this->text = text;
  FT_Library* ft = getFT();
  FT_New_Face(*ft, "fonts/FreeSans.ttf", 0, &face);
  FT_Set_Pixel_Sizes(face, 0, size);
}

void Textbox::draw(float elapsed, ShaderData& shader, mat4* MVP){
  float x = pos.x, y = pos.y, sx = 0.1, sy = 0.1;

  const char *p;
  FT_GlyphSlot g = face->glyph;

  glUseProgram(0);
  glPushMatrix();

  glPushAttrib(GL_ENABLE_BIT);
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);

  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  const char* str = text.c_str();
  for(p=str; *p; p++) {
    if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
      continue;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
  
    float x2 = x + g->bitmap_left * sx;
      float y2 = -y - g->bitmap_top * sy;
      float w = g->bitmap.width * sx;
      float h = g->bitmap.rows * sy;
   
      GLfloat box[4][4] = {
          {x2,     -y2    , 0, 0},
          {x2,     -y2 - h, 0, 1},
          {x2 + w, -y2 - h, 1, 1},
          {x2 + w, -y2    , 1, 0},
      };
      
    glBegin(GL_QUADS);
      glTexCoord2f(box[0][2], box[0][3]); glVertex2f(box[0][0], box[0][1]);
      glTexCoord2f(box[1][2], box[1][3]); glVertex2f(box[1][0], box[1][1]);
      glTexCoord2f(box[2][2], box[2][3]); glVertex2f(box[2][0], box[2][1]);
      glTexCoord2f(box[3][2], box[3][3]); glVertex2f(box[3][0], box[3][1]);
      glTexCoord2f(box[0][2], box[0][3]);
    glEnd();
    
    x += (g->advance.x >> 6) * sx;
    y += (g->advance.y >> 6) * sy;
  }

  glDeleteTextures(1, &tex);

  glPopAttrib();
  glPopMatrix();
}

void Textbox::setText(string t){
  text = t;
}
