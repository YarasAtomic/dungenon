#include "gui.h"

ScrollMenu::ScrollMenu()
{
    this->width = 150;
    itemCount = 10;
    itemHeight = 40;
    height = itemCount * itemHeight;
}

ScrollMenu::ScrollMenu(unsigned int iCount, unsigned int iHeight, unsigned int width,void (*call)(std::string,std::string))
{
    this->width = width;
    itemCount = iCount;
    itemHeight = iHeight;
    height = iCount * iHeight;
    interactionCall = call;
}

void ScrollMenu::addItem(std::string name ,std::string id)
{
    std::pair<std::string,std::string> pair(id,name);
    items.insert(pair);
}

void ScrollMenu::update()
{
    if(items.size()>itemCount)
    {
        if (IsKeyDown(KEY_DOWN)) {
            scrollOffset += 1;
            if (scrollOffset > items.size() - itemCount) {
                scrollOffset = items.size() - itemCount;
            }
        }
        if (IsKeyDown(KEY_UP)) {
            scrollOffset -= 1;
            if (scrollOffset < 0) {
                scrollOffset = 0;
            }
        }
    }

}

void ScrollMenu::updateInteraction(unsigned int posX, unsigned int posY)
{
    bool pressed = false;
    int mouseX = 0;
    int mouseY = 0;

    int currentGesture = GetGestureDetected();

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
    // If mouse click is detected, tapping will detect a click in (0,0)
    {
        mouseX = GetMouseX() - posX;
        mouseY = GetMouseY() - posY;
        // If the click happens inside the menu
        if(!(mouseX<=0||mouseY<=0||mouseX>width||mouseY>height)) pressed = true;
    }
    if(currentGesture == GESTURE_TAP)
    // If touch screen tap is detected
    {
        Vector2 touchPos = GetTouchPosition(0);
        // If the tap happens inside the mennu
        if(!(touchPos.x - posX<0||touchPos.y - posY<0||touchPos.x - posX>width||touchPos.y - posY>height))
        {
            pressed = true;
            // Only update if it happens inside the menu
            mouseX = touchPos.x - posX;
            mouseY = touchPos.y - posY; 
        }
    }

    if(pressed)
    {
        if(mouseX<width-scrollBarWidth)
        // Interacting with items
        {
            // Get id from the current view of items
            int selectedFromView = (float)mouseY/(float)height*itemCount;

            int i = 0;
            for(auto iterator : items)
            {
                // Get the item that has that view id
                if(i==selectedFromView+scrollOffset&&interactionCall!=nullptr)
                    interactionCall(iterator.second,iterator.first);
                i++;
            }

        }
        else
        // Interacting with scroll bar
        {
            std::cout << "Scroll" << std::endl;
            scrolling = true;
            scrollingX = mouseX;
            scrollingY = mouseY;
        }
    }

    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) scrolling = false;

    if(scrolling)
    {

    }
}

void ScrollMenu::draw(int posX, int posY)
{
    const int margin = 10;
    // Draw the menu items
    int i = 0;
    for (auto iterator : items)
    {
        if(i>=scrollOffset &&  i < scrollOffset+itemCount)
        {
            DrawText(iterator.second.c_str(),posX+margin,posY+margin+(i - scrollOffset)*itemHeight,20,DARKGRAY);
        }
        i++;
    }

    if(items.size()>itemCount)
    {
        // Draw scrollbar if needed
        DrawRectangle(width - scrollBarWidth, 0, scrollBarWidth, height, GRAY);
        float scrollBarHeight = (float)height * itemCount / items.size();
        DrawRectangle(width - scrollBarWidth, (float)scrollOffset * height / items.size(),
                        scrollBarWidth, scrollBarHeight, DARKGRAY);
    }
}

void ScrollMenu::clear()
{
    items.clear();
}