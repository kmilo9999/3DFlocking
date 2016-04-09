#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>

#include <GL/glew.h>


class Texture
{
	

public:

	Texture();
    Texture(GLenum TextureTarget, const std::string& FileName);

    bool Load();

    void Bind(GLenum TextureUnit);
	std::string type;
	std::string m_fileName;
private:
    
    GLenum m_textureTarget;
    GLuint m_textureObj;
	
};


#endif	

