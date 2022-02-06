//GLSL是一种类C语言，为图形计算量身定制
/*
	#version version_number
	in type in_variable_name;
	in type in_variable_name;
	...
	out type out_variable_name;
	...
	uniform type uniform_name;
	...
	int main()
	{
		处理输入并进行一些图形操作
		处理输出结果到输出变量

	}

*/
#include<glad/glad.h>
#include<glfw3.h>
#include <iostream>
using namespace std;
void FrameBufferCallBack(GLFWwindow* window, int height, int width);
void exit(GLFWwindow* window);
float vertex[12] = {0.5f,0.5f,0.0f,-0.5f,0.5f,0.0f,-0.5f,-0.5f,0.0f,0.5f,-0.5f,0.0f};
unsigned int index[6] = {0,1,3,1,2,3};
int success;
char info[512];
const char* VertexShaderSource = 
	"#version 330 core\n"
	"layout(location=0)in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
	"}\0";
const char* FragmentShaderSource = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform vec4 color;\n"
	"void main()\n"
	"{\n"
	"FragColor=color;\n"
	"}\0";

int main()
{/*
 1.初始化窗口，调整窗口大小
 2.添加必要的窗口回调函数和辅助函数
 3.添加顶点数据
 4.设置顶点属性
 5.构建着色器
 6.链接着色器，启用连接程序
 7.开始绘画
 */

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800,600,"test",NULL,NULL);
	if (window == NULL)
	{
		cout << "Failed to create a WINDOW" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout<<"Failed to initialize GLAD"<<endl;
		return -1;

	}
	glViewport(0,0,800,600);
	glfwSetFramebufferSizeCallback(window,FrameBufferCallBack);
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1,&VBO);
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	unsigned int VertexShader,FragmentShader,ShaderProgram;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(VertexShader,1,&VertexShaderSource,NULL);
	glCompileShader(VertexShader);
	glGetShaderiv(VertexShader,GL_COMPILE_STATUS,&success);
	if (!success)
	{
		glGetShaderInfoLog(VertexShader,512,NULL,info);
		cout << "ERROR::SHADER::VERTEXSHADER::FAILED\n" <<info<<endl;
	}
	glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShader(FragmentShader);
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, info);
		cout << "ERROR::SHADER::FRAGMENTSHADER::FAILED\n" <<info<< endl;
	}
	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram,VertexShader);
	glAttachShader(ShaderProgram,FragmentShader);
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{	
		glGetProgramInfoLog(ShaderProgram, 512, NULL, info);
		std::cout << "ERROR::LINK::COMPILATION_FAILED\n" << info << std::endl;
	}
	glUseProgram(ShaderProgram);
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
	float TimeValue,Value1,Value2;
	int VertexColorLocation;
	while (!glfwWindowShouldClose(window))
	{
		exit(window);
		glClearColor(0.1f,0.2f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glfwGetTime()获取运行的秒数
		//在之前的FragmentShaderSource设置了一个名为color的uniform变量，可以去前面看一下
		TimeValue = glfwGetTime();
		Value1 = (sin(TimeValue) / 2.0f) + 0.5f;
		Value2 = (cos(TimeValue)/2.0f);
		//glGetUniformLocation:查询当前激活的着色器链接程序里uniform变量的位置，如果未查到则返回-1
		VertexColorLocation = glGetUniformLocation(ShaderProgram,"color");
		glUseProgram(ShaderProgram);
		//glUniform4f:设置某个四维float uniform变量的值，对应的还有三维二维等，也有int uniform、double uniform等,举个例子：glUniform3d
		glUniform4f(VertexColorLocation,Value2,Value1,0.0f,1.0f);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1,&VBO);
	glDeleteProgram(ShaderProgram);
	return 0;
}
void FrameBufferCallBack(GLFWwindow* window,int height,int width)
{
	glViewport(0,0,height,width);
}
void exit(GLFWwindow* window)
{
	if (glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window,true);
	}
}
