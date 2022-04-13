#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// https://www.codeproject.com/articles/1102603/accessing-json-data-with-cplusplus
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>

#include "ant.h"
#include "anthill.h"
#include "food.h"
#include "pheromone.h"
#include "tile.h"
#include "world.h"
#include "utils.h"

const int black[3] = {0, 0, 0};
const int blue[3] =  {0, 30, 200};
const int yellow[3] =  {250, 250, 50};
const int cyan[3] =  {40, 230, 230};
const int orange[3] =  {250, 140, 0};
const int purple[3] =  {200, 0, 240};
const int green[3] =  {0, 120, 0};
const int redWine[3] =  {140, 0, 0};
const int totalColors = 8;
const int * colors[8] = {black, blue, yellow, cyan, orange, purple, green, redWine};
    

void World::setup()
{
    config = readJson();

    setupSDL();
    setupGrid();

    setupChart();
    addAntsAndHills();
    addFoods();
    
    update();
    //setupThreads();
}

void World::setupSDL()
{
    SDL_Init(SDL_INIT_VIDEO);
    int w = getWidth() * getSquareSize();
    int h = getHeight() * getSquareSize();
    window = SDL_CreateWindow("FormiguinhaZ",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,w,h,0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


    SDL_Surface * antSurface = SDL_LoadBMP("ant.bmp");
    antTexture = SDL_CreateTextureFromSurface(renderer, antSurface);
    SDL_FreeSurface(antSurface);

    SDL_Surface * foodSurface = SDL_LoadBMP("food.bmp");
    foodTexture = SDL_CreateTextureFromSurface(renderer, foodSurface);
    SDL_FreeSurface(foodSurface);

    SDL_Surface * foodySurface = SDL_LoadBMP("foody.bmp");
    foodyTexture = SDL_CreateTextureFromSurface(renderer, foodySurface);
    SDL_FreeSurface(foodySurface);

    SDL_Surface * notFoodSurface = SDL_LoadBMP("notFood.bmp");
    notFoodTexture = SDL_CreateTextureFromSurface(renderer, notFoodSurface);
    SDL_FreeSurface(notFoodSurface);

    SDL_Surface * anthillSurface = SDL_LoadBMP("anthill.bmp");
    anthillTexture = SDL_CreateTextureFromSurface(renderer, anthillSurface);
    SDL_FreeSurface(anthillSurface);

    SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void World::draw()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    int w = getWidth();
    int h = getHeight();
    int ss = getSquareSize();
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    for (int x = 0; x < w; x+=2)
    {
        for (int y = 0; y < h; y++)
        {
            SDL_Rect rect = {(x + (y%2)) * ss, y * ss, ss, ss};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    /*for (Anthill * i : m_anthills)
    {
        int hx = i->getx();
        int hy = i->gety();
        SDL_SetRenderDrawColor(renderer, 75, 55, 35, 255);
        SDL_Rect rect = {hx * ss, hy * ss, ss, ss};
        SDL_RenderFillRect(renderer, &rect);
    }*/

    for (Pheromone * i : m_pheromones)
    {
        int px = i->getx();
        int py = i->gety();
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 30);
        SDL_Rect rect = {px * ss, py * ss, ss, ss};
        SDL_RenderFillRect(renderer, &rect);
    }

    for (Ant * i : m_ants)
    {
        const int * c = colors[i->getAnthillIndex() % totalColors];
        SDL_SetTextureColorMod(antTexture, c[0], c[1], c[2]);
        SDL_SetTextureAlphaMod(antTexture, 100);
        SDL_Rect rect = {i->getx() * ss, i->gety() * ss, ss, ss};
        SDL_RenderCopy(renderer, antTexture, NULL, &rect);
        if (i->mode == bring)
        {
            SDL_RenderCopy(renderer, foodyTexture, NULL, &rect);
        }
    }
    
    for (Anthill * i : m_anthills)
    {
        const int * c = colors[i->getIndex() % totalColors];
        SDL_SetTextureColorMod(anthillTexture, c[0], c[1], c[2]);
        SDL_Rect rect = {i->getx() * ss, i->gety() * ss, ss, ss};
        SDL_RenderCopy(renderer, anthillTexture, NULL, &rect);
    }
    
    for (Food * i : m_foods)
    {
        int fx = i->getx();
        int fy = i->gety();
        int cr = i->currentFood;
        if (cr == 0)
        {
            SDL_Rect rect = {fx * ss, fy * ss, ss, ss};
            SDL_RenderCopy(renderer, notFoodTexture, NULL, &rect);
        }else{
            SDL_Rect rect = {fx * ss, fy * ss, ss, ss};
            SDL_RenderCopy(renderer, foodTexture, NULL, &rect);
        }
    }

    SDL_RenderPresent(renderer);
}

Json::Value World::readJson()
{
    std::ifstream configFile (CONFIGURATION_PATH);
    
    Json::Reader reader;
    Json::Value configData;

    if (!reader.parse(configFile, configData, false))
    {
        std::cout << "O arquivo não pode ser lido. Verifique o caminho especificado\n";
    }

    return configData;
    
}

void World::setupThreads()
{
    const int nThreads = config["nThreads"].asInt();
    int curThread = nThreads;

    while (curThread--)
    {       
        // TEMPORARIO PARA TESTAR THREADS
        // https://stackoverflow.com/questions/10673585/start-thread-with-member-function
        m_threads.push_back(new std::thread(&World::update, this));
    }

    for(std::thread * t : m_threads)
    { 

        t->join();
        delete t;
    }
}

void World::setupChart()
{   
    const int height = getHeight();
    const int width = getWidth();

    const int nAnts = config["anthills"].size();
    
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            Tile * tilePointer = tileFactory(j, i, nAnts);
            m_chart.push_back(tilePointer);
        }
    }
}

