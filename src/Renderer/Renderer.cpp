#include "Renderer.h"


namespace Renderer {
	
	ShaderProgram::ShaderProgram(const std::string& vertex_shader, const std::string& fragment_shader)
	{
		GLuint vertexShaderID;
		GLuint fragmentShaderID;

		if (!createShader(vertex_shader, GL_VERTEX_SHADER, vertexShaderID)) {
			std::cerr << "vertex shader compile time error!" << std::endl;
			return;
		}

		if (!createShader(fragment_shader, GL_FRAGMENT_SHADER, fragmentShaderID)) {
			std::cerr << "fragment shader compile time error!" << std::endl;
			glDeleteProgram(vertexShaderID);
			return;
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShaderID);
		glAttachShader(m_ID, fragmentShaderID);
		glLinkProgram(m_ID);

		GLint success;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

		if (!success) {
			GLchar infoLog[256];
			glGetProgramInfoLog(m_ID, 256, nullptr, infoLog);
			std::cerr << "LINK TIME ERROR: " << infoLog << std::endl;
		}
		else {
			m_isCompiled = true;
		}

		glDeleteProgram(vertexShaderID);
		glDeleteProgram(fragmentShaderID);

	}


	bool ShaderProgram::createShader(const std::string& source, GLenum shaderType, GLuint& shaderID) {

		shaderID = glCreateShader(shaderType);

		const char* code = source.c_str();

		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);

		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

		if (!success) {

			GLchar infoLog[256];
			glGetProgramInfoLog(shaderID, 256, nullptr, infoLog);
			std::cerr << "ERROR::CREATE_SHADER COMPILE TIME error: " << infoLog << std::endl;
			return false;
		}

		return true;
	}


	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(m_ID);
	}


	void ShaderProgram::use() {
		glUseProgram(m_ID);
	}


	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
	{
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_ID = 0;

	}


	ShaderProgram& Renderer::ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept {
		glDeleteProgram(m_ID);
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;
		return *this;
	}
}