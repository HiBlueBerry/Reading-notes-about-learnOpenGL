#include<glad/glad.h>
#include<glfw3.h>
#include<iostream>
float vertices[9] = {-0.5f,-0.5f,0.0f,0.5f,-0.5f,0.0f,0.0f,0.5f,0.0f};
const char*VertexShaderSource = 
	"#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char* FragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";
int main()
{	//创建VAO(vertex array object)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	//绑定VAO
	glBindVertexArray(VAO);	
	//创建VBO并根据VBO的ID赋予一块VBO对象的内存
	unsigned int VBO;//VetexBufferObject,顶点缓存对象
	glGenBuffers(1,&VBO);
	//顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER,使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	//将定义的顶点数据复制到缓存中
	//glBufferData参数：目标缓冲的类型，传输数据的大小，传输的实际数据，管理方式
	//glBufferData管理方式有三种：
	//GL_STATIC_DRAW ：数据不会或几乎不会改变。
	//GL_DYNAMIC_DRAW：数据会被改变很多。
	//GL_STREAM_DRAW ：数据每次绘制时都会改变。
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	//创建Vertexshader对象(顶点着色器)，使用ID进行引用
	unsigned int VertexShader;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);//创建的是顶点着色器
	//把着色器源码附加到着色器对象上并进行编译
	//glShaderSource:要编译的着色器对象，传递源码字符串的数量，源码，第四个参数我不知道干什么用
	glShaderSource(VertexShader,1,&VertexShaderSource,NULL);
	glCompileShader(VertexShader);
	//检测glCompileShader是否编译成功
	int success;
	char infoLog[512];
	//检测着色器编译是否成功
	glGetShaderiv(VertexShader,GL_COMPILE_STATUS,&success);
	if (!success)
	{	//获取编译信息
		glGetShaderInfoLog(VertexShader,512,NULL,infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

	}
	//创建片段着色器(FragmentShader),使用ID进行引用,进行与创建VertexShader同样的操作,可以自己不看教程码一遍
	unsigned int FragmentShader;
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader,1,&FragmentShaderSource,NULL);
	glCompileShader(FragmentShader);
	int success1;
	char infoLog1[512];
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success1);
	if (!success)
	{	
		glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog1);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog1 << std::endl;

	}
	//创建着色器程序对象(ShaderProgramObject)
	unsigned int ShaderProgram;
	//glCreateProgram创建一个程序，并且返回新创建对象的ID引用
	ShaderProgram = glCreateProgram();
	//把之前编译的两个着色器附加到程序对象上
	glAttachShader(ShaderProgram,VertexShader);
	glAttachShader(ShaderProgram,FragmentShader);
	//链接两个着色器
	glLinkProgram(ShaderProgram);
	//检测链接着色器程序编译是否成功
	int success2;
	char infoLog2[512];
	//注意这里是glGetProgramiv而不是glGetShaderiv
	glGetProgramiv(ShaderProgram,GL_LINK_STATUS,&success2);
	if (!success)
	{	//注意这里是glGetProgramInfoLog而不是glGetShaderInfoLog
		glGetProgramInfoLog(ShaderProgram,512,NULL,infoLog2);
		std::cout<< "ERROR::LINK::COMPILATION_FAILED\n" << infoLog2 << std::endl;
	}
	//激活程序对象
	glUseProgram(ShaderProgram);
	//删除着色器对象（用不到了）
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
	//解析顶点数据
	//glVertexAttribPointer【直译一下就是OpenGL的顶点属性指引器】参数详解
	//第一个参数指定顶点属性的位置值(Location),之前顶点着色器使用layout(location=0)【个人理解应该是图层位置】定义了position顶点属性的位置值(location)，希望把数据传到这一个顶点属性中，故传入0
	//第二个参数指定顶点属性的大小【个人理解应该说的是顶点向量的维度】。顶点属性是一个vec3,它由3个值组成，所以大小是3
	//第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中的vec都是由浮点数值组成)
	//第四个参数定义是否希望数据被标准化,这里是GL_FALSE，如果设置为True,数据会被映射到0(对于signed类型是-1)到1之间。
	//第五个参数是步长(Stride),指示连续的顶点属性之间的间隔,由于下个顶点xyz数据在3个float之后,因此步长设置为3*sizeof(float),注意我们知道这个位置数据是紧密排列的(两个顶点属性之间没有空隙),故可以设置为0来让OpenGL自行决定步长为多少(数值必须要紧密排列才可以这样)
	//第六个参数类型是void*,故需要进行类型转化，表示位置数据在缓冲中起始位置的偏移量(Offset),由于位置数据在数组的开头，所以这里是0
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	//以顶点属性位置值为参数，启用顶点属性(顶点属性默认是禁用的)
	glEnableVertexAttribArray(0);
	//【PS:目前为止完成了将顶点数组放到缓冲中;设置顶点属性指针;设置并链接着色器。原书中设置顶点属性指针放到了设置着色器后面讲解，此处代码结构与原书保持一致】
	glBindVertexArray(VAO);
	//glDrawArrays使用当前激活的着色器，之前配置的顶点属性和VBO的顶点数据来绘制图元(Primitive)
	//glDrawArrays:打算绘制的图元的类型,顶点数组的起始索引,绘制顶点的个数
	glDrawArrays(GL_TRIANGLES,0,3);
	return 0;
}
