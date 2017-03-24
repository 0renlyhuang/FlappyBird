#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "gl\glew.h"
#include "gl\freeglut.h"
#include "gl\GL.h"


using std::string;
using std::ios;
using std::stringstream;
using std::cout;
using std::endl;
using std::ifstream;

// 对OpenGL创建着色器的一个封装
class Shader {
public:
	Shader(const char *vertexPath, const char *fragmentPath) {
		string vertexCode, fragmentCode;
		{
			FileHelper vertexFile(vertexPath);
			FileHelper fragmentFile(fragmentPath);
			vertexCode = vertexFile.read();
			fragmentCode = fragmentFile.read();
		}

		const GLchar *vShaderCode = vertexCode.c_str();
		const GLchar *fShaderCode = fragmentCode.c_str();

		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		program_ = glCreateProgram();
		glAttachShader(program_, vertex);
		glAttachShader(program_, fragment);
		glLinkProgram(program_);
		glGetProgramiv(program_, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program_, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}


	Shader(const char *vertexPath, const char *geometryPath, const char *fragmentPath) {
		string vertexCode, geometryCode, fragmentCode;
		{
			FileHelper vertexFile(vertexPath);
			FileHelper geometryFile(geometryPath);
			FileHelper fragmentFile(fragmentPath);
			vertexCode = vertexFile.read();
			geometryCode = geometryFile.read();
			fragmentCode = fragmentFile.read();
		}

		const GLchar *vShaderCode = vertexCode.c_str();
		const GLchar *gShaderCode = geometryCode.c_str();
		const GLchar *fShaderCode = fragmentCode.c_str();


		GLuint vertex, geometry, fragment;
		GLint success;
		GLchar infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		program_ = glCreateProgram();
		glAttachShader(program_, vertex);
		glAttachShader(program_, geometry);
		glAttachShader(program_, fragment);
		glLinkProgram(program_);
		glGetProgramiv(program_, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program_, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(geometry);
		glDeleteShader(fragment);
	}


	void use() const noexcept { glUseProgram(program_); }

	GLuint getProgram() const noexcept { return program_; }

private:
	GLuint program_;

	class FileHelper {
	public:
		FileHelper(const char *filePath) : path_(filePath), isLoaded_(false),
			s_(), content_() {
			s_.exceptions(std::ifstream::badbit);
		}
		~FileHelper() {
			if (isLoaded_)
				s_.close();
		}

		FileHelper(const FileHelper&) = delete;
		FileHelper &operator=(const FileHelper&) = delete;

		string read() {
			if (isLoaded_)
				return content_;

			try {
				isLoaded_ = true;
				s_.open(path_, ios::in);
				stringstream ss;
				ss << s_.rdbuf();
				content_ = ss.str();
				return content_;
			}
			catch (std::ifstream::failure &e) {
				cout << "Failed to load file: " << e.what() << endl;
				return "";
			}
		}

	private:
		ifstream s_;
		string path_;
		string content_;
		bool isLoaded_;
	};

};

#endif // !SHADER_H

