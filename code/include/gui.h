#include <raylib.h>
#include <iostream>
#include <vector>
#include <map>

const unsigned int MAX_TOUCH_POINTS = 2;

class ScrollMenu
{
int scrollOffset = 0;
unsigned int itemCount = 10;
unsigned int itemHeight = 40;
unsigned int height;
unsigned int width;
unsigned int scrollBarWidth = 10;
bool scrolling = false;
unsigned int scrollingX;
unsigned int scrollingY;
void (*interactionCall)(std::string,std::string) = nullptr;
std::map<std::string,std::string> items;

public:

ScrollMenu();

ScrollMenu(unsigned int iCount, unsigned int iHeight, unsigned int width,void (*call)(std::string,std::string));

void addItem(std::string,std::string);

void update(void);

void updateInteraction(unsigned int, unsigned int);

void draw(int, int);

void clear();
};