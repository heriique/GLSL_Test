#include "Pipeline.h"


Pipeline::Pipeline()
{
	glm::vec4 position = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0),
								 glm::vec3(0.0, 0.0, 0.0),
								 glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 model(1.0f);
	model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	
}


Pipeline::~Pipeline()
{
}
