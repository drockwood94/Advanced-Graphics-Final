#include <sb6.h>
#include <sb6ktx.h>
#include <vmath.h>
#include <object.h>
#include <assert.h> 
#include <iostream>
#include "TGALoader.h"
#include "OBJLoader.h"
#include <shader.h>
#include <Windows.h>
#include <ctime>

constexpr auto STACKS = 18;
constexpr auto SLICES = 36;
constexpr auto PI = 3.14159;
class advanced_graphics_final : public sb6::application
{
	void init()
	{
		static const char title[] = "Final Project - Ambient Occlusion";

		sb6::application::init();

		memcpy(info.title, title, sizeof(title));

	}

	struct vertex
	{
		// Position
		float x;
		float y;
		float z;
		float w;

		// Normal
		float xi;
		float yi;
		float zi;
		float wi;

		// texture coordinates
		float s;
		float t;

		// Color
		float r;
		float g;
		float b;
		float a;
	};

	void createVBOfromMesh(NS_OBJLOADER::MESH* Mesh, GLuint *vao, long *numTriangles)
	{
		(*numTriangles) = 0;
		for (int i = 0; i < Mesh->m_Faces.size(); i++)
		{
			if (Mesh->m_Faces.at(i).m_Vertices.size() == 3)
			{
				(*numTriangles)++;
			}
			else if (Mesh->m_Faces.at(i).m_Vertices.size() == 4)
			{
				(*numTriangles)++;
				(*numTriangles)++;
			}

		}


		vertex *data = new vertex[(*numTriangles) * 3]();

		glGenVertexArrays(1, vao);
		glBindVertexArray(*vao);

		(*numTriangles) = 0;
		for (int i = 0; i < Mesh->m_Faces.size(); i++)
		{
			if (Mesh->m_Faces.at(i).m_Vertices.size() == 3)
			{
				(*numTriangles)++;

				for (int j = 0; j < 3; j++)
				{
					data[((*numTriangles) - 1) * 3 + j].x = Mesh->m_Positions.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiPositionID).x;
					data[((*numTriangles) - 1) * 3 + j].y = Mesh->m_Positions.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiPositionID).y;
					data[((*numTriangles) - 1) * 3 + j].z = Mesh->m_Positions.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiPositionID).z;
					data[((*numTriangles) - 1) * 3 + j].w = 1;
					data[((*numTriangles) - 1) * 3 + j].r = 1;
					data[((*numTriangles) - 1) * 3 + j].g = Mesh->m_Normals.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiNormalID).y;
					data[((*numTriangles) - 1) * 3 + j].b = 1;
					data[((*numTriangles) - 1) * 3 + j].a = 1;
					data[((*numTriangles) - 1) * 3 + j].s = Mesh->m_TexCoords.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiTexCoordID).m_fTexCoordU;
					data[((*numTriangles) - 1) * 3 + j].t = Mesh->m_TexCoords.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiTexCoordID).m_fTexCoordV;
					data[((*numTriangles) - 1) * 3 + j].xi = Mesh->m_Normals.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiNormalID).x;
					data[((*numTriangles) - 1) * 3 + j].yi = Mesh->m_Normals.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiNormalID).y;
					data[((*numTriangles) - 1) * 3 + j].zi = Mesh->m_Normals.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiNormalID).z;
				}
			}
			else if (Mesh->m_Faces.at(i).m_Vertices.size() == 4)
			{
				(*numTriangles)++;
				for (int j = 0; j < 3; j++) //first triangle = 0,1,2
				{
					data[((*numTriangles) - 1) * 3 + j].x = Mesh->m_Positions.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiPositionID).x;
					data[((*numTriangles) - 1) * 3 + j].y = Mesh->m_Positions.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiPositionID).y;
					data[((*numTriangles) - 1) * 3 + j].z = Mesh->m_Positions.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiPositionID).z;
					data[((*numTriangles) - 1) * 3 + j].w = 1;
					data[((*numTriangles) - 1) * 3 + j].r = 0;
					data[((*numTriangles) - 1) * 3 + j].g = Mesh->m_Normals.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiNormalID).y;
					data[((*numTriangles) - 1) * 3 + j].b = 0;
					data[((*numTriangles) - 1) * 3 + j].a = 0;
					data[((*numTriangles) - 1) * 3 + j].s = Mesh->m_TexCoords.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiTexCoordID).m_fTexCoordU;
					data[((*numTriangles) - 1) * 3 + j].t = Mesh->m_TexCoords.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiTexCoordID).m_fTexCoordV;
					data[((*numTriangles) - 1) * 3 + j].xi = Mesh->m_Normals.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiNormalID).x;
					data[((*numTriangles) - 1) * 3 + j].yi = Mesh->m_Normals.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiNormalID).y;
					data[((*numTriangles) - 1) * 3 + j].zi = Mesh->m_Normals.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiNormalID).z;
				}
				(*numTriangles)++;
				int k = 0;//0,1,2
				for (int j = 0; j < 4; j++) //second triangle = 0,2,3
				{
					if (j == 1)
						j = 2;
					data[((*numTriangles) - 1) * 3 + k].x = Mesh->m_Positions.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiPositionID).x;
					data[((*numTriangles) - 1) * 3 + k].y = Mesh->m_Positions.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiPositionID).y;
					data[((*numTriangles) - 1) * 3 + k].z = Mesh->m_Positions.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiPositionID).z;
					data[((*numTriangles) - 1) * 3 + k].w = 1;
					data[((*numTriangles) - 1) * 3 + k].r = 0;
					data[((*numTriangles) - 1) * 3 + k].g = Mesh->m_Normals.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiNormalID).y;
					data[((*numTriangles) - 1) * 3 + k].b = 0;
					data[((*numTriangles) - 1) * 3 + k].a = 1;
					data[((*numTriangles) - 1) * 3 + k].s = Mesh->m_TexCoords.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiTexCoordID).m_fTexCoordU;
					data[((*numTriangles) - 1) * 3 + k].t = Mesh->m_TexCoords.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiTexCoordID).m_fTexCoordV;
					data[((*numTriangles) - 1) * 3 + k].xi = Mesh->m_Normals.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiNormalID).x;
					data[((*numTriangles) - 1) * 3 + k].yi = Mesh->m_Normals.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiNormalID).y;
					data[((*numTriangles) - 1) * 3 + k].zi = Mesh->m_Normals.at(Mesh->m_Faces.at(i).m_Vertices.at(j).m_uiNormalID).z;
					k++;
				}

			}

		}
		GLuint vbo;
		// Allocate and initialize a buffer object
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, *numTriangles * sizeof(vertex) * 3, data, GL_STATIC_DRAW);

		// Set up two vertex attributes - first positions
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
			sizeof(vertex), (void *)offsetof(vertex, x));
		glEnableVertexAttribArray(0);

		// Now colors
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
			sizeof(vertex), (void *)offsetof(vertex, r));
		glEnableVertexAttribArray(1);

		// Now textures
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			sizeof(vertex), (void *)offsetof(vertex, s));
		glEnableVertexAttribArray(2);

		// Now textures
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE,
			sizeof(vertex), (void *)offsetof(vertex, xi));
		glEnableVertexAttribArray(3);

		delete[] data;
		glBindVertexArray(0);

	}
	
	// https://stackoverflow.com/questions/686353/c-random-float-number-generation
	// This method of generating a random float in C++ seems to be the standard
	static inline float rand_float() 
	{
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}

	virtual void startup()
	{
		viewMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
		normalViewMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
		modelMatrix = vmath::translate(0.0f, 0.0f, 3.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		GLuint  vs_ao, fs_ao, vs_phong, fs_phong;

		vs_ao = sb6::shader::load("ao.vert", GL_VERTEX_SHADER);
		fs_ao = sb6::shader::load("ao.frag", GL_FRAGMENT_SHADER);
		vs_phong = sb6::shader::load("phong.vert", GL_VERTEX_SHADER);
		fs_phong = sb6::shader::load("phong.frag", GL_FRAGMENT_SHADER);

		char bufferAo[1024], bufferPhong[1024];
		glGetShaderInfoLog(vs_ao, 1024, NULL, bufferAo);
		glGetShaderInfoLog(vs_phong, 1024, NULL, bufferPhong);
		OutputDebugStringA(bufferAo);
		OutputDebugStringA(bufferPhong);

		render_prog_ao = glCreateProgram();
		glAttachShader(render_prog_ao, vs_ao);
		glAttachShader(render_prog_ao, fs_ao);
		glLinkProgram(render_prog_ao);

		render_prog_phong = glCreateProgram();
		glAttachShader(render_prog_phong, vs_phong);
		glAttachShader(render_prog_phong, fs_phong);
		glLinkProgram(render_prog_phong);

		glDeleteShader(vs_ao);
		glDeleteShader(fs_ao);
		glDeleteShader(vs_phong);
		glDeleteShader(fs_phong);

		int loglen;
		glGetProgramInfoLog(render_prog_ao, 1024, &loglen, bufferAo);
		OutputDebugStringA(bufferAo);

		glGetProgramInfoLog(render_prog_phong, 1024, &loglen, bufferPhong);
		OutputDebugStringA(bufferPhong);


		GLint status;
		glGetProgramiv(render_prog_ao, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
			exit(1);
		glGetProgramiv(render_prog_phong, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
			exit(1);

		uniformsAo.ssao_radius = glGetUniformLocation(render_prog_ao, "ssao_radius");
		uniformsAo.point_count = glGetUniformLocation(render_prog_ao, "point_count");
		uniformsAo.NumSteps = glGetUniformLocation(render_prog_ao, "NumSteps");

		uniformsPhong.model = glGetUniformLocation(render_prog_phong, "model");
		uniformsPhong.view = glGetUniformLocation(render_prog_phong, "view");
		uniformsPhong.projection = glGetUniformLocation(render_prog_phong, "projection");
		uniformsPhong.lightSource = glGetUniformLocation(render_prog_phong, "lightSource");
		uniformsPhong.normalMatrix = glGetUniformLocation(render_prog_phong, "normalMatrix");


		glGenVertexArrays(1, &vaoSphere);
		glBindVertexArray(vaoSphere);

		vmath::vec4 normalVec;
		vertex *circle = new vertex[(STACKS + 1) * (SLICES + 1)]();
		int numVertices = 0;
		for (int lat = -90; lat <= 90; lat += 180 / STACKS)
		{
			for (int lon = 0; lon <= 360; lon += 360 / SLICES)
			{


				float radius = cos((float)lat * PI / 180.0f);
				circle[numVertices].y = sin((float)lat * PI / 180.0f);
				circle[numVertices].x = cos((float)lon * PI / 180.0f)*radius;
				circle[numVertices].z = sin((float)lon * PI / 180.0f)*radius;
				circle[numVertices].w = 1;
				normalVec = vmath::vec4(circle[numVertices].x, circle[numVertices].y, circle[numVertices].z, 0.0f);
				//vmath::normalize(normalVec);

				circle[numVertices].xi = normalVec[0];
				circle[numVertices].yi = normalVec[1];
				circle[numVertices].zi = normalVec[2];
				circle[numVertices].wi = normalVec[3];
				circle[numVertices].s = (float)lon / 360.0f;
				circle[numVertices].t = (float)(lat + 90) / 180.0f;
				circle[numVertices].r = circle[numVertices].z;
				circle[numVertices].g = circle[numVertices].x;
				circle[numVertices].b = circle[numVertices].y;
				circle[numVertices].a = 1;
				numVertices++;
			}
		}

		GLuint vbo;
		// Allocate and initialize a buffer object
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, ((STACKS + 1) * (SLICES + 1)) * sizeof(vertex), circle, GL_STATIC_DRAW);

		// Set up two vertex attributes - first positions
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
			sizeof(vertex), (void *)offsetof(vertex, x));
		glEnableVertexAttribArray(0);

		// Now colors
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
			sizeof(vertex), (void *)offsetof(vertex, r));
		glEnableVertexAttribArray(1);

		// Now textures
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			sizeof(vertex), (void *)offsetof(vertex, s));
		glEnableVertexAttribArray(2);

		// Now textures
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE,
			sizeof(vertex), (void *)offsetof(vertex, xi));
		glEnableVertexAttribArray(3);

		GLuint *sphereIndices = new GLuint[STACKS * SLICES * 6]();
		GLuint triIndex = 0;
		for (GLuint stack = 0; stack < STACKS; ++stack)
		{
			for (GLuint slice = 0; slice < SLICES; ++slice)
			{
				// Bottom tri of the quad
				sphereIndices[triIndex++] = (stack * (SLICES + 1)) + slice; //1
				sphereIndices[triIndex++] = (stack * (SLICES + 1)) + ((slice + 1) % (SLICES + 1));//2
				sphereIndices[triIndex++] = ((stack + 1) * (SLICES + 1)) + ((slice + 1) % (SLICES + 1));//3
																										// Top tri of the quad
				sphereIndices[triIndex++] = (stack * (SLICES + 1)) + slice; //1
				sphereIndices[triIndex++] = ((stack + 1) * (SLICES + 1)) + ((slice + 1) % (SLICES + 1));//3
				sphereIndices[triIndex++] = ((stack + 1) * (SLICES + 1)) + (slice % (SLICES + 1));//4
			}
		}


		// Create and bind a BO for index data
		glGenBuffers(1, &iSphereBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iSphereBuffer);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, STACKS * SLICES * 6 * sizeof(GLuint), sphereIndices, GL_STATIC_DRAW);

		delete[] sphereIndices;
		delete[] circle;

		moveLeft = false;
		moveRight = false;
		moveUp = false;
		moveDown = false;
		objectView = false;
		useAOprog = false;
		lookUp = lookDown = lookLeft = lookRight = false;
		ssao_radius = 0.05;
		point_count = 90;
		NumSteps = 4.0f;

		glGenTextures(2, textureColor);
		glGenTextures(2, textureNormal);

		//////////////////////////////////////
		//				Tree				//
		//////////////////////////////////////
		// Load Actual Texture
		glActiveTexture(GL_TEXTURE0);

		// Load texture from file
		glBindTexture(GL_TEXTURE_2D, textureColor[1]);
		NS_TGALOADER::LoadTGATexture("skin_albedo.tga", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);

		// Load Bump Map
		glActiveTexture(GL_TEXTURE1);

		// Load texture from file
		glBindTexture(GL_TEXTURE_2D, textureNormal[1]);
		NS_TGALOADER::LoadTGATexture("skin_normal.tga", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);

		NS_OBJLOADER::MESH treeMesh;
		NS_OBJLOADER::map<std::string, NS_OBJLOADER::MATERIAL> treeMaterials;
		const char *treeFile = "Lowpoly_tree_sample.obj"; // "Pacmannblend.obj"
		LoadOBJFile(treeFile, treeMesh, treeMaterials);
		createVBOfromMesh(&treeMesh, &treeVAO, &Tree_Triangles);

		//////////////////////////////////////
		//				SPHERE				//
		//////////////////////////////////////

		// Load Actual Texture
		glActiveTexture(GL_TEXTURE0);

		// Load texture from file
		glBindTexture(GL_TEXTURE_2D, textureColor[0]);
		NS_TGALOADER::LoadTGATexture("albedo.tga", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);

		// Load Bump Map
		glActiveTexture(GL_TEXTURE1);

		// Load texture from file
		glBindTexture(GL_TEXTURE_2D, textureNormal[0]);
		NS_TGALOADER::LoadTGATexture("normal.tga", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);

		

		globalPerspective = vmath::perspective(45.0f, (float)w / (float)h, 0.1f, 50.0f);

		// setup frame buffers
		// render scene once and retrieve all geometrical information from objects and store in collection of textures called the �G-buffer�
		// This is the "geometry pass"
		// Sources: 
		//		https://github.com/openglsuperbible/sb6code/blob/master/src/ssao/ssao.cpp
		//		https://learnopengl.com/Advanced-Lighting/SSAO

		glGenFramebuffers(1, &frameBufferG);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferG);

		glGenTextures(1, &gPosition);
		glGenTextures(1, &gNormal);
		glGenTextures(1, &gAlbedo);

		glBindTexture(GL_TEXTURE_2D, gPosition);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, 2048, 2048);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 2048, 2048);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, gAlbedo);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, 2048, 2048);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, gPosition, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, gNormal, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, gAlbedo, 0);

		// Declare draw_buffers to allow the second shader program to reference data written by shader progam 1
		// Source:		https://github.com/openglsuperbible/sb6code/blob/master/src/ssao/ssao.cpp
		// Explanation:	https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawBuffers.xhtml
		draw_buffers[0] = GL_COLOR_ATTACHMENT0;
		draw_buffers[1] = GL_COLOR_ATTACHMENT1;
		glDrawBuffers(2, draw_buffers);

		glGenVertexArrays(1, &frameBufferAo);

		////////////////////////////////////////////////////////////////////////////// 
		//			Generate random points for SSAO Sampling						//
		//////////////////////////////////////////////////////////////////////////////
		// Source:	https://github.com/openglsuperbible/sb6code/blob/master/src/ssao/ssao.cpp

		int i;
		kernel_points point_data;

		for (i = 0; i < 256; i++)
		{
			do
			{
				point_data.point[i][0] = rand_float() * 2.0f - 1.0f;
				point_data.point[i][1] = rand_float() * 2.0f - 1.0f;
				point_data.point[i][2] = rand_float();
				point_data.point[i][3] = 0.0f;
			} while (length(point_data.point[i]) > 1.0f);
			normalize(point_data.point[i]);
		}
		for (i = 0; i < 256; i++)
		{
			point_data.random_vectors[i][0] = rand_float();
			point_data.random_vectors[i][1] = rand_float();
			point_data.random_vectors[i][2] = rand_float();
			point_data.random_vectors[i][3] = rand_float();
		}

		glGenBuffers(1, &points_buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, points_buffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(kernel_points), &point_data, GL_STATIC_DRAW);
	}

	void checkInput()
	{
		vmath::mat4 temp;
		float x, y, z, xRot, yRot, zRot;
		x = y = z = xRot = yRot = zRot = 0;

		if (moveLeft)
			x -= .1;
		if (moveRight)
			x += .1;
		if (moveUp)
			z -= .1;
		if (moveDown)
			z += .1;
		modelMatrix = modelMatrix * vmath::translate(x, y, z);

		if (lookDown)
			modelMatrix = modelMatrix * vmath::rotate(1.0f, 1.0f, 0.0f, 0.0f);
		if (lookUp)
			modelMatrix = modelMatrix * vmath::rotate(-1.0f, 1.0f, 0.0f, 0.0f);
		if (lookLeft)
			modelMatrix = modelMatrix * vmath::rotate(1.0f, 0.0f, 1.0f, 0.0f);
		if (lookRight)
			modelMatrix = modelMatrix * vmath::rotate(-1.0f, 0.0f, 1.0f, 0.0f);

		gluInvertMatrix(modelMatrix, temp);
		viewMatrix = temp;
	}

	virtual void render(double currentTime)
	{
		if (resize)
		{
			resize = false;
			glViewport(0, 0, w, h);
			globalPerspective = vmath::perspective(45.0f, (float)w / (float)h, 0.1f, 50.0f);
		}

		useAOprog ? glBindFramebuffer(GL_FRAMEBUFFER, frameBufferG) : glBindFramebuffer(GL_FRAMEBUFFER, 0);

		checkInput();
		clearBuffers();

		glUseProgram(render_prog_phong);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColor[0]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureNormal[0]);

		// Draw sphere 1
		for (int i = -10; i < 10; i++) {
			for (int j = -10; j <= 0; j++) {
				glUniform4f(uniformsPhong.lightSource, 10.0f, 3.0f, 10.0f, 1.0f);

				glUniformMatrix4fv(uniformsPhong.projection, 1, GL_FALSE, globalPerspective);
				glUniformMatrix4fv(uniformsPhong.view, 1, GL_FALSE, viewMatrix);
				glUniformMatrix4fv(uniformsPhong.model, 1, GL_FALSE, (vmath::scale(0.4f) * vmath::translate(0.4f, -1.0f, 0.0f) * vmath::rotate(180.0f, 1.0f, 0.0f, 0.0f) * vmath::rotate(15.0f, 0.0f, 0.0f, 1.0f)));
				glUniformMatrix4fv(uniformsPhong.normalMatrix, 1, GL_FALSE, vmath::rotate(180.0f, 1.0f, 0.0f, 0.0f) * vmath::rotate(15.0f, 0.0f, 0.0f, 1.0f));

				glBindVertexArray(vaoSphere);
				glDrawElements(GL_TRIANGLES, SLICES * STACKS * 6, GL_UNSIGNED_INT, (void*)0);
			}
		}

		// Draw sphere 2
		for (int i = -10; i < 10; i++) {
			for (int j = -10; j <= 0; j++) {
				glUniform4f(uniformsPhong.lightSource, 10.0f, 3.0f, 10.0f, 1.0f);

				glUniformMatrix4fv(uniformsPhong.projection, 1, GL_FALSE, globalPerspective);
				glUniformMatrix4fv(uniformsPhong.view, 1, GL_FALSE, viewMatrix);
				glUniformMatrix4fv(uniformsPhong.model, 1, GL_FALSE, (vmath::scale(0.3f) * vmath::translate(-2.0f, 0.0f, 0.0f) * vmath::rotate(180.0f, 1.0f, 0.0f, 0.0f) * vmath::rotate(15.0f, 0.0f, 0.0f, 1.0f)));
				glUniformMatrix4fv(uniformsPhong.normalMatrix, 1, GL_FALSE, vmath::rotate(180.0f, 1.0f, 0.0f, 0.0f) * vmath::rotate(15.0f, 0.0f, 0.0f, 1.0f));

				glBindVertexArray(vaoSphere);
				glDrawElements(GL_TRIANGLES, SLICES * STACKS * 6, GL_UNSIGNED_INT, (void*)0);
			}
		}

		// Draw tree
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColor[1]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureNormal[1]);

		glBindVertexArray(treeVAO);
		glUniform4f(uniformsPhong.lightSource, 10.0f, 3.0f, 10.0f, 1.0f);
		glUniformMatrix4fv(uniformsPhong.projection, 1, GL_FALSE, vmath::perspective(45.0f, (float)w / (float)h, 0.1f, 200.0f));
		glUniformMatrix4fv(uniformsPhong.view, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(uniformsPhong.model, 1, GL_FALSE, vmath::translate(0.0f, -2.0f, 0.0f) * vmath::scale(0.3f) * vmath::rotate(0.0f, 0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformsPhong.normalMatrix, 1, GL_FALSE, vmath::rotate(0.0f, 0.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, Tree_Triangles * 3);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		// Actually perform the AO pass
		if (useAOprog) {
			// Bind the random point data so that the ambient occlusion stuff can take random samples from the image
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, points_buffer);
			glUseProgram(render_prog_ao);
			glUniform1f(uniformsAo.ssao_radius, ssao_radius);
			glUniform1ui(uniformsAo.point_count, point_count);
			glUniform1f(uniformsAo.NumSteps, NumSteps);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gPosition);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gNormal);

			glDisable(GL_DEPTH_TEST);
			glBindVertexArray(frameBufferAo);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glEnable(GL_DEPTH_TEST);
		}
	}

	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vaoSphere);
		glDeleteProgram(render_prog_phong);
		glDeleteProgram(render_prog_ao);
	}

	virtual void onResize(int w, int h)
	{
		info.windowWidth = w;
		info.windowHeight = h;
		this->w = w;
		this->h = h;
		resize = true;
	}

	virtual void onMouseMove(int mouseX, int mouseY)
	{
	}

	virtual void onKey(int key, int action)
	{
		//http://www.glfw.org/docs/3.0/group__keys.html
		if (action == 1)
			switch (key)
			{
			case 283:
				lookDown = true;
				break;
			case 82:
				ssao_radius += 0.01;
				break;
			case 70:
				ssao_radius -= 0.01;
				break;
			case 84:
				point_count += 1;
				break;
			case 71:
				point_count -= 1;
				break;
			case 89:
				NumSteps += 1.0;
				break;
			case 72:
				NumSteps -= 1.0;
				break;
			case 284:
				lookUp = true;
				break;
			case 285:
				lookLeft = true;
				break;
			case 286:
				lookRight = true;
				break;
			case 87:
				moveUp = true;
				break;
			case 83:
				moveDown = true;
				break;
			case 65:
				moveLeft = true;
				break;
			case 68:
				moveRight = true;
				break;
			case 81:
				useAOprog = !useAOprog;
				break;
			}
		else if (action == 0)
			switch (key)
			{
			case 283:
				lookDown = false;
				break;
			case 284:
				lookUp = false;
				break;
			case 285:
				lookLeft = false;
				break;
			case 286:
				lookRight = false;
				break;
			case 87:
				moveUp = false;
				break;
			case 83:
				moveDown = false;
				break;
			case 65:
				moveLeft = false;
				break;
			case 68:
				moveRight = false;
				break;
			}

	}

	void clearBuffers() {
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		static const GLfloat one = 1.0f;
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_COLOR, 1, black);
		glClearBufferfv(GL_DEPTH, 0, &one);
	}

	bool gluInvertMatrix(const float m[16], float invOut[16])
	{
		float inv[16], det;
		int i;

		inv[0] = m[5] * m[10] * m[15] -
			m[5] * m[11] * m[14] -
			m[9] * m[6] * m[15] +
			m[9] * m[7] * m[14] +
			m[13] * m[6] * m[11] -
			m[13] * m[7] * m[10];

		inv[4] = -m[4] * m[10] * m[15] +
			m[4] * m[11] * m[14] +
			m[8] * m[6] * m[15] -
			m[8] * m[7] * m[14] -
			m[12] * m[6] * m[11] +
			m[12] * m[7] * m[10];

		inv[8] = m[4] * m[9] * m[15] -
			m[4] * m[11] * m[13] -
			m[8] * m[5] * m[15] +
			m[8] * m[7] * m[13] +
			m[12] * m[5] * m[11] -
			m[12] * m[7] * m[9];

		inv[12] = -m[4] * m[9] * m[14] +
			m[4] * m[10] * m[13] +
			m[8] * m[5] * m[14] -
			m[8] * m[6] * m[13] -
			m[12] * m[5] * m[10] +
			m[12] * m[6] * m[9];

		inv[1] = -m[1] * m[10] * m[15] +
			m[1] * m[11] * m[14] +
			m[9] * m[2] * m[15] -
			m[9] * m[3] * m[14] -
			m[13] * m[2] * m[11] +
			m[13] * m[3] * m[10];

		inv[5] = m[0] * m[10] * m[15] -
			m[0] * m[11] * m[14] -
			m[8] * m[2] * m[15] +
			m[8] * m[3] * m[14] +
			m[12] * m[2] * m[11] -
			m[12] * m[3] * m[10];

		inv[9] = -m[0] * m[9] * m[15] +
			m[0] * m[11] * m[13] +
			m[8] * m[1] * m[15] -
			m[8] * m[3] * m[13] -
			m[12] * m[1] * m[11] +
			m[12] * m[3] * m[9];

		inv[13] = m[0] * m[9] * m[14] -
			m[0] * m[10] * m[13] -
			m[8] * m[1] * m[14] +
			m[8] * m[2] * m[13] +
			m[12] * m[1] * m[10] -
			m[12] * m[2] * m[9];

		inv[2] = m[1] * m[6] * m[15] -
			m[1] * m[7] * m[14] -
			m[5] * m[2] * m[15] +
			m[5] * m[3] * m[14] +
			m[13] * m[2] * m[7] -
			m[13] * m[3] * m[6];

		inv[6] = -m[0] * m[6] * m[15] +
			m[0] * m[7] * m[14] +
			m[4] * m[2] * m[15] -
			m[4] * m[3] * m[14] -
			m[12] * m[2] * m[7] +
			m[12] * m[3] * m[6];

		inv[10] = m[0] * m[5] * m[15] -
			m[0] * m[7] * m[13] -
			m[4] * m[1] * m[15] +
			m[4] * m[3] * m[13] +
			m[12] * m[1] * m[7] -
			m[12] * m[3] * m[5];

		inv[14] = -m[0] * m[5] * m[14] +
			m[0] * m[6] * m[13] +
			m[4] * m[1] * m[14] -
			m[4] * m[2] * m[13] -
			m[12] * m[1] * m[6] +
			m[12] * m[2] * m[5];

		inv[3] = -m[1] * m[6] * m[11] +
			m[1] * m[7] * m[10] +
			m[5] * m[2] * m[11] -
			m[5] * m[3] * m[10] -
			m[9] * m[2] * m[7] +
			m[9] * m[3] * m[6];

		inv[7] = m[0] * m[6] * m[11] -
			m[0] * m[7] * m[10] -
			m[4] * m[2] * m[11] +
			m[4] * m[3] * m[10] +
			m[8] * m[2] * m[7] -
			m[8] * m[3] * m[6];

		inv[11] = -m[0] * m[5] * m[11] +
			m[0] * m[7] * m[9] +
			m[4] * m[1] * m[11] -
			m[4] * m[3] * m[9] -
			m[8] * m[1] * m[7] +
			m[8] * m[3] * m[5];

		inv[15] = m[0] * m[5] * m[10] -
			m[0] * m[6] * m[9] -
			m[4] * m[1] * m[10] +
			m[4] * m[2] * m[9] +
			m[8] * m[1] * m[6] -
			m[8] * m[2] * m[5];

		det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		if (det == 0)
			return false;

		det = 1.0 / det;

		for (i = 0; i < 16; i++)
			invOut[i] = inv[i] * det;

		return true;
	}

