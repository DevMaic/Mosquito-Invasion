#include <GL/glut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>
#include "variaveis.h"

int checkCollision(float enemyx, float enemyy, float enemySize, float bulletx, float bullety) {
    return (bulletx < enemyx + enemySize && bulletx + 0.5 > enemyx &&
            bullety < enemyy && bullety + 1 > enemyy - enemySize);
}

void renderEnemies() {
    glPushMatrix();
    glTranslatef(0, 10, 0);
    glTranslatef(enemyx, enemyy, 0);

    glEnable(GL_TEXTURE_2D); // Habilita texturas
    glBindTexture(GL_TEXTURE_2D, enemyTexture); // Vincula a textura dos inimigos

    glBegin(GL_QUADS);
    for (int i = 0; i < numEnemies; i++) {
        if (enemies[i].active == 1) {
            glTexCoord2f(0.0, 0.0); glVertex2f(enemies[i].x, enemies[i].y);
            glTexCoord2f(1.0, 0.0); glVertex2f(enemies[i].x + enemySize, enemies[i].y);
            glTexCoord2f(1.0, 1.0); glVertex2f(enemies[i].x + enemySize, enemies[i].y - enemySize);
            glTexCoord2f(0.0, 1.0); glVertex2f(enemies[i].x, enemies[i].y - enemySize);
        }
    }
    glEnd();

    glDisable(GL_TEXTURE_2D); // Desabilita texturas
    glPopMatrix();
}

void renderBullet() {
    if (!bullet.active) {
        bullet.x = shipx + 50;
        bullet.y = shipy + 95;
        bullet.active = 1;
    }

    glPushMatrix();
    glTranslatef(-0.25, 0, 0);
    if (bullet.active == 1) {
        glBegin(GL_QUADS);
        glVertex2f(bullet.x, bullet.y);
        glVertex2f(bullet.x, bullet.y + 1);
        glVertex2f(bullet.x + 0.5, bullet.y + 1);
        glVertex2f(bullet.x + 0.5, bullet.y);
        glEnd();
    }
    glPopMatrix();
}

// Função para carregar textura usando stb_image
GLuint loadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    
    if (!data) {
        printf("Erro ao carregar a imagem %s\n", filename);
        exit(1);
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texture;
}

void defineMostRightAndMostLeftEnemy() {
	for(int i=0;i<numEnemies;i++) {
		if(enemies[i].active==1) {
			if((enemies[i].x)<mostLeftEnemyPosition) {
				mostLeftEnemyIndex=i%8;
				mostLeftEnemyPosition=enemies[i].x;
			}
			if((enemies[i].x)>mostRightEnemyPosition) {
				mostRightEnemyIndex=i%8;
				mostRightEnemyPosition=enemies[i].x;
			}	
		}	
	}
	
	mostLeftEnemyPosition = 100;
	mostRightEnemyPosition = 0;
}

void initGameState() {
    enemies = (gameObject*)malloc(numEnemies * sizeof(gameObject));
    if (!enemies) {
        printf("Failed to allocate memory for enemies\n");
        exit(1);
    }

    int index = 0;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            enemies[index].x = (enemySpacing * col * 1.5);
            enemies[index].y = (enemySpacing * row * 1.5);
            enemies[index].active = 1;
            index++;
        }
    }
    
    defineMostRightAndMostLeftEnemy();
	
    bullet.x = 0;
    bullet.y = 0;
    bullet.active = 0;
	
    glClearColor(0.0, 0, 0.0, 1.0); // Cor de fundo preta

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 100, 100, 0); // Mapeando as coordenadas da tela para 0 a 100 em X e Y
    glMatrixMode(GL_MODELVIEW);

    // Carregar a textura da nave
    shipTexture = loadTexture("./assets/nave.png");
    if (shipTexture == 0) {
        printf("Erro ao carregar a textura da nave: '%s'\n", stbi_failure_reason());
    }

    // Carregar a textura dos inimigos
    enemyTexture = loadTexture("./assets/enemy.png");
    if (enemyTexture == 0) {
        printf("Erro ao carregar a textura dos inimigos: '%s'\n", stbi_failure_reason());
    }
    
    gameLogoTexture = loadTexture("./assets/gamelogo.png");
    if (gameLogoTexture == 0) {
        printf("Erro ao carregar texturas: '%s'\n", stbi_failure_reason());
    }
}

// Função de atualização
void update(int value) {
	if(gameState==1) {
		if (enemyx >= (17+((7-mostRightEnemyIndex)*10)) || enemyx < (5-(mostLeftEnemyIndex*10))) {
	        passo *= -1;
	        enemyy += 2;
	        defineMostRightAndMostLeftEnemy();
	        gameSpeed-=15;
    	}
	    
	    enemyx += passo;
	} else {
		if(gameState==0 || gameState ==2) {
			initialTextVisibility=!initialTextVisibility;
		}
	}
    
    glutPostRedisplay(); // Marca a janela atual como necessitando de re-desenho
    glutTimerFunc(gameSpeed, update, 0); // Define o próximo callback de atualização em 16ms (~60 FPS)
}

// Função de entrada (teclado)
void handleKeys(unsigned char key, int x, int y) {
    if (key == 32) { // ESC para sair
        if(gameState==0)
			gameState=1;
    }
    if(key == 27) {
    	if(gameState==1)
			gameState=2;
		else
			if(gameState==2)
				gameState=1;
	}
}

