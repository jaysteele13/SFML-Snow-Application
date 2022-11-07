#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>

// Window dimensions
#define winWidth 960
#define winHeight 540

//snow method
void Snowfall(std::vector<sf::Sprite>& snowArr);

//random method
double dRand(double fMin, double fMax);

//random position for ghost
sf::Vector2f getGhostPos(std::vector<sf::Vector2f> positions);

int main()
{
    //DISCLAIMER: Got carried away trying to make a ghost and in doing so this is over 300 Lines long, sorry.
    //Also, you may have to change the paths to work according to the system you are on as CLion didn't like it when I
    //just referenced the file name.


    //declaring var for frame rate and ghost entry
    int numOfSnowflakesPerFrame = 125;
    int ghostRandEntry = dRand(2,4);

    float dt; //delta time - make framerate independant: *dt * mul
    float mul = numOfSnowflakesPerFrame;

    //set background gif up
    sf::Texture texBackground; //make image and restraints smaller
    texBackground.loadFromFile("/Users/jaysteele/Documents/CLionProjects/Snowfall/Spritesheet.png");

    sf::IntRect  rectSourceSprite(0, 0, 960, 540); // a variable which tracts my image
    sf:: Sprite backgroundSprite(texBackground, rectSourceSprite); //cuts up spritesheet into individual images

    //set ghosts walking gifs up. Side Note -> (would have done it more efficient if I knew how, arrays didn't work how I wanted them too)
    sf::Texture texGhost;
    texGhost.loadFromFile("/Users/jaysteele/Documents/CLionProjects/Snowfall/SecondWoman.png");

    sf::Texture anotherTexGhost;
    anotherTexGhost.loadFromFile("/Users/jaysteele/Documents/CLionProjects/Snowfall/FollowerWoman.png");

    sf::IntRect  ghostSourceSprite(0, 0, 128, 320); // a variable which tracts my image
    sf:: Sprite ghostSprite(texGhost, ghostSourceSprite); //cuts up spritesheet into individual images

    sf::IntRect  anotherGhostSourceSprite(0, 0, 128, 320); // a variable which tracts my image
    sf::Sprite anotherGhostSprite(anotherTexGhost, anotherGhostSourceSprite);

    //set snow image up
    sf::Texture texSnow;
    texSnow.loadFromFile("/Users/jaysteele/Documents/CLionProjects/Snowfall/tinySnow.png");

    //setting snow array
    std::vector<sf::Sprite> snowflakes;
    snowflakes.reserve(numOfSnowflakesPerFrame); //give array numOfSnowflake spaces

    //Ocean of Clocks
    sf::Clock backgroundClock; //to track the delay of animation
    sf::Clock ghostAnimClock; //track women walking anim
    sf::Clock ghostsClock; //track ghosts
    sf::Clock anotherGhostsClock; //tracks second ghost - couldn't find an efficient way :(
    sf::Clock clock; //to track the framerate
    sf::Clock ghostFadeClock; //fading times

    //Setting the window dimensions
    sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), "Merry Crimmis, have a coffee x"); //declare window size



    //adding music
    sf::Music music;

    if(!music.openFromFile("/Users/jaysteele/Documents/CLionProjects/Snowfall/Let It Snow Lofi Remix.wav")) {
        std::cout << "music wont load :/" << std::endl;
    }

    music.play(); //play music

    if(music.getStatus() != sf::Music::Status::Playing) //loop music
        music.play();


    //setting framerate
    window.setFramerateLimit(numOfSnowflakesPerFrame); //so program is stable



    //ghost array of positions
    std::vector<sf::Vector2f> ghostPos;

    //populating with available positions - could have made using random
    ghostPos.push_back(sf::Vector2f(400, 200)); //if 0 have this rotation, direction, speed, scale
    ghostPos.push_back(sf::Vector2f(610, 130));
    ghostPos.push_back(sf::Vector2f(300, 180));
    ghostPos.push_back(sf::Vector2f(630, 100));

    //set ghost available positions
    ghostSprite.setPosition(getGhostPos(ghostPos)); //set initial position
    anotherGhostSprite.setPosition(getGhostPos(ghostPos));

    while(window.isOpen())
    {

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

        }

        dt = clock.restart().asSeconds(); //attempting delta time

        window.clear();

        window.draw(backgroundSprite); //draw animated background

        sf::Time elapsed = ghostsClock.getElapsedTime();// Elapsed time since last ghost

        Snowfall(snowflakes); // populate array

        //Make blizzard
        for(int i = 0; i < snowflakes.size(); i++)
        {
            double snowSpeed; //declare var

            //Initialise wind direction between 2 vars
            //adding directions
            double rightWindDirection = dRand(1.0, 5.0);
            //double leftWindDirection = dRand(-1.0, -5.0); //alter wind slightly - Had to take out as it was to messy.
            double midWindDirection = dRand(-1.0, 2.0);

            snowflakes[i].setTexture(texSnow); //make snow into sprite

            //changing speed based on size of snowflake
            if(snowflakes[i].getScale().x > 0.5) { //
                snowSpeed = dRand(1 *dt * mul, 1.5 *dt * mul); //large is slowest
            }
            else if(snowflakes[i].getScale().x > 0.2)
                snowSpeed = dRand(2 *dt * mul, 2.8 *dt * mul); //medium size
            else
                snowSpeed = dRand(3 *dt * mul, 5 *dt * mul); //smallest is fastest

            if(snowflakes[i].getRotation() > 200)
                snowflakes[i].move(rightWindDirection, snowSpeed); //generating right random movement for flakes
            else
                snowflakes[i].move(midWindDirection, snowSpeed); //generating mid random movement for flakes


            window.draw(snowflakes[i]); //drawing the snow

            // Don't draw snowflakes that are outside of the window //within loop
            if(snowflakes[i].getPosition().y >= winHeight)
            {
                snowflakes.erase(snowflakes.begin() + i);
            }
        }


        //loop ghosts

        if(ghostsClock.getElapsedTime().asSeconds() > 0) {


            double offset = dRand(-1, -4);
            double speed = dRand(-1, 4);

            int ghostTime = elapsed.asMilliseconds() /5;

            if(ghostTime < 180) {
                ghostSprite.setColor(sf::Color(255, 255, 255, ghostTime));
            }

            if(ghostsClock.getElapsedTime().asSeconds() > 4) {
                ghostSprite.setColor(sf::Color(255, 255, 255, -ghostTime));
            }


            ghostSprite.move(offset, speed);

            window.draw(ghostSprite);


            //second glitching ghost
            if(ghostsClock.getElapsedTime().asSeconds() > ghostRandEntry) {

                sf::Time anotherElapsed = anotherGhostsClock.getElapsedTime();// Elapsed time since second ghost arrives

                int anotherGhostTime = (anotherElapsed.asMilliseconds() /5);

                if(anotherGhostTime < 200) { //trying to make ghost fade in and out
                    anotherGhostSprite.setColor(sf::Color(255, 255, 255, ghostTime));
                }

                if(anotherGhostsClock.getElapsedTime().asSeconds() > 1) {
                    anotherGhostSprite.setColor(sf::Color(255, 255, 255, -ghostTime));
                }

                int anotherOffset = dRand(offset, offset+0.5); //give ghost a bit of diversity

                anotherGhostSprite.move(anotherOffset, speed);
                window.draw(anotherGhostSprite); //drawing scond ghost

            }
        }

        if (ghostsClock.getElapsedTime().asSeconds() > 7) { //7 = time before animation is cut off ad reset

            //changing positions
            ghostSprite.setPosition(getGhostPos(ghostPos));
            anotherGhostSprite.setPosition(getGhostPos(ghostPos));

            ghostRandEntry = dRand(1,3); //reset when second ghost comes in

            //making positions different
            while(anotherGhostSprite.getPosition() == ghostSprite.getPosition()) {
                anotherGhostSprite.setPosition(getGhostPos(ghostPos));
            }

            ghostsClock.restart(); //reset ghost clock

        }



        //animating background
        if(backgroundClock.getElapsedTime().asMilliseconds() > 160) { //change image every 130 ms
            if(rectSourceSprite.left == 1920) {
                rectSourceSprite.left = 0; // reset rows
                rectSourceSprite.top+=540;
            } //change column

            else {
                rectSourceSprite.left+= 960;
            }

            if(rectSourceSprite.top == 2160) { // if at max column
                rectSourceSprite.top = 0; //reset
            }

            backgroundSprite.setTextureRect(rectSourceSprite);
            backgroundClock.restart();
        }

        //animating ghosts - only way to do it, unfortunately.
        if(ghostAnimClock.getElapsedTime().asMilliseconds() > 80) { //change image every 130 ms
            int ghostOffset = 128; //var which decides which direction
            if(ghostSourceSprite.left == 896) {
                ghostSourceSprite.left = 0;
                anotherGhostSourceSprite.left = 0;
            }
            else {
                ghostSourceSprite.left += ghostOffset;
                anotherGhostSourceSprite.left += ghostOffset;
            }

            ghostSprite.setTextureRect(ghostSourceSprite); //allow animation
            anotherGhostSprite.setTextureRect(anotherGhostSourceSprite); //allow animation
            ghostAnimClock.restart();
        }

        window.display();
    }
    return 0;
}

