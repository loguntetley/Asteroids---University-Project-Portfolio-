
#include "include/freeglut.h"	
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <random>
#include <string>
#define _USE_MATH_DEFINES
#include "math.h"

using namespace std;
    
GLfloat red = 1.0, green = 1.0, blue = 1.0;
GLfloat playerAngle = 90.0f;
GLfloat rotspeed = 5.0f;
int scorePoints = 0;

class Projectile
{
public:
	pair<GLfloat, GLfloat> ProjCoords;
	GLfloat ProjAngle, v;
	Projectile(pair<GLfloat, GLfloat> _ProjCoords, GLfloat _angle)
	{
		ProjCoords = _ProjCoords;
		ProjAngle = _angle;
	}
	void update()
	{
		//to update the vectors co-ordiantes so that the projectile keep advaning along its angle
		ProjCoords.first += cos(3.14/180.0f * ProjAngle)*0.1;
		ProjCoords.second += sin(3.14 / 180.0f *  ProjAngle )*0.1;
	}
	void draw()
	{
		glBegin(GL_POLYGON);
		//create the projectiles shape
		for (int i = 0; i < 3; i++)
		{
			glColor3f(0, 1, 0);
			glVertex2f(
				ProjCoords.first + 2 * cos(3.14f / 180 * (ProjAngle + 360 / 3 * i)),
				ProjCoords.second + 2 * sin(3.14f / 180 * (ProjAngle + 360 / 3 * i)));
		}
			glEnd();
	};
};
	vector<Projectile> TriProjectile;

	class TriPlayer
	{
	public:
		pair<GLint, GLint> playCoords;
		GLfloat ProjAng, triTipX, triTipY, newTipX, newTipY;
		TriPlayer(pair<GLint, GLint> _playCoords)
		{
			playCoords = _playCoords;
		}
		void update()
		{
			//update the current position of the tip of the player
			triTipX = newTipX;
			triTipY = newTipY;
		}
		void draw()
		{
			glBegin(GL_POLYGON);
			for (int i = 0; i < 3; i++)
			{
				if (i == 0) { glColor3f(0.5, 0.5, 0); }
				else { glColor3f(0.5, 0, 0); }
				if (i == 0) 
				{
					//generate the player within the centre of the screen
					glVertex2f(
						playCoords.first + 50 + 10 * cos(3.14f / 180 * (playerAngle + 360 / 3 * i)),
						playCoords.second + 50 + 10 * sin(3.14f / 180 * (playerAngle + 360 / 3 * i)));
					newTipX = playCoords.first + 50 + 10 * cos(3.14f / 180 * (playerAngle + 360 / 3 * i));
					newTipY = playCoords.second + 50 + 10 * sin(3.14f / 180 * (playerAngle + 360 / 3 * i));
				}
				glVertex2f(
					playCoords.first + 50 + 10 * cos(3.14f / 180 * (playerAngle + 360 / 3 * i)),
					playCoords.second + 50 + 10 * sin(3.14f / 180 * (playerAngle + 360 / 3 * i))
				);
			}
			glEnd();
		};
	};
	vector<TriPlayer> playerPosition;

	class Asteroid
	{
	public:
		pair<GLfloat, GLfloat> coords;//centre of asteroid (coords go from 0, 100)
		GLfloat angle, rotspeed, astNum, distance;
		bool alive;
		Asteroid(pair<GLfloat, GLfloat> _coords)
		{
			coords = _coords;
			angle = 0;
			rotspeed = 0.01f;
			alive = true;
			astNum;
		}
		void update(vector<Projectile>& _Plist)
		{
			angle += rotspeed;
			ProjectileChecker(_Plist);
		}
		void draw()
		{
			glBegin(GL_POLYGON);
			//Too draw each point of the asteroid
			for (int i = 0; i < 6; i++)
			{
				glColor3f(0, 0, 1);
				glVertex2f(
					coords.first + 10 * cos(3.14f / 180 * (angle + 360 / 6 * i)),
					coords.second + 10 * sin(3.14f / 180 * (angle + 360 / 6 * i))
				);
			}
			glEnd();
		}
		void ProjectileChecker(vector<Projectile> _TriProjectile)
		{
			for (Projectile & p: _TriProjectile)
			{
				for (int i = 0; i < 5; i++)
				{
					//check if the Projectile is within range of the astroid
					GLfloat x1, x2, y1, y2;
					x2 = p.ProjCoords.first += cos(3.14 / 180.0f * p.ProjAngle);
					y2 = p.ProjCoords.second += sin(3.14 / 180.0f * p.ProjAngle);
					distance = sqrt(pow((x2 - coords.first), 2) + pow((y2 - coords.second), 2));
					if (distance < 10)
					{
						alive = false;
						astNum = i;
					}
					else 
					{
						alive = true;
					}	
				}
			}
		}
	};
	vector<Asteroid> astList;

	void reshape(int width, int height)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, width, height);
		glOrtho(0, 100, 0, 100, -1.0, 1.0);
	}

	void keyboard(unsigned char key, int x, int y)
	{
		switch (key)
		{
		case 'a':
			playerAngle += rotspeed;
			break;
		case 'd':
			playerAngle -= rotspeed;
			break;
		case ' ':
			TriProjectile.push_back(Projectile({ 50,50 }, playerAngle));
			break;
		}
		glutPostRedisplay();
	}

	void astDeleter()
	{
		for (Asteroid& values : astList) 
		{
			for (int i = 0; i < 1; i++)
			{
				if (values.alive == false)
				{
					//an iterator to select which astroid after collison needs to be erased from the vector
					vector<Asteroid>::iterator selecter = astList.begin();
					for (size_t i = 0; i < astList.size(); i++)
					{
						if ((*selecter).astNum == values.astNum)
						{
							break;
						}
						selecter++;
					}
					astList.erase(selecter);
					scorePoints += 1;
					return;
				}
			}
		}
	}

	void scoreDiscplay()
	{
		glColor3f(1, 1, 1);
		glRasterPos2f(65, 90);
		//updates the score value
		string points = "Curret Score: " + to_string(scorePoints);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, reinterpret_cast<const unsigned char*>(points.c_str()));
	}

	void instructions()
	{
		cout << "The aim of this game is to destroy as many asteroids as possible and get the highest score " << endl;
		cout << "Press the 'SPACE' to Shoot a bullet from the tip of the player " << endl;
		cout << "Press the 'A' key to Rotate anti-clockwise and 'D' to Rotate clockwise " << endl;
	}

	void display(void)
	{
		srand(time(NULL));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//create the positons of the player
		if (playerPosition.size() < 1)
		{
			GLfloat playPosOneX = cos(3.14f / 180 * (playerAngle + 360 / 3 * 0));
			GLfloat playPosOneY = sin(3.14f / 180 * (playerAngle + 360 / 3 * 0));
			playerPosition.push_back(TriPlayer({ playPosOneX,playPosOneY }));
		}
		//create the postions for each astroid and place them into the vector
		if (astList.size() < 1) {
			for (int i = 0; i < 5; i++) {
				float xPos, yPos;
				xPos = (rand() % 100);
				yPos = (rand() % 100);
				astList.push_back(Asteroid({ xPos,yPos }));
			}
		}
		for (TriPlayer& p : playerPosition) {
			p.update();
			p.draw();
		}
		for (Asteroid& a : astList) {
			a.update(TriProjectile);
			a.draw();
		}
		for (Projectile& b : TriProjectile)
		{
			b.update();
			b.draw();
		}
		scoreDiscplay();
		astDeleter();
		glutSwapBuffers();
		glutPostRedisplay();
	}

	void init(void){glClearColor(0.0, 0.0, 0.0, 0.0);}

	int main(int argc, char** argv)
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(400, 400);
		glutInitWindowPosition(100, 100);
		glutCreateWindow("Course Work One Astroids");
		init();
		srand(time(NULL));
		instructions();
		glutKeyboardFunc(keyboard);
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutMainLoop();
		return 0;
	}