private:
	struct
	{
		GLint       model;
		GLint       view;
		GLint       projection;
		GLint       normalMatrix;
		GLint		lightSource;
	} uniformsPhong;
	
	// Set up variables required for AO
	struct {
		GLint       ssao_radius;
		GLint       point_count;
		GLint		NumSteps;
	} uniformsAo;
	GLfloat ssao_radius;
	GLfloat NumSteps;
	GLuint point_count;

	struct kernel_points
	{
		vmath::vec4 point[256];
		vmath::vec4	random_vectors[256];
	};
	GLuint points_buffer, gPosition, gNormal, gAlbedo;

	GLenum draw_buffers[2];

	GLuint frameBufferG;
	GLuint frameBufferAo;
	
	GLuint          render_prog_ao, render_prog_phong;

	GLuint          vaoSphere;
	GLuint			iSphereBuffer;
	GLuint			treeVAO;
	long			Tree_Triangles;

	vmath::mat4		modelMatrix;
	vmath::mat4		viewMatrix;
	vmath::mat4		normalViewMatrix;
	vmath::mat4		globalPerspective;
	sb6::object     object;
	int h, w;

	GLuint textureColor[2];
	GLuint textureNormal[2];

	bool moveLeft, moveRight, moveUp, moveDown, lookLeft, lookRight, lookUp, lookDown, resize, objectView, useAOprog;
};

DECLARE_MAIN(advanced_graphics_final)