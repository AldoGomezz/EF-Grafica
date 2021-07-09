#ifndef __MESH_H__
#define __MESH_H__

#include<glutil.h>
#include<vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TextCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	glm::u32 id;
	std::string type;
	std::string path;
};

class Mesh
{
public:	
	std::vector<Vertex> vertices;
	std::vector<glm::u32> indices;
	std::vector<Texture> texture;

	glm::u32 VAO;

	Mesh(std::vector<Vertex> vertices, std::vector<glm::u32> indices, std::vector<Texture> texture) :
		vertices(vertices), indices(indices), texture(texture) {
		setupMesh();
	}

	void Draw(Shader* shader) {
		glm::u32 diffuseNr = 1;
		glm::u32 specularNr = 1;
		glm::u32 normalNr = 1;
		glm::u32 heightNr = 1;

		for (glm::u32 i = 0; i < texture.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			std::string number;
			std::string name = texture[i].type;
			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseNr++);

			}
			else if (name == "texture_specular") {
				number = std::to_string(specularNr++);
			}
			else if (name == "texture_normal") {
				number = std::to_string(normalNr++);
			}
			else if (name == "texture_height") {
				number = std::to_string(heightNr++);
			}
		}
	}
private:
	glm::u32 vbo, ebo;
	void setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray;
	}
};



#endif // !__MESH_H__
