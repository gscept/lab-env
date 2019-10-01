//------------------------------------------------------------------------------
// window.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "window.h"
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION 1
#include "nanovg_gl.h"
#include <ostream>
#include <iostream>

namespace Display
{

	#ifdef __WIN32__
	#define APICALLTYPE __stdcall
	#else
	#define APICALLTYPE
	#endif
	
	static void GLAPIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		switch (id)
		{
			// ignore non-significant error/warning codes
			case 131218 | 131185 | 131169 | 131204:
				return;
		}
		
		std::string msg("[OPENGL DEBUG MESSAGE] ");
	
		// print error severity
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_LOW:
			msg.append("<Low severity> ");
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			msg.append("<Medium severity> ");
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			msg.append("<High severity> ");
			break;
		}
	
		// append message to output
		msg.append(message);
	
		// print message
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			printf("Error: %s\n", msg.c_str());
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			printf("Performance issue: %s\n", msg.c_str());
			break;
		default:		// Portability, Deprecated, Other
			break;
		}
	}
	
	int32 Window::WindowCount = 0;
	
	Window::Window() :
		width(1024),
		height(768),
		title("gscept Lab Environment"),
		window(nullptr),
		vg(nullptr)
	{ }
	
	Window::~Window()
	{
		
	}
	
	void Window::StaticKeyPressCallback(GLFWwindow* win, int32 key, int32 scancode, int32 action, int32 mods)
	{
		Window* window = (Window*)glfwGetWindowUserPointer(win);
		if (ImGui::IsMouseHoveringAnyWindow())
		{
			ImGui_ImplGlfwGL3_KeyCallback(win, key, scancode, action, mods);
		}
		else if (nullptr != window->keyPressCallback) 
			window->keyPressCallback(key, scancode, action, mods);
	}
	
	void Window::StaticMousePressCallback(GLFWwindow* win, int32 button, int32 action, int32 mods)
	{
		Window* window = (Window*)glfwGetWindowUserPointer(win);
		if (ImGui::IsMouseHoveringAnyWindow())
		{
			ImGui_ImplGlfwGL3_MouseButtonCallback(win, button, action, mods);
		}
		else if (nullptr != window->mousePressCallback)
			window->mousePressCallback(button, action, mods);
	}
	
	void Window::StaticMouseMoveCallback(GLFWwindow* win, float64 x, float64 y)
	{
		Window* window = (Window*)glfwGetWindowUserPointer(win);
		if (nullptr != window->mouseMoveCallback)
			window->mouseMoveCallback(x, y);
	}
	
	void Window::StaticMouseEnterLeaveCallback(GLFWwindow* win, int32 mode)
	{
		Window* window = (Window*)glfwGetWindowUserPointer(win);
		if (nullptr != window->mouseLeaveEnterCallback) 
			window->mouseLeaveEnterCallback(mode == 0);
	}
	
	void Window::StaticMouseScrollCallback(GLFWwindow* win, float64 x, float64 y)
	{
		Window* window = (Window*)glfwGetWindowUserPointer(win);
		if (ImGui::IsMouseHoveringAnyWindow())
		{
			ImGui_ImplGlfwGL3_ScrollCallback(win, x, y);
		}
		else if (nullptr != window->mouseScrollCallback)
			window->mouseScrollCallback(x, y);
	}
	
	void Window::Resize() const
	{
		if (nullptr != this->window)
		{
			glfwSetWindowSize(this->window, this->width, this->height);
	
			// setup viewport
			glViewport(0, 0, this->width, this->height);
		}
	}
	
	void Window::Retitle() const
	{
		if (nullptr != this->window) 
			glfwSetWindowTitle(this->window, this->title.c_str());
	}
	
	bool Window::Open()
	{
		if (WindowCount == 0)
		{
			if (!glfwInit())
			{
				glfwTerminate();
				return false;
			}
		}
	
		// setup window
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 8);
	
		// open window
		this->window = glfwCreateWindow(this->width, this->height, this->title.c_str(), nullptr, nullptr);
		if (this->window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		
		glfwMakeContextCurrent(this->window);
	
		if (this->window != nullptr && WindowCount == 0)
		{
			glewExperimental = true;
			const GLenum res = glewInit();
			assert(res == GLEW_OK);
			if (!(GLEW_VERSION_4_0))
			{
				printf("[WARNING]: OpenGL 4.0+ is not supported on this hardware!\n");
				glfwDestroyWindow(this->window);
				this->window = nullptr;
				return false;
			}
	
			// setup debug callback
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(GLDebugCallback, NULL);
			GLuint unusedIds;
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
	
			// setup stuff
			glEnable(GL_FRAMEBUFFER_SRGB);
			glEnable(GL_LINE_SMOOTH);
			glEnable(GL_POLYGON_SMOOTH);
			glEnable(GL_MULTISAMPLE);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	
			// setup viewport
			glViewport(0, 0, this->width, this->height);
		}
	
		glfwSetWindowUserPointer(this->window, this);
		glfwSetKeyCallback(this->window, StaticKeyPressCallback);
		glfwSetMouseButtonCallback(this->window, StaticMousePressCallback);
		glfwSetCursorPosCallback(this->window, StaticMouseMoveCallback);
		glfwSetCursorEnterCallback(this->window, StaticMouseEnterLeaveCallback);
		glfwSetScrollCallback(this->window, StaticMouseScrollCallback);
		
		// setup imgui implementation
	    ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(this->window, false);
		glfwSetCharCallback(window, ImGui_ImplGlfwGL3_CharCallback);
	
		// setup nanovg
		this->vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
	
		// increase window count and return result
		WindowCount++;
		return this->window != nullptr;
	}
	
	void Window::Close()
	{
		if (nullptr != this->window) glfwDestroyWindow(this->window);
		this->window = nullptr;
		WindowCount--;
		if (WindowCount == 0)
		{
			ImGui_ImplGlfwGL3_Shutdown();
			glfwTerminate();
		}
	}
	
	void Window::MakeCurrent() const
	{
		glfwMakeContextCurrent(this->window);
	}

	void Window::Update()
	{
		glfwPollEvents();	
	}
	
	void Window::SwapBuffers()
	{
		if (this->window)
		{
			if (nullptr != this->nanoFunc)
			{
				int32 fbWidth, fbHeight;
				glClear(GL_STENCIL_BUFFER_BIT);
				glfwGetWindowSize(this->window, &this->width, &this->height);
				glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
				nvgBeginFrame(this->vg, this->width, this->height, (float)fbWidth / (float) this->width);
				this->nanoFunc(this->vg);
				nvgEndFrame(this->vg);
			}
			
			if (nullptr != this->uiFunc)
			{
				ImGui_ImplGlfwGL3_NewFrame();
				this->uiFunc();
				ImGui::Render();
			}
			
			glfwSwapBuffers(this->window);
		}
	}
	
	void Window::SetCursorPosition(double x, double y) const
	{
		glfwSetCursorPos(this->window, x, y);
	}
	
	void Window::HideCursor(bool value) const
	{
		glfwSetInputMode(this->window, GLFW_CURSOR, value ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
	}
} 