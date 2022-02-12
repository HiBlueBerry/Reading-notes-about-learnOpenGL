#include<glad/glad.h>
#include<glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
using namespace std;
int success;
void FramebufferSizeCallback1(GLFWwindow* window, int width, int height);
glm::vec3 processInput(GLFWwindow* window, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp,float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
const char* VertexShaderSource =
"#version 330 core\n"
"layout(location=0)in vec3 aPos;\n"
"layout(location=1)in vec3 aColor;\n"
"layout(location=2)in vec2 aTexCoord;\n"
"uniform mat4 transform;"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"gl_Position=transform*vec4(aPos.x,aPos.y,aPos.z,1.0f);\n"
"ourColor=aColor;\n"
"TexCoord=aTexCoord;\n"
"}\0";
//使用GLSL内建的texture函数来采样纹理颜色
//GLSL内置一个Sampler(采样器，有1D、2D、3D等)类型
//texture:纹理采样器，纹理坐标
const char* FragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"in vec3 ourColor;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
"FragColor=texture(ourTexture,TexCoord)* vec4(ourColor, 1.0);"
"}\0"
;
char info[512];
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
int index[] = { 0,1,3,1,2,3,0,4,1,1,4,5,1,5,6,6,2,1,2,6,3,3,7,6,3,7,4,4,0,3,7,4,6,6,4,5 };
//设置鼠标初始位置以及灵敏度
float lastX=400.0f, lastY=300.0f;
float sensitivity = 0.02f;
//设置俯仰角(pitch)，偏航角(yaw)，滚转角(roll)
float pitch=0.0f, yaw=-90.0f, roll=0.0f;
//设置一个bool值判断鼠标是否第一次进入窗口中，其作用请见下面的mouse_callback()函数
bool firstIn=true;
float fov=45.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection;
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "test", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback1);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window,scroll_callback);
	unsigned int VAO, VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int VertexShader, FragmentShader, ShaderProgram;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(VertexShader, 1, &VertexShaderSource, NULL);
	glCompileShader(VertexShader);
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, info);
		cout << "ERROR::SHADER::VERTEXSHADER::FAILED\n" << info << endl;
	}
	glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShader(FragmentShader);
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, info);
		cout << "ERROR::SHADER::FRAGMENTSHADER::FAILED\n" << info << endl;
	}
	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
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
	glUniform1i(glGetUniformLocation(ShaderProgram, "ourTexture"), 0);
	int width, height, nrChannels;
	//stbi_load:纹理图片路径，宽度，高度，颜色通道数，你所需要的通道数
	unsigned char* data = stbi_load("C:\\Users\\Windows DEMO\\Pictures\\中秋\\测试.png", &width, &height, &nrChannels, 0);
	cout << data;
	unsigned int texture;
	//生成texture对象并进行绑定
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	//为当前绑定的纹理对象设置纹理环绕和纹理过滤方式
	//纹理采用str(如果使用3D纹理则有r属性)坐标轴，这类似于x,y,z
	//glTexParameteri:纹理目标类型，指定设置选项或者纹理轴，指定环绕方式或者过滤方式
	//环绕方式有GL_REPEAT（重复）GL_MIRRORED_REPEAT（镜像重复）GL_CLAMP_TO_EDGE(重复纹理边缘)和GL_CLAMP_TO_BORDER（指定边缘颜色）
	//PS:当使用GL_CLAMP_TO_BORDER时请同时使用glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);其中borderColor是一个float数组
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//过滤方式有GL_NEAREST（邻近过滤）和GL_LINEAR（线性过滤）
	//关于多级渐远（既然是渐远那说明只能用到GL_TEXTURE_MIN_FILTER上）纹理有GL_NEAREST_MIPMAP_NEAREST，GL_LINEAR_MIPMAP_NEAREST	，GL_NEAREST_MIPMAP_LINEAR，GL_LINEAR_MIPMAP_LINEAR。
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//使用glTexImage2D载入图片数据生成一个纹理
	//glTexImage2D参数:
	//第一个指定纹理目标类型
	//第二个参数为纹理指定多级渐远纹理的级别，0是基本级别
	//第三个参数指示把纹理存储为何种格式
	//第四个和第五个参数指示纹理的宽度和高度
	//第六个参数应该总是被设为0(不知道为什么)
	//第七个和第八个参数指示源图的格式和数据类型,在这里使用了RGB加载图像，并把它们储存为char(byte)数组
	//第九个参数是图像真正的数据
	//当调用glTexImage2D时，当前绑定的纹理对象就会被附加上纹理图像，目前仅有基本级别的纹理图像被加载了
	//如果要使用多级渐远纹理，必须手动设置所有不同的图像(不断递增参数二)，或者在生成纹理后调用glGenerateMipmap自动生成多级渐远纹理
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//必须生成多级渐远纹理
	glGenerateMipmap(GL_TEXTURE_2D);
	//生成纹理后记得释放图像内存
	stbi_image_free(data);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::lookAt（用于生成一个相机，确定它的位置以及朝向，用mat4存储）：相机位置，相机目标，上向量。（它创建了一组三维基，使用的是施密特正交化，其中一个基底方向是【相机目标-相机位置】）
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	//由变换顺序可知最终得到的坐标向量=投影矩阵*观察矩阵*模型矩阵*原坐标向量（注意，矩阵运算不满足交换律，应该从右向左阅读矩阵乘法）
	glm::mat4 trans = projection * view * model;;
	//启用Z缓冲，判断像素重合点z值，保留z值低的像素属性
	glEnable(GL_DEPTH_TEST);
	//同理还有glDisable(....)
	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	float currentTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//每次渲染之前清除深度缓冲，否则前一帧的深度信息仍保留在缓冲中
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUseProgram(ShaderProgram);
		glBindVertexArray(VAO);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		cameraPos=processInput(window,cameraPos,cameraFront,cameraUp,deltaTime);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		trans = projection * view * model;
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
		model = glm::mat4(1.0f);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}
void FramebufferSizeCallback1(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
glm::vec3 processInput(GLFWwindow* window,glm::vec3 cameraPos,glm::vec3 cameraFront,glm::vec3 cameraUp,float deltaTime)
{
		float cameraSpeed = 2.5f*deltaTime; 
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	return cameraPos;
}
void mouse_callback(GLFWwindow*window,double xpos,double ypos)
{
		//判断鼠标是否第一次进入窗口，这是为了防止刚进入窗口时摄像机突然跳一下
		if (firstIn == true)
		{
			lastX = xpos;
			lastY = ypos;
			cout << lastX << ";" << lastY;
			firstIn = false;
		}
		//计算前后两帧鼠标位置偏移量
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw += xoffset;
		pitch += yoffset;
		//添加限制，俯仰角超过九十度时视角会翻转
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		//通过pitch和yaw计算得到真正的方向向量
		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);
	
}
void scroll_callback(GLFWwindow* window,double xoffset,double yoffset)
{	//控制透视视野
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
	projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
}
