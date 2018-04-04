#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>
#include "gl_core_4_5.h"

enum TextureFormat{
	TEX_FORMAT_RGB32F,
	TEX_FORMAT_RGBA,
	TEX_FORMAT_RGB,
	TEX_FORMAT_RG,
	TEX_FORMAT_R,
	TEX_FORMAT_DEPTH,
	TEX_FORMAT_COUNT
};

enum WrapMode{
	WRAP_MODE_REPEAT,
	WRAP_MODE_CLAMP,
	WRAP_MODE_MIRROR,
	WRAP_MODE_COUNT
};

class Texture {
public:
	Texture();
	virtual ~Texture();

	void LoadFromFile(std::string filename, TextureFormat format, WrapMode wrap, bool compress=true);
	void Init(TextureFormat format, WrapMode wrap, unsigned int width, unsigned int height, bool compress=false, void *data=NULL);
	int getTexture(){
		return _texture;
	}
	void DeleteTexture();
	void Bind();
	static void InitDefaultTex();

private:
	GLuint _texture;
	TextureFormat _format;
	WrapMode _wrap;
};

#endif /* TEXTURE_H_ */
