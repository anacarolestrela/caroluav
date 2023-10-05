#include "mission1.hpp"
#define DISTANCE_F_B 50
#define DISTANCE_R_L 30
#define DZ 50
#define HEIGHT_UP 100 //LARC 220  
#define STOP_TIME 6
#define LAND_TIME 7
#define TAKEOFF_TIME 3
#define MOV_TIME 7

using namespace bahiart;
void submapeamento();

void Mission1::not_initialized()
{
    this->commands->init();
    this->commands->streamon(2); 
    this->commands->init(); 

    this->droneState->setCurrentState(DroneState::States::TAKING_OFF);
    this->droneState->setInfoPads(DroneState::Pads::NO_PAD); 
}

void Mission1::taking_off()
{
    //Mensagens(takeoff);
    std::cout<< "FAZENDO TAKEOFF"<<std::endl;
    this->commands->takeoff(TAKEOFF_TIME); 
    this->commands->stop(STOP_TIME);
    this->commands->forward(DISTANCE_F_B, MOV_TIME);  //mudar 225 ----->COMENTEI PARA O TESTE
    // this->commands->forward(50, 7); 
    


    this->droneState->setCurrentState(DroneState::States::EXECUTING);
}

void Mission1::executing()
{   this->commands->init();
    this->commands->init();
    this->commands->init();
    Geometry::Point2D drone_pos(0, 0);
    int xz = 0;
    int yz = 0;
    int zona[xz][yz];

    

    /* esperando */
    std::cout<< "SUBINDO"<<std::endl;  
    this->commands->up(10,7);

    std::cout<< "INDO PARA ZONA [1][0]"<<std::endl; 

    // TESTE FAZENDO O DETECT NA ZONA [0,0]
    // std::cout << " TESTE ZONA [0,0]" << std::endl; 
    // std::cout << " FAZENDO O DETECT " << std::endl; 
    // this->commands->stop(8); 
    // if (*(this->commands->getId())==1)
    // {
    //     std::cout << "-----TARGET----" << std::endl; 
    //     this->commands->land(LAND_TIME);
    //     this->commands->init();
    //     this->commands->takeoff(TAKEOFF_TIME);

    // }
    // else
    // {
    //     std::cout << "NÃO ENCONTROU" << std::endl; 
    // }

    std::cout << "MUDANDO DE ZONA" << std::endl; 
    std::cout<<"INDO P FRENTE"<<std::endl;
    this->commands->forward(DZ, 7); 
    // this->commands->forward(100, 7);
     
    drone_pos.setY(drone_pos.getY() + DZ);
    yz++;
    std::cout<< "ZONA [" <<xz<<"]["<<yz<< "]"<<std::endl;
    //Mensagens(lbf);
    std::cout<< "POUSANDO BASE FIXA"<<std::endl; 
    this->commands->land(LAND_TIME);
    this->commands->init();
    this->commands->takeoff(TAKEOFF_TIME);


    /* loop mapeamento */
    // while (yz != 0 && xz != 3)

    int xzFinal = 3;
    int yzFinal = 0;

    while(xz != xzFinal &&  yz!= yzFinal) //teste1 
    {
        std::cout<<"INICIANDO MAPEAMENTO"<<std::endl;
        /* confere se zona é par */ 
        if (xz % 2 == 0) 
        {
            std::cout<<"zona par"<<std::endl;
            std::cout<<"INDO P FRENTE"<<std::endl;
            this->commands->init();
            this->commands->up(10,7);
            this->commands->forward(DISTANCE_F_B,MOV_TIME);
            this->commands->init();
            this->commands->stop(8);
            this->commands->init();
            // this->commands->forward(100, 7); 
            yz++;
            drone_pos.setY(drone_pos.getY() + DZ);
            std::cout<< "ZONA [" <<xz<<"]["<<yz<< "]"<<std::endl;
            this->commands->stop(8); 
            /* fazendo o detect, id=1 é target */
            if (*(this->commands->getId())==1)
            {
                std::cout << "-----TARGET----" << std::endl; 
                this->commands->land(LAND_TIME);
                this->commands->init();
                this->commands->takeoff(TAKEOFF_TIME);

            }
            else
            {
                std::cout << "NÃO ENCONTROU" << std::endl; 
            }
            if(yz == 3) 
            {
                std::cout<<"INDO P DIREITA"<<std::endl;
                this->commands->right(DISTANCE_R_L,MOV_TIME);
                drone_pos.setX(drone_pos.getX() + DZ);
                xz++;
                std::cout<< "ZONA [" <<xz<<"]["<<yz<< "]"<<std::endl;
            } 
        }
        /* xz é impar */
        else
        {
            std::cout<<"zona impar"<<std::endl;
            this->commands->stop(STOP_TIME);
            this->commands->init(); 
            /* fazendo o detect */
            if (*(this->commands->getId())==1)
            {
                std::cout << "-----TARGET----" << std::endl; 
                this->commands->land(LAND_TIME);
                this->commands->init();
                this->commands->takeoff(TAKEOFF_TIME);
            
            }
            else
            {
                std::cout << "NÃO ENCONTROU" << std::endl; 
            }
            std::cout<<"INDO P TRAS"<<std::endl;
            // this->commands->back(60,MOV_TIME);
            this->commands->go(0, -50, 0, 4, 7);
            // this->commands->back(100, 8); 
            this->commands->init();
            this->commands->stop(8);
            this->commands->init();
            yz--;
            drone_pos.setY(drone_pos.getY() - DZ);
            std::cout<< "ZONA [" <<xz<<"]["<<yz<< "]"<<std::endl;
            this->commands->stop(STOP_TIME);
            this->commands->init(); 

            if (yz ==0)
            {
                std::cout<<"INDO P DIREITA"<<std::endl;
                this->commands->right(DISTANCE_R_L,MOV_TIME);
                xz++;
                drone_pos.setX(drone_pos.getX() + DZ);
                std::cout<< "ZONA [" <<xz<<"]["<<yz<< "]"<<std::endl;
            }
            

        }

    }
    /* saiu do loop, acabou mapeamento; voltar para a base do takeoff 
    3 movimentos de 2m para a esquerda */
    this->commands->left(DISTANCE_R_L, MOV_TIME); 
    this->commands->left(DISTANCE_R_L, MOV_TIME);
    this->commands->left(DISTANCE_R_L, MOV_TIME);
    this->commands->land(); 

    // //TESTE:
    // std::cout << "VOLTANDO PARA A BASE" << std::endl; 
    // this->commands->init();
    // this->commands->back(300, 8);
    // this->commands->left(300, 8);

    if(*(this->commands->getId())==0)
    {
        std::cout << "-----TAKEOFF-----" << std::endl; 
        this->commands->land(); 
    }
    else
    {
        std::cout << " NÃO ENCONTROU TAKEOFF, POUSANDO MESMO ASSIM " << std::endl;
        this->commands->land(); 
    }
    this->droneState->setCurrentState(DroneState::States::LANDED); 

}

void Mission1::lining_up()
{
}

void submapeamento()    //submapeamento de zona

{   
    int sbz_x = 0;
    int sbz_y = 0;
    //abaixar o drone para uma altura onde o campo de visao seja de 50X50
    this->commands->letf(75,7);
    this->commands->back(75,7); 

    while (sbz_x != 4 && sbz_y != 4)
    {
        if(sbz_x % = 0)
        {

            this->commands->forward(50,7); //de 50 em 50 cm  o drone para e faz um detec de 8segs
            this->commands->stop(8);
           //faz detect
            this->commands->init();
            sbz_y++;
            if(sbz_y = 4)
            {
                this->commands->right(50,7);
                sbz_x++;
            }
        }
        if(sbz_x % != 0)
        {

            this->commands->back(50,7); //de 50 em 50 cm  o drone para e faz um detec de 8segs
            this->commands->stop(8);
           //faz detect
            this->commands->init();
            sbz_y--;
            if(sbz_y = 0)
            {
                this->commands->right(50,7);
                sbz_x++;
            }
        }     
    }
}
