#include "main.hpp"
#include "sound.hpp"
// opcje do kompilowania: g++ -lBox2D -lGL -lSDL -lopenal -laudio
// nieużywane -lGLU -lglut

using namespace std;

const int FPS=60;

// Wymiary okna
const int WIDTH=1024;
const int HEIGHT=480;

// Metry na piksele i odwrotnie
const float M2P=50;
const float P2M=1/M2P;

int GLOBAL_TX;
int GLOBAL_TY;

void initView()
{
    //ustawienia projekcji
    glMatrixMode(GL_PROJECTION);
    glOrtho(0,WIDTH,HEIGHT,0,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.2f,0.2f,0.2f,1);
}

void drawSquare(b2Vec2 *points,b2Vec2 center, float angle)
{
    // Kolor rysowanego kwadratu
    glColor3f(1,1,1);
    glPushMatrix();
        glTranslatef(center.x*M2P,center.y*M2P,0);
        glRotatef(angle*180.0/M_PI,0,0,1);
        glBegin(GL_QUADS);
            for(int i=0;i<4;i++)
                glVertex2f(points[i].x*M2P,points[i].y*M2P);
        glEnd();
    glPopMatrix();
}
inline float GetPlayerX()
{
    return actor->GetPosition().x;
}

inline float GetPlayerY()
{
    return actor->GetPosition().y;
}

template <class C>
inline C clamp( C val, C t, C b )
{
    return val < t ? ( val > b ? b : val ) : t;
}

void moveCamera()
{
    static float x = M2P * GetPlayerX();
    //static float y = M2P * GetPlayerY();

    float dx = WIDTH * -0.5f + M2P * GetPlayerX();
    //float dy = HEIGHT * -0.66f + M2P * clamp( GetPlayerY() , -20.0f , -0.1f );

    float l = 0.1f;

    float move_coef = WIDTH * 0.3f;
    float move_barrier = WIDTH * 0.3f;

    if( dx < x - move_barrier )
        x += ( dx - (x - move_coef) ) * l * 4.0f;
    else if( dx > x + move_barrier )
        x += ( dx - (x + move_coef) ) * l * 4.0f;

    GLOBAL_TX = (int)x;

    glTranslatef( -x , 0 , 0.0f );
}





void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();

    moveCamera();

    b2Body* tmp=world->GetBodyList();
    while(tmp)
    {
        b2PolygonShape * shape = (b2PolygonShape*)tmp->GetFixtureList()->GetShape();

        for(int i=0; i<4;i++)
            points[i]=shape->GetVertex(i);
        drawSquare(points,tmp->GetWorldCenter(),tmp->GetAngle());
        tmp=tmp->GetNext();
    }

    glPopMatrix();

}



b2Body* addRect(int x, int y, int w, int h, bool dynamic=true)
{
    b2BodyDef bodydef;
    bodydef.position.Set(x*P2M,y*P2M);
    if(dynamic)
        bodydef.type=b2_dynamicBody;

    b2Body* body=world->CreateBody(&bodydef);

    b2PolygonShape shape;
    shape.SetAsBox(P2M*w/2,P2M*h/2);

    b2FixtureDef fixturedef;
    fixturedef.shape=&shape;
    fixturedef.density=1.0f;
    body->CreateFixture(&fixturedef);

    return 0;
}

void initWorld()
{
    // world creation
    const b2Vec2 gravity(0.0f,8.3f);
    world = new b2World(gravity);
    addRect(WIDTH/2,HEIGHT-50,WIDTH,30,false);
    addRect(WIDTH,HEIGHT-20,WIDTH,30,false);
    addRect(WIDTH*2,HEIGHT-70,WIDTH,30,false);
    initActor();
}

void initActor()
{
    b2BodyDef actorBodyDef;
    actorBodyDef.type = b2_dynamicBody;
    actorBodyDef.position.Set(100*P2M,40*P2M);
    actor = world->CreateBody(&actorBodyDef);

    b2PolygonShape actorBox;
    actorBox.SetAsBox(30*P2M, 60*P2M);

    b2FixtureDef actorFixtureDef;
    actorFixtureDef.shape = &actorBox;
    actorFixtureDef.density = 1.0f;
    actorFixtureDef.friction = 1.1f;
    actorFixtureDef.restitution = 0.8f;

    actor->CreateFixture(&actorFixtureDef);
}


int main ()
{
    // initialize SDL video
    SDL_Init(SDL_INIT_VIDEO);
    // make sure SDL cleans up before exit
    atexit(SDL_Quit);
    // create a new window
    SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_OPENGL);
    //Tytuł okna bez ikony
    SDL_WM_SetCaption("Gra SDL", NULL);

    initWorld();
    initView();
    sound::init();

    // program main loop
    bool exit = false;
    while (!exit)
    {
        start=SDL_GetTicks();
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                    // exit if the window is closed
                case SDL_QUIT:
                {
                    exit = true;
                    break;
                }
                // check for keypresses
                case SDL_KEYDOWN:
                    {
                        // exit if ESCAPE is pressed
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            exit = true;
                            break;
                        }
                        if (event.key.keysym.sym == SDLK_LEFT)
                        {
                            actor->ApplyTorque(-2500,1);
                            break;
                        }

                        if (event.key.keysym.sym == SDLK_r)
                        {
                            delete world;
                            world = NULL;
                            initWorld();
                        }


                        if(event.key.keysym.sym == SDLK_SPACE)
                        {
                            float impulse = actor->GetMass() * 5;
                            actor->ApplyLinearImpulse(b2Vec2(10,-impulse), actor->GetWorldCenter(), true);
                            sound::play(3);
                        }

                    }
                case SDL_MOUSEBUTTONDOWN:
                {
                    if(event.button.button==SDL_BUTTON_LEFT)
                    {
                        addRect(event.button.x+GLOBAL_TX,event.button.y+GLOBAL_TY,10,10,true);
                        //play();
                        break;
                    }
                    if(event.button.button==SDL_BUTTON_RIGHT)
                    {
                        addRect(event.button.x+GLOBAL_TX,event.button.y+GLOBAL_TY,60,30,true);
                        //play();
                        break;
                    }
                }
            } // end switch
        } // end of message processing

        // DRAWING STARTS HERE

        display();

        // DRAWING ENDS HERE

        // update the b2 engine
        world->Step(1.0/60.0,8,3);

        SDL_GL_SwapBuffers();

        //Ograniczamy FPS do 60
        if(1000.0/60>SDL_GetTicks()-start)
            SDL_Delay(1000.0/60-(SDL_GetTicks()-start));

    } // end main loop
    // all is well ;)
    sound::clean();
    printf("Exited cleanly\n");
    cout << '\a';
    return 0;
}
