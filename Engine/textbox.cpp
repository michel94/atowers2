#include "textbox.hpp"

Textbox::Textbox(vec2 pos, int size, string text) : Quad(0, pos, vec2(0, size)){
  setShader("textured");
  FT_Library* ft = getFT();
  FT_New_Face(*ft, "fonts/FreeSans.ttf", 0, &face);
  FT_Set_Pixel_Sizes(face, 0, size);

  setText(text);
}

void Textbox::draw(float elapsed, ShaderData& shader, mat4* MVP){
  float x = pos.x, y = pos.y, sx = 0.1, sy = 0.1;

  const char *p;
  FT_GlyphSlot g = face->glyph;

  glPushAttrib(GL_ENABLE_BIT);
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);

  int n;
  const char* str = text.c_str();
  for(n=0, p=str; *p; p++, n++) {
    if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
      continue;

    float x2 = x + g->bitmap_left * sx;
    float y2 = y + g->bitmap_top * sy;
    float w = g->bitmap.width * sx;
    float h = g->bitmap.rows * sy;

    mat4 localMVP = translate(*MVP, vec3(x2, y2, 0));
    localMVP = scale(localMVP, vec3(w, -h, 0));
    glUniformMatrix4fv(shader["MVP"], 1, GL_FALSE, &localMVP[0][0]);
    Drawable::drawTexturedQuads(model->elementsVBO, model->verticesVBO, model->elements.size(), textures[n], model->texCoordsVBO);

    x += (g->advance.x >> 6) * sx;
    y += (g->advance.y >> 6) * sy;
  }

  glPopAttrib();

}

void Textbox::setText(string t){
  text = t;

  glDeleteTextures(textures.size(), &textures[0]);
  textures.resize(t.size());

  const char *p;
  FT_GlyphSlot g = face->glyph;

  glPushAttrib(GL_ENABLE_BIT);
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);

  const char* str = text.c_str();
  int n;
  for(n=0, p=str; *p; p++, n++) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
      continue;

    int width = g->bitmap.width , height = g->bitmap.rows;
    GLubyte* expanded_data = new GLubyte[ 2 * width * height];

    for(int j=0; j <height;j++) {
        for(int i=0; i < width; i++){
            expanded_data[2*(i+j*width)+0] = expanded_data[2*(i+j*width)+1] =
                g->bitmap.buffer[i + g->bitmap.width*j];
        }
    }

    glColor3f(1.0,1.0,1.0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

    textures[n] = tex;

  }

}

