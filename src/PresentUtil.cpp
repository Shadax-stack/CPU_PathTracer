#include <glad/glad.h>
#include <memory>
#include <stdio.h>
#include "PresentUtil.h"

// This is all the messy OpenGL code that is used to present the result of the path tracer to the window

static GLuint FullscreenQuad, PresentShader;

inline static unsigned int CompileShader(const char* source, unsigned int type) {
#define glCall(x) x
	//std::cout << source << std::endl;
	unsigned int Shader = glCall(glCreateShader(type));
	glCall(glShaderSource(Shader, 1, &source, nullptr));
	glCall(glCompileShader(Shader));
	int CompileStatus;
	glCall(glGetShaderiv(Shader, GL_COMPILE_STATUS, &CompileStatus));
	//Since GL_FALSE is false and GL_TRUE is true, we can use CompileStatus like a regular old boolean now
	if (CompileStatus) {
		return Shader;
	}
	else {
		int ShaderLogLength;
		glCall(glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &ShaderLogLength));
		std::unique_ptr<char> ShaderCompileLog(new char[static_cast<unsigned long long>(ShaderLogLength) + 1ULL]);
		ShaderCompileLog.get()[ShaderLogLength] = '\0';
		glCall(glGetShaderInfoLog(Shader, ShaderLogLength, &ShaderLogLength, ShaderCompileLog.get()));
		printf("Compiler error:%s\n", ShaderCompileLog.get());
		//ThrowException(ShaderCompileLog.get());
		return 0;
	}
}


void InitOpenGL(void) {
	gladLoadGL();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	GLuint Buffer;

	glGenBuffers(1, &FullscreenQuad);
	glGenVertexArrays(1, &Buffer);

	glBindVertexArray(FullscreenQuad);
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);

	float Vertices[] =
	{
		-1.0f,  1.0f,  0.0f, 1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f, -1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,  1.0f,  1.0f,  1.0f, 1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, 0, 4 * sizeof(GLfloat), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, 0, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//glDeleteBuffers(1, &Buffer);

	GLuint VertexShader, FragmentShader;

	const char* VS_source =
		"#version 330 core\n"
		"layout (location = 0) in vec2 a_Position;"
		"layout(location = 1) in vec2 a_TexCoord; "
		"out vec2 fTexCoords;"
		"void main() {\n"
		"gl_Position = vec4(a_Position, 0.0f, 1.0f);"
		"fTexCoords = a_TexCoord;"
		"}";

	const char* FS_source =
		"#version 330 core\n"
		"in vec2 fTexCoords;"
		"out vec3 color;"
		"uniform sampler2D BackBuffer;"
		"void main() {color = texture2D(BackBuffer, fTexCoords).rgb;}";

	VertexShader = CompileShader(VS_source, GL_VERTEX_SHADER);
	FragmentShader = CompileShader(FS_source, GL_FRAGMENT_SHADER);

	PresentShader = glCreateProgram();

	glAttachShader(PresentShader, VertexShader);
	glAttachShader(PresentShader, FragmentShader);
	glLinkProgram(PresentShader);
	glValidateProgram(PresentShader);
}

float ConvertLinearToSRGB(float X) {
	if (X < 0.0031308) {
		X *= 12.92f;
		return X;
	} else {
		X = powf(X, 1.0f / 2.4f);
		X *= 1.055f;
		X -= 0.055f;
		return X;
	}
}

void InitRendererResources(Texture<ColorRGB_32F>* Tex, unsigned int* ScrTex) {
	glGenTextures(1, ScrTex);
	glBindTexture(GL_TEXTURE_2D, *ScrTex);
	// Preallocate memory
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)Tex->GetWidth(), (GLsizei)Tex->GetHeight(), 0, GL_RGB, GL_FLOAT, nullptr);
	// Set settings of the texture
	const static float BorderColor[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, BorderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void FreeRendererResources(Texture<ColorRGB_32F>* Tex, unsigned int* ScrTex) {
	glDeleteTextures(1, ScrTex);
}

void CheckedTextureBind(unsigned int Tex) {
	static unsigned int CurrentlyBoundTexture = 0;

	if (Tex != CurrentlyBoundTexture) {
		glBindTexture(GL_TEXTURE_2D, Tex);
		CurrentlyBoundTexture = Tex;
	}
}

void PresentImage(Texture<ColorRGB_32F>* Tex, unsigned int ScrTex) {
	CheckedTextureBind(ScrTex);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(PresentShader);

	// Apply gamma correction
	for (size_t Y = 0; Y < Tex->GetHeight(); Y++) {
		for (size_t X = 0; X < Tex->GetWidth(); X++) {
			ColorRGB_32F Color = Tex->Sample(X, Y);

			Color.R = ConvertLinearToSRGB(Color.R);
			Color.G = ConvertLinearToSRGB(Color.G);
			Color.B = ConvertLinearToSRGB(Color.B);

			Tex->Write(X, Y, Color);
		}
	}


	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)Tex->GetWidth(), (GLsizei)Tex->GetHeight(), GL_RGB, GL_FLOAT, Tex->GetRawData());
	glDrawArrays(GL_TRIANGLES, 0, 6);
}