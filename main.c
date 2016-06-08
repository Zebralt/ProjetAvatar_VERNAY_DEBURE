/*****************************************************************************
 File: TD3-avatar-skel.c
 
 Informatique Graphique IG1
 IFIPS
 Christian Jacquemin, Université Paris 11
 
 Copyright (C) 2007 University Paris 11
 This file is provided without support, instruction, or implied
 warranty of any kind.  University Paris 11 makes no guarantee of its
 fitness for a particular purpose and is not liable under any
 circumstances for any damages or loss whatsoever arising from the use
 or inability to use this file or items derived from it.
 ******************************************************************************/
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define    windowWidth 800
#define    windowHeight 800

#define RED   0
#define GREEN 0
#define BLUE  0
#define ALPHA 1
#define INCREMENT_ANGLE_ROTATION 90
#define VITESSE_MARCHE 0.04
#define VITESSE_COURSE 0.3

#define true  1
#define false 0

#define KEY_ESPACE 32
#define KEY_ESC 27
#define KEY_L 108
#define KEY_M 109
#define KEY_O 111
#define KEY_P 112
#define KEY_R 114
#define KEY_S 115
#define KEY_W 119
#define KEY_U 117
#define KEY_I 105
#define KEY_J 106
#define KEY_K 107
#define KEY_Y 121
#define KEY_H 104
#define PI 3.1415926535898

float t = 0.f;
float delta = 10.f;
float k = 0.001f;
float K = 0.002f;
int TraceEcran = false;
int RangFichierStockage = 0;
int direction = 1; // 1 vers l'avant, -1 vers l'arrière (après demi-tour)
time_t timerDerniereAction;

int  Ma_Tete;
int  Mon_Oeil;
int  Mon_Nez;
int  Mon_Chapeau;
int  Mon_Tronc;
int  Mon_Tronc_Disque;
int  Mon_Bouton;
int  Mon_Bras;
int  Mon_AvantBras;
int  Ma_Main;
int  Mon_Doigt;
int  Ma_Cuisse;
int  Mon_Mollet;
int  Mon_Pied;
int  Ma_Fleur;
int  Mon_Petale;
int  Ma_BarreChaise;
int  Mon_AssiseChaise;
int  Mon_Repere;

enum lateralite{ Gauche = 0, Droit };

float sin_k_t=0;

float angle_Bras[2];
float angle_AvantBras[2];
float angle_Main[2];
float angle_Cuisse[2];
float angle_Mollet[2];

float angle_Bras_Ini[2] = {-30.0, 15.0};
float angle_AvantBras_Ini[2] = {0.0, 15.0};
float angle_Main_Ini[2] = {0.0, 15.0};
float angle_Cuisse_Ini[2] = {20.0, -20.0};
float angle_Mollet_Ini[2] = {0.0, -20.0};

float amplitude_Bras;
float amplitude_AvantBras;
float amplitude_Main;
float amplitude_Cuisse;
float amplitude_Mollet;

float med_Bras;
float med_AvantBras;
float med_Main;
float med_Cuisse;
float med_Mollet;

static GLfloat light_position0[] = { 15.0 , 15.0 , 15.0 , 0.0 };
static GLfloat light_position1[] = { 15.0 , 15.0 , -15.0 , 0.0 };

static GLfloat ambient_light0[] = { 0.0 , 0.0 , 0.0 , 0.0 };
static GLfloat diffuse_light0[] = { 0.7 , 0.7 , 0.7 , 1.0 };
static GLfloat specular_light0[] = { 0.1 , 0.1 , 0.1 , 0.1 };

static GLfloat ambient_light1[] = { 0.50 , 0.50 , 0.50 , 1.0 };
static GLfloat diffuse_light1[] = { 0.5 , 1.0 , 1.0 , 1.0 };
static GLfloat specular_light1[] = { 0.5 , 1.0 , 1.0 , 1.0 };

int Step = 0;
int latence =4;

enum STATUT_AVATAR {STOP, WALK, RUN, SEAT, TO_STOP};
enum STATUT_AVATAR statut;

enum MODE {AUTO, MANUEL};
enum MODE mode = MANUEL;

int rotationAvatar = 0;

