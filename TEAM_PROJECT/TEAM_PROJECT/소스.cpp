#pragma once
#pragma comment (lib, "strmiids.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "freeglut.lib")
#include <stdio.h>
#include <Windows.h>
#include <dshow.h> 

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "ReadObj.h"
#define PIE 3.1415926
#define BIAUS 0.8f
using namespace std;

#define WINDOW_WITDH   1200
#define WINDOW_HEIGHT   860

#define OBJECT_COLOR   glm::vec3(0.2, 0.2, 0.2)
#define LIGHT_AMBIENT   glm::vec3(0.5, 0.5, 0.5)
#define LIGHT_POS      glm::vec3(1000.0, 1000.0, 000.0)
#define LIGHT_COLOR      glm::vec3(1.0, 1.0, 1.0)

#define FILE_NAME      "hexahedron.obj"
#define CAR_FILE_NAME      "car.obj"
#define BEACON_FILE_NAME      "sphere.obj"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid keyUp(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);
void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();
void init();
void crush(int left, int right, int top, int bottom);
void crush_up();
void crush_down();
void crush_left();
void crush_right();

void Score_m(int n);
bool check_collision(float x1, float z1, float w1, float h1, float x2, float z2, float w2, float h2);
void sound(LPCWSTR sound);
void obj_init();
GLuint fragmentShader;
GLuint vertexShader;
GLuint s_program;

GLuint vao[3];
GLuint vbo[3];
GLuint ebo;

float* vPosData;
float* vNormalData;
float* vTextureCoordinateData;
int* indexData;
int vertexCount;
int indexCount;

float* vPosData_2;
float* vNormalData_2;
float* vTextureCoordinateData_2;
int* indexData_2;
int vertexCount_2;
int indexCount_2;

float* vPosData_3;
float* vNormalData_3;
float* vTextureCoordinateData_3;
int* indexData_3;
int vertexCount_3;
int indexCount_3;

float degree_cameraRotate = 0.0f;
float degree_lightPos = 0.0f;
float lrx = 0.0f;
float L_Color_R = 1.0f, L_Color_G = 1.0f, L_Color_B = 1.0f, cz = 20.0f, cy = 10.0f, test_x = 0.0f, test_y = 10.0f, test_z = 0.0f, acl_pedal = 0.0f, handle_ry = 0.0f, handle_dx = 1.0f, handle_dz = 0.0f;
float car_tx = 0.0f, car_tz = 0.0f, cx = 0.0f, tire_rz = 0.0f;
int score = 100;

bool on_focusBackPoint = false;
bool on_focusSelfPoint = false;
bool on_collision = false;
bool is_acl_pedal = false, is_brake = false, indicator_lamp_l = false, indicator_lamp_r = false, indicator_lamp_l_time = false, indicator_lamp_r_time = false;
bool check_count[10] = { false, }, beacon_G_time = false, beacon_Y_time = false, beacon_R_time = false, is_T_PK = false, is_T_claer = false, is_crush = false;
bool is_all_claer = false, is_finish = false;
bool start_game = true;
int count_see_degree = 1;
float see_degree_speed = 2.0f;
float see_degree = 8.0f;
int i = 0;

struct OBJ { //define shape
    float top, left, right, bottom;
};
OBJ a[13];

typedef struct SCORE {
    bool top = false;
    bool middle = false;
    bool bottom = false;
    bool right_up = false;
    bool right_down = false;
    bool left_up = false;
    bool left_down = false;
};

typedef struct GEAR {
    bool reverse;
    bool drive;
    bool parking;
};

GEAR Gear;

