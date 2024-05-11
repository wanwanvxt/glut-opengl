#include <cmath>
#include <cstdio>
#include <functional>
#include <iostream>
#include <map>
#include "glut.h"

#define PI 3.14159265358979323846
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// vector 3
struct Vec3 {
 public:
  float x = 0, y = 0, z = 0;
  Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

// mau sac
struct Color {
 public:
  float r = 1, g = 1, b = 1, a = 1;
  Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
  Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
};

#pragma region camera
// khoang cach tu camera den trục y
float disCamToY = 80;

// goc cui/ngua
float viewAngle = 0;

// goc nhin voi truc x
float angleH = PI / 4;

// vi tri đat camera
Vec3 camPos = Vec3(disCamToY * cosf(angleH), 20, disCamToY* sinf(angleH));

// di chuyen trai phai
void moveLR(float _offsetAngleH) {
  angleH += _offsetAngleH;

  camPos.x = disCamToY * cosf(angleH);
  camPos.z = disCamToY * sinf(angleH);
}
// di chuyen tien/lui - thu/phong
void moveFB(float _offsetDis) {
  disCamToY += _offsetDis;
  if (disCamToY < 0) {
    disCamToY = 0;
  }

  camPos.x = disCamToY * cosf(angleH);
  camPos.z = disCamToY * sinf(angleH);
}
// di chuyen len/xuong
void moveUD(float _offsetY) {
  camPos.y += _offsetY;
}
// cui/ngua
void rotateUD(float _offsetAngle) {
  viewAngle += _offsetAngle;
  if (viewAngle > PI) {
    viewAngle = PI;
  } else if (viewAngle < -PI) {
    viewAngle = -PI;
  }
}
#pragma endregion

#pragma region anh sang
bool light0 = false;
bool light1 = false;

// nguon sang 0
void setLight0() {
  float lightPos0[] = {0, 20, 0, 1};             // vi tri
  float lightDir0[] = {-1, -1, -1};              // huong chieu
  float lightColor0[] = {1, 1, 0.6, 1};          // mau anh sang
  float lightSpecular0[] = {0.8, 0.8, 0.45, 1};  // mau phan xa
  float lightAmbient0[] = {1, 1, 1, 0};          // mau moi truong
  float lightSpotExp0 = 200;                     // do tap trung
  float lightSpotCutoff0 = 120;                  // goc rong chum sang

  glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDir0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0);
  glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, &lightSpotExp0);
  glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, &lightSpotCutoff0);

  if (light0) {
    glEnable(GL_LIGHT0);
  } else {
    glDisable(GL_LIGHT0);
  }
}

// nguon sang 1
void setLight1() {
  float lightPos1[] = {camPos.x, camPos.y - 1, camPos.z, 1};  // vi tri
  float lightDir1[] = {-camPos.x, camPos.y - 1, -camPos.z};   // huong chieu
  float lightColor1[] = {0.85, 0.9, 0.55, 1};                 // mau anh sang
  float lightSpecular1[] = {0.8, 0.8, 0, 1};                  // mau phan xa
  float lightAmbient1[] = {1, 1, 1, 0};                       // mau moi truong
  float lightSpotExp1 = 200;                                  // do tap trung
  float lightSpotCutoff1 = 30;  // goc rong chum sang

  glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDir1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
  // glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient1);
  glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, &lightSpotExp1);
  glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, &lightSpotCutoff1);

  if (light1) {
    glEnable(GL_LIGHT1);
  } else {
    glDisable(GL_LIGHT1);
  }
}

// mau sac vat lieu
void setMaterial(Color _col, float _shininess) {
  // mau vat lieu
  float materialColor[] = {_col.r, _col.g, _col.b, _col.a};
  // mau phan xa
  float materialSpecular[] = {_col.r * 0.75, _col.g * 0.75, _col.b * 0.75,
                              _col.a * 0.75};
  // do bong
  float materialShininess = _shininess;

  glColor3f(_col.r, _col.g, _col.b);
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
  // glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
  glMaterialfv(GL_FRONT, GL_SHININESS, &materialShininess);
}

//
void setLight() {
  setLight0();
  setLight1();

  glEnable(GL_LIGHTING);
  if (!light0 && !light1) {
    glDisable(GL_LIGHTING);
  }
}

