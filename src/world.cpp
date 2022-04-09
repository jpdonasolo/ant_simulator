#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <list>

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


void World::setup()
{
    config = readJson();

    // setupThreads();

    setupGrid();

    setupChart();
    addAntsAndHills();
    addFoods();
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

/*
void World::setupThreads()
{
    const int nThreads = config["nThreads"].asInt();
    int curThread = nThreads;

    while (curThread--)
    {
        m_threads.push_back(new std::thread(update));
    }
    
}
*/

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
            m_chart.push_back(*(tilePointer));
        }
    }
}

void World::setupGrid()
{   
    m_grid.clear();
    const int heightPlusWalls = getHeight() + 2;
    const int widthPlusWalls = getWidth() + 2;

    for (int curx = 0; curx < widthPlusWalls; curx++)
    {
        for (int cury = 0; cury < heightPlusWalls; cury++)
        {
            if (curx == 0 || curx == widthPlusWalls - 1
                || cury == 0 || cury == heightPlusWalls - 1)
            {
                m_grid.push_back('X');
            }
            else
            {
                m_grid.push_back(' ');
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
        m_anthills.push_back(*(ah));

        Tile * anthillTile = &(m_chart[posToInt(ah->getx(), ah->gety())]);
        anthillTile->isAnthill = true;

        for(int i = 0; i<ah->getPopu(); i++){
            Ant * ant = new Ant(ah->getx(), ah->gety(), anthillIndex);
            int uniform[4] = {1,1,1,1};
            ant->face = randDir(uniform);
            m_ants.push_back(*(ant));
        }

        ++anthillIndex;
    }
}

void World::addFoods()
{
    Json::Value foodsInfo = config["foods"];

    for (Json::Value foodInfo : foodsInfo)
    {
        Food * fd = foodFactory(foodInfo);
        m_foods.push_back(*(fd));

        Tile * foodTile = &(m_chart[posToInt(fd->getx(),fd->gety())]);
        foodTile->isFood = true;
    }
}

int World::posToInt(int posx, int posy){
    return posx + getWidth() * posy;
}

/* Grid precisa de uma função diferente, pois as dimensões não são as
 * mesmas, devido às margens.
*/
int World::posToIntGrid(int posx, int posy){
    return (posx + 1) + (getWidth() + 2) * (posy + 1);
}

void World::print()
{   
    setupGrid();

    addEntitiesToGrid(m_ants);
    addEntitiesToGrid(m_foods);
    addEntitiesToGrid(m_anthills);
    
    const int widthPlusWalls = getWidth() + 2;
    const int heightPlusWalls = getHeight() + 2;

    for(int i = 0; i < widthPlusWalls; ++i){
        for(int j = 0; j < widthPlusWalls; ++j){
            std::cout << m_grid[i*(heightPlusWalls) + j];
            if(j==heightPlusWalls-1){
                std::cout << std::endl;
            }
        }
    }
    std::cout << std::endl;
}

template <class ListOrVector>
void World::addEntitiesToGrid(ListOrVector entities)
{   
    for (auto it = entities.begin() ; it != entities.end(); ++it)
    {   
        m_grid[posToIntGrid(it->getx(), it->gety())] = it->getMarker();
    }
}

void World::leavePhero(Ant * ant)
{
    Pheromone * phero = new Pheromone(ant->getx(), ant->gety(), ant->getAnthillIndex(), config["pheroLifetime"].asInt());
    Tile * pheroTile = &(m_chart[posToInt(ant->getx(), ant->gety())]);
    pheroTile->pheroList[ant->getAnthillIndex()] ++;
    m_pheromones.push_back(*(phero));
}

template <class EntityGrid>
int World::getEntityIndex(int posx, int posy, EntityGrid entities)
{   
    int index = -1;
    for (auto it = entities.begin() ; it != entities.end(); ++it)
    {   
        index ++;
        if(it->getx() == posx && it->gety() == posy)
        {
            break;
        }
    }
    return index;
}

void World::checkFood(Ant & ant)
{
    Tile * antTile = &(m_chart[posToInt(ant.getx(), ant.gety())]);
    if(antTile->isFood == true)
    {
        int foodIndex = getEntityIndex(ant.getx(), ant.gety(), m_foods);
        Food * food = &(m_foods[foodIndex]); 
        if(food->currentFood > 0 && ant.mode == seek)
        {
            food->currentFood--;
            ant.mode = bring;
        }
    }
}

void World::checkAnthill(Ant & ant)
{
    Tile * antTile = &(m_chart[posToInt(ant.getx(), ant.gety())]);
    if(antTile->isAnthill == true)
    {
        int anthillIndex = getEntityIndex(ant.getx(), ant.gety(), m_anthills);
        if(anthillIndex == ant.getAnthillIndex() && ant.mode == bring)
        {
            ant.mode = seek;
        }
    }
}


bool World::checkInvalidCoordinates(int posx, int posy)
{   
    if(posx >= getWidth() || posx < 0 || posy >= getHeight() || posy < 0){ return true; }
    return false;
}

void World::look(Ant & ant)
{
    Tile * antTile = &(m_chart[posToInt(ant.getx(), ant.gety())]);
    const int vision = config["vision"].asInt();
    std::list<std::vector<int>> pointsToCheck;
    switch(ant.face){
        case north:
            for(int i =0; i< vision; i ++){
                for (int j=-vision; j < vision+1; j++){
                    std::vector<int> coords;
                    coords.push_back(ant.getx()+j);
                    coords.push_back(ant.gety()-i);
                    pointsToCheck.push_back(coords);
                }
            }
            break;
        case east:
            for(int i =0; i< vision; i ++){
                for (int j=-vision; j < vision+1; j++){
                    std::vector<int> coords;
                    coords.push_back(ant.getx()+i);
                    coords.push_back(ant.gety()+j);
                    pointsToCheck.push_back(coords);
                }
            }
            break;
        case south:
            for(int i =0; i< vision; i ++){
                for (int j=-vision; j < vision+1; j++){
                    std::vector<int> coords;
                    coords.push_back(ant.getx()+j);
                    coords.push_back(ant.gety()+i);
                    pointsToCheck.push_back(coords);
                }
            }
            break;
        case west:
            for(int i =0; i< vision; i ++){
                for (int j=-vision; j < vision+1; j++){
                    std::vector<int> coords;
                    coords.push_back(ant.getx()-i);
                    coords.push_back(ant.gety()+j);
                    pointsToCheck.push_back(coords);
                }
            }
            break;
    }

    auto point = pointsToCheck.begin();
    while (point != pointsToCheck.end()) 
    {
        if(checkInvalidCoordinates((*point)[0], (*point)[1]) || ((*point)[0] == ant.getx() && (*point)[1] == ant.gety()))
        {
            point = pointsToCheck.erase(point);
        }else{
            ++point;
        }
    }
    std::vector<Tile*> tilesVision;
    for(auto it = pointsToCheck.begin(); it != pointsToCheck.end(); it++)
    {       
        tilesVision.push_back(&(m_chart[posToInt((*it)[0],(*it)[1])]));
    }
    int maxPhero = 0;
    Tile * tileObj;

    for(Tile* tileP : tilesVision)
    {
        
        if((tileP->pheroList)[ant.getAnthillIndex()]>maxPhero)
        {   
            maxPhero = (tileP->pheroList)[ant.getAnthillIndex()];
            tileObj = tileP;
        }
    }

    // checa se tá bring, direciona formigueiro
    if(ant.mode == bring)
    {
        ant.lookTo(m_anthills[ant.getAnthillIndex()].getx(), m_anthills[ant.getAnthillIndex()].gety());
    }else if(maxPhero > 0)
    {
    // caso contrario, checa por tiles com feromonio de comida aos lados, exceto no seu tile
        ant.lookTo(tileObj->getx(), tileObj->gety());
    }else if((antTile->pheroList)[ant.getAnthillIndex()]>0)
    {
    // caso haja nenhum, checa se seu tile esta com feromonio (virar direção)
        ant.face = mapInd((mapDir(ant.face)+2)%4);
    }else{
    // caso nenhuma das condicoes anteriores, ande para frente com prob 6, 3 pros lados e 1 para trás
        int weight[4];
        weight[mapDir(ant.face)] = 10;
        weight[(mapDir(ant.face)+1)%4] = 2;
        weight[(mapDir(ant.face)-1)%4] = 2;
        weight[(mapDir(ant.face)+2)%4] = 1;
        ant.face = randDir(weight);
    }
}

void World::walk(Ant & ant)
{   
    int uniformWeightN[4] = {0,1,1,1};
    int uniformWeightE[4] = {1,0,1,1};
    int uniformWeightS[4] = {1,1,0,1};
    int uniformWeightW[4] = {1,1,1,0};
    switch(ant.face)
    {
        case north:
            if(ant.gety()==0){
                ant.face = randDir(uniformWeightN);
            }else{
                ant.suby();
            }
            break;
        case east:
            if(ant.getx()==getWidth()-1){
                ant.face = randDir(uniformWeightE);
            }else{
                ant.addx();
            }
            break;
        case south:
            if(ant.gety()==getHeight()-1){
                ant.face = randDir(uniformWeightS);
            }else{
                ant.addy();
            }
            break;
        case west:
            if(ant.getx()==0){
                ant.face = randDir(uniformWeightW);
            }else{
                ant.subx();
            }
            break;
        default:
            break;
    }
}

void World::update()
{   
    // update pheromones
    auto pheromone = m_pheromones.begin();
    while (pheromone != m_pheromones.end()) 
    {
        pheromone->remainingLife--;
        if(pheromone->remainingLife == 0)
        {
            pheromone = m_pheromones.erase(pheromone);
        }else{
            ++pheromone;
        }
    }

    // update ants
    for (Ant & ant : m_ants)
    {   
        checkFood(ant);
        checkAnthill(ant);
        if(ant.mode == bring){ leavePhero(&ant); }
        look(ant);
        walk(ant);
    }

    // update food / restore food
    /*
    for (Food & food : m_foods)
    {

    }
    */
}