float positionAvatar[2] = {0.0, 0.0};
float positionChaise[2] = {0, -20.0};

// orientation de la scene sous les 3 axes
float orientationScene[3] = {-60.0, 0, -150};

// déclaration des fonctions
GLvoid initGL();
void run();
void walk();
void stop();
void init_scene();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);
void doRandomAction();
GLvoid window_timer();
void Faire_Composantes();
void Dessine_Repere();
void render_scene();

int main(int argc, char **argv)
{
    // initialisation  des paramètres de GLUT en fonction
    // des arguments sur la ligne de commande
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    
    // définition et création de la fenêtre graphique
    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Avatar animé");
    
    // initialisation de OpenGL et de la scène
    initGL();
    init_scene();
    
    // choix des procédures de callback pour
    // le tracé graphique
    glutDisplayFunc(&window_display);
    // le redimensionnement de la fenêtre
    glutReshapeFunc(&window_reshape);
    // la gestion des événements clavier
    glutKeyboardFunc(&window_key);
    // fonction appelée régulièrement entre deux gestions d´événements
    glutTimerFunc(latence,&window_timer,Step);
    
    // la boucle prinicipale de gestion des événements utilisateur
    glutMainLoop();
    
    return 1;
}

// initialisation du fond de la fenêtre graphique : noir opaque

GLvoid initGL()
{
    // initialisation de l´éclairement
    
    // définition de deux source lumineuses
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light0);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light1);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    
    // activation de l´éclairement
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    // propriétés matérielles des objets
    // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambientanddiffuse);
    // glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    // glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glShadeModel( GL_SMOOTH );
    glEnable(GL_COLOR_MATERIAL); // IMPORTANT POUR LES OBJETS ECLAIRES 
    
    // initialisation du fond
    glClearColor(RED, GREEN, BLUE, ALPHA);
    // z-buffer
    glEnable(GL_DEPTH_TEST);
}

//fonction pour permettre à l'avatar de courir
void run()
{
    //si il est assit
    if (statut == SEAT)
    {
        positionAvatar[1] = positionChaise[1] + 6.1;
        direction = 1;
    }
    
    if (statut != RUN)
    {
        if (statut == STOP)
            delta = 10.f;
        
        delta *= 9.01;
        K = 0.003f;
        
        amplitude_Bras
        = 1 * (angle_Bras_Ini[ Droit ] - angle_Bras_Ini[ Gauche ]);
        amplitude_AvantBras
        = 1 * (angle_AvantBras_Ini[ Droit ] - angle_AvantBras_Ini[ Gauche ]);
        amplitude_Main
        = 1 * (angle_Main_Ini[ Droit ] - angle_Main_Ini[ Gauche ]);
        amplitude_Cuisse
        = 1 * (angle_Cuisse_Ini[ Droit ] - angle_Cuisse_Ini[ Gauche ]);
        amplitude_Mollet
        = 1 * (angle_Mollet_Ini[ Droit ] - angle_Mollet_Ini[ Gauche ]);
        med_Bras
        = 1 * (angle_Bras_Ini[ Droit ] + angle_Bras_Ini[ Gauche ]);
        med_AvantBras
        = 1 * (angle_AvantBras_Ini[ Droit ] + angle_AvantBras_Ini[ Gauche ]);
        med_Main
        = 1 * (angle_Main_Ini[ Droit ] + angle_Main_Ini[ Gauche ]);
        med_Cuisse
        = 1 * (angle_Cuisse_Ini[ Droit ] + angle_Cuisse_Ini[ Gauche ]);
        med_Mollet
        = 1 * (angle_Mollet_Ini[ Droit ] + angle_Mollet_Ini[ Gauche ]);
    }
    statut = RUN;
}

