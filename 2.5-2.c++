#include<glad/glad.h>
#include<glfw3.h>
#include <iostream>
using namespace std;
void FrameBufferCallBack(GLFWwindow* window, int height, int width);
void exit(GLFWwindow* window);
float vertex[24] = {0.5f,0.5f,0.0f,1.0f,0.0f,0.0f,-0.5f,0.5f,0.0f,0.0f,1.0f,0.0f,-0.5f,-0.5f,0.0f,0.0f,0.0f,1.0f,0.5f,-0.5f,0.0f,0.5f,0.5f,0.5f};
unsigned int index[6] = {0,1,3,1,2,3};
int success;
char info[512];
//location是变量的位置值，类似于标签
const char* VertexShaderSource = 
	"#version 330 core\n"
	"layout(location=0)in vec3 aPos;\n"
	"layout(location=1)in vec3 aColor;\n"
	"out vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	"gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
	"ourColor=aColor;\n"
	"}\0";
  //VertexShader将它的ourColor变量传给FragmentShader的ourColor
const char* FragmentShaderSource = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;"
	"uniform vec3 color;"
	"void main()\n"
	"{\n"
	"FragColor=vec4(ourColor,1.0);\n"
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
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
  //启用当前绑定的VBO的位置值为0的数组，并且将前一句代码所指代的所有数据放进去
	glEnableVertexAttribArray(0);
  //因为vertex数组改成了{（顶点位置1+顶点颜色1）,（顶点位置2+顶点颜色2）,（顶点位置3+顶点颜色3）,（顶点位置4+顶点颜色4）}的形式，颜色值偏移3个float的大小，即从vertex[3]开始数
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
  //启用当前绑定的VBO的位置值为1的数组，并且将前一句代码所指代的所有数据放进去
  glEnableVertexAttribArray(1);
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
		glUseProgram(ShaderProgram);
		glBindVertexArray(VAO);
    //使用location=0的数据进行绘制
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
