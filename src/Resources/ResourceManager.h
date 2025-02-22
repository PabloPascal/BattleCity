#pragma once 

#include <string>
#include <memory>
#include <map>

namespace Renderer {
	class ShaderProgram;
}


class ResourceManager {
public:
	ResourceManager(const std::string& executablePath);
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	
	std::shared_ptr<Renderer::ShaderProgram> loadShaders(const std::string& ShaderName,
		const std::string& vertexPath,const std::string& fragmentPath);

	std::shared_ptr<Renderer::ShaderProgram> getShader(const std::string& ShaderName) const;

private:
	std::string getFileString(const std::string& relativeFilePath);

	typedef std::map<std::string, std::shared_ptr<Renderer::ShaderProgram>> shaderProgramsMap;
	shaderProgramsMap m_shaderPrograms;

	std::string m_path;

};