//fonction pour permettre à l'avatar de marcher
void walk()
{
    if (statut == SEAT)
    {
        positionAvatar[1] = positionChaise[1] + 6.1;
        direction = 1;
    }
    if ((statut == RUN) || (statut == STOP))
        delta = 10.f;
    statut = WALK;
    
    amplitude_Bras
    = .5 * (angle_Bras_Ini[ Droit ] - angle_Bras_Ini[ Gauche ]);
    amplitude_AvantBras
    = .5 * (angle_AvantBras_Ini[ Droit ] - angle_AvantBras_Ini[ Gauche ]);
    amplitude_Main
    = .5 * (angle_Main_Ini[ Droit ] - angle_Main_Ini[ Gauche ]);
    amplitude_Cuisse
    = .5 * (angle_Cuisse_Ini[ Droit ] - angle_Cuisse_Ini[ Gauche ]);
    amplitude_Mollet
    = .5 * (angle_Mollet_Ini[ Droit ] - angle_Mollet_Ini[ Gauche ]);
    
    med_Bras
    = .5 * (angle_Bras_Ini[ Droit ] + angle_Bras_Ini[ Gauche ]);
    med_AvantBras
    = .5 * (angle_AvantBras_Ini[ Droit ] + angle_AvantBras_Ini[ Gauche ]);
    med_Main
    = .5 * (angle_Main_Ini[ Droit ] + angle_Main_Ini[ Gauche ]);
    med_Cuisse
    = .5 * (angle_Cuisse_Ini[ Droit ] + angle_Cuisse_Ini[ Gauche ]);
    med_Mollet
    = .5 * (angle_Mollet_Ini[ Droit ] + angle_Mollet_Ini[ Gauche ]);
}

void stop()
{
    if (statut == SEAT)
    {
        positionAvatar[1] = positionChaise[1] + 6.1;
        direction = 1;
    }
    statut = TO_STOP;
}

void init_scene()
{
    // initialise des display lists des composantes cylindriques du corps
    Faire_Composantes();
    
    walk();
}

// fonction de call-back pour l´affichage dans la fenêtre

GLvoid window_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    render_scene(); 
    
    // trace la scène grapnique qui vient juste d'être définie
    glFlush();
}

// fonction de call-back pour le redimensionnement de la fenêtre

GLvoid window_reshape(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20, 20, -20, 20, -1000, 1000);
    // glFrustum(-20, 20, -20, 20, 10, 1000);
    // glScalef(10, 10, 10);
    
    // toutes les transformations suivantes s´appliquent au modèle de vue
    glMatrixMode(GL_MODELVIEW);
}

// fonction de call-back pour la gestion des événements clavier

GLvoid window_key(unsigned char key, int x, int y)
{
    switch (key) {
        case KEY_ESC:
            exit(1);
            break;
        case '+':
            delta *= 1.01;
            break;
        case '-':
            delta /= 1.01;
            break;
        case KEY_R:
            if (mode == MANUEL)
                run();
            break;
        case KEY_W:
            if (mode == MANUEL)
                walk();
            break;
        case KEY_S:
            if (mode == MANUEL)
                stop();
            break;
        case KEY_O:
            if (mode == MANUEL)
                direction = 1;
            break;
        case KEY_K:
            if ((mode == MANUEL) && (statut != SEAT))
                rotationAvatar = (rotationAvatar + INCREMENT_ANGLE_ROTATION + 360) % 360;
            break;
        case KEY_M:
            if ((mode == MANUEL) && (statut != SEAT))
                rotationAvatar = (rotationAvatar - INCREMENT_ANGLE_ROTATION + 360) % 360;
            break;
        case KEY_L:
            if (mode == MANUEL)
                direction = -1;
            break;
        case KEY_Y:
            orientationScene[0] = orientationScene[0] + 5;
            break;
        case KEY_U:
            orientationScene[0] = orientationScene[0] - 5;
            break;
        case KEY_H:
            orientationScene[2] = orientationScene[2] + 5;
            break;
        case KEY_J:
            orientationScene[2] = orientationScene[2] - 5;
            break;
        case KEY_ESPACE:
            if (mode == AUTO)
                mode = MANUEL;
            else
            {
                mode = AUTO;
                doRandomAction();
            }
            break;
        default:
            printf ("La touche %d n´est pas active.\n", key);
            break;
    }
}