#pragma endregion

#pragma region xyz
// bat/tat truc xyz tung vat the
bool objAxis = false;
bool grid = false;

// truc xyz
void drawOriginAxis() {
  if (glIsEnabled(GL_LIGHTING)) {
    glDisable(GL_LIGHTING);
  }
  glPushMatrix();
  glBegin(GL_LINES);

  // truc x
  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(500, 0, 0);

  // truc y
  glColor3f(0, 1, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 500, 0);

  // truc z
  glColor3f(0, 0, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 500);

  glEnd();
  glPopMatrix();
}

// truc xyz cua vat the
void drawObjAxis(float _objX, float _objY, float _objZ) {
  if (objAxis) {
    glPushMatrix();
    glBegin(GL_LINES);

    // truc x
    glColor3f(1, 0, 0);
    glVertex3f(_objX, _objY, _objZ);
    glVertex3f(_objX + 10, _objY, _objZ);

    // truc y
    glColor3f(0, 1, 0);
    glVertex3f(_objX, _objY, _objZ);
    glVertex3f(_objX, _objY + 10, _objZ);

    // truc z
    glColor3f(0, 0, 1);
    glVertex3f(_objX, _objY, _objZ);
    glVertex3f(_objX, _objY, _objZ + 10);

    glEnd();
    glPopMatrix();
  }
}

// grid
void drawGrid() {
  if (grid) {
    if (glIsEnabled(GL_LIGHTING)) {
      glDisable(GL_LIGHTING);
    }
    glPushMatrix();
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);

    for (float i = -200; i <= 200; i += 2.5) {
      glVertex3f(i, 0, -200);
      glVertex3f(i, 0, i == 0 ? 0 : 200);

      glVertex3f(-200, 0, i);
      glVertex3f(i == 0 ? 0 : 200, 0, i);
    }

    glEnd();
    glPopMatrix();
  }
}
#pragma endregion

#pragma region vat the
void drawObjBegin(Vec3 _pos, Vec3 _scale, Vec3 _rotate, float _rotateAngle) {
  glPushMatrix();
  drawObjAxis(_pos.x, _pos.y, _pos.z);
  glTranslatef(_pos.x, _pos.y, _pos.z);
  glRotatef(_rotateAngle, _rotate.x, _rotate.y, _rotate.z);
  glScalef(_scale.x, _scale.y, _scale.z);
}
//
void drawObjEnd() {
  glPopMatrix();
}

// ve hinh tru/hinh non
void drawCylinder(float _baseRadius,
                  float _topRadius,
                  float _height,
                  float _baseInnerRadius = 0,
                  float _topInnerRadius = 0) {
  GLUquadric* qobj = gluNewQuadric();
  gluCylinder(qobj, _baseRadius, _topRadius, _height, 20, 20);

  gluQuadricOrientation(qobj, GLU_INSIDE);
  gluDisk(qobj, _baseInnerRadius, _baseRadius, 20, 1);

  glTranslatef(0, 0, _height);

  gluQuadricOrientation(qobj, GLU_INSIDE);
  gluDisk(qobj, _topInnerRadius, _topRadius, 20, 1);

  gluDeleteQuadric(qobj);
}

// ve den pin
void drawFlashlight(Vec3 _pos = Vec3(0, 0, 0),
                    Vec3 _scale = Vec3(1, 1, 1),
                    Vec3 _rotate = Vec3(0, 0, 0),
                    float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  // than
  glPushMatrix();
  glTranslatef(0, 0, -1);
  setMaterial(Color(0.05, 0.05, 0.05), 100);
  drawCylinder(0.25, 0.25, 2);
  glPopMatrix();

  // dau
  glPushMatrix();
  glTranslatef(0, 0, 0.5);
  setMaterial(Color(0.05, 0.05, 0.05), 100);
  drawCylinder(0, 0.375, 0.75, 0, 0.375);
  glPopMatrix();

  // vanh dau
  glPushMatrix();
  glTranslatef(0, 0, 1.25);
  setMaterial(Color(0.05, 0.05, 0.05), 100);
  drawCylinder(0.4, 0.4, 0.1, 0, 0.375);
  glPopMatrix();

  drawObjEnd();
}