void set_score_light(SCORE& s, int score);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100, 0);
    glutInitWindowSize(WINDOW_WITDH, WINDOW_HEIGHT);
    glutCreateWindow("Example1");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";

    ReadObj(FILE_NAME, vPosData, vNormalData, vTextureCoordinateData, indexData, vertexCount, indexCount);
    ReadObj(CAR_FILE_NAME, vPosData_2, vNormalData_2, vTextureCoordinateData_2, indexData_2, vertexCount_2, indexCount_2);
    ReadObj(BEACON_FILE_NAME, vPosData_3, vNormalData_3, vTextureCoordinateData_3, indexData_3, vertexCount_3, indexCount_3);

    InitShader();
    InitBuffer();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutTimerFunc(20, TimerFunction, 1);
    glutTimerFunc(500, TimerFunction, 2);
    glutTimerFunc(20, TimerFunction, 3);
    glutTimerFunc(100, TimerFunction, 4);
    init();
    obj_init();

    glutKeyboardUpFunc(keyUp);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
}
void init() {
    Gear.drive = false;
    Gear.reverse = false;
    Gear.parking = false;
}
GLvoid drawScene()
{
    glClearColor(0, 0, 0, 1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glUseProgram(s_program);

    glm::mat4 modelTransform = glm::mat4(1.0f);
    modelTransform = glm::translate(modelTransform, glm::vec3(0, 0, 0));

    glm::mat4 vTransform = glm::mat4(1.0f);
    glm::mat4 pTransform = glm::mat4(1.0f);

    if (on_focusSelfPoint == false)
    {//-------------3인칭 시점
        vTransform = glm::lookAt(glm::vec3(0.0f, 3.5f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        vTransform = glm::translate(vTransform, glm::vec3(0.0f, -3.5f, -5.0f));
        if (on_focusBackPoint == true)
            vTransform = glm::rotate(vTransform, glm::radians(-handle_ry - 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));//후면
        else
            vTransform = glm::rotate(vTransform, glm::radians(-handle_ry + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));//앞면

        vTransform = glm::translate(vTransform, glm::vec3(-car_tx + 1.5f, 0.1f, (-car_tz - 8.0f)));
        pTransform = glm::perspective(glm::radians(60.0f), WINDOW_WITDH / (float)WINDOW_HEIGHT, 0.001f, 1000.f);
    }
    else {
        //-------------1인칭시점
        vTransform = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        vTransform = glm::translate(vTransform, glm::vec3(0.0f, -0.0f, -5.0f));
        if (on_focusBackPoint == true)
            vTransform = glm::rotate(vTransform, glm::radians(-handle_ry - 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));//후면
        else
            vTransform = glm::rotate(vTransform, glm::radians(-handle_ry + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));//앞면

        vTransform = glm::translate(vTransform, glm::vec3(-car_tx + 1.5f, -2.0f, -car_tz - 8.0f));
        pTransform = glm::perspective(glm::radians(see_degree), WINDOW_WITDH / (float)WINDOW_HEIGHT, 0.01f, 1000.f);

    }


    GLuint modelTransformLocation = glGetUniformLocation(s_program, "g_modelTransform");
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(modelTransform));

    GLuint viewLocation = glGetUniformLocation(s_program, "g_view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(vTransform));

    GLuint projectLocation = glGetUniformLocation(s_program, "g_projection");
    glUniformMatrix4fv(projectLocation, 1, GL_FALSE, glm::value_ptr(pTransform));

    glm::vec3 lightAmbient_lamp = glm::vec3(2.0, 2.0, 2.0);
    GLuint lightAmbientLocation_lamp = glGetUniformLocation(s_program, "g_lightAmbient");
    glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);

    glm::vec3 lightAmbient = LIGHT_AMBIENT;
    GLuint lightAmbientLocation = glGetUniformLocation(s_program, "g_lightAmbient");
    glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);

    glm::vec4 lightPos = glm::rotate(glm::mat4(1.0f), glm::radians(lrx), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(LIGHT_POS, 1.0f);
    GLuint lightPosLocation = glGetUniformLocation(s_program, "g_lightPos");
    glUniform3fv(lightPosLocation, 1, (float*)&lightPos);

    glm::vec3 lightColor = glm::vec3(L_Color_R, L_Color_G, L_Color_B);
    GLuint lightColorLocation = glGetUniformLocation(s_program, "g_lightColor");
    glUniform3fv(lightColorLocation, 1, (float*)&lightColor);

    glm::vec3 objColor = OBJECT_COLOR;
    GLuint objColorLocation = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation, 1, (float*)&objColor);


    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.1f);
    GLuint cameraPosLocation = glGetUniformLocation(s_program, "g_cameraPos");
    glUniform3fv(cameraPosLocation, 1, (float*)&cameraPos);

    //바닥
    glm::mat4 floor = glm::mat4(1.0f);
    floor = glm::scale(floor, glm::vec3(1.1f, 0.01f, 1.1f));
    glBindVertexArray(vao[0]);
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(floor));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);


    //자동차
    glBindVertexArray(vao[1]);

    glm::mat4 car = glm::mat4(1.0f);
    car = glm::translate(car, glm::vec3(car_tx - 1.5f, 0, 8 + car_tz));
    car = glm::rotate(car, glm::radians(handle_ry), glm::vec3(0.0, 1.0, 0.0));
    car = glm::translate(car, glm::vec3(-(car_tx - 1.5f), 0, -(8 + car_tz)));

    car = glm::translate(car, glm::vec3(car_tx, 0, car_tz));
    


    glm::vec3 car_color = glm::vec3(0.8, 0.8, 0.0);
    GLuint objColorLocation_car = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_car, 1, (float*)&car_color);


    glm::mat4 car_1 = glm::mat4(1.0f);
    car_1 = car_1 * car;
    car_1 = glm::scale(car_1, glm::vec3(1.5f, 0.5f, 0.8f));
    car_1 = glm::translate(car_1, glm::vec3(-1, 1.8f, 10));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(car_1));
    glDrawElements(GL_TRIANGLES, indexCount_2, GL_UNSIGNED_INT, 0);

    if (on_focusSelfPoint == false)
    {
        glm::mat4 car_2 = glm::mat4(1.0f);
        car_2 = car_2 * car;
        car_2 = glm::scale(car_2, glm::vec3(0.5f, 0.5f, 0.5f));
        car_2 = glm::translate(car_2, glm::vec3(-3, 2.8, 16));
        glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(car_2));
        glDrawElements(GL_TRIANGLES, indexCount_2, GL_UNSIGNED_INT, 0);
    }

    glm::vec3 car_window_color = glm::vec3(0.1, 0.1, 0.5);
    GLuint objColorLocation_window = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_window, 1, (float*)&car_window_color);

    //자동차 창문
    glm::mat4 car_window = glm::mat4(1.0f);
    car_window = car_window * car;
    car_window = glm::scale(car_window, glm::vec3(0.1f, 0.4f, 0.4f));
    car_window = glm::translate(car_window, glm::vec3(-10, 3.5, 20));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(car_window));
    if (on_focusSelfPoint == false)
        glDrawElements(GL_TRIANGLES, indexCount_2, GL_UNSIGNED_INT, 0);

    //전조등
    glBindVertexArray(vao[2]);
    glm::vec3 head_light_color = glm::vec3(0.3, 0.0, 0.0);
    GLuint objColorLocation_head_color = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_head_color, 1, (float*)&head_light_color);

    glm::mat4 car_head_light_r = glm::mat4(1.0f);
    car_head_light_r = car_head_light_r * car;
    car_head_light_r = glm::scale(car_head_light_r, glm::vec3(0.05f, 0.05f, 0.05f));
    car_head_light_r = glm::translate(car_head_light_r, glm::vec3(-6, 15, 152));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(car_head_light_r));
    glDrawElements(GL_TRIANGLES, indexCount_3, GL_UNSIGNED_INT, 0);

    glm::mat4 car_head_light_l = glm::mat4(1.0f);
    car_head_light_l = car_head_light_l * car;
    car_head_light_l = glm::scale(car_head_light_l, glm::vec3(0.05f, 0.05f, 0.05f));
    car_head_light_l = glm::translate(car_head_light_l, glm::vec3(-6, 15, 168));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(car_head_light_l));
    glDrawElements(GL_TRIANGLES, indexCount_3, GL_UNSIGNED_INT, 0);

    glBindVertexArray(vao[1]);
    //후미등
    if (is_brake == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    else {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::vec3 car_lamp_color = glm::vec3(0.8, 0.0, 0.0);
    GLuint objColorLocation_lamp = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_lamp, 1, (float*)&car_lamp_color);

    if (indicator_lamp_r == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (indicator_lamp_l == true && is_brake == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 car_rear_lamp_r = glm::mat4(1.0f);
    car_rear_lamp_r = car_rear_lamp_r * car;
    car_rear_lamp_r = glm::scale(car_rear_lamp_r, glm::vec3(0.2f, 0.2f, 0.2f));
    car_rear_lamp_r = glm::translate(car_rear_lamp_r, glm::vec3(-14.5, 4, 43.1));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(car_rear_lamp_r));
    glDrawElements(GL_TRIANGLES, indexCount_2, GL_UNSIGNED_INT, 0);

    if (indicator_lamp_r == true) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }
    if (indicator_lamp_l == true && is_brake == false) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }

    glm::mat4 car_rear_lamp_l = glm::mat4(1.0f);
    car_rear_lamp_l = car_rear_lamp_l * car;
    car_rear_lamp_l = glm::scale(car_rear_lamp_l, glm::vec3(0.2f, 0.2f, 0.2f));
    car_rear_lamp_l = glm::translate(car_rear_lamp_l, glm::vec3(-14.5, 4, 36.9));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(car_rear_lamp_l));
    glDrawElements(GL_TRIANGLES, indexCount_2, GL_UNSIGNED_INT, 0);

    //다시 원래 앰비언트로
    glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);

    //타이어 
    glm::vec3 tire_color = glm::vec3(0.1, 0.1, 0.1);
    GLuint objColorLocation_tire = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_tire, 1, (float*)&tire_color);

    glm::mat4 tire_rf = glm::mat4(1.0f);
    tire_rf = tire_rf * car;

    tire_rf = glm::translate(tire_rf, glm::vec3(-0.5f, 0.25f, 8.8f));
    tire_rf = glm::rotate(tire_rf, glm::radians(tire_rz), glm::vec3(0.0, 0.0, 1.0));
    tire_rf = glm::translate(tire_rf, glm::vec3(0.5f, -0.25f, -8.8f));

    tire_rf = glm::scale(tire_rf, glm::vec3(0.25f, 0.25f, 0.1f));
    tire_rf = glm::translate(tire_rf, glm::vec3(-2, 1, 88));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(tire_rf));
    glDrawElements(GL_TRIANGLES, indexCount_2, GL_UNSIGNED_INT, 0);

    glm::mat4 tire_lf = glm::mat4(1.0f);
    tire_lf = tire_lf * car;


    tire_lf = glm::translate(tire_lf, glm::vec3(-0.5f, 0.25f, 7.2f));
    tire_lf = glm::rotate(tire_lf, glm::radians(tire_rz), glm::vec3(0.0, 0.0, 1.0));
    tire_lf = glm::translate(tire_lf, glm::vec3(0.5f, -0.25f, -7.2f));


    tire_lf = glm::scale(tire_lf, glm::vec3(0.25f, 0.25f, 0.1f));
    tire_lf = glm::translate(tire_lf, glm::vec3(-2, 1, 72));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(tire_lf));
    glDrawElements(GL_TRIANGLES, indexCount_2, GL_UNSIGNED_INT, 0);

    glm::mat4 tire_rb = glm::mat4(1.0f);
    tire_rb = tire_rb * car;

    tire_rb = glm::translate(tire_rb, glm::vec3(-2.5f, 0.25f, 8.8f));
    tire_rb = glm::rotate(tire_rb, glm::radians(tire_rz), glm::vec3(0.0, 0.0, 1.0));
    tire_rb = glm::translate(tire_rb, glm::vec3(2.5f, -0.25f, -8.8f));

    tire_rb = glm::scale(tire_rb, glm::vec3(0.25f, 0.25f, 0.1f));
    tire_rb = glm::translate(tire_rb, glm::vec3(-10.0f, 1, 88));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(tire_rb));
    glDrawElements(GL_TRIANGLES, indexCount_2, GL_UNSIGNED_INT, 0);

    glm::mat4 tire_lb = glm::mat4(1.0f);
    tire_lb = tire_lb * car;

    tire_lb = glm::translate(tire_lb, glm::vec3(-2.5f, 0.25f, 7.2f));
    tire_lb = glm::rotate(tire_lb, glm::radians(tire_rz), glm::vec3(0.0, 0.0, 1.0));
    tire_lb = glm::translate(tire_lb, glm::vec3(2.5f, -0.25f, -7.2f));

    tire_lb = glm::scale(tire_lb, glm::vec3(0.25f, 0.25f, 0.1f));
    tire_lb = glm::translate(tire_lb, glm::vec3(-10.0f, 1, 72));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(tire_lb));
    glDrawElements(GL_TRIANGLES, indexCount_2, GL_UNSIGNED_INT, 0);

    //장애물
    glBindVertexArray(vao[0]);

    glm::vec3 obj_wall_color = glm::vec3(0.0, 0.5, 0.1);
    GLuint objColorLocation_o_w = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_o_w, 1, (float*)&obj_wall_color);

    glm::mat4 obj_wall_1 = glm::mat4(1.0f);//x 0 z 6
    obj_wall_1 = glm::scale(obj_wall_1, glm::vec3(0.6f, 0.04f, 0.05f));
    obj_wall_1 = glm::translate(obj_wall_1, glm::vec3(1, 28, 120.0f));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall_1));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall_2 = glm::mat4(1.0f);
    obj_wall_2 = glm::scale(obj_wall_2, glm::vec3(0.05f, 0.04f, 0.2f));
    obj_wall_2 = glm::translate(obj_wall_2, glm::vec3(-98.0f, 28, 42.0f));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall_2));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall_3 = glm::mat4(1.0f);
    obj_wall_3 = glm::scale(obj_wall_3, glm::vec3(0.25f, 0.04f, 0.05f));
    obj_wall_3 = glm::translate(obj_wall_3, glm::vec3(34.0f, 28, -80.0f));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall_3));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall_4 = glm::mat4(1.0f);
    obj_wall_4 = glm::scale(obj_wall_4, glm::vec3(0.4f, 0.04f, 0.05f));
    obj_wall_4 = glm::translate(obj_wall_4, glm::vec3(10.0f, 28, 0.0f));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall_4));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall_5 = glm::mat4(1.0f);
    obj_wall_5 = glm::scale(obj_wall_5, glm::vec3(0.2f, 0.04f, 0.1f));
    obj_wall_5 = glm::translate(obj_wall_5, glm::vec3(-25.0f, 28, -5));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall_5));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall_6 = glm::mat4(1.0f);
    obj_wall_6 = glm::scale(obj_wall_6, glm::vec3(0.05f, 0.04f, 0.4f));
    obj_wall_6 = glm::translate(obj_wall_6, glm::vec3(-130, 28, -10));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall_6));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall_7 = glm::mat4(1.0f);
    obj_wall_7 = glm::scale(obj_wall_7, glm::vec3(0.15f, 0.04f, 0.18f));
    obj_wall_7 = glm::translate(obj_wall_7, glm::vec3(-30, 28, -34.5));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall_7));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall_8 = glm::mat4(1.0f);
    obj_wall_8 = glm::scale(obj_wall_8, glm::vec3(0.5f, 0.04f, 0.05f));
    obj_wall_8 = glm::translate(obj_wall_8, glm::vec3(4, 28, -150));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall_8));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall_9 = glm::mat4(1.0f);
    obj_wall_9 = glm::scale(obj_wall_9, glm::vec3(0.1f, 0.04f, 0.4f));
    obj_wall_9 = glm::translate(obj_wall_9, glm::vec3(10, 28, -10));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall_9));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);


    //---장애물 아래
    glBindVertexArray(vao[0]);

    glm::vec3 obj_wall2_color = glm::vec3(0.9, 0.9, 0.9);
    GLuint objColorLocation_o_w2 = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_o_w2, 1, (float*)&obj_wall2_color);

    glm::mat4 obj_wall2_1 = glm::mat4(1.0f);//x 0 z 6
    obj_wall2_1 = glm::scale(obj_wall2_1, glm::vec3(0.6f, 0.02f, 0.05f));
    obj_wall2_1 = glm::translate(obj_wall2_1, glm::vec3(1, 26, 120.0f));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall2_1));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall2_2 = glm::mat4(1.0f);
    obj_wall2_2 = glm::scale(obj_wall2_2, glm::vec3(0.05f, 0.02f, 0.2f));
    obj_wall2_2 = glm::translate(obj_wall2_2, glm::vec3(-98.0f, 26, 42.0f));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall2_2));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall2_3 = glm::mat4(1.0f);
    obj_wall2_3 = glm::scale(obj_wall2_3, glm::vec3(0.25f, 0.02f, 0.05f));
    obj_wall2_3 = glm::translate(obj_wall2_3, glm::vec3(34.0f, 26, -80.0f));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall2_3));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall2_4 = glm::mat4(1.0f);
    obj_wall2_4 = glm::scale(obj_wall2_4, glm::vec3(0.4f, 0.02f, 0.05f));
    obj_wall2_4 = glm::translate(obj_wall2_4, glm::vec3(10.0f, 26, 0.0f));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall2_4));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);


    glm::mat4 obj_wall2_5 = glm::mat4(1.0f);
    obj_wall2_5 = glm::scale(obj_wall2_5, glm::vec3(0.2f, 0.02f, 0.1f));
    obj_wall2_5 = glm::translate(obj_wall2_5, glm::vec3(-25.0f, 26, -5));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall2_5));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall2_6 = glm::mat4(1.0f);
    obj_wall2_6 = glm::scale(obj_wall2_6, glm::vec3(0.05f, 0.02f, 0.4f));
    obj_wall2_6 = glm::translate(obj_wall2_6, glm::vec3(-130, 26, -10));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall2_6));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall2_7 = glm::mat4(1.0f);
    obj_wall2_7 = glm::scale(obj_wall2_7, glm::vec3(0.15f, 0.02f, 0.18f));
    obj_wall2_7 = glm::translate(obj_wall2_7, glm::vec3(-30, 26, -34.5));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall2_7));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall2_8 = glm::mat4(1.0f);
    obj_wall2_8 = glm::scale(obj_wall2_8, glm::vec3(0.5f, 0.02f, 0.05f));
    obj_wall2_8 = glm::translate(obj_wall2_8, glm::vec3(4, 26, -150));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall2_8));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 obj_wall2_9 = glm::mat4(1.0f);
    obj_wall2_9 = glm::scale(obj_wall2_9, glm::vec3(0.1f, 0.02f, 0.4f));
    obj_wall2_9 = glm::translate(obj_wall2_9, glm::vec3(10, 26, -10));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(obj_wall2_9));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);


    //라운드 벽1
    glm::vec3 round_wall_color_11 = glm::vec3(1.0, 1.0, 0.0);
    objColorLocation_o_w = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_o_w, 1, (float*)&round_wall_color_11);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_11 = glm::mat4(1.0f);
    round_wall_11 = glm::scale(round_wall_11, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_11 = glm::translate(round_wall_11, glm::vec3(-1100, 140, 90));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_11));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_13 = glm::mat4(1.0f);
    round_wall_13 = glm::scale(round_wall_13, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_13 = glm::translate(round_wall_13, glm::vec3(-1100, 140, 50));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_13));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_15 = glm::mat4(1.0f);
    round_wall_15 = glm::scale(round_wall_15, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_15 = glm::translate(round_wall_15, glm::vec3(-1100, 140, 10));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_15));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);


    glm::mat4 round_wall_17 = glm::mat4(1.0f);
    round_wall_17 = glm::scale(round_wall_17, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_17 = glm::translate(round_wall_17, glm::vec3(-1100, 140, -30));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_17));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);


    glm::mat4 round_wall_19 = glm::mat4(1.0f);
    round_wall_19 = glm::scale(round_wall_19, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_19 = glm::translate(round_wall_19, glm::vec3(-1100, 140, -70));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_19));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::vec3 round_wall_color_12 = glm::vec3(0.3, 0.3, 0.3);
    objColorLocation_o_w = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_o_w, 1, (float*)&round_wall_color_12);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_12 = glm::mat4(1.0f);
    round_wall_12 = glm::scale(round_wall_12, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_12 = glm::translate(round_wall_12, glm::vec3(-1100, 140, 70));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_12));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_14 = glm::mat4(1.0f);
    round_wall_14 = glm::scale(round_wall_14, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_14 = glm::translate(round_wall_14, glm::vec3(-1100, 140, 30));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_14));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_16 = glm::mat4(1.0f);
    round_wall_16 = glm::scale(round_wall_16, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_16 = glm::translate(round_wall_16, glm::vec3(-1100, 140, -10));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_16));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_18 = glm::mat4(1.0f);
    round_wall_18 = glm::scale(round_wall_18, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_18 = glm::translate(round_wall_18, glm::vec3(-1100, 140, -50));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_18));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_10 = glm::mat4(1.0f);
    round_wall_10 = glm::scale(round_wall_10, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_10 = glm::translate(round_wall_10, glm::vec3(-1100, 140.0f, -90));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_10));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    //라운드 벽2
    glm::vec3 round_wall_color_21 = glm::vec3(1.0, 1.0, 0.0);
    objColorLocation_o_w = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_o_w, 1, (float*)&round_wall_color_11);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_21 = glm::mat4(1.0f);
    round_wall_21 = glm::scale(round_wall_21, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_21 = glm::translate(round_wall_21, glm::vec3(90, 140, -1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_21));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_23 = glm::mat4(1.0f);
    round_wall_23 = glm::scale(round_wall_23, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_23 = glm::translate(round_wall_23, glm::vec3(50, 140, -1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_23));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_25 = glm::mat4(1.0f);
    round_wall_25 = glm::scale(round_wall_25, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_25 = glm::translate(round_wall_25, glm::vec3(10, 140, -1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_25));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);


    glm::mat4 round_wall_27 = glm::mat4(1.0f);
    round_wall_27 = glm::scale(round_wall_27, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_27 = glm::translate(round_wall_27, glm::vec3(-30, 140, -1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_27));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);


    glm::mat4 round_wall_29 = glm::mat4(1.0f);
    round_wall_29 = glm::scale(round_wall_29, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_29 = glm::translate(round_wall_29, glm::vec3(-70, 140, -1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_29));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::vec3 round_wall_color_22 = glm::vec3(0.3, 0.3, 0.3);
    objColorLocation_o_w = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_o_w, 1, (float*)&round_wall_color_22);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_22 = glm::mat4(1.0f);
    round_wall_22 = glm::scale(round_wall_22, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_22 = glm::translate(round_wall_22, glm::vec3(70, 140, -1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_22));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_24 = glm::mat4(1.0f);
    round_wall_24 = glm::scale(round_wall_24, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_24 = glm::translate(round_wall_24, glm::vec3(30, 140, -1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_24));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_26 = glm::mat4(1.0f);
    round_wall_26 = glm::scale(round_wall_26, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_26 = glm::translate(round_wall_26, glm::vec3(-10, 140, -1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_26));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_28 = glm::mat4(1.0f);
    round_wall_28 = glm::scale(round_wall_28, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_28 = glm::translate(round_wall_28, glm::vec3(-50, 140, -1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_28));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_20 = glm::mat4(1.0f);
    round_wall_20 = glm::scale(round_wall_20, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_20 = glm::translate(round_wall_20, glm::vec3(-90, 140.0f, -1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_20));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    //----------------라운드 벽3
    glm::vec3 round_wall_color_31 = glm::vec3(1.0, 1.0, 0.0);
    objColorLocation_o_w = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_o_w, 1, (float*)&round_wall_color_31);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_31 = glm::mat4(1.0f);
    round_wall_31 = glm::scale(round_wall_31, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_31 = glm::translate(round_wall_31, glm::vec3(1100, 140, 90));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_31));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_33 = glm::mat4(1.0f);
    round_wall_33 = glm::scale(round_wall_33, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_33 = glm::translate(round_wall_33, glm::vec3(1100, 140, 50));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_33));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_35 = glm::mat4(1.0f);
    round_wall_35 = glm::scale(round_wall_35, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_35 = glm::translate(round_wall_35, glm::vec3(1100, 140, 10));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_35));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);



    glm::mat4 round_wall_37 = glm::mat4(1.0f);
    round_wall_37 = glm::scale(round_wall_37, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_37 = glm::translate(round_wall_37, glm::vec3(1100, 140, -30));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_37));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);


    glm::mat4 round_wall_39 = glm::mat4(1.0f);
    round_wall_39 = glm::scale(round_wall_39, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_39 = glm::translate(round_wall_39, glm::vec3(1100, 140, -70));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_39));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);


    //-----------
    glm::vec3 round_wall_color_32 = glm::vec3(0.3, 0.3, 0.3);
    objColorLocation_o_w = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_o_w, 1, (float*)&round_wall_color_32);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_32 = glm::mat4(1.0f);
    round_wall_32 = glm::scale(round_wall_32, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_32 = glm::translate(round_wall_32, glm::vec3(1100, 140, 70));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_32));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_34 = glm::mat4(1.0f);
    round_wall_34 = glm::scale(round_wall_34, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_34 = glm::translate(round_wall_34, glm::vec3(1100, 140, 30));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_34));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_36 = glm::mat4(1.0f);
    round_wall_36 = glm::scale(round_wall_36, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_36 = glm::translate(round_wall_36, glm::vec3(1100, 140, -10));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_36));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_38 = glm::mat4(1.0f);
    round_wall_38 = glm::scale(round_wall_38, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_38 = glm::translate(round_wall_38, glm::vec3(1100, 140, -50));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_38));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_30 = glm::mat4(1.0f);
    round_wall_30 = glm::scale(round_wall_30, glm::vec3(0.01f, 0.01f, 0.11f));
    round_wall_30 = glm::translate(round_wall_30, glm::vec3(1100, 140.0f, -90));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_30));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    //---------------------라운드벽4

    glm::vec3 round_wall_color_41 = glm::vec3(1.0, 1.0, 0.0);
    objColorLocation_o_w = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_o_w, 1, (float*)&round_wall_color_41);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_41 = glm::mat4(1.0f);
    round_wall_41 = glm::scale(round_wall_41, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_41 = glm::translate(round_wall_41, glm::vec3(90, 140, 1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_41));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_43 = glm::mat4(1.0f);
    round_wall_43 = glm::scale(round_wall_43, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_43 = glm::translate(round_wall_43, glm::vec3(50, 140, 1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_43));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_45 = glm::mat4(1.0f);
    round_wall_45 = glm::scale(round_wall_45, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_45 = glm::translate(round_wall_45, glm::vec3(10, 140, 1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_45));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);



    glm::mat4 round_wall_47 = glm::mat4(1.0f);
    round_wall_47 = glm::scale(round_wall_47, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_47 = glm::translate(round_wall_47, glm::vec3(-30, 140, 1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_47));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);


    glm::mat4 round_wall_49 = glm::mat4(1.0f);
    round_wall_49 = glm::scale(round_wall_49, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_49 = glm::translate(round_wall_49, glm::vec3(-70, 140, 1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_49));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);


    //-----------
    glm::vec3 round_wall_color_42 = glm::vec3(0.3, 0.3, 0.3);
    objColorLocation_o_w = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_o_w, 1, (float*)&round_wall_color_42);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_42 = glm::mat4(1.0f);
    round_wall_42 = glm::scale(round_wall_42, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_42 = glm::translate(round_wall_42, glm::vec3(70, 140, 1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_42));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_44 = glm::mat4(1.0f);
    round_wall_44 = glm::scale(round_wall_44, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_44 = glm::translate(round_wall_44, glm::vec3(30, 140, 1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_44));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_46 = glm::mat4(1.0f);
    round_wall_46 = glm::scale(round_wall_46, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_46 = glm::translate(round_wall_46, glm::vec3(-10, 140, 1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_46));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_48 = glm::mat4(1.0f);
    round_wall_48 = glm::scale(round_wall_48, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_48 = glm::translate(round_wall_48, glm::vec3(-50, 140, 1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_48));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 round_wall_40 = glm::mat4(1.0f);
    round_wall_40 = glm::scale(round_wall_40, glm::vec3(0.11f, 0.01f, 0.01f));
    round_wall_40 = glm::translate(round_wall_40, glm::vec3(-90, 140.0f, 1100));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(round_wall_40));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    //정지선
    glBindVertexArray(vao[1]);
    glm::vec3 stop_line_color = glm::vec3(1.0, 1.0, 1.0);
    GLuint objColorLocation_stop = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_stop, 1, (float*)&stop_line_color);

    glm::mat4 stop_line = glm::mat4(1.0f);
    stop_line = glm::scale(stop_line, glm::vec3(0.1f, 0.1f, 3.0f));
    stop_line = glm::translate(stop_line, glm::vec3(40, 1.0f, 1));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(stop_line));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glm::mat4 spd_stop_line = glm::mat4(1.0f);
    spd_stop_line = glm::scale(spd_stop_line, glm::vec3(0.1f, 0.1f, 1.5f));
    spd_stop_line = glm::translate(spd_stop_line, glm::vec3(35.0f, 1.0f, -6.3f));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(spd_stop_line));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (is_T_PK == true) {
        glm::vec3 T_line_color = glm::vec3(0.0f, 0.8f, 0.0f);
        GLuint objColorLocation_beacon_line = glGetUniformLocation(s_program, "g_objectColor");
        glUniform3fv(objColorLocation_beacon_line, 1, (float*)&T_line_color);
    }
    else if (is_T_PK == false) {
        glUniform3fv(objColorLocation_stop, 1, (float*)&stop_line_color);
    }
    glm::mat4 T_stop_line = glm::mat4(1.0f);
    T_stop_line = glm::scale(T_stop_line, glm::vec3(0.1f, 0.1f, 3.0f));
    T_stop_line = glm::translate(T_stop_line, glm::vec3(-32, 1.0f, -1));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(T_stop_line));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (is_all_claer == true) {
        glm::vec3 clear_color = glm::vec3(0.0f, 0.8f, 0.0f);
        GLuint objColorLocation_clear = glGetUniformLocation(s_program, "g_objectColor");
        glUniform3fv(objColorLocation_clear, 1, (float*)&clear_color);
    }
    else if (is_all_claer == false) {
        glm::vec3 false_color = glm::vec3(0.8f, 0.0f, 0.0f);
        GLuint objColorLocation_false = glGetUniformLocation(s_program, "g_objectColor");
        glUniform3fv(objColorLocation_false, 1, (float*)&false_color);
    }

    glm::mat4 goal_line = glm::mat4(1.0f);
    goal_line = glm::scale(goal_line, glm::vec3(3.0f, 0.1f, 0.1f));
    goal_line = glm::translate(goal_line, glm::vec3(-2.6f, 1.0f, 57.0f));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(goal_line));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    //점수판
    glm::vec3 score_board_color = glm::vec3(0.2, 0.2, 0.2);
    GLuint objColorLocation_board = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_board, 1, (float*)&score_board_color);

    glm::mat4 score_board = glm::mat4(1.0f);
    score_board = glm::scale(score_board, glm::vec3(1.2, 0.6, 0.05f));
    score_board = glm::translate(score_board, glm::vec3(-6.8, 4.16, 220));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score_board));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    SCORE Score_1, Score_2, Score_3;
    //점수표시
    if (is_all_claer == true)
    {
        int score1 = score / 100;
        int score2 = (score - score1 * 100) / 10;
        int score3 = score % 10;

        set_score_light(Score_1, score1);
        set_score_light(Score_2, score2);
        set_score_light(Score_3, score3);
    }
    glm::vec3 score_color = glm::vec3(0.5, 0.0, 0.0);
    GLuint objColorLocation_score = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_score, 1, (float*)&score_color);

    if (Score_1.top == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_1.top == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score1_1 = glm::mat4(1.0f);
    score1_1 = glm::scale(score1_1, glm::vec3(0.3, 0.05f, 0.05f));
    score1_1 = glm::translate(score1_1, glm::vec3(-24.75, 60, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score1_1));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_1.middle == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_1.middle == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score1_2 = glm::mat4(1.0f);
    score1_2 = glm::scale(score1_2, glm::vec3(0.3, 0.05f, 0.05f));
    score1_2 = glm::translate(score1_2, glm::vec3(-24.75, 50, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score1_2));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_1.bottom == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_1.bottom == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score1_3 = glm::mat4(1.0f);
    score1_3 = glm::scale(score1_3, glm::vec3(0.3, 0.05f, 0.05f));
    score1_3 = glm::translate(score1_3, glm::vec3(-24.75, 40, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score1_3));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_1.right_up == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_1.right_up == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score1_4 = glm::mat4(1.0f);
    score1_4 = glm::scale(score1_4, glm::vec3(0.05, 0.3, 0.05f));
    score1_4 = glm::translate(score1_4, glm::vec3(-170 + 16, 9.1, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score1_4));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_1.right_down == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_1.right_down == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score1_5 = glm::mat4(1.0f);
    score1_5 = glm::scale(score1_5, glm::vec3(0.05, 0.3, 0.05f));
    score1_5 = glm::translate(score1_5, glm::vec3(-170 + 16, 7.5, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score1_5));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_1.left_up == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_1.left_up == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score1_6 = glm::mat4(1.0f);
    score1_6 = glm::scale(score1_6, glm::vec3(0.05, 0.3, 0.05f));
    score1_6 = glm::translate(score1_6, glm::vec3(-158 + 16, 9.1, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score1_6));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_1.left_down == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_1.left_down == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score1_7 = glm::mat4(1.0f);
    score1_7 = glm::scale(score1_7, glm::vec3(0.05, 0.3, 0.05f));
    score1_7 = glm::translate(score1_7, glm::vec3(-158 + 16, 7.5, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score1_7));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    //------
    if (Score_2.top == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_2.top == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score2_1 = glm::mat4(1.0f);
    score2_1 = glm::scale(score2_1, glm::vec3(0.3, 0.05f, 0.05f));
    score2_1 = glm::translate(score2_1, glm::vec3(-27.3, 60, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score2_1));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_2.middle == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_2.middle == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score2_2 = glm::mat4(1.0f);
    score2_2 = glm::scale(score2_2, glm::vec3(0.3, 0.05f, 0.05f));
    score2_2 = glm::translate(score2_2, glm::vec3(-27.3, 50, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score2_2));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_2.bottom == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_2.bottom == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score2_3 = glm::mat4(1.0f);
    score2_3 = glm::scale(score2_3, glm::vec3(0.3, 0.05f, 0.05f));
    score2_3 = glm::translate(score2_3, glm::vec3(-27.3, 40, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score2_3));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_2.right_up == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_2.right_up == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score2_4 = glm::mat4(1.0f);
    score2_4 = glm::scale(score2_4, glm::vec3(0.05, 0.3, 0.05f));
    score2_4 = glm::translate(score2_4, glm::vec3(-170, 9.1, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score2_4));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_2.right_down == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_2.right_down == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score2_5 = glm::mat4(1.0f);
    score2_5 = glm::scale(score2_5, glm::vec3(0.05, 0.3, 0.05f));
    score2_5 = glm::translate(score2_5, glm::vec3(-170, 7.5, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score2_5));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_2.left_up == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_2.left_up == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score2_6 = glm::mat4(1.0f);
    score2_6 = glm::scale(score2_6, glm::vec3(0.05, 0.3, 0.05f));
    score2_6 = glm::translate(score2_6, glm::vec3(-158, 9.1, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score2_6));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_2.left_down == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_2.left_down == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score2_7 = glm::mat4(1.0f);
    score2_7 = glm::scale(score2_7, glm::vec3(0.05, 0.3, 0.05f));
    score2_7 = glm::translate(score2_7, glm::vec3(-158, 7.5, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score2_7));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    //-----
    if (Score_3.top == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_3.top == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score3_1 = glm::mat4(1.0f);
    score3_1 = glm::scale(score3_1, glm::vec3(0.3, 0.05f, 0.05f));
    score3_1 = glm::translate(score3_1, glm::vec3(-27.3 - 2.55, 60, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score3_1));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_3.middle == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_3.middle == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score3_2 = glm::mat4(1.0f);
    score3_2 = glm::scale(score3_2, glm::vec3(0.3, 0.05f, 0.05f));
    score3_2 = glm::translate(score3_2, glm::vec3(-27.3 - 2.55, 50, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score3_2));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_3.bottom == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_3.bottom == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score3_3 = glm::mat4(1.0f);
    score3_3 = glm::scale(score3_3, glm::vec3(0.3, 0.05f, 0.05f));
    score3_3 = glm::translate(score3_3, glm::vec3(-27.3 - 2.55, 40, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score3_3));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_3.right_up == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_3.right_up == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score3_4 = glm::mat4(1.0f);
    score3_4 = glm::scale(score3_4, glm::vec3(0.05, 0.3, 0.05f));
    score3_4 = glm::translate(score3_4, glm::vec3(-170 - 16, 9.1, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score3_4));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_3.right_down == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_3.right_down == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score3_5 = glm::mat4(1.0f);
    score3_5 = glm::scale(score3_5, glm::vec3(0.05, 0.3, 0.05f));
    score3_5 = glm::translate(score3_5, glm::vec3(-170 - 16, 7.5, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score3_5));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_3.left_up == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_3.left_up == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score3_6 = glm::mat4(1.0f);
    score3_6 = glm::scale(score3_6, glm::vec3(0.05, 0.3, 0.05f));
    score3_6 = glm::translate(score3_6, glm::vec3(-158 - 16, 9.1, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score3_6));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    if (Score_3.left_down == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (Score_3.left_down == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }

    glm::mat4 score3_7 = glm::mat4(1.0f);
    score3_7 = glm::scale(score3_7, glm::vec3(0.05, 0.3, 0.05f));
    score3_7 = glm::translate(score3_7, glm::vec3(-158 - 16, 7.5, 219));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(score3_7));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);



    //신호등

    glm::vec3 beacon_color = glm::vec3(0.1, 0.1, 0.1);
    GLuint objColorLocation_beacon = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_beacon, 1, (float*)&beacon_color);

    glm::mat4 beacon = glm::mat4(1.0f);
    beacon = glm::scale(beacon, glm::vec3(0.5f, 0.8f, 1.6f));
    beacon = glm::translate(beacon, glm::vec3(-7, 5.0f, 1.55));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(beacon));
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    //초록불
    glBindVertexArray(vao[2]);
    if (beacon_G_time == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);

    }
    if (beacon_G_time == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }
    glm::vec3 beacon_g_color = glm::vec3(0.0f, 0.5f, 0.0f);
    GLuint objColorLocation_beacon_G = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_beacon_G, 1, (float*)&beacon_g_color);

    glm::mat4 beacon_1 = glm::mat4(1.0f);
    beacon_1 = glm::scale(beacon_1, glm::vec3(0.05f, 0.05f, 0.05f));
    beacon_1 = glm::translate(beacon_1, glm::vec3(-60, 80, 30));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(beacon_1));
    glDrawElements(GL_TRIANGLES, indexCount_3, GL_UNSIGNED_INT, 0);

    //노란불
    if (beacon_Y_time == true) {

        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (beacon_Y_time == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }
    glm::vec3 beacon_y_color = glm::vec3(0.5f, 0.5f, 0.0f);
    GLuint objColorLocation_beacon_Y = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_beacon_Y, 1, (float*)&beacon_y_color);

    glm::mat4 beacon_2 = glm::mat4(1.0f);
    beacon_2 = glm::scale(beacon_2, glm::vec3(0.05f, 0.05f, 0.05f));
    beacon_2 = glm::translate(beacon_2, glm::vec3(-60, 80, 50));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(beacon_2));
    glDrawElements(GL_TRIANGLES, indexCount_3, GL_UNSIGNED_INT, 0);

    //빨간불
    if (beacon_R_time == true) {
        glUniform3fv(lightAmbientLocation_lamp, 1, (float*)&lightAmbient_lamp);
    }
    if (beacon_R_time == false) {
        glUniform3fv(lightAmbientLocation, 1, (float*)&lightAmbient);
    }
    glm::vec3 beacon_r_color = glm::vec3(0.5f, 0.0f, 0.0f);
    GLuint objColorLocation_beacon_R = glGetUniformLocation(s_program, "g_objectColor");
    glUniform3fv(objColorLocation_beacon_R, 1, (float*)&beacon_r_color);

    glm::mat4 beacon_3 = glm::mat4(1.0f);
    beacon_3 = glm::scale(beacon_3, glm::vec3(0.05f, 0.05f, 0.05f));
    beacon_3 = glm::translate(beacon_3, glm::vec3(-60, 80, 70));
    glUniformMatrix4fv(modelTransformLocation, 1, GL_FALSE, glm::value_ptr(beacon_3));
    glDrawElements(GL_TRIANGLES, indexCount_3, GL_UNSIGNED_INT, 0);

    glutSwapBuffers();                        // 화면에 출력하기

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

