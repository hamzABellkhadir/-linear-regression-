#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h> // -------------------utuliser la bub sdl 
// ----------------------------------------width and height of windows
static const int width = 500;
static const int height = 500;

//-----------------------------------------structure 
typedef struct{
	double a,b;
	double learning_rate;
}Model;


typedef struct {
	 double x,y;
}Point;

typedef struct PointElemnts{
	Point * point;
	struct PointElemnts * next;
}PointElemnts;



// -----------------------------------------------constructor initialisation des structures de donnee 

Model * CreatModel(double a,double b,double learning_rate){
	Model *CModel =(Model *)malloc(sizeof(Model));
	CModel->a= a;
	CModel->b=b;
	CModel->learning_rate=learning_rate;	
	return CModel; 
}

Point *CreatPoint(double x,double y){
	Point *Cpoint = (Point *)malloc(sizeof(Point));
	Cpoint->x=x;
	Cpoint->y =y;
	return Cpoint;
}
PointElemnts *CreatPointElemnts(Point * p){
	PointElemnts *Cpe = (PointElemnts * )malloc(sizeof(PointElemnts));
	Cpe->point = p;
	Cpe->next=NULL;
	return Cpe;
}
// -------------------------------------------------------add point to elements -> remplir la list chainee
void  AddPointElements(Point * p,PointElemnts ** list){
	PointElemnts *Cpe = CreatPointElemnts(p);
	// ajouter l'element au debut de la liste
	Cpe->next = *list;
	*list= Cpe; 
}





// ------------------------------------------------------------------------code 

// function hepls us to find the error and rotate  the ligne 
void findModel(PointElemnts *list, Model *model){
	double error; /*pour calculer l'erreur*/
	double x,y,predicted_y;
	PointElemnts*p;
	for(p=list;p!=NULL;p=p->next){
		x=p->point->x;
		y=p->point->y;
		predicted_y=(model->a)*x+(model->b);
		error=y-predicted_y;
		model->a =model->a + (error*x)*model->learning_rate;
		model->b =model->b+ error*0.1;	
//		printf("test (%.2f , %.2f )   ",((model->a)*x+(model->b) - p->point->y)*0.01,p->point->y);
	}
	
}

// fonction a pour but de dessiner les points et la ligne
void drawe(PointElemnts *list,SDL_Renderer *renderer,Model *model){
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	PointElemnts *ptr=list;
	SDL_Rect rect;
	//printf("%x",ptr);
	while(ptr!=NULL){
   		rect.x = ptr->point->x;
   		rect.y = ptr->point->y;
 		rect.h = 7;
   		rect.w = 7;
		SDL_RenderDrawRect(renderer, &rect);
		 SDL_RenderFillRect(renderer, &rect);
		ptr=ptr->next;
	}
	// Exécutez la fonction findmodule pour modifier la valeur de a et b afin de pouvoir tracer la courbe
	findModel(list,model);
	// prendre deux point quelqanque, est calculer le y1 et y2 de chaque point en utilisant le module (a, b);
	double x1=0,x2=100000,y1,y2;
	y1=(model->a)*x1 + (model->b);
	y2=(model->a)*x2 + (model->b);
	SDL_SetRenderDrawColor(renderer,205, 247, 238, 0);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}



int main(int argc, char **argv)
{
	// initialisation du module et la liste
	Model *model = CreatModel(1,0,0.00001);
    PointElemnts *list=NULL;
    // condition pour quitter la boucle de sdl
    bool running = true;
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a SDL window
    SDL_Window *window = SDL_CreateWindow("Linear Regression par hamza belkhadir", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

    // Create a renderer (accelerated and in sync with the display refresh rate)
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Event event;
    
    
    while(running)
    {   		
                
        // Process events
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = false;
            }
            // écouter l'événement de clic
             if(event.type == SDL_MOUSEBUTTONDOWN){
             	// enregistrer les points à la liste
				Point *p=CreatPoint(event.motion.x,event.motion.y);
    			AddPointElements(p,&list);
//			    printf("Current mouse position : (%d, %d)\n", event.motion.x, event.motion.y);
		  }
        }
        
        // Draw
		drawe(list,renderer,model);
		
        // Show what was drawn
		SDL_SetRenderDrawColor(renderer, 73, 74, 72, 0);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }
	

    // Release resources
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}