// ve san
void drawFloor(Vec3 _pos = Vec3(0, 0, 0),
               Vec3 _scale = Vec3(1, 1, 1),
               Vec3 _rotate = Vec3(0, 0, 0),
               float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  glPushMatrix();
  glTranslatef(0, 0, 0);
  glScalef(5, 1, 5);
  setMaterial(Color(0.3, 0.3, 0.3), 100);
  glutSolidCube(1);
  glPopMatrix();

  drawObjEnd();
}

// ve tuong
void drawWall(Vec3 _pos = Vec3(0, 0, 0),
              Vec3 _scale = Vec3(1, 1, 1),
              Vec3 _rotate = Vec3(0, 0, 0),
              float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  glPushMatrix();
  glTranslatef(0, 0, 0);
  glScalef(5, 5, 1);
  setMaterial(Color(0.9, 0.9, 0.9), 100);
  glutSolidCube(1);
  glPopMatrix();

  drawObjEnd();
}

// ve tran
void drawCeil(Vec3 _pos = Vec3(0, 0, 0),
              Vec3 _scale = Vec3(1, 1, 1),
              Vec3 _rotate = Vec3(0, 0, 0),
              float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  glPushMatrix();
  glTranslatef(0, 0, 0);
  glScalef(5, 1, 5);
  setMaterial(Color(0.7, 0.7, 0.7), 100);
  glutSolidCube(1);
  glPopMatrix();

  drawObjEnd();
}