void generateStrokeCharacter(char* text, float x, float y, float scale) {
	glPushMatrix();
				
	float textWidth = 0;
    for (const char* p = text; *p; p++) {
        textWidth += glutStrokeWidth(GLUT_STROKE_ROMAN, *p);
    }

    // Centraliza o texto e aplica a transformação para inverter o eixo y
    glPushMatrix();
    glTranslatef(x - textWidth * scale / 2, y, 0);
    glScalef(scale, -scale, scale);  // Inverte o eixo y

    // Desenha cada caractere
    for (const char* p = text; *p; p++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    }

    glPopMatrix();
}

void renderText() {
	if(gameState==1) {
		char scoreText[20];
	    
	    glColor3f(1.0, 1.0, 1.0); // Configurar a cor do texto (RGB)
	    glRasterPos2f(5, 3); // Configurar a posição do texto (x, y, z)    
	    snprintf(scoreText, sizeof(scoreText), "Score: %d", score); // Converter a pontuação para string
	    
	    generateStrokeCharacter(scoreText,10,3, 0.02);
	} else {
		if(gameState==0) {
			if(initialTextVisibility) {
				char scoreText[50];
		    
			    snprintf(scoreText, sizeof(scoreText), "Pressione ESPACO para jogar"); // Converter a pontuação para string
				
				generateStrokeCharacter(scoreText, 50, 60, 0.02);
			}
		} else {
			if(initialTextVisibility) {
				char scoreText[50];
		    
			    glColor3f(1.0, 1.0, 1.0); // Configurar a cor do texto (RGB)
			    glRasterPos2f(40, 60); // Configurar a posição do texto (x, y, z)    
			    snprintf(scoreText, sizeof(scoreText), "Pausado"); // Converter a pontuação para string
			
				generateStrokeCharacter(scoreText, 50, 50, 0.02);
			}	
		}
	}	
}

// Função de renderização
void display() {
	glLoadIdentity();
	glColor3f(1.0f, 1.0f, 1.0f); // Define a cor para branco para que a textura não seja alterada
    glClear(GL_COLOR_BUFFER_BIT);
	renderText();
	
	if(gameState==1) {
	    renderEnemies();
	
	    glPushMatrix();
	    glTranslatef(shipx, 7.5, 0);
	
	    glEnable(GL_TEXTURE_2D); // Habilita texturas
	    glBindTexture(GL_TEXTURE_2D, shipTexture); // Vincula a textura da nave
	
	    glBegin(GL_QUADS);
	    glTexCoord2f(0.0, 0.0); glVertex2f(50 - shipWidth / 2, 90 + shipHeight / 2);
	    glTexCoord2f(1.0, 0.0); glVertex2f(50 + shipWidth / 2, 90 + shipHeight / 2);
	    glTexCoord2f(1.0, 1.0); glVertex2f(50 + shipWidth / 2, 90 - shipHeight / 2);
	    glTexCoord2f(0.0, 1.0); glVertex2f(50 - shipWidth / 2, 90 - shipHeight / 2);
	    glEnd();
	
	    glDisable(GL_TEXTURE_2D); // Desabilita texturas
	    glPopMatrix();
	
	    renderBullet();
	} else {
		if(gameState==0) {
			glEnable(GL_TEXTURE_2D); // Habilita texturas
		    glBindTexture(GL_TEXTURE_2D, gameLogoTexture); // Vincula a textura da nave
		
		    glBegin(GL_QUADS);
			    glTexCoord2f(1.0, 0.0); glVertex2f(50 - 40 / 2, 45 + 20 / 2);
			    glTexCoord2f(0.0, 0.0); glVertex2f(50 + 40 / 2, 45 + 20 / 2);
			    glTexCoord2f(0.0, 1.0); glVertex2f(50 + 40 / 2, 45 - 20 / 2);
			    glTexCoord2f(1.0, 1.0); glVertex2f(50 - 40 / 2, 45 - 20 / 2);
		    glEnd();
		
		    glDisable(GL_TEXTURE_2D);  
		}
	}
	
	glutSwapBuffers();
}

void updateShip(int value) {
	if(gameState==1) {
		if (bullet.active)
        	bullet.y -= 0.75;

	    if (bullet.y <= 0) {
	        bullet.active = 0;
	        bullet.y = shipy;
	    }
	     
	    for (int i = 0; i < numEnemies; i++) {
	        if (enemies[i].active) {
	            if (checkCollision(enemies[i].x + enemyx, enemies[i].y + enemyy, enemySize, bullet.x, bullet.y)) { // Se colisão detectada
	                enemies[i].active = 0;
	                bullet.active = 0;
	                score+=5;
	                enemiesLeft--;
	            }
	        }
	    }
	}
	
	if(enemiesLeft==0) {
		initGameState();
	}
	
    // Marca a janela atual como necessitando de re-desenho
    glutPostRedisplay();
    glutTimerFunc(16, updateShip, 0); // Define o próximo callback de atualização em 16ms (~60 FPS)
}

void handleSpecialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT)
        shipx += shipx >= 45 ? 0 : 2;
    if (key == GLUT_KEY_LEFT)
        shipx -= shipx <= -45 ? 0 : 2;
    if(key == GLUT_KEY_F11)
    	if(isFullScreen==0) {
    		glutFullScreen();
    		isFullScreen=!isFullScreen;
    	}
    	else {
    		glutReshapeWindow(windowWidth, windowHeight);
    		isFullScreen=!isFullScreen;
    	}
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 100, 100, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Game Loop com GLUT");

    initGameState();
	
	glutReshapeFunc(reshape);
    glutDisplayFunc(display); // Callback de renderização
    glutKeyboardFunc(handleKeys); // Callback de teclado
    glutSpecialFunc(handleSpecialKeys);
    glutTimerFunc(16, update, 0); // Callback de atualização
    glutTimerFunc(16, updateShip, 0);

    glutMainLoop(); // Entra no loop principal do GLUT
    return 0;
}
