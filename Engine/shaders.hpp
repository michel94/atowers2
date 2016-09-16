#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "opengl.h"

using namespace std;

class ShaderData{
public:
	ShaderData(){
		
	}
	~ShaderData();

	int operator[] (const string& var) {
		return variableIds[var];
	}
	GLuint getProgram(){
		return programId;
	}
	void setVariable(const string& var, GLuint id){
		variableIds[var] = id;
	}

	GLuint programId;
  string name;
private:
	map<string, int> variableIds;

};

typedef map<string, ShaderData*> ShaderMap;

ShaderData* loadShader(const string basePath);
ShaderData* loadShader(const string vertex_file_path, const string fragment_file_path);
