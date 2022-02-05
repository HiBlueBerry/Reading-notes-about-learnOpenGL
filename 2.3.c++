#include<glad/glad.h>
#include<glfw3.h>
#include <iostream>
using namespace std;
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	//我好不容易心动一次，你却让我输的这么彻底，焯！！！
	//也许OpenGl版本号是3.3吧.....
	GLFWwindow* window = glfwCreateWindow(800,600,"LearnOpenGL",NULL,NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//初始化glad，如果成功则不输出错误
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	//glViewport:前两个控制窗口左下角位置，后两个控制宽度和高度（像素）
	glViewport(0,0,800,600);
	//openGL使用glViewport设置的宽高进行2D转换，将GL中的坐标转化为屏幕坐标
	//处理过的GL坐标范围为-1到1，在上述例子中-1到1之间的值将会被映射到（0，800）和（0，600）中
	//每当窗口调整大小的时候调用以下函数
	glfwSetFramebufferSizeCallback(window,FramebufferSizeCallback);
	//添加渲染循环(Render Loop),防止绘制后立即退出
	//glfwWindowShouldClose检查GLFW是否被要求退出
	while (!glfwWindowShouldClose(window))
	{/*输入*/

		processInput(window);
	 
	 /*渲染指令*/
		//设置清空屏幕后应显示的颜色
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		//清空屏幕的颜色缓冲，接受一个缓冲位（Buffer Bit)，有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT三种
		glClear(GL_COLOR_BUFFER_BIT);
	 
	 /*检查并且调用事件，交换缓冲*/

		//检查是否触发什么事件（键鼠互动），更新窗口状态，并调用对应回调函数
		glfwSwapBuffers(window);
		//交换颜色缓冲（存储GLFW窗口每一个像素颜色值），并将输出显示在屏幕中（更新屏幕）
		glfwPollEvents();
		//使用缓冲交换的目的是避免图像割裂感和图像闪烁的问题，因为生成的图像并不是同时绘制出的，是从上到下，从左到右的顺序逐步生成的
	}
	//渲染结束后释放内存
	glfwTerminate();
	return 0;
}
//回调函数，调整窗口大小
void FramebufferSizeCallback(GLFWwindow*window,int width,int height)
{
	glViewport(0,0,width,height);
}
//辅助函数，获取输入，输入ESC则关闭窗口
void processInput(GLFWwindow *window)
{	//glfwGetKey函数需要一个窗口和一个按键作为输入，并返回这个按键是否正在被按下，如果按下则返回GLFW_PRESS,否则返回GLFW_RELEASE
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{	//设置窗口是否应该关闭
		glfwSetWindowShouldClose(window, true);
	}
}

//TODO:OpenGL幕后使用glViewport中定义的位置和宽高进行2D坐标的转换，将OpenGL中的位置坐标转换为你的屏幕坐标。例如，OpenGL中的坐标(-0.5, 0.5)有可能（最终）被映射为屏幕中的坐标(200,450)。注意，处理过的OpenGL坐标范围只为-1到1，因此我们事实上将(-1到1)范围内的坐标映射到(0, 800)和(0, 600)。
//TODO:双缓冲：应用程序使用单缓冲绘图时可能会存在图像闪烁的问题。 这是因为生成的图像不是一下子被绘制出来的，而是按照从左到右，由上而下逐像素地绘制而成的。最终图像不是在瞬间显示给用户，而是通过一步一步生成的，这会导致渲染的结果很不真实。为了规避这些问题，我们应用双缓冲渲染窗口应用程序。前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲，这样图像就立即呈显出来，之前提到的不真实感就消除了
//TODO:glClearColor函数是一个状态设置函数，而glClear函数则是一个状态使用的函数，它使用了当前的状态来获取应该清除为的颜色。
