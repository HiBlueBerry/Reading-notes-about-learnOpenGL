//该代码与原书代码有较大出入
//该代码与原书代码有较大出入
//该代码与原书代码有较大出入
//该代码与原书代码有较大出入
//该代码与原书代码有较大出入
#include<glad/glad.h>
#include<glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
void FrameCallBack(GLFWwindow* window, int width, int height);
char info[512];
int success;
float vertex[] = {
	 0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
	 0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	-0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
	 0.5f,  0.5f,-0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
	 0.5f, -0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	-0.5f, -0.5f,-0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
	-0.5f,  0.5f,-0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
};
float NormalVector[] = {

	0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f

}; 
int index[] = { 0,1,3,1,2,3,0,4,1,1,4,5,1,5,6,6,2,1,2,6,3,3,7,6,3,7,4,4,0,3,7,4,6,6,4,5 };
const char* Vsource = "#version 330 core\n"
"layout(location=0)in vec3 aPos;\n"
"layout(location=1)in vec3 NV;"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"void main()\n"
"{\n"
"gl_Position=projection*view*model*vec4(aPos,1.0);\n"
"Normal=mat3(transpose(inverse(model)))*NV;\n"
"FragPos=vec3(model*vec4(aPos,1.0f));"
"}\0";
//法线变换矩阵等于模型矩阵左上角的逆矩阵的转置矩阵,大部分的资源都会将法线矩阵定义为应用到模型-观察矩阵(Model-view Matrix)上的操作，但是由于我们只在世界空间中进行操作（不是在观察空间），我们只使用模型矩阵
//注意我们还要把被处理过的矩阵强制转换为3×3矩阵，来保证它失去了位移属性以及能够乘以vec3的法向量。
//用到了reflect(入射向量,法向量)函数，这个是根据入射向量和法向量计算出出射向量
//冯氏模型：环境光(ambient)+漫反射光(diffuse)+镜面反射光(specular)
const char* Fsource = "#version 330 core\n"
"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"out vec4 FragColor;\n"
"uniform vec3 lightPos=vec3(1.2f, 1.0f, 2.0f);\n"
"uniform vec3 objectColor;\n"
"uniform vec3 lightColor;\n"
"void main()\n"
"{\n"
"vec3 norm = normalize(Normal);\n"
"vec3 lightDir=normalize(lightPos-FragPos);\n"
"vec3 cameraPos=vec3(0.0f,0.0f,3.0f);\n"
"vec3 viewDir=normalize(cameraPos-FragPos);\n"
"vec3 reflectDir=reflect(-lightDir,norm);\n"
"float spec=pow(max(dot(viewDir,reflectDir),0.0),32);"
"float diff=max(dot(norm,lightDir),0.0);"
"FragColor=vec4((diff+0.2f+0.5*spec)*lightColor*objectColor,1.0);\n"
"}\0";
glm::vec3 cameraFront = glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f,1.0f,0.0f);
glm::mat4 view = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 projection=glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);
glm::vec3 lightPos = glm::vec3(1.2f,1.0f,2.0f);
int main()
{
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800,600,"test",NULL,NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		
	}
	glViewport(0,0,800,600);
	glfwSetWindowSizeCallback(window,FrameCallBack);
	unsigned int VAO, VBO, EBO,NVO;
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&NVO);
	glGenBuffers(1,&EBO);
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertex),vertex,GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, NVO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(NormalVector),NormalVector,GL_STATIC_DRAW);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(1);
	unsigned int VS, FS,SP;
	VS = glCreateShader(GL_VERTEX_SHADER);
	FS = glCreateShader(GL_FRAGMENT_SHADER);
	SP = glCreateProgram();
	glShaderSource(VS,1,&Vsource,NULL);
	glCompileShader(VS);
	glGetShaderiv(VS,GL_COMPILE_STATUS,&success);
	if (!success)
	{
		glGetShaderInfoLog(VS,512,NULL,info);
		std::cout << info << std::endl;
	}
	glShaderSource(FS,1,&Fsource,NULL);
	glCompileShader(FS);
	glGetShaderiv(FS, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(FS, 512, NULL, info);
		std::cout << info << std::endl;
	}
	glAttachShader(SP,VS);
	glAttachShader(SP, FS);
	glLinkProgram(SP);
	glGetProgramiv(SP, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(SP, 512, NULL, info);
		std::cout << "ERROR::LINK::COMPILATION_FAILED\n" << info << std::endl;
	}
	glUseProgram(SP);
	float time = sin((float)glfwGetTime());
	glUniform3f(glGetUniformLocation(SP, "objectColor"),1.0f, 0.5f,0.31f);
	glUniform3f(glGetUniformLocation(SP, "lightColor"), 1.0f, 1.0f, 1.0f);
	glDeleteShader(VS);
	glDeleteShader(FS);
	glEnable(GL_DEPTH_TEST);
	glUniformMatrix4fv(glGetUniformLocation(SP, "projection"),1,GL_FALSE,glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(SP, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(SP, "model"), 1, GL_FALSE, glm::value_ptr(model));
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f,0.1f,0.1f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glUseProgram(SP);
		time = sin((float)glfwGetTime());
		glUniform3f(glGetUniformLocation(SP, "objectColor"), 1.0f, time, 0.31f);
		model = glm::rotate(model, glm::radians(-55.0f*(float)(glfwGetTime())), glm::vec3(-0.5f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(SP, "model"), 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1.0f);
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();

		

	}
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1,&VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}
void FrameCallBack(GLFWwindow* window, int width, int height)
{
	glViewport(0,0,width,height);
}