//fonction qui donne un comportement aléatoire à l'avatar en mode automatique
void doRandomAction()
{
    timerDerniereAction = time(NULL);
    switch(rand()%6)
    {
        case 0:
            run();
            break;
        case 1:
            walk();
            break;
        case 2:
            stop();
            break;
        case 3:
            direction = direction * -1;
            break;
        case 4:
            if (statut != SEAT)
                rotationAvatar = (rotationAvatar + INCREMENT_ANGLE_ROTATION + 360) % 360;
            break;
        case 5:
            if (statut != SEAT)
                rotationAvatar = (rotationAvatar - INCREMENT_ANGLE_ROTATION + 360) % 360;
            break;
        default:
            break;
    }
}

// fonction de call-back appelée régulièrement

GLvoid window_timer()
{
    float vitesse;
    if (mode == AUTO)
        if (time(NULL)-timerDerniereAction > 3) // toutes les 5s
            doRandomAction();
    // demi-tour si trop près du bord
    if ((positionAvatar[0] <= -40)
        || (positionAvatar[0] >= 40)
        || (positionAvatar[1] <= -40)
        || (positionAvatar[1] >= 40))
        rotationAvatar = (rotationAvatar + 180) % 360;
    // attendre que l'avatar soit en position droite pour le considérer comme arrêté
    if ((statut == TO_STOP) && (fabs(sin_k_t) < 0.1))
    {
        statut = STOP;
        delta=0;
    }
    if ((statut != STOP) && (statut != SEAT))
    {
        if (((direction == 1) && (statut != TO_STOP))
            || ((direction == -1) && (statut == TO_STOP)))
            t += delta;
        else
            t -= delta;
        sin_k_t = sin(k*t);
    
        angle_Bras[Gauche] = med_Bras + amplitude_Bras*sin_k_t;
        angle_Bras[Droit] = med_Bras - amplitude_Bras*sin_k_t;
        angle_AvantBras[Gauche] = med_AvantBras + amplitude_AvantBras*sin_k_t;
        angle_AvantBras[Droit] = med_AvantBras - amplitude_AvantBras*sin_k_t;
        angle_Main[Gauche] = med_Main + amplitude_Main*sin_k_t;
        angle_Main[Droit] = med_Main - amplitude_Main*sin_k_t;
    
        angle_Cuisse[Gauche] = med_Cuisse + amplitude_Cuisse*sin_k_t;
        angle_Cuisse[Droit] = med_Cuisse - amplitude_Cuisse*sin_k_t;
        angle_Mollet[Gauche] = med_Mollet + amplitude_Mollet*sin_k_t;
        angle_Mollet[Droit] = med_Mollet - amplitude_Mollet*sin_k_t;
    
        // On déplace la position de l'avatar en fonction de sa vitesse et de son angle de rotation
        if (statut == RUN)
            vitesse = VITESSE_COURSE;
        else
            vitesse = VITESSE_MARCHE;
        if (rotationAvatar == 0)
            positionAvatar[1] = (positionAvatar[1]+vitesse * direction);
        else if (rotationAvatar == 90)
             positionAvatar[0] = (positionAvatar[0]-vitesse * direction);
        else if (rotationAvatar == 180)
             positionAvatar[1] = (positionAvatar[1]-vitesse * direction);
        else if (rotationAvatar == 270)
             positionAvatar[0] = (positionAvatar[0]+vitesse * direction);
        else
            printf("\nRotation non gérée : %d", rotationAvatar);
        
        // teste de la proximité avec la chaise
        if ((positionAvatar[0] >= (positionChaise[0]-6))
             && (positionAvatar[0] <= (positionChaise[0]+6))
             && (positionAvatar[1] >= (positionChaise[1]-6))
             && (positionAvatar[1] <= (positionChaise[1]+6)))
        {
            positionAvatar[0] = positionChaise[0];
            positionAvatar[1] = positionChaise[1];
            rotationAvatar = 0;
            statut = SEAT;
        }
    }
    glutTimerFunc(latence,&window_timer,++Step);
    
    glutPostRedisplay();
}

