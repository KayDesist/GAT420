#include "GOLScene.h"
#include "Random.h" 

const color_t white{ 255,255,255,255 };
const color_t black{ 0,0,0,0 };



bool GOLScene::Initialize()
{ 
    m_renderer.Initialize(); 
    m_renderer.CreateWindow("GOL", 800, 600); 

    m_input.Initialize(); 
    m_input.Update(); 

    m_framebuffer = std::make_unique<Framebuffer>(m_renderer, m_renderer.m_width / 2, m_renderer.m_height / 2);
    m_CellsA= std::make_unique<Cells<uint8_t>>(m_framebuffer->m_width,m_framebuffer->m_height);
    m_CellsB= std::make_unique<Cells<uint8_t>>(m_framebuffer->m_width, m_framebuffer->m_height); 

    return true;
}

void GOLScene::Update()
{ 
    Scene::Update(); 
    m_frameCount++; 

    Cells<uint8_t>* currentCells = (m_frameCount & 1) ? m_CellsA.get() : m_CellsB.get();
    Cells<uint8_t>* nextCells = (m_frameCount & 1) ? m_CellsB.get() : m_CellsA.get(); 

    std::filebuf; (nextCells->m_data.begin(), nextCells->m_data.end(), 0); 

    if (m_input.GetKeyDown(SDL_SCANCODE_SPACE)) {
        for (int i = 0; i < nextCells->m_data.size(); i++) {
            nextCells->m_data[i] = (random(50) == 0) ? 1 : 0;
        }
    } 

    for (int y = 1; y < nextCells->m_height - 1; y++) {
        for (int x = 1; x < nextCells->m_width - 1; x++) {
            int count = 0; 

            count += currentCells->Read(x-1,y-1);
            count += currentCells->Read(x-1,y);
            count += currentCells->Read(x-1,y+1);
            count += currentCells->Read(x,y-1);
            count += currentCells->Read(x,y+1);
            count += currentCells->Read(x+1,y-1);
            count += currentCells->Read(x+1, y);
            count += currentCells->Read(x+1,y+1); 

            //do the game of life rules 
            uint8_t currentState = currentCells->Read(x, y); 
            if (currentState) {

                if (count == 2 || count == 3) {

                    nextCells->Write(x, y, 1);
                }
                else {
                    nextCells->Write(x, y, 0);
                } 

            } 
            else {
                if (count == 3) nextCells->Write(x, y, 1);
            }

        }

    } 
    //write cells to framebuffer 
    m_framebuffer->Clear(color_t{ 0,0,0,255 }); 
    for (int i = 0; i < nextCells->m_data.size(); i++) {
        m_framebuffer->m_buffer[i] = (nextCells->m_data[i] ? white : black);
    }

}

void GOLScene::Draw()
{ 

    //update framebuffer
    m_framebuffer->Update();

    //show screen 
    m_renderer = *m_framebuffer;
    SDL_RenderPresent(m_renderer.m_renderer);

}
