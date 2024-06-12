typedef struct { // Estrutura para representar um inimigo
    float x, y; // Posi��o do objeto
    int active;	
    GLuint texture;
} gameObject;

gameObject bullet;
gameObject* enemies;

float enemyx = 5, //posi��o do inimigo em x
 enemyy = 0, //posi��o do inimigo em y
 shipx = 0, //posi��o da nave em x
 shipy = 0,	//posi��o da nave em y
 bulletx = 0, //posi��o do proj�til em x
 bullety = 0, //posi��o do proj�til em y
 passo = 1, //quantidade que o inimigo se move em y toda vez que chega a um dos limites da tela
 enemySpacing = 7, //espa�o entre os inimigos
 enemySize = 5, //tamanho dos inimigos
 shipWidth=12.0f, //largura da textura da nave
 shipHeight=12.0f, //altura da textura da nave
 mostLeftEnemyPosition=100, //usado pra verificar quem � o inimigo ativo mais a esquerda
 mostRightEnemyPosition=0; //usado pra verificar quem � o inimigo ativo mais a direita,
 						   // pois � ele que define quando chegamos no limite direito da tela

GLuint shipTexture, //textura da nave
 enemyTexture, //textura dos inimigos
 gameLogoTexture; //logo do jogo

int mostLeftEnemyIndex, //posi��o no vetor do inimigo ativo mais a esquerda
 mostRightEnemyIndex, //posi��o no vetor do inimigo ativo mais a esquerda
 score=0, //usado para guardar a pontua��o do jogador 
 gameState=0, //indica se o jogo est� no menu=0, iniciado=1, ou em pausa=2
 initialTextVisibility=1, //usado para gerar o efeito de texto piscando
 rows = 4, //numero de linhas de inimigos
 cols = 8, //n�mero de colunas de inimigos
 textWidht=40, //largura do texto
 textHeight=20, //altura do texto
 windowWidth=800, //largura inicial da janela
 windowHeight=600, //altura inicial da janela
 isFullScreen=0, //indica se o jogo est� no modo janela=0, ou fullscreen=1
 gameSpeed=528, //velocidade inicial do jogo
 numEnemies = rows * cols, //numero total de inimigos
 enemiesLeft = numEnemies; //numero de inimigos ainda vivos
