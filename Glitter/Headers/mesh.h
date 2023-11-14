#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <algorithm>
#include "shader.h"

#define INFINITY 1e9

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

struct Box {
    glm::vec3 max;
    glm::vec3 min;
    glm::vec3 center;
};

class Mesh {
public:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader& shader);
    Box getBoundingBox();
private:
	// render data
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
	this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

Box Mesh::getBoundingBox() {
    glm::vec3 min = glm::vec3(INFINITY);
    glm::vec3 max = glm::vec3(-INFINITY);
    for(unsigned int i = 0; i < vertices.size(); i++) {
        min.x = std::min(vertices[i].Position.x, min.x);
        min.y = std::min(vertices[i].Position.y, min.y);
        min.z = std::min(vertices[i].Position.z, min.z);

        max.x = std::max(vertices[i].Position.x, max.x);
        max.y = std::max(vertices[i].Position.y, max.y);
        max.z = std::max(vertices[i].Position.z, max.z);
    }
    glm::vec3 center = (min + max) / 2.0f;
    return Box{max, min, center};
}

void Mesh::setupMesh() {

	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0); // 解绑
}

void Mesh::Draw(Shader& shader) {
	unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++) {
    	glActiveTexture(GL_TEXTURE0 + i);
    	std::string number;
    	std::string name = textures[i].type;
    	if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
        else if(name == "texture_normal")
            number = std::to_string(normalNr++);
        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
#endif


