void World::setupGrid()
{   
    const int heightPlusWalls = getHeight() + 2;
    const int widthPlusWalls = getWidth() + 2;

    for(int cury = 0; cury < heightPlusWalls; cury++)
    {
        for(int curx = 0; curx < widthPlusWalls; curx++)
        {
            if ((curx == 0 && cury == 0) || (curx == widthPlusWalls - 1 && cury == heightPlusWalls - 1))
            {
                m_grid_base.push_back('/');
            }
            else if ((curx == widthPlusWalls - 1 && cury == 0) || (curx == 0 && cury == heightPlusWalls - 1))
            {
                m_grid_base.push_back('\\');
            }
            else if ((curx > widthPlusWalls - 2) || (curx < 1))
            {
                m_grid_base.push_back('|');
            }
            else if ((cury < 1) || (cury > heightPlusWalls - 2))
            {
                m_grid_base.push_back('-');
            }
            else
            {
                m_grid_base.push_back(' ');
            }
        }
    }
}

void World::addAntsAndHills()
{
    Json::Value anthillsInfo = config["anthills"];

    int anthillIndex = 0;
    for (Json::Value anthillInfo : anthillsInfo)
    {
        Anthill * ah = anthillFactory(anthillInfo, anthillIndex);
        m_anthills.push_back(ah);

        Tile * anthillTile = m_chart[posToInt((*ah).getx(),(*ah).gety())];
        anthillTile->isAnthill = true;

        for(int i = 0; i < ah->getPopu(); i++){
            Ant * ant = new Ant(ah->getx(), ah->gety(), anthillIndex, this);
            int uniform[4] = {1,1,1,1};
            ant->face = randDir(uniform);
            m_ants.push_back(ant);
        }

        ++anthillIndex;
    }
}

void World::addFoods()
{
    Json::Value foodsInfo = config["foods"];

    for (Json::Value foodInfo : foodsInfo)
    {
        Food * fd = foodFactory(foodInfo, this);
        m_foods.push_back(fd);

        Tile * foodTile = m_chart[posToInt(fd->getx(),fd->gety())];

        if(fd->currentFood>0)
        {
            foodTile->hasFood = true;
        }
    }
}

int World::posToInt(int posx, int posy)
{
    return posx + getWidth() * posy;
}

void World::print()
{   
    // Esse caractere é traduzido para o comando de clear screen
    // do sistema operacional
    // https://stackoverflow.com/questions/17335816/clear-screen-using-c
    // Testado em Windows e Linux
    std::cout << "\033[2J\033[1;1H";

    const int widthPlusWalls = getWidth() + 2;
    const int heightPlusWalls = getHeight() + 2;

    std::vector<std::string> m_grid(widthPlusWalls*heightPlusWalls,""); 
    for(int i = 0; i < heightPlusWalls; ++i){
        for(int j = 0; j < widthPlusWalls; ++j){
            m_grid[(i*widthPlusWalls)+j] += m_grid_base[(i*widthPlusWalls)+j];
        };
    };

    addEntitiesToGrid(m_pheromones, m_grid);
    addEntitiesToGrid(m_ants, m_grid);
    addEntitiesToGrid(m_foods, m_grid);
    addEntitiesToGrid(m_anthills, m_grid);

    for(int i = 0; i < heightPlusWalls; ++i){
        for(int j = 0; j < widthPlusWalls; ++j){
            std::cout << m_grid[i*(widthPlusWalls) + j];
            if(j==widthPlusWalls-1){
                std::cout << std::endl;
            }
        }
    }
    std::cout << std::endl;
}

template <class ListOrVector>
void World::addEntitiesToGrid(ListOrVector entities, std::vector<std::string> & m_grid)
{   
    for (auto it : entities)
    {   
        std::string marker = "";
        // marker += it->getColor();
        marker += it->getMarker();
        marker += reset1;
        m_grid[(it->getx() + 1) + (getWidth() + 2)*(it->gety()+1)] = marker;
    }
}

void World::update()
{   
    /* Ordem de Updates
     *
     * Pheromones
     * Ants
     * Food
     * AntHill (?)
     *
     * Cada uma dessas classes deve ter um método `update`, que irá
     * utilizar outros métodos para fazer a entidade cumprir seu papel
     * naquele turno.
     *
     * Esses outros métodos devem implementar mecanismos de controle
     * para evitar uma condição de corrida.
     *
     * Como fazer o mecanismo de controle dos Tiles?
     *
     * Deve acontecer uma sincronização após o update de cada tipo de
     * objeto.
     */

    bool running = true;
	SDL_Event event;

    while(running)
    {
        // Primaki
        // capture event in the window
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
				running = false;

            if(event.type == SDL_MOUSEBUTTONUP)
            {
                std::cout << "Pause\n";
            }
        }
        // update pheromones
        auto pheroIt = m_pheromones.begin();
        while (pheroIt != m_pheromones.end()) 
        {
            // pheroIt is increased inside update function
            (*pheroIt)->update(pheroIt);
        }

        // update ants
        for (Ant * ant : m_ants)
        {   
            ant->update();
        }

        // update food
        for (Food * food : m_foods)
        {
            food->update();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        // print();
        draw();
    }      
    return;
}

// Primaki
World::~World()
{
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}