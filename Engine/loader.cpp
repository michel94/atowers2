#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <string.h>
#include "loader.hpp"

static map<string, GLuint> textureCache;
static map<string, Model*> meshes;

int Loader::loadPng(string file_name) {
  // This function was originally written by David Grayson for
  // https://github.com/DavidEGrayson/ahrs-visualizer
  if(textureCache.find(file_name) != textureCache.end())
    return textureCache[file_name];
  
  int *width = new int, *height = new int;
  png_byte header[8];

  FILE *fp = fopen(file_name.c_str(), "rb");
  if (fp == 0)
  {
    perror(file_name.c_str());
    return 0;
  }

  // read the header
  fread(header, 1, 8, fp);

  if (png_sig_cmp(header, 0, 8))
  {
    fprintf(stderr, "error: %s is not a PNG.\n", file_name.c_str());
    fclose(fp);
    return 0;
  }

  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr)
  {
    fprintf(stderr, "error: png_create_read_struct returned 0.\n");
    fclose(fp);
    return 0;
  }

  // create png info struct
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    fprintf(stderr, "error: png_create_info_struct returned 0.\n");
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    fclose(fp);
    return 0;
  }

  // create png info struct
  png_infop end_info = png_create_info_struct(png_ptr);
  if (!end_info)
  {
    fprintf(stderr, "error: png_create_info_struct returned 0.\n");
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
    fclose(fp);
    return 0;
  }

  // the code in this if statement gets called if libpng encounters an error
  if (setjmp(png_jmpbuf(png_ptr))) {
    fprintf(stderr, "error from libpng\n");
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(fp);
    return 0;
  }

  // init png reading
  png_init_io(png_ptr, fp);

  // let libpng know you already read the first 8 bytes
  png_set_sig_bytes(png_ptr, 8);

  // read all the info up to the image data
  png_read_info(png_ptr, info_ptr);

  // variables to pass to get info
  int bit_depth, color_type;
  png_uint_32 temp_width, temp_height;

  // get info about png
  png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
    NULL, NULL, NULL);

  if (width){ *width = temp_width; }
  if (height){ *height = temp_height; }

  if (bit_depth != 8)
  {
    fprintf(stderr, "%s: Unsupported bit depth %d.  Must be 8.\n", file_name.c_str(), bit_depth);
    return 0;
  }

  GLint format;
  switch(color_type)
  {
  case PNG_COLOR_TYPE_RGB:
    format = GL_RGB;
    break;
  case PNG_COLOR_TYPE_RGB_ALPHA:
    format = GL_RGBA;
    break;
  default:
    fprintf(stderr, "%s: Unknown libpng color type %d.\n", file_name.c_str(), color_type);
    return 0;
  }

  // Update the png info struct.
  png_read_update_info(png_ptr, info_ptr);

  // Row size in bytes.
  int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

  // glTexImage2d requires rows to be 4-byte aligned
  rowbytes += 3 - ((rowbytes-1) % 4);

  // Allocate the image_data as a big block, to be given to opengl
  png_byte * image_data = (png_byte *)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
  if (image_data == NULL)
  {
    fprintf(stderr, "error: could not allocate memory for PNG image data\n");
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(fp);
    return 0;
  }

  // row_pointers is for pointing to image_data for reading the png with libpng
  png_byte ** row_pointers = (png_byte **)malloc(temp_height * sizeof(png_byte *));
  if (row_pointers == NULL)
  {
    fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    fclose(fp);
    return 0;
  }

  // set the individual row_pointers to point at the correct offsets of image_data
  for (unsigned int i = 0; i < temp_height; i++)
  {
    row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
  }

  // read the png into image_data through row_pointers
  png_read_image(png_ptr, row_pointers);

  // Generate the OpenGL texture object
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format, GL_UNSIGNED_BYTE, image_data);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // clean up
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
  free(image_data);
  free(row_pointers);
  fclose(fp);
  textureCache[file_name] = texture;
  return texture;
}

