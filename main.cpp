#include <cstdlib>
#include <iostream>
#include<math.h>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"

using namespace std;
using namespace sf;

const int W = 1200;         // largeur de la fenetre
const int H = 800;          // hauteur de la fenetre
const int rayon = 60;       // vaisseau est un cercle

float DEGTORAD = 0.017453f;

class Animation
{
    public :
    float Frame, speed;
    Sprite sprite;
    vector<IntRect> frames;
    
    Animation(){}
    
    Animation(Texture &t, int x, int y, int w, int h, int count, float Speed)
    {
        Frame = 0;
        speed = Speed;
        
        for (int i=0;i<count;i++)
            frames.push_back(IntRect(x+i*w,y,w,h));
        sprite.setTexture(t);
        sprite.setOrigin(w/2,h/2);
        sprite.setTextureRect(frames[0]);
    }
    
    void update()
    {
        Frame += speed;
        int n = frames.size();
        if (Frame >= n) Frame -= n;
        if (n>0) sprite.setTextureRect( frames[int(Frame)]);
    }
    
};


int main ()
{
    RenderWindow app(VideoMode(W,H), "SPAAACE");
    app.setFramerateLimit(60);
    
    /// TEXTURES \\\
    
    Texture t1,t2,t3;
    t1.loadFromFile(resourcePath() + "fond.png");
    t2.loadFromFile(resourcePath() + "spaceship.png");
    t3.loadFromFile(resourcePath() + "explosion.png");
    // rajouter textutes astres
    
    /// SPRITES \\\
    
    Sprite sFond(t1),sPlayer(t2),sExplosion(t3);
    sPlayer.setTextureRect(IntRect(0,-20,1000,500)); //regler le pb de "centre de masse" #rotation
    sPlayer.setOrigin(400,400);
    
    sExplosion.setPosition(400,400); // hors ecran ? pk non visible ?
    float Frame = 0;
    float animSpeed = 0.4;
    int frameCount = 20;
    
    float x=300, y=300;
    float dx=0,dy=0,angle=0;
    bool thrust;
    
    
    while (app.isOpen())
    {
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();
        }
        
        ///ANIMATION DU SPRITE \\\
        Frame += animSpeed;
        if (Frame<frameCount) Frame -= frameCount;
        sExplosion.setTextureRect(IntRect(int(Frame)*192,0,192,192)); // 20 images de 192*192px
        
        
        if (Keyboard::isKeyPressed(Keyboard::Right)) angle +=3;
        if (Keyboard::isKeyPressed(Keyboard::Left)) angle -=3;
        if (Keyboard::isKeyPressed(Keyboard::Up)) thrust=true;
        else thrust=false;
        
        /// MOUVEMENT DU VAISSEAU \\\
        // incorporer rk4
        
        if (thrust)
        {
            dx+=cos(angle*DEGTORAD)*0.2;
            dy+=sin(angle*DEGTORAD)*0.2;
        }
        else
        {
            dx*=0.99;
            dy*=0.99;
        }
        
        int Vmax=15;
        float V = sqrt(dx*dx+dy*dy);
        if(V>Vmax)
        {
            dx *= Vmax/V;
            dy *= Vmax/V;
        }
        
        x+=dx;
        y+=dy;
        
        if (x > W) x=0; if (x < 0) x=W;
        if (y > H) y=0; if (y < 0) y=H;
        
        sPlayer.setPosition(x, y);
        sPlayer.setRotation(angle+90);
        
        
        /// AFFICHAGE \\\
        
        app.clear();
        app.draw(sFond);
        app.draw(sPlayer);
        app.draw(sExplosion);
        app.display();
    }
    
    return 0;
}