// ve banh xe
void drawWheel(Vec3 _pos = Vec3(0, 0, 0),
               Vec3 _scale = Vec3(1, 1, 1),
               Vec3 _rotate = Vec3(0, 0, 0),
               float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  glPushMatrix();
  glTranslatef(0, 0, -0.5);
  setMaterial(Color(0.8, 0.8, 0.8), 100);
  drawCylinder(1, 1, 1);
  glPopMatrix();

  // lop
  glPushMatrix();
  glTranslatef(0, 0, -0.75);
  setMaterial(Color(0, 0, 0), 100);
  drawCylinder(1, 1, 1.5, 1, 1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(0, 0, -0.75);
  setMaterial(Color(0, 0, 0), 100);
  drawCylinder(2.25, 2.5, 0.25, 1, 0);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(0, 0, 0.5);
  setMaterial(Color(0, 0, 0), 100);
  drawCylinder(2.5, 2.25, 0.25, 0, 1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(0, 0, -0.5);
  setMaterial(Color(0, 0, 0), 100);
  drawCylinder(2.5, 2.5, 1);
  glPopMatrix();

  drawObjEnd();
}

// ve xe
void drawCar(Vec3 _pos = Vec3(0, 0, 0),
             Vec3 _scale = Vec3(1, 1, 1),
             Vec3 _rotate = Vec3(0, 0, 0),
             float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  // than
  glPushMatrix();
  glTranslatef(0, 0, 0);
  setMaterial(Color(0, 0.5, 0.25), 100);
  glBegin(GL_QUADS);
  // duoi
  glVertex3f(5, 0, 10);
  glVertex3f(-5, 0, 10);
  glVertex3f(-5, 0, -10);
  glVertex3f(5, 0, -10);
  // tren
  glVertex3f(5, 4, 9);
  glVertex3f(-5, 4, 9);
  glVertex3f(-5, 4, -9);
  glVertex3f(5, 4, -9);
  // truoc
  glVertex3f(5, 0, 10);
  glVertex3f(-5, 0, 10);
  glVertex3f(-5, 4, 9);
  glVertex3f(5, 4, 9);
  // sau
  glVertex3f(5, 0, -10);
  glVertex3f(-5, 0, -10);
  glVertex3f(-5, 4, -9);
  glVertex3f(5, 4, -9);
  // trai
  glVertex3f(5, 0, 10);
  glVertex3f(5, 4, 9);
  glVertex3f(5, 4, -9);
  glVertex3f(5, 0, -10);
  // phai
  glVertex3f(-5, 0, 10);
  glVertex3f(-5, 4, 9);
  glVertex3f(-5, 4, -9);
  glVertex3f(-5, 0, -10);
  glEnd();
  glPopMatrix();

  // mui
  glPushMatrix();
  glTranslatef(0, 0, 0);
  setMaterial(Color(0, 0.5, 0.25), 100);
  glBegin(GL_QUADS);
  // duoi
  glVertex3f(5, 4, 5);
  glVertex3f(-5, 4, 5);
  glVertex3f(-5, 4, -5);
  glVertex3f(5, 4, -5);
  // tren
  glVertex3f(5, 8, 4.5);
  glVertex3f(-5, 8, 4.5);
  glVertex3f(-5, 8, -4.5);
  glVertex3f(5, 8, -4.5);
  // truoc
  glVertex3f(5, 4, 5);
  glVertex3f(-5, 4, 5);
  glVertex3f(-5, 8, 4.5);
  glVertex3f(5, 8, 4.5);
  // sau
  glVertex3f(5, 4, -5);
  glVertex3f(-5, 4, -5);
  glVertex3f(-5, 8, -4.5);
  glVertex3f(5, 8, -4.5);
  // trai
  glVertex3f(5, 4, 5);
  glVertex3f(5, 8, 4.5);
  glVertex3f(5, 8, -4.5);
  glVertex3f(5, 4, -5);
  // phai
  glVertex3f(-5, 4, 5);
  glVertex3f(-5, 8, 4.5);
  glVertex3f(-5, 8, -4.5);
  glVertex3f(-5, 4, -5);
  glEnd();
  glPopMatrix();

  // banh xe
  glPushMatrix();
  drawWheel(Vec3(5, 0, 6), Vec3(1, 1, 1), Vec3(0, 1, 0), 90);
  glPopMatrix();
  //
  glPushMatrix();
  drawWheel(Vec3(-5, 0, 6), Vec3(1, 1, 1), Vec3(0, 1, 0), 90);
  glPopMatrix();
  //
  glPushMatrix();
  drawWheel(Vec3(-5, 0, -6), Vec3(1, 1, 1), Vec3(0, 1, 0), 90);
  glPopMatrix();
  //
  glPushMatrix();
  drawWheel(Vec3(5, 0, -6), Vec3(1, 1, 1), Vec3(0, 1, 0), 90);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 0.5, 9.5);
  glScalef(10.5, 1, 1);
  setMaterial(Color(0.3, 0.3, 0.3), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(0, 0.5, -9.5);
  glScalef(10.5, 1, 1);
  setMaterial(Color(0.3, 0.3, 0.3), 100);
  glutSolidCube(1);
  glPopMatrix();

  drawObjEnd();
}

// ve may nang xe
void drawCarLift(Vec3 _pos = Vec3(0, 0, 0),
                 Vec3 _scale = Vec3(1, 1, 1),
                 Vec3 _rotate = Vec3(0, 0, 0),
                 float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  // be duoi
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glScalef(3, 0.5, 6);
  setMaterial(Color(1, 0.85, 0), 100);
  glutSolidCube(1);
  glPopMatrix();

  // be tren
  glPushMatrix();
  glTranslatef(0, 3.75, 0);
  glScalef(3, 0.5, 6);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  glutSolidCube(1);
  glPopMatrix();

  // truc duoi
  glPushMatrix();
  glTranslatef(1, 1, 0);
  glRotatef(60, 1, 0, 0);
  glScalef(0.25, 4, 0.25);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(1, 1, 0);
  glRotatef(-60, 1, 0, 0);
  glScalef(0.25, 4, 0.25);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-1, 1, 0);
  glRotatef(60, 1, 0, 0);
  glScalef(0.25, 4, 0.25);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-1, 1, 0);
  glRotatef(-60, 1, 0, 0);
  glScalef(0.25, 4, 0.25);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  glutSolidCube(1);
  glPopMatrix();

  // truc tren
  glPushMatrix();
  glTranslatef(1, 2.75, 0);
  glRotatef(60, 1, 0, 0);
  glScalef(0.25, 4, 0.25);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(1, 2.75, 0);
  glRotatef(-60, 1, 0, 0);
  glScalef(0.25, 4, 0.25);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-1, 2.75, 0);
  glRotatef(60, 1, 0, 0);
  glScalef(0.25, 4, 0.25);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-1, 2.75, 0);
  glRotatef(-60, 1, 0, 0);
  glScalef(0.25, 4, 0.25);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  glutSolidCube(1);
  glPopMatrix();

  drawObjEnd();
}