GLvoid TimerFunction(int value)
{
    if (start_game)
    {
        sound(L"sound/방향키안내.wav");
        start_game = false;
    }
    switch (value)
    {
    case 1:
        if (Gear.drive && is_crush == false) {
            car_tx += (0.01f + acl_pedal) * handle_dx;
            car_tz -= (0.01f + acl_pedal) * handle_dz;
            tire_rz -= (10.0f + acl_pedal * 100);
        }
        if (Gear.reverse && is_crush == false) {
            car_tx -= (0.01f + acl_pedal) * handle_dx;
            car_tz += (0.01f + acl_pedal) * handle_dz;
            tire_rz += (10.0f + acl_pedal * 100);
        }

        if (is_acl_pedal == false) {
            acl_pedal -= 0.001f;
            if (acl_pedal <= 0) {
                acl_pedal += 0.001f;
            }
        }
        if (car_tx > 11) {
            crush_left();
            is_crush = true;
            sound(L"sound/충돌.mp3");
        }
        if (car_tx < -8) {
            crush_right();
            is_crush = true;
            sound(L"sound/충돌.mp3");
        }
        if (car_tz > 1.5) {
            crush_down();
            is_crush = true;
            sound(L"sound/충돌.mp3");
        }
        if (car_tz + 8.0f < -10.5f) {
            crush_up();
            is_crush = true;
            sound(L"sound/충돌.mp3");
        }
        if (on_collision)
            glutTimerFunc(100, TimerFunction, 7);
        //충동체크 x가 bottom ~ top  z가 right ~ left
        if (-5.4 - BIAUS < car_tx - 1.5f && car_tx - 1.5f < 6.5 + BIAUS && car_tz + 8.0f > 5.5f - BIAUS && car_tz + 8.0f < 6.5f + BIAUS) {
            crush(-5.4 - BIAUS, 6.5 + BIAUS, 6.5f + BIAUS, 5.5f - BIAUS);
            sound(L"sound/충돌.mp3");
        }
        for (int i = 1; i < 9; i++) {
            if (a[i].bottom - BIAUS < car_tx - 1.5f && car_tx - 1.5f < a[i].top + BIAUS && car_tz + 8.0f > a[i].right - BIAUS && car_tz + 8.0f < a[i].left + BIAUS) {
                crush(a[i].bottom - BIAUS, a[i].top + BIAUS, a[i].left + BIAUS, a[i].right - BIAUS);
                sound(L"sound/충돌.mp3");
                i++;
            }
        }
        //T자 성공 여부
        if (-5.0f < car_tx - 1.5f && car_tx - 1.5f < -4.0f && -4.0f < car_tz + 8.0f && car_tz + 8.0f < -2.0f) {
            is_T_PK = true;
        }
        else {
            is_T_PK = false;
        }
        if (Gear.parking && is_T_PK && !is_T_claer) {
            is_T_claer = true;
            sound(L"sound/t자통과.wav");
        }

        glutTimerFunc(20, TimerFunction, 1);
        break;

    case 2:
        if (indicator_lamp_l_time)
            indicator_lamp_l = !indicator_lamp_l;

        if (indicator_lamp_r_time)
            indicator_lamp_r = !indicator_lamp_r;
        glutTimerFunc(500, TimerFunction, 2);
        break;

    case 3://체점
        if (7 < car_tx - 1.5f && car_tx - 1.5f < 11 && 5.9 < car_tz + 8.0f && car_tz + 8.0f < 6.1 && !indicator_lamp_l_time && !check_count[0]) {
            sound(L"sound/방향지시등감점.wav");
            Score_m(5);
            check_count[0] = true;
        }
        if (-3.0f < car_tx - 1.5f && car_tx - 1.5f < -1.0f && 0.5f < car_tz + 8.0f && car_tz + 8.0f < 5.5f && !indicator_lamp_r_time && !check_count[1]) {
            Score_m(5);
            sound(L"sound/방향지시등감점.wav");
            check_count[1] = true;
        }
        else if (-3.0f < car_tx - 1.5f && car_tx - 1.5f < -1.0f && 0.5f < car_tz + 8.0f && car_tz + 8.0f < 5.5f && indicator_lamp_r_time && !check_count[1]) {
            check_count[1] = true;
        }
        if (1 < car_tx - 1.5f && car_tx - 1.5f < 3.0f && 0.5f < car_tz + 8.0f && car_tz + 8.0f < 5.5f && !check_count[2] && beacon_R_time == true) {
            sound(L"sound/신호위반감점.wav");
            Score_m(20);
            check_count[2] = true;
        }
        if (-5.5f < car_tx - 1.5f && car_tx - 1.5f < -5.0f && 0.5f < car_tz + 8.0f && car_tz + 8.0f < 5.5f && !is_T_claer && !check_count[3]) {
            sound(L"sound/t자감점.wav");
            Score_m(25);
            check_count[3] = true;
        }

        if (3.5f < car_tx - 1.5f && car_tx - 1.5f < 3.7f && -11.0f < car_tz + 8.0f && car_tz + 8.0f < -8.0f && acl_pedal < 0.06 && !check_count[4]) {
            sound(L"sound/가속구간감점.wav");
            Score_m(10);
            check_count[4] = true;
            check_count[2] = false;
        }
        if (-11 < car_tx - 1.5f && car_tx - 1.5f < -7 && 0.3 < (float)car_tz + 8.0f && (float)car_tz + 8.0f < 0.5 && !indicator_lamp_r_time && !check_count[5]) {
            sound(L"sound/방향지시등감점.wav");
            Score_m(5);
            check_count[5] = true;
        }
        if (-7.1 < (float)car_tx - 1.5f && (float)car_tx - 1.5f < -6.8f && -11.0f < car_tz + 8.0f && car_tz + 8.0f < -8.0f && !indicator_lamp_r_time && !check_count[6]) {
            sound(L"sound/방향지시등감점.wav");
            Score_m(5);
            check_count[6] = true;
        }
        if (6 < car_tx - 1.5f && car_tx - 1.5f < 7 && -3.5f < car_tz + 8.0f && car_tz + 8.0f < -0.5f && !check_count[7]) {
            is_all_claer = true;
            check_count[7] = true;
        }
        if (6 < car_tx - 1.5f && car_tx - 1.5f < 7 && -3.5f < car_tz + 8.0f && car_tz + 8.0f < -0.5f && !check_count[7]) {
            is_all_claer = true;
            check_count[7] = true;
        }
        if (-11 < car_tx - 1.5f && (float)car_tx - 1.5f < -5.5 && 6.0f < car_tz + 8.0f && car_tz + 8.0f < 11.0f && !check_count[8] && Gear.parking && is_all_claer == true) {

            if (score >= 70) {
                sound(L"sound/합격.wav");

            }
            else
            {
                sound(L"sound/불합격.wav");
            }
            check_count[8] = true;
        }
        glutTimerFunc(20, TimerFunction, 3);
        break;
    case 4:
        beacon_G_time = true;
        beacon_R_time = false;
        beacon_Y_time = false;
        glutTimerFunc(9000, TimerFunction, 5);
        break;
    case 5:
        beacon_Y_time = true;
        beacon_G_time = false;
        beacon_R_time = false;
        glutTimerFunc(1000, TimerFunction, 6);
        break;
    case 6:
        beacon_R_time = true;
        beacon_Y_time = false;
        beacon_G_time = false;
        glutTimerFunc(5000, TimerFunction, 4);
        break;
    case 7: //층돌시 카메라 흔들림  
        if (on_collision == true)
        {
            if (see_degree_speed <= 0.0f)
            {
                see_degree_speed = 2.0f;
                see_degree = 8.0f;
                count_see_degree = 1;
                on_collision = false;
            }
            if (count_see_degree == 1)
            {
                see_degree += see_degree_speed;
                see_degree_speed -= 0.5f;
                if (see_degree > 8.0f)
                    count_see_degree = -1;
            }
            else if (count_see_degree == -1)
            {
                see_degree -= see_degree_speed;
                see_degree_speed -= 0.5f;
                if (see_degree < 4.0f)
                    count_see_degree = 1;
            }
        }
        else
        {
            see_degree_speed = 2.0f;
            see_degree = 8.0f;
            count_see_degree = 1;
        }

        glutTimerFunc(100, TimerFunction, 7);

        break;
    }
    glutPostRedisplay();

}

