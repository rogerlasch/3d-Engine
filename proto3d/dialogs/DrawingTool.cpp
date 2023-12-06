

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <GL/gl.h>
#include"DrawingTool.h"

using namespace std;

DrawingTool::DrawingTool(wxWindow* parent, wxWindowID id , const int* attribList):
wxGLCanvas(parent, id, attribList) {
        m_context = new wxGLContext(this);
    }

DrawingTool::~DrawingTool(){
}

    void DrawingTool::OnPaint(wxPaintEvent& event){
this->draw();
    }

    void DrawingTool::draw(){
    wxPaintDC dc(this);
        SetCurrent(*m_context);

    // Obter as dimensões do cliente da janela
    int width, height;
    GetClientSize(&width, &height);

    // Definir o viewport para o tamanho do cliente
    glViewport(0, 0, width, height);

        // Limpar o buffer
glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Define a cor de fundo (preto no exemplo)
        glClear(GL_COLOR_BUFFER_BIT);

        // Aqui você pode inserir seu código OpenGL para renderizar
        // Configurar a matriz de projeção (ortográfica no exemplo)
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 800, 0, 600, -1, 1);  // Defina a janela de projeção conforme necessário

        // Configurar a matriz de modelo (posição e escala do triângulo)
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Limpar o buffer de cor
        glClear(GL_COLOR_BUFFER_BIT);

        // Definir a cor de desenho (vermelho)
        glColor3f(0.0f, 0.0f, 0.0f);

        // Desenhar o triângulo
        glBegin(GL_TRIANGLES);
        glVertex3f(10, 0, 0);
        glVertex3f(500, 500, 0);
        glVertex3f(1000, 0, 0);
        glEnd();

        // Trocar os buffers
        SwapBuffers();
    }