// ve may 1
void drawMachine1(Vec3 _pos = Vec3(0, 0, 0),
                  Vec3 _scale = Vec3(1, 1, 1),
                  Vec3 _rotate = Vec3(0, 0, 0),
                  float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  // than
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glScalef(2.5, 4, 2.5);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  glutSolidCube(1);
  glPopMatrix();

  // tren
  glPushMatrix();
  glTranslatef(0, 0, 0);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  glBegin(GL_QUADS);
  glVertex3f(1.25, 2, 1.25);
  glVertex3f(-1.25, 2, 1.25);
  glVertex3f(-1.25, 4, -1.25);
  glVertex3f(1.25, 4, -1.25);
  //
  glVertex3f(1.25, 4, -1.25);
  glVertex3f(-1.25, 4, -1.25);
  glVertex3f(-1.25, 2, -1.25);
  glVertex3f(1.25, 2, -1.25);
  glEnd();
  //
  glBegin(GL_TRIANGLES);
  glVertex3f(1.25, 4, -1.25);
  glVertex3f(1.25, 2, -1.25);
  glVertex3f(1.25, 2, 1.25);
  //
  glVertex3f(-1.25, 4, -1.25);
  glVertex3f(-1.25, 2, -1.25);
  glVertex3f(-1.25, 2, 1.25);
  glEnd();
  glPopMatrix();

  // be
  glPushMatrix();
  glTranslatef(0, -2, 0);
  glScalef(3, 0.5, 3);
  setMaterial(Color(0.85, 0.65, 0.15), 100);
  glutSolidCube(1);
  glPopMatrix();

  // nut
  glPushMatrix();
  glTranslatef(0, 3, 0);
  glRotatef(atan2f(2, 2.5) * 180 / PI, 1, 0, 0);
  glScalef(2, 0.25, 2);
  setMaterial(Color(0.2, 0.3, 0.3), 100);
  glutSolidCube(1);
  glPopMatrix();

  drawObjEnd();
}

// ve may 2
void drawMachine2(Vec3 _pos = Vec3(0, 0, 0),
                  Vec3 _scale = Vec3(1, 1, 1),
                  Vec3 _rotate = Vec3(0, 0, 0),
                  float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  // de
  glPushMatrix();
  glTranslatef(0, -2, 0);
  glScalef(3, 0.5, 8.5);
  setMaterial(Color(0.3, 0.12, 0.12), 100);
  glutSolidCube(1);
  glPopMatrix();

  // than
  glPushMatrix();
  glTranslatef(0, 0, -4);
  setMaterial(Color(0.65, 0.15, 0.15), 100);
  drawCylinder(2, 2, 8);
  glPopMatrix();

  ////
  glPushMatrix();
  glTranslatef(0, 2, -2.5);
  glScalef(2, 2, 1);
  setMaterial(Color(1, 1, 0.75), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-1.25, 3, -3);
  glRotatef(90, 0, 1, 0);
  setMaterial(Color(0.8, 0.35, 0.35), 100);
  drawCylinder(0.5, 0.5, 2.5);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(0, 2, 0);
  glScalef(1.5, 1, 5);
  setMaterial(Color(0.75, 0.75, 0.75), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(0, 0, 2.5);
  glRotatef(-90, 1, 0, 0);
  setMaterial(Color(0.8, 0.35, 0.35), 100);
  drawCylinder(0.5, 0.5, 3);
  glPopMatrix();

  drawObjEnd();
}

// ve may 3
void drawMachine3(Vec3 _pos = Vec3(0, 0, 0),
                  Vec3 _scale = Vec3(1, 1, 1),
                  Vec3 _rotate = Vec3(0, 0, 0),
                  float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  // de
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glScalef(4, 0.25, 3);
  setMaterial(Color(0.2, 0.3, 0.3), 100);
  glutSolidCube(1);
  glPopMatrix();

  // binh
  glPushMatrix();
  glTranslatef(1, 0, -0.5);
  glRotatef(-90, 1, 0, 0);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  drawCylinder(0.8, 0.8, 10);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-1, 0, -0.5);
  glRotatef(-90, 1, 0, 0);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  drawCylinder(0.8, 0.8, 10);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(1, 10, -0.5);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  glutSolidSphere(0.8, 20, 20);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-1, 10, -0.5);
  setMaterial(Color(0.25, 0.5, 0.7), 100);
  glutSolidSphere(0.8, 20, 20);
  glPopMatrix();

  // van
  glPushMatrix();
  glTranslatef(1, 10, -0.5);
  glRotatef(-90, 1, 0, 0);
  setMaterial(Color(0.1, 0.1, 0.1), 100);
  drawCylinder(0.125, 0.125, 1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-1, 10, -0.5);
  glRotatef(-90, 1, 0, 0);
  setMaterial(Color(0.1, 0.1, 0.1), 100);
  drawCylinder(0.125, 0.125, 1);
  glPopMatrix();

  // lung
  glPushMatrix();
  glTranslatef(1.75, 5, -1.25);
  glScalef(0.25, 10, 0.25);
  setMaterial(Color(0.2, 0.3, 0.3), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-1.75, 5, -1.25);
  glScalef(0.25, 10, 0.25);
  setMaterial(Color(0.2, 0.3, 0.3), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(0, 5, -1.25);
  glScalef(4, 3, 0.25);
  setMaterial(Color(0.2, 0.3, 0.3), 100);
  glutSolidCube(1);
  glPopMatrix();

  drawObjEnd();
}