void crush(int left, int right, int top, int bottom) {
    int mx = (left + right) / 2;
    int mz = (top + bottom) / 2;
    //아박
    if (left < car_tx - 1.5f && car_tx - 1.5f < right && car_tz + 8.0f < mz) {
        crush_down();
        is_crush = true;
    }
    //위박
    if (left < car_tx - 1.5f && car_tx - 1.5f < right && car_tz + 8.0f > mz) {
        crush_up();
        is_crush = true;
    }
    //우박
    if (bottom < car_tz + 8.0f && car_tz + 8.0f < top && car_tx - 1.5f > mx) {
        crush_right();
        is_crush = true;
    }
    //좌박
    if (bottom < car_tz + 8.0f && car_tz + 8.0f < top && car_tx - 1.5f < mx) {
        crush_left();
        is_crush = true;
    }
}

void crush_up() {

    if (Gear.drive) {
        car_tx -= (0.01f + acl_pedal) * handle_dx;
        car_tz += (0.01f + acl_pedal) * handle_dz + 0.2f;
        tire_rz += (10.0f + acl_pedal * 100);
    }
    if (Gear.reverse) {
        car_tx += (0.01f + acl_pedal) * handle_dx;
        car_tz -= (0.01f + acl_pedal) * handle_dz - 0.2f;
        tire_rz -= (10.0f + acl_pedal * 100);
    }
    

    on_collision = true;
    Score_m(5);

}
void crush_down() {

    if (Gear.drive) {
        car_tx -= (0.01f + acl_pedal) * handle_dx;
        car_tz += (0.01f + acl_pedal) * handle_dz - 0.2f;
        tire_rz += (10.0f + acl_pedal * 100);
    }
    if (Gear.reverse) {
        car_tx += (0.01f + acl_pedal) * handle_dx;
        car_tz -= (0.01f + acl_pedal) * handle_dz + 0.2f;
        tire_rz -= (10.0f + acl_pedal * 100);
    }

    on_collision = true;
   
    Score_m(5);

}
void crush_left() {

    if (Gear.drive) {
        car_tx -= (0.01f + acl_pedal) * handle_dx + 0.2f;
        car_tz += (0.01f + acl_pedal) * handle_dz;
        tire_rz += (10.0f + acl_pedal * 100);
    }
    if (Gear.reverse) {
        car_tx += (0.01f + acl_pedal) * handle_dx - 0.2f;
        car_tz -= (0.01f + acl_pedal) * handle_dz;
        tire_rz -= (10.0f + acl_pedal * 100);
    }

    on_collision = true;
    
    Score_m(5);

}
void crush_right() {

    if (Gear.drive) {
        car_tx -= (0.01f + acl_pedal) * handle_dx - 0.2f;
        car_tz += (0.01f + acl_pedal) * handle_dz;
        tire_rz += (10.0f + acl_pedal * 100);
    }
    if (Gear.reverse) {
        car_tx += (0.01f + acl_pedal) * handle_dx + 0.2f;
        car_tz -= (0.01f + acl_pedal) * handle_dz;
        tire_rz -= (10.0f + acl_pedal * 100);
    }

    on_collision = true;
   
    Score_m(5);

}