Model* Loader::loadModel(string name){
  if(meshes.find(name) != meshes.end())
    return meshes[name];

  Model* model;
  if(name == "cube"){
    model = new Model({
      0.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,

      0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 0.0f, 1.0f
    },{
      0,1,5,4,
      6,7,3,2,
      1,3,7,5,
      4,6,2,0,
      11,10,9,8
    },{
      0.0f,0.0f, 0.5f,0.0f, 0.5f,0.0f, 0.0f,0.0f,
      0.0f,1.0f, 0.5f,1.0f, 0.5f,1.0f, 0.0f,1.0f,
      0.5f,0.0f, 0.5f,1.0f, 1.0f,1.0f, 1.0f,0.0f
    }
   );

  }else{
    model = loadObj(name);
  }
  meshes[name] = model;

  return model;
}

void push(vector<float>& v, vec3 el){
  v.push_back(el.x);
  v.push_back(el.y);
  v.push_back(el.z);
}

void push(vector<float>& v, vec2 el){
  v.push_back(el.x);
  v.push_back(el.y);
}

Model* Loader::loadObj(string path){
  vector<vec3> vertices;
  vector<vec3> normals;
  vector<vec2> uvs;
  vector<Face> faces;

  int l = path.find_last_of("/");
  string name = path.substr(l+1, string::npos);
  string texture_path = path + "/" + name + ".png";
  string obj_path = path + "/" + name + ".obj";
  
  FILE *file = fopen(obj_path.c_str(), "r");

  if(!file){
    printf("OBJ file \"%s\" not found\n", obj_path.c_str());
    return NULL;
  }

  int tmp=1;
  char buffer[1000];
  char line[256], *tmp_c;
  while(fscanf(file, "%s", line) != EOF){
    vec3 coord;
    vec2 coord2;
    Face face;
    if(!strcmp(line, "v")){
      tmp = fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
      vertices.push_back(coord);
    } else if(!strcmp(line, "vn")){
      tmp = fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
      normals.push_back(coord);
    } else if(!strcmp(line, "vt")){
      tmp = fscanf(file, "%f %f", &coord2.x, &coord2.y);
      uvs.push_back(coord2);
    } else if(!strcmp(line, "f")){
      tmp = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &face.v_index[0], &face.t_index[0], &face.n_index[0],
      &face.v_index[1], &face.t_index[1], &face.n_index[1], &face.v_index[2], &face.t_index[2], &face.n_index[2]);

      faces.push_back(face);
    }else if(!strcmp(line, "#")){
      fgets(buffer, 1000, file);
    }else if(!strcmp(line, "g")){
      fgets(buffer, 1000, file);
    }
  }

  vector<float> out_vertices, out_normals;
  vector<float> out_uvs;
  vector<GLuint> out_elements;
  out_elements.resize(faces.size()*3);
  for(uint64_t i = 0; i < faces.size(); i++){
    out_elements.push_back(i*3);
    out_elements.push_back(i*3+1);
    out_elements.push_back(i*3+2);

    push(out_vertices, vertices[faces[i].v_index[0] - 1]);
    push(out_normals, normals[faces[i].n_index[0] - 1]);
    push(out_uvs, uvs[faces[i].t_index[0] - 1]);
    
    push(out_vertices, vertices[faces[i].v_index[1] - 1]);
    push(out_normals, normals[faces[i].n_index[1] - 1]);
    push(out_uvs, uvs[faces[i].t_index[1] - 1]);

    push(out_vertices, vertices[faces[i].v_index[2] - 1]);
    push(out_normals, normals[faces[i].n_index[2] - 1]);
    push(out_uvs, uvs[faces[i].t_index[2] - 1]);
  }

  fclose(file);
  
  Model* model = new Model(out_vertices, out_elements, out_uvs);
  model->texture = loadPng(texture_path.c_str());
  return model;
}