// ve gia do banh xe
void drawWheelMount(Vec3 _pos = Vec3(0, 0, 0),
                    Vec3 _scale = Vec3(1, 1, 1),
                    Vec3 _rotate = Vec3(0, 0, 0),
                    float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  // be do
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glScalef(2, 0.25, 20);
  setMaterial(Color(0.8, 0.7, 0.5), 100);
  glutSolidCube(1);
  glPopMatrix();

  // thanh doc
  glPushMatrix();
  glTranslatef(-2.5, 2, 0);
  glScalef(0.25, 0.25, 20);
  setMaterial(Color(0.8, 0.7, 0.5), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(2.5, 2, 0);
  glScalef(0.25, 0.25, 20);
  setMaterial(Color(0.8, 0.7, 0.5), 100);
  glutSolidCube(1);
  glPopMatrix();

  // chan
  glPushMatrix();
  glTranslatef(0.5, 0, 10);
  glRotatef(-45, 0, 0, 1);
  glScalef(0.25, 6, 0.25);
  setMaterial(Color(0.3, 0.25, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-0.5, 0, 10);
  glRotatef(45, 0, 0, 1);
  glScalef(0.25, 6, 0.25);
  setMaterial(Color(0.3, 0.25, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(0.5, 0, -10);
  glRotatef(-45, 0, 0, 1);
  glScalef(0.25, 6, 0.25);
  setMaterial(Color(0.3, 0.25, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-0.5, 0, -10);
  glRotatef(45, 0, 0, 1);
  glScalef(0.25, 6, 0.25);
  setMaterial(Color(0.3, 0.25, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();

  // chan de
  glPushMatrix();
  glTranslatef(0, -2, 10);
  glScalef(5, 0.25, 1);
  setMaterial(Color(0.3, 0.25, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(0, -2, -10);
  glScalef(5, 0.25, 1);
  setMaterial(Color(0.3, 0.25, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();

  // thanh ngang
  glPushMatrix();
  glTranslatef(0, 2, 10);
  glScalef(5, 0.25, 0.25);
  setMaterial(Color(0.8, 0.7, 0.5), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(0, 2, -10);
  glScalef(5, 0.25, 0.25);
  setMaterial(Color(0.8, 0.7, 0.5), 100);
  glutSolidCube(1);
  glPopMatrix();

  // banh xe
  glPushMatrix();
  drawWheel(Vec3(0, 2.5, 9.25));
  glPopMatrix();
  //
  glPushMatrix();
  drawWheel(Vec3(0, 2.5, 7.75));
  glPopMatrix();
  //
  glPushMatrix();
  drawWheel(Vec3(0, 2.5, 6.25));
  glPopMatrix();
  //
  glPushMatrix();
  drawWheel(Vec3(0, 2.5, 4.75));
  glPopMatrix();
  //
  glPushMatrix();
  drawWheel(Vec3(0, 2.5, 3.25));
  glPopMatrix();

  drawObjEnd();
}

// ve ban
void drawTable(Vec3 _pos = Vec3(0, 0, 0),
               Vec3 _scale = Vec3(1, 1, 1),
               Vec3 _rotate = Vec3(0, 0, 0),
               float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  // chan
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glScalef(0.25, 0.25, 5);
  setMaterial(Color(1, 0.85, 0.65), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glScalef(5, 0.25, 0.25);
  setMaterial(Color(1, 0.85, 0.65), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(0, 2, 0);
  glScalef(0.25, 4, 0.25);
  setMaterial(Color(1, 0.85, 0.65), 100);
  glutSolidCube(1);
  glPopMatrix();

  // mat ban
  glPushMatrix();
  glTranslatef(0, 4, 0);
  glRotatef(-90, 1, 0, 0);
  setMaterial(Color(1, 0.4, 0.3), 100);
  drawCylinder(3, 3, 0.25);
  glPopMatrix();

  drawObjEnd();
}

// ve ghe
void drawChair(Vec3 _pos = Vec3(0, 0, 0),
               Vec3 _scale = Vec3(1, 1, 1),
               Vec3 _rotate = Vec3(0, 0, 0),
               float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  // than
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glScalef(3, 3, 3);
  setMaterial(Color(0.5, 0, 0), 100);
  glutSolidCube(1);
  glPopMatrix();

  // chan
  glPushMatrix();
  glTranslatef(1.25, -1.25, 1.25);
  glScalef(0.25, 1, 0.25);
  setMaterial(Color(0.2, 0.2, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(1.25, -1.25, -1.25);
  glScalef(0.25, 1, 0.25);
  setMaterial(Color(0.2, 0.2, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-1.25, -1.25, -1.25);
  glScalef(0.25, 1, 0.25);
  setMaterial(Color(0.2, 0.2, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-1.25, -1.25, 1.25);
  glScalef(0.25, 1, 0.25);
  setMaterial(Color(0.2, 0.2, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();

  drawObjEnd();
}

// ve sofa
void drawSofa(Vec3 _pos = Vec3(0, 0, 0),
              Vec3 _scale = Vec3(1, 1, 1),
              Vec3 _rotate = Vec3(0, 0, 0),
              float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  // than
  glPushMatrix();
  glTranslatef(0, 0, 0);
  glScalef(10, 3, 5);
  setMaterial(Color(0.5, 0, 0), 100);
  glutSolidCube(1);
  glPopMatrix();

  // lung
  glPushMatrix();
  glTranslatef(0, 3.25, -2);
  glScalef(10, 3.5, 1);
  setMaterial(Color(0.6, 0.2, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();

  // ke tay
  glPushMatrix();
  glTranslatef(4.5, 2, 0);
  glScalef(1, 1, 5);
  setMaterial(Color(0.6, 0.2, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-4.5, 2, 0);
  glScalef(1, 1, 5);
  setMaterial(Color(0.6, 0.2, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();

  // chan
  glPushMatrix();
  glTranslatef(4.75, -1.25, 2.25);
  glScalef(0.25, 1, 0.25);
  setMaterial(Color(0.2, 0.2, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(4.75, -1.25, -2.25);
  glScalef(0.25, 1, 0.25);
  setMaterial(Color(0.2, 0.2, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-4.75, -1.25, -2.25);
  glScalef(0.25, 1, 0.25);
  setMaterial(Color(0.2, 0.2, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();
  //
  glPushMatrix();
  glTranslatef(-4.75, -1.25, 2.25);
  glScalef(0.25, 1, 0.25);
  setMaterial(Color(0.2, 0.2, 0.2), 100);
  glutSolidCube(1);
  glPopMatrix();

  drawObjEnd();
}

// ve gara
void drawGarage(Vec3 _pos = Vec3(0, 0, 0),
                Vec3 _scale = Vec3(1, 1, 1),
                Vec3 _rotate = Vec3(0, 0, 0),
                float _rotateAngle = 0) {
  drawObjBegin(_pos, _scale, _rotate, _rotateAngle);

  // san
  drawFloor(Vec3(0, 0, 0), Vec3(10, 1, 15));

  // tuong trai
  drawWall(Vec3(-25, 12, 0), Vec3(15, 5, 1), Vec3(0, 1, 0), 90);

  // tuong phai
  drawWall(Vec3(0, 12, -28.5), Vec3(3.5, 5, 1), Vec3(0, 1, 0), 90);

  // tuong sau
  drawWall(Vec3(0, 12, -37.5), Vec3(10, 5, 1));

  // tran
  drawCeil(Vec3(-12.5, 24, -28.5), Vec3(5, 1, 3.5));

  // xe
  drawCar(Vec3(-12, 3, 25));

  // may nang 1
  drawCarLift(Vec3(10, 0.75, 10));
  // may nang 2
  drawCarLift(Vec3(20, 0.75, 10));

  // may 1
  drawMachine1(Vec3(22, 2.75, 3), Vec3(1, 1, 1), Vec3(0, 1, 0), -90);
  // may 2
  drawMachine2(Vec3(-22, 2.625, 0));
  // may 3
  drawMachine3(Vec3(-22, 0.75, 10), Vec3(1, 1, 1), Vec3(0, 1, 0), 90);
  // may 3
  drawMachine3(Vec3(3, 0.75, -35.5));

  // gia do banh xe
  drawWheelMount(Vec3(22, 2.625, -25));

  // banh xe 1
  drawWheel(Vec3(17.5, 1.375, -10), Vec3(1, 1, 1), Vec3(1, 0, 0), 90);
  // banh xe 2
  drawWheel(Vec3(18, 2.875, -9.5), Vec3(1, 1, 1), Vec3(1, 0, 0), 90);

  // ban
  drawTable(Vec3(-12.5, 0.75, -27));

  // ghe 1
  drawChair(Vec3(-16, 2.25, -22));
  // ghe 2
  drawChair(Vec3(-6, 2.25, -25));
  // sofa
  drawSofa(Vec3(-12.5, 2.25, -34));

  drawObjEnd();
}

// danh muc ban ve
int objIndex = 0;
std::map<int, std::function<void(Vec3, Vec3, Vec3, float)>> objMap = {
    {0, drawGarage},   {1, drawFlashlight}, {2, drawFloor},
    {3, drawWall},     {4, drawCeil},       {5, drawWheel},
    {6, drawCar},      {7, drawCarLift},    {8, drawMachine1},
    {9, drawMachine2}, {10, drawMachine3},  {11, drawWheelMount},
    {12, drawTable},   {13, drawChair},     {14, drawSofa},
};
#pragma endregion

void display() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(camPos.x, camPos.y, camPos.z, 0,
            camPos.y + disCamToY * tanf(viewAngle), 0, 0, 1, 0);

  // ve truc xyz
  drawOriginAxis();
  drawGrid();

  // anh sang
  setLight();

  // ve den pin theo camera
  drawFlashlight(Vec3(camPos.x, camPos.y - 1, camPos.z), Vec3(1, 1, 1),
                 Vec3(0, 1, 0), -(90 + angleH * 180 / PI));

  // ve vat the
  objMap[objIndex](Vec3(0, 0, 0), Vec3(1, 1, 1), Vec3(0, 0, 0), 0);

  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'w':  // tien
      moveFB(-0.25);
      break;
    case 's':  // lui
      moveFB(0.25);
      break;
    case 'a':  // trai
      moveLR(PI / 180.0);
      break;
    case 'd':  // phai
      moveLR(-PI / 180.0);
      break;
    case 32:  // len
      moveUD(0.25);
      break;
    case 'c':  // xuong
      moveUD(-0.25);
      break;
    case 'j':  // cui
      rotateUD(-PI / 180.0);
      break;
    case 'k':  // ngua
      rotateUD(PI / 180.0);
      break;

    case 'g':  // bat/tat grid
      grid = !grid;
      break;
    case 't':  // bat/tat truc xyz cua vat the
      objAxis = !objAxis;
      break;

    case 'l':  // bat/tat den tran
      light0 = !light0;
      break;
    case 'f':  // bat/tat den pin
      light1 = !light1;
      break;

    case 9:  // chuyen doi ban ve
      ++objIndex;
      if (objIndex >= objMap.size()) {
        objIndex = 0;
      }
      break;
  }

  glutPostRedisplay();
}

int main(int argc, char* argv[]) {
  printf("G: hien thi luoi\n");
  printf("T: hien thi truc vat the\n");
  printf("Tab: doi ban ve\n");
  printf("---\n");
  printf("A/D: trai/phai\n");
  printf("W/S: tien/lui\n");
  printf("Space/C: len/xuong\n");
  printf("J/K: cui/ngua\n");
  printf("---\n");
  printf("F: den pin\n");
  printf("L: den tran\n");

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Gara oto");

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glutMainLoop();

  return 0;
}