GLvoid keyUp(unsigned char key, int x, int y) {
    switch (key)
    {
    case'i':
        is_acl_pedal = false;
        break;
    case 'u':
        acl_pedal = 0;
        is_brake = false;
        break;
    }
    glutPostRedisplay();
}

void Score_m(int n) {
    score -= n;

    printf("-%d감점\n", n);
    printf("현재 점수:%d\n", score);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '0':
        degree_cameraRotate = 0.0f;
        degree_lightPos = 0.0f;
        lrx = 0.0f;
        L_Color_R = 1.0f, L_Color_G = 1.0f, L_Color_B = 1.0f, cz = 20.0f, cy = 10.0f, test_x = 0.0f, test_y = 10.0f, test_z = 0.0f, acl_pedal = 0.0f, handle_ry = 0.0f, handle_dx = 1.0f, handle_dz = 0.0f;
        car_tx = 0.0f, car_tz = 0.0f, cx = 0.0f, tire_rz = 0.0f;
        score = 100;

        on_focusBackPoint = false;
        on_focusSelfPoint = false;
        on_collision = false;
        is_acl_pedal = false, is_brake = false, indicator_lamp_l = false, indicator_lamp_r = false, indicator_lamp_l_time = false, indicator_lamp_r_time = false;
        beacon_G_time = false, beacon_Y_time = false, beacon_R_time = false, is_T_PK = false, is_T_claer = false;
        is_all_claer = false, is_finish = false;
        start_game = false;
        count_see_degree = 1;
        see_degree_speed = 2.0f;
        see_degree = 8.0f;
        i = 0;
        Gear.drive = false;
        Gear.parking = true;
        Gear.reverse = false;
        for (int i = 0; i < 10; i++)
        {
            check_count[i] = false;
        }
        break;
    case '1':
        on_focusSelfPoint = true;
        break;
    case '3':
        on_focusSelfPoint = false;
        break;
    case 'a':
        degree_cameraRotate -= 2.0f;

        break;
    case 'd':
        degree_cameraRotate += 2.0f;

        break;
    case 'q'://드라이브
        if (is_brake == true) {
            Gear.drive = true;
            Gear.reverse = false;
            Gear.parking = false;
        }
        on_focusBackPoint = false;
        break;
    case 'w'://후진 기어
        if (is_brake == true) {
            Gear.drive = false;
            Gear.reverse = true;
            Gear.parking = false;
            on_focusBackPoint = true;
        }

        break;
    case 'e'://파킹 키어
        if (is_brake == true) {
            Gear.drive = false;
            Gear.reverse = false;
            Gear.parking = true;
        }

        break;
    case 'u'://브레이크 패달
        is_brake = true;
        acl_pedal -= 0.002f;
        if (acl_pedal <= -0.012f) {
            acl_pedal += 0.002f;
            tire_rz = 0;
        }
        break;
    case 'b':
        sound(L"sound/자동차경적.mp3");
        break;
    case 'i'://가속 패달
        is_acl_pedal = true;
        acl_pedal += 0.002f;
        is_crush = false;
        break;
    case 'j'://좌측 핸들
        handle_ry += 1.0f;
        handle_dx = cos(handle_ry / 180 * PIE);
        handle_dz = sin(handle_ry / 180 * PIE);
        break;
    case 'k'://우측핸들
        handle_ry -= 1.0f;
        handle_dx = cos(handle_ry / 180 * PIE);
        handle_dz = sin(handle_ry / 180 * PIE);
        break;
    case 'h'://좌측 깜빡이
        indicator_lamp_l_time = !indicator_lamp_l_time;
        indicator_lamp_r_time = false;
        indicator_lamp_l = false;
        indicator_lamp_r = false;
        if (indicator_lamp_l_time || indicator_lamp_r_time)
            sound(L"sound/방향지시등소리.mp3");
        break;
    case 'l'://우측 깜빡이
        indicator_lamp_r_time = !indicator_lamp_r_time;
        indicator_lamp_l_time = false;
        indicator_lamp_r = false;
        indicator_lamp_l = false;
        if (indicator_lamp_l_time || indicator_lamp_r_time)
            sound(L"sound/방향지시등소리.mp3");
        break;
    case 'y':
        sound(L"sound/방향키안내.wav");
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

char* filetobuf(const char* file)
{
    FILE* fptr;
    long length;
    char* buf;
    fopen_s(&fptr, file, "rb"); // Open file for reading
    if (!fptr) // Return NULL on failure
        return NULL;
    fseek(fptr, 0, SEEK_END); // Seek to the end of the file
    length = ftell(fptr); // Find out how many bytes into the file we are
    buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
    fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
    fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
    fclose(fptr); // Close the file
    buf[length] = 0; // Null terminator
    return buf; // Return the buffer
}


void make_vertexShaders()
{
    char* vertexsource = filetobuf("vertex.glsl");

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, 0);
    glCompileShader(vertexShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        cerr << "ERROR: vertex shader error\n" << errorLog << endl;
    }
}

void make_fragmentShaders()
{
    char* fragmentsource = filetobuf("fragment.glsl");

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, 0);
    glCompileShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        cerr << "ERROR: fragment shader error\n" << errorLog << endl;
    }
}

void InitBuffer()
{
    glGenVertexArrays(3, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(3, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, vPosData, GL_STATIC_DRAW);
    int posLocation = glGetAttribLocation(s_program, "in_position");
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
    glEnableVertexAttribArray(posLocation);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, vNormalData, GL_STATIC_DRAW);
    int normalLocation = glGetAttribLocation(s_program, "in_normal");
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
    glEnableVertexAttribArray(normalLocation);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 2, vTextureCoordinateData, GL_STATIC_DRAW);
    int uvLocation = glGetAttribLocation(s_program, "in_uv");
    glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
    glEnableVertexAttribArray(uvLocation);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), indexData, GL_STATIC_DRAW);

    glBindVertexArray(vao[1]);
    glGenBuffers(3, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount_2 * sizeof(float) * 3, vPosData_2, GL_STATIC_DRAW);
    int posLocation_2 = glGetAttribLocation(s_program, "in_position");
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
    glEnableVertexAttribArray(posLocation);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount_2 * sizeof(float) * 3, vNormalData_2, GL_STATIC_DRAW);
    int normalLocation_2 = glGetAttribLocation(s_program, "in_normal");
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
    glEnableVertexAttribArray(normalLocation);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount_2 * sizeof(float) * 2, vTextureCoordinateData_2, GL_STATIC_DRAW);
    int uvLocation_2 = glGetAttribLocation(s_program, "in_uv");
    glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
    glEnableVertexAttribArray(uvLocation);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount_2 * sizeof(int), indexData_2, GL_STATIC_DRAW);

    glBindVertexArray(vao[2]);
    glGenBuffers(3, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount_3 * sizeof(float) * 3, vPosData_3, GL_STATIC_DRAW);
    int posLocation_3 = glGetAttribLocation(s_program, "in_position");
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
    glEnableVertexAttribArray(posLocation);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount_3 * sizeof(float) * 3, vNormalData_3, GL_STATIC_DRAW);
    int normalLocation_3 = glGetAttribLocation(s_program, "in_normal");
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
    glEnableVertexAttribArray(normalLocation);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount_3 * sizeof(float) * 2, vTextureCoordinateData_3, GL_STATIC_DRAW);
    int uvLocation_3 = glGetAttribLocation(s_program, "in_uv");
    glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
    glEnableVertexAttribArray(uvLocation);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount_3 * sizeof(int), indexData_3, GL_STATIC_DRAW);
}

void InitShader()
{
    make_vertexShaders();
    make_fragmentShaders();

    s_program = glCreateProgram();

    glAttachShader(s_program, vertexShader);
    glAttachShader(s_program, fragmentShader);
    glLinkProgram(s_program);

    //checkCompileErrors(s_program, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(s_program);
}

bool check_collision(float x1, float z1, float w1, float h1, float x2, float z2, float w2, float h2)
{
    bool vert_collision = false;
    bool horiz_collision = false;

    if (x1 - w1 < x2 + w2 && x1 + w1 > x2 - w2)//w
        horiz_collision = true;
    if (z1 - h1 < z2 + h2 && z1 + h1 > z2 - h2)//h
        vert_collision = true;
    if (horiz_collision && vert_collision)
        return true;

    return false;
}

void set_score_light(SCORE& s, int score)
{
    if (score == 0)
    {
        s.top = true;
        s.middle = false;
        s.bottom = true;
        s.right_up = true;
        s.right_down = true;
        s.left_up = true;
        s.left_down = true;
        return;
    }
    else if (score == 1)
    {
        s.top = false;
        s.middle = false;
        s.bottom = false;
        s.right_up = true;
        s.right_down = true;
        s.left_up = false;
        s.left_down = false;
        return;
    }
    else if (score == 2)
    {
        s.top = true;
        s.middle = true;
        s.bottom = true;
        s.right_up = true;
        s.right_down = false;
        s.left_up = false;
        s.left_down = true;
        return;
    }
    else if (score == 3)
    {
        s.top = true;
        s.middle = true;
        s.bottom = true;
        s.right_up = true;
        s.right_down = true;
        s.left_up = false;
        s.left_down = false;
        return;
    }
    else if (score == 4)
    {
        s.top = false;
        s.middle = true;
        s.bottom = false;
        s.right_up = true;
        s.right_down = true;
        s.left_up = true;
        s.left_down = false;
        return;
    }
    else if (score == 5)
    {
        s.top = true;
        s.middle = true;
        s.bottom = true;
        s.right_up = false;
        s.right_down = true;
        s.left_up = true;
        s.left_down = false;
        return;
    }
    else if (score == 6)
    {
        s.top = true;
        s.middle = true;
        s.bottom = true;
        s.right_up = false;
        s.right_down = true;
        s.left_up = true;
        s.left_down = true;
        return;
    }
    else if (score == 7)
    {
        s.top = true;
        s.middle = false;
        s.bottom = false;
        s.right_up = true;
        s.right_down = true;
        s.left_up = false;
        s.left_down = false;
        return;
    }
    else if (score == 8)
    {
        s.top = true;
        s.middle = true;
        s.bottom = true;
        s.right_up = true;
        s.right_down = true;
        s.left_up = true;
        s.left_down = true;
        return;
    }
    else if (score == 9)
    {
        s.top = true;
        s.middle = true;
        s.bottom = false;
        s.right_up = true;
        s.right_down = true;
        s.left_up = true;
        s.left_down = false;
        return;
    }
    return;
}
void obj_init()
{
    float obj_size = 20;
    a[1].top = -4.4;
    a[1].left = 10.4;
    a[1].bottom = -5.4;
    a[1].right = 5.5;


    a[2].top = 11;
    a[2].left = -3.5;
    a[2].bottom = 6;
    a[2].right = -4.5;


    a[3].top = 8;
    a[3].left = 0.5;
    a[3].bottom = 0;
    a[3].right = -0.5;

    a[4].top = -3;
    a[4].left = 0.5;
    a[4].bottom = -7;
    a[4].right = -1.5;


    a[5].top = -6;
    a[5].left = -1.5;
    a[5].bottom = -7;
    a[5].right = -8;


    a[6].top = -3;
    a[6].left = -4.5;
    a[6].bottom = -6;
    a[6].right = -8;


    a[7].top = 7;
    a[7].left = -7.0;
    a[7].bottom = -3;
    a[7].right = -8;


    a[8].top = 2;
    a[8].left = -0.5;
    a[8].bottom = 0;
    a[8].right = -7;
}
void sound(LPCWSTR sound) {

    IGraphBuilder* pGraph = NULL;
    IMediaControl* pControl = NULL;
    IMediaEvent* pEvent = NULL;
    // Initialize the COM library.
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        printf("ERROR - Could not initialize COM library"); return;
    }
    // Create the filter graph manager and query for interfaces. 
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
        IID_IGraphBuilder, (void**)&pGraph);
    if (FAILED(hr)) {
        printf("ERROR - Could not create the Filter Graph Manager."); return;
    }
    hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
    hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);
    // Build the graph. IMPORTANT: Change this string to a file on your system. 
    hr = pGraph->RenderFile(sound, NULL);
    if (SUCCEEDED(hr)) {
        // Run the graph.
        hr = pControl->Run();
        if (SUCCEEDED(hr)) {
            long evCode;
            //      pEvent->WaitForCompletion(INFINITE, &evCode);
                  // Note: Do not use INFINITE in a real application, because it 
                  // can block indefinitely. 
        }
    }
    pControl->Release();
    //   pEvent->Release(); 
    pGraph->Release();
    CoUninitialize();

}