#define GLM_FORCE_RADIANS
#include "Texture.h"
#include <IL/il.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

Texture::Texture() {
	_texture = 0;
}

Texture::~Texture() {

}

void Texture::LoadFromFile(std::string filename, TextureFormat format,
		WrapMode wrap, bool compress) {

	cerr << "Loading texture " << filename << endl;
	ILuint image = ilGenImage();
	ilBindImage(image);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ILstring tex_path = (ILstring) filename.c_str();

	ILboolean success = ilLoadImage(tex_path);

	if (!success) {
		cerr << "Could not load image " << filename << endl;
		cerr << "Exiting..." << endl;
		exit(1);
	}

	ILenum f, t;
	switch (format) {
	case TEX_FORMAT_RGB32F:
		f = IL_RGB;
		t = IL_FLOAT;
		break;
	case TEX_FORMAT_RGBA:
	case TEX_FORMAT_RGB:
	case TEX_FORMAT_RG:
		f = IL_RGBA;
		t = IL_UNSIGNED_BYTE;
		break;
	case TEX_FORMAT_R:
	case TEX_FORMAT_DEPTH:
		f = IL_LUMINANCE;
		t = IL_UNSIGNED_BYTE;
		break;
	default:
		break;
	}

	ilConvertImage(f, t);
	unsigned int w, h;
	w = ilGetInteger(IL_IMAGE_WIDTH);
	h = ilGetInteger(IL_IMAGE_HEIGHT);
	if (w == 0 || h == 0) {
		cerr << "Invalid Texture..." << endl;
		exit(1);
	}
	void *data = ilGetData();

	Init(format, wrap, w, h, compress, data);

	ilBindImage(0);
	ilDeleteImage(image);

	cerr << "Loaded Texture " << filename << endl;
}

void Texture::Init(TextureFormat format, WrapMode wrap, unsigned int width,
		unsigned int height, bool compress, void *data) {
	GLint gl_format, gl_type, gl_wrap, gl_internal_format;

	switch (format) {
	case TEX_FORMAT_RGB32F:
		gl_format = GL_RGB;
		gl_type = GL_FLOAT;
		if (compress)
			gl_internal_format = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
		else
			gl_internal_format = GL_RGB32F;
		break;
	case TEX_FORMAT_RGBA:
	case TEX_FORMAT_RGB:
		gl_format = GL_RGBA;
		gl_type = GL_UNSIGNED_BYTE;
		if (compress)
			gl_internal_format = GL_COMPRESSED_RGBA_BPTC_UNORM;
		else
			gl_internal_format = GL_RGBA8;
		break;
	case TEX_FORMAT_RG:
		gl_format = GL_RGBA;
		gl_type = GL_UNSIGNED_BYTE;
		if (compress)
			gl_internal_format = GL_COMPRESSED_RG_RGTC2;
		else
			gl_internal_format = GL_RG8;
		break;
	case TEX_FORMAT_R:
		gl_format = GL_RED;
		gl_type = GL_UNSIGNED_BYTE;
		if (compress)
			gl_internal_format = GL_COMPRESSED_RED_RGTC1;
		else
			gl_internal_format = GL_R8;
		break;
	case TEX_FORMAT_DEPTH:
		gl_format = GL_RED;
		gl_type = GL_UNSIGNED_BYTE;
		gl_internal_format = GL_DEPTH_COMPONENT24;
		break;
	default:
		break;
	}

	switch (wrap) {
	case WRAP_MODE_REPEAT:
		gl_wrap = GL_REPEAT;
		break;
	case WRAP_MODE_CLAMP:
		gl_wrap = GL_CLAMP_TO_EDGE;
		break;
	case WRAP_MODE_MIRROR:
		gl_wrap = GL_MIRRORED_REPEAT;
		break;
	default:
		gl_wrap = GL_REPEAT;
		break;
	}
	gl_wrap = GL_REPEAT;
	glGenTextures(1, &_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrap);

	if (data) {
		//glTexImage2D(GL_TEXTURE_2D, 0, gl_internal_format, width, height, 0, gl_format, gl_type, data);
		int mipmap_levels = (int) log2(fmaxf(float(width), float(height)));
		glTexStorage2D(GL_TEXTURE_2D, mipmap_levels, gl_internal_format, width, height);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, gl_format, gl_type, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		float anisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
				anisotropy);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, gl_internal_format, width, height, 0, gl_format,
				gl_type, data);
	}
	glActiveTexture(GL_TEXTURE0);
	_format = format;
	_wrap = wrap;

	glFlush();
}

void Texture::DeleteTexture() {
	glDeleteTextures(1, &_texture);
}

void Texture::InitDefaultTex() {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int data = 0xFFFFFFFF;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			&data);

}

void Texture::Bind() {
	glBindTexture(GL_TEXTURE_2D, _texture);
}