void Faire_Composantes() {
    GLUquadricObj* qobj;
    
    // attribution des indentificateurs de display lists
    Ma_Tete =  glGenLists(6);
    Mon_Tronc = Ma_Tete + 1;
    Mon_Bras = Ma_Tete + 2;
    Mon_AvantBras = Ma_Tete + 3;
    Ma_Cuisse = Ma_Tete + 4;
    Mon_Mollet = Ma_Tete + 5;
    Mon_Chapeau = Ma_Tete + 6;
    Mon_Oeil = Ma_Tete + 7;
    Mon_Nez = Ma_Tete + 8;
    Ma_Main = Ma_Tete + 9;
    Mon_Pied = Ma_Tete + 10;
    Mon_Bouton = Ma_Tete + 11;
    Mon_Doigt = Ma_Tete + 12;
    Ma_Fleur = Ma_Tete + 13;
    Mon_Petale = Ma_Tete + 14;
    Ma_BarreChaise = Ma_Tete + 15;
    Mon_AssiseChaise = Ma_Tete + 16;
    Mon_Tronc_Disque = Ma_Tete + 17;
    
    // compilation de la display list des sphères
    glNewList(Ma_Tete, GL_COMPILE);
    glutSolidSphere(1.5, 20, 20);
    glEndList();
    
    glNewList(Mon_Oeil, GL_COMPILE);
    glutSolidSphere(0.15, 20, 20);
    glEndList();
    
    glNewList(Mon_Nez, GL_COMPILE);
    glutSolidSphere(0.3, 20, 20);
    glEndList();
    
    glNewList(Ma_Main, GL_COMPILE);
    glutSolidSphere(0.5, 20, 20);
    glEndList();
    
    glNewList(Mon_Pied, GL_COMPILE);
    glutSolidSphere(0.5, 20, 20);
    glEndList();
    
    glNewList(Mon_Bouton, GL_COMPILE);
    glutSolidSphere(0.3, 20, 20);
    glEndList();
    
    glNewList(Mon_Doigt, GL_COMPILE);
    glutSolidSphere(0.5, 20, 20);
    glEndList();
    
    glNewList(Ma_Fleur, GL_COMPILE);
    glutSolidSphere(0.35, 20, 20);
    glEndList();
    
    glNewList(Mon_Petale, GL_COMPILE);
    glutSolidSphere(0.3, 20, 20);
    glEndList();
    
    glNewList(Mon_AssiseChaise, GL_COMPILE);
    glutSolidCube(5);
    glEndList();
    
    // allocation d´une description de quadrique
    qobj = gluNewQuadric();
    // la quadrique est pleine
    gluQuadricDrawStyle(qobj, GLU_FILL);
    // les ombrages, s´il y en a, sont doux
    gluQuadricNormals(qobj, GLU_SMOOTH);
    
    // compilation des display lists des cylindres
    glNewList(Mon_Tronc, GL_COMPILE);
    gluCylinder(qobj, 2.5, 2.5, 7.0, 20, 20);
    glEndList();
    
    // compilation des display lists des cylindres
    glNewList(Mon_Tronc_Disque, GL_COMPILE);
    gluDisk(qobj, 0.0, 2.5, 20, 20);
    glEndList();
    
    glNewList(Mon_Bras, GL_COMPILE);
    gluCylinder(qobj, 0.5, 0.5, 5.0, 20, 20);
    glEndList();
    
    glNewList(Mon_AvantBras, GL_COMPILE);
    gluCylinder(qobj, 0.5, 0.25, 5.0, 20, 20);
    glEndList();
    
    glNewList(Ma_Cuisse, GL_COMPILE);
    gluCylinder(qobj, 1.25, 0.75, 5.0, 20, 20);
    glEndList();
    
    glNewList(Mon_Mollet, GL_COMPILE);
    gluCylinder(qobj, 0.75, 0.25, 5.0, 20, 20);
    glEndList();
    
    glNewList(Mon_Chapeau, GL_COMPILE);
    gluCylinder(qobj, 1.0, 0, 3, 20, 20);
    glEndList();
    
    glNewList(Ma_BarreChaise, GL_COMPILE);
    gluCylinder(qobj, 0.4, 0.5, 5, 20, 20);
    glEndList();
}

void  Dessine_Repere() {
    glNewList(Mon_Repere, GL_COMPILE);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-10 , 0 , 0);
    glVertex3f(10 , 0 , 0);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0 , -10 , 0);
    glVertex3f(0 , 10 , 0);
    glEnd();
    glPointSize( 10.0 );
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(10.0 , 0 , 0);
    glEnd();
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0 , 10.0 , 0);
    glEnd();
    glEndList();
}

