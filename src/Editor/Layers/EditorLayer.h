#include "core/Layer.h"
#include <iostream>

class EditorLayer : public LANE::Layer
{
public:
    void Update()
    {
        
    }

    void OnEvent(LANE::EventType eType, LANE::Event* event)
    {
        if (eType == LANE::EventType::Key)
        {
            LANE::KeyEvent* keyEvent = (LANE::KeyEvent*)(event);

            if (keyEvent->type == LANE::KeyEventType::KeyStroke)
            {
                std::cout << static_cast<char>(keyEvent->key);
            }
            
        }
    }
};