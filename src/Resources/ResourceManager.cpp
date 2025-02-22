#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"


#include <iostream>
#include <fstream>
#include <sstream>

ResourceManager::ResourceManager(const std::string& executablePath) {

	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);

}


std::string ResourceManager::getFileString(const std::string& relativeFilePath)
{
	std::fstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in, std::ios::binary);

	if (!f.is_open()) {
		std::cerr << "Can't open the file" << relativeFilePath << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << f.rdbuf();


	return buffer.str();
}



std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& ShaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexString = getFileString(vertexPath);

	if (vertexString.empty()) {
		std::cerr << "no vertex shader" << std::endl;
		return nullptr;
	}

	std::string fragmentString = getFileString(fragmentPath);

	if (fragmentString.empty()) {
		std::cerr << "no fragmenr shader" << std::endl;
		return nullptr;
	}

	auto& newShader = m_shaderPrograms.emplace(ShaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;

	if (newShader->isCompiled()) {
		return newShader;
	}
	else {
		std::cerr << "Can't load shaderProgram!: " << vertexPath << fragmentPath << std::endl;
		return nullptr;
	}
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShader(const std::string& ShaderName) const
{
	auto it = m_shaderPrograms.find(ShaderName);
	if (it == m_shaderPrograms.end()) {
		std::cerr << "Can't find shader program!" << std::endl;
		return nullptr;
	}

	return it->second;
}
