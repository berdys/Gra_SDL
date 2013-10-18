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
const float M2P=60;
const float P2M=1/M2P;

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


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    b2Body* tmp=world->GetBodyList();

    while(tmp)
    {
        for(int i=0; i<4;i++)
            points[i]=((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);

        drawSquare(points,tmp->GetWorldCenter(),tmp->GetAngle());
        tmp=tmp->GetNext();
    }
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
    play();

    return 0;
}

void initWorld()
{
    // world creation
    b2Vec2 gravity(0.0f,8.3f);
    world = new b2World(gravity);
    addRect(WIDTH/2,HEIGHT-50,WIDTH,10,false);

}


int main ()
{
    init();
    // initialize SDL video
    SDL_Init(SDL_INIT_VIDEO);

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_OPENGL);

    //Tytuł okna bez ikony
    SDL_WM_SetCaption("Gra SDL", NULL);


    initWorld();
    initView();

    // program main loop
    bool done = false;
    while (!done)
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
                done = true;
                break;
            }
            // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        done = true;
                        break;
                    }

                    if (event.key.keysym.sym == SDLK_r)
                    {
                        delete world;
                        world = NULL;
                        initWorld();
                    }

                }
            case SDL_MOUSEBUTTONDOWN:
            {
                if(event.button.button==SDL_BUTTON_LEFT)
                {
                    addRect(event.button.x,event.button.y,10,10,true);
                    break;
                }
            }
            case SDL_BUTTON_RIGHT:
            {
                if(event.button.button==SDL_BUTTON_RIGHT)
                {
                    addRect(event.button.x,event.button.y,60,30,true);
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

        // finally, update the screen :)
        //SDL_Flip(screen);
        SDL_GL_SwapBuffers();

        //Ograniczamy FPS do 60
        if(1000.0/60>SDL_GetTicks()-start)
            SDL_Delay(1000.0/60-(SDL_GetTicks()-start));

    } // end main loop
    clean();
    // all is well ;)
    printf("Exited cleanly\n");
    cout << '\a';
    return 0;
}