//random method that makes it easier to generate random numbers
double dRand(double dMin, double dMax)
{
    double d = (double)rand() / RAND_MAX;
    return dMin + d * (dMax - dMin);
}

// Create a snowflake and store it in an array
void Snowfall(std::vector<sf::Sprite>& snowArr)
{
    sf::Sprite snow; //making snow

    //initialising rand vars
    double rScale = dRand(0.1, 0.61);
    double rAlpha = dRand(80, 200);
    double rRotate = dRand(0, 360);

    snow.scale(sf::Vector2f( rScale, rScale)); // make random varibles that some are randomly bigger and some are randomly smaller*
    //random rotation
    snow.rotate(rRotate);

    //for wind position
    if(snow.getRotation() > 200 ) {
        snow.setPosition(dRand(-winWidth, 0), dRand(-winHeight, 0)); //sets snow to come from right hand side
    }
    else
        snow.setPosition(dRand(winWidth, 0), 0); //sets snow to come from middle

    /* else if(snow.getRotation() > 100 ) {
   //snow.setPosition(winWidth, dRand(-winHeight, winHeight)); //sets snow to come from left hand side
}
*/

    //snow is more clear based on size
    if(snow.getScale().x <  0.2) { //if snow is less than half the size
        snow.setColor(sf::Color(255, 255, 255, rAlpha));
    }
    else if(snow.getScale().x <  0.53) { //if snow is less than half the size
        double newA = dRand(150, 230);
        snow.setColor(sf::Color(255, 255, 255, newA));
    }
    else if (snow.getScale().x >  0.53){
        double rColour = dRand(200, 255);
        snow.setColor(sf::Color(200, 238, rColour, 255)); //crystal blue
    }
    snowArr.emplace_back(snow); //insert individual snow into the array
}

sf::Vector2f getGhostPos(std::vector<sf::Vector2f> positions) {
    //return random number between 0 and length of array
    return positions[dRand(0, positions.size())];
}