void render_scene()
{
    float facteurAmplitude;
    float variationTemps;
    long i;
    i = (clock()/5000);
    if ((i%2) == 1)
        variationTemps = (i%10);
    else
        variationTemps = 10-(i%10);
    
    // rotation de la scene suivant les 3 axes
    glRotatef(orientationScene[0], 1, 0, 0);
    glRotatef(orientationScene[1], 0, 1, 0);
    glRotatef(orientationScene[2], 0, 0, 1);
    
    // chaise, position fixe
    glPushMatrix();
        glTranslatef(positionChaise[0]-2.5, positionChaise[1], -10);
    
        glColor3f(0.5, 0.4, 0.0);
        glCallList(Ma_BarreChaise);
    
        glPushMatrix();
            glTranslatef(5, 0, 0);
            glCallList(Ma_BarreChaise);
    
            glPushMatrix();
                glTranslatef(0, -5, 0);
                glCallList(Ma_BarreChaise);
    
                glPushMatrix();
                    glTranslatef(-5, 0, 0);
                    glCallList(Ma_BarreChaise);
    
                    glPushMatrix(); // siege
                        glTranslatef(2.5, 2.5, 5);
                        glScalef(1.0, 1.0, 0.2);
                        glCallList(Mon_AssiseChaise);
                        glScalef(1.0, 1.0, 5.0);
    
                        glPushMatrix(); // dossier
                            glTranslatef(0.0, -2.5, 2.5);
                            glScalef(1.0, 0.2, 1.0);
                            glCallList(Mon_AssiseChaise);
                            glScalef(1.0, 5.0, 1.0);
    
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();

    // Avatar
    // déplacement horizontal selon l´axe Oy pour donner
    // une impression de déplacement horizontal accompagnant
    // la marche
    glTranslatef(positionAvatar[0], positionAvatar[1],  0);
    glRotatef(rotationAvatar, 0, 0, 1);
    
    // tracé du tronc, aucune transformation n´est
    // requise
    if (statut == SEAT)
        glTranslatef(0.0, -2.0, -3.4);
    
    glColor3f(1.0, 1.0, 1.0);
    glCallList(Mon_Tronc);
    glCallList(Mon_Tronc_Disque);
    glTranslatef(0.0, 0.0, 7);
    glCallList(Mon_Tronc_Disque);
    glTranslatef(0.0, 0.0, -7);
    
    glPushMatrix();
        glTranslatef(0, 2.6, 6);
        glColor3f(0.4, 0, 0.6);
        glCallList(Mon_Bouton);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0, 2.6, 4);
        glCallList(Mon_Bouton);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0, 2.6, 2);
        glCallList(Mon_Bouton);
    glPopMatrix();
    
    // tracé de la tête avec une translation positive
    // selon Oz pour la placer au-dessus du tronc
    // les appels à glPushMatrix et glPopMatrix servent
    // à sauvegarder et restaurer le contexte graphique
    glPushMatrix();
        glTranslatef(0, 0, 8.5);
        glColor3f(0.8, 0.6, 0.6);
        glCallList(Ma_Tete);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0, 0, 9.5);
        if (mode==MANUEL)
            glColor3f(0.4, 0, 0.6);
        else
            glColor3f(0.2, 0.2, 0.8);
        glCallList(Mon_Chapeau);
    
        glPushMatrix();
            glTranslatef(0, 0.5, 1.4);
            glColor3f(1.0, 1.0, 0);
            glCallList(Ma_Fleur);
    
            glPushMatrix();
                glTranslatef(0.3, 0, 0);
                glColor3f(0.8, 0.3, 0.5);
                glCallList(Mon_Petale);
            glPopMatrix();
    
            glPushMatrix();
                glTranslatef(-0.3, 0, 0);
                glColor3f(0.8, 0.3, 0.5);
                glCallList(Mon_Petale);
            glPopMatrix();
    
            glPushMatrix();
                glTranslatef(0, 0, 0.35);
                glColor3f(0.8, 0.3, 0.5);
                glCallList(Mon_Petale);
            glPopMatrix();
    
            glPushMatrix();
                glTranslatef(0, 0.1, -0.4);
                glColor3f(0.8, 0.3, 0.5);
                glCallList(Mon_Petale);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0.5, 1.4, 9.0);
        glColor3f(0.0, 0.0, 1.0);
        glCallList(Mon_Oeil); //oeil gauche
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(-0.5, 1.4, 9.0);
        glColor3f(0.0, 0.0, 1.0);
        glCallList(Mon_Oeil); //oeil droit
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0, 1.5, 8.3);
        glColor3f(1.0, 0.0, 0.0);
        glCallList(Mon_Nez);
    glPopMatrix();
    
    // tracé de la cuisse droite avec une translation vers
    // la droite et une rotation de 180° autour de Ox
    // pour l´orienter vers le bas
    glPushMatrix();
        glTranslatef(1.25, 0, 0);
        glRotatef(180, 1, 0, 0);
        glColor3f(0.2, 0.2, 0.8);
        if (statut == SEAT)
            glRotatef(90, 1, 0, 0);
        else
            glRotatef(angle_Cuisse[Droit], 1, 0, 0);
    
        glCallList(Ma_Cuisse);  //cuisse droite
    
        glPushMatrix();
            glTranslatef(0, 0, 5);
            glColor3f(0.8, 0.6, 0.6);
            if (statut == SEAT)
                glRotatef(-90 + variationTemps, 1, 0, 0);
            else if (statut == STOP)
                glRotatef(0, 1, 0, 0);
            else
                glRotatef(angle_Mollet[Droit], 1, 0, 0);
            glCallList(Mon_Mollet); //mollet droit
    
            glPushMatrix();
                glTranslatef(0, -0.4, 5);
                glColor3f(1.0, 0.0, 0.0);
                glScaled(1.5, 3.0, 1);
                glCallList(Mon_Pied); //pied droit
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    
    // pour tracer le mollet, on reste dans le même
    // contexte graphique et on translate de
    // +5 selon Oz afin de mettre le repère au niveau
    // du genou
    
    // cuisse et mollet gauches
    // seule la translation initiale change
    
    glPushMatrix();
        glTranslatef(-1.25, 0, 0);
        glRotatef(180, 1, 0, 0);
        glColor3f(0.2, 0.2, 0.8);
        if (statut == SEAT)
            glRotatef(90, 1, 0, 0);
        else
            glRotatef(angle_Cuisse[Gauche], 1, 0, 0);
        glCallList(Ma_Cuisse);  // cuisse gauche
    
        glPushMatrix();
            glTranslatef(0, 0, 5);
            glColor3f(0.8, 0.6, 0.6);
            if (statut == SEAT)
                glRotatef(-90 - variationTemps, 1, 0, 0);
            else if (statut == STOP)
                glRotatef(0, 1, 0, 0);
            else
                glRotatef(angle_Mollet[Gauche], 1, 0, 0);
            glCallList(Mon_Mollet); // mollet gauche
    
            glPushMatrix();
                glTranslatef(0, -0.4, 5);
                glColor3f(1.0, 0.0, 0.0);
                glScaled(1, 2.5, 1);
                glCallList(Mon_Pied); // pied gauche
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    
    // tracé du bras droit avec une translation vers
    // la droite et vers le haut composée avec une 
    // rotation de 180° autour de Ox pour l´orienter
    // vers le bas
    
    // pour tracer l´avant-bras, on reste dans le même
    // contexte graphique et on translate de
    // +5 selon Oz afin de mettre le repère au niveau
    // du coude
    
    glPushMatrix();
        glTranslatef(3, 0, 7);
        glRotatef(180, 1, 0, 0);
        glColor3f(0.4, 0, 0.6);
        if (statut != SEAT)
            glRotatef(angle_Bras[Droit], 1, 0, 0);
        glCallList(Mon_Bras);  //bras droit
    
        glPushMatrix();
            glTranslatef(0, 0, 5);
            glColor3f(0.8, 0.6, 0.6);
            if (statut == SEAT)
                glRotatef(80.0, 1, -0.2, 0.1);
            else
                glRotatef(angle_AvantBras[Droit], 1, 0, 0);
            glCallList(Mon_AvantBras); //avant-bras droit
    
            glPushMatrix();
                glTranslatef(0, 0, 5.375);
                if (statut != SEAT)
                    glRotatef(angle_Main[Droit], 1, 0, 0);
                else
                    glRotatef(variationTemps, 1, 0, 0);
    
                glCallList(Ma_Main); //main droite
    
                glPushMatrix();
                    glTranslatef(0, 0, 0.4);
                    glScaled(0.1, 0.2, 1.8);
                    glCallList(Mon_Doigt); //doigt main droite
                glPopMatrix();
    
                glPushMatrix();
                    glTranslatef(0.2, 0, 0.4);
                    glScaled(0.1, 0.2, 1.5);
                    glCallList(Mon_Doigt); //doigt main droite
                glPopMatrix();
    
                glPushMatrix();
                    glTranslatef(-0.2, 0, 0.4);
                    glScaled(0.1, 0.2, 1.5);
                    glCallList(Mon_Doigt); //doigt main droite
                glPopMatrix();
    
                glPushMatrix();
                    glTranslatef(0.4, 0, 0.4);
                    glScaled(0.1, 0.2, 1);
                    glCallList(Mon_Doigt); //doigt main droite
                glPopMatrix();
    
                glPushMatrix();
                    glTranslatef(-0.4, 0, 0.4);
                    glScaled(0.1, 0.2, 0.8);
                    glCallList(Mon_Doigt); //doigt main droite
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    
    // bras et avant-bras gauches
    // seule la translation initiale change
    
    glPushMatrix();
        glTranslatef(-3, 0, 7);
        glRotatef(180, 1, 0, 0);
        glColor3f(0.4, 0, 0.6);
        if (statut != SEAT)
            glRotatef(angle_Bras[Gauche], 1, 0, 0);
        if (statut == STOP)
            glRotatef(80.0, 1.0, 0.2, 0.0);
        glCallList(Mon_Bras);  //bras gauche
    
        glPushMatrix();
            glTranslatef(0, 0, 5);
            glColor3f(0.8, 0.6, 0.6);
            if (statut == SEAT)
                glRotatef(80.0, 1, 0.2, 0.1);
            else
                glRotatef(angle_AvantBras[Gauche], 1, 0, 0);
            if (statut == STOP)
            {
                glRotatef(100.0 + variationTemps/3, 0.0, 1.0, 0.0);
            //    glRotatef(40.0, 1.0, 0.0, 0.0);
            }
            glCallList(Mon_AvantBras); //avant-bras gauche
    
            glPushMatrix();
                glTranslatef(0, 0, 5.375);
                if (statut != SEAT)
                    glRotatef(angle_Main[Gauche], 1, 0, 0);
                else
                    glRotatef(variationTemps, 1, 0, 0);
                glCallList(Ma_Main); //main gauche
    
                glPushMatrix();
                    glTranslatef(0, 0, 0.4);
                    glScaled(0.1, 0.2, 1.8);
                    glCallList(Mon_Doigt); //doigt main gauche
                glPopMatrix();
    
                glPushMatrix();
                    glTranslatef(0.2, 0, 0.4);
                    glScaled(0.1, 0.2, 1.5);
                    glCallList(Mon_Doigt); //doigt main gauche
                glPopMatrix();
    
                glPushMatrix();
                    glTranslatef(-0.2, 0, 0.4);
                    glScaled(0.1, 0.2, 1.5);
                    glCallList(Mon_Doigt); //doigt main gauche
                glPopMatrix();
    
                glPushMatrix();
                    glTranslatef(0.4, 0, 0.4);
                    glScaled(0.1, 0.2, 0.8);
                    glCallList(Mon_Doigt); //doigt main gauche
                glPopMatrix();
    
                glPushMatrix();
                    glTranslatef(-0.4, 0, 0.4);
                    glScaled(0.1, 0.2, 1);
                    glCallList(Mon_Doigt); //doigt main gauche
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    
    if (statut == SEAT)
        glTranslatef(0.0, 0.0, 4.0);

    // permutation des buffers lorsque le tracé est achevé
    glutSwapBuffers();
}
