#include <iostream>
#include <glad/glad.h>
#include <string>

namespace Renderer {

	class ShaderProgram{
	public:
		ShaderProgram(const std::string& vertex_shader, const std::string& fragment_shader);
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

		~ShaderProgram();

		void use();
		bool isCompiled() { return m_isCompiled; }

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;


	private:

		GLuint m_ID;
		bool m_isCompiled = false;
		bool createShader(const std::string& vertex_shader, GLenum shaderType, GLuint& shaderID);


	};

}

