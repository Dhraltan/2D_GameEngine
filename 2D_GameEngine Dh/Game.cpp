#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"

Map* map;

SDL_Renderer* Game::renderer = nullptr;
std::vector<ColliderComponent*> Game::colliders;
Manager manager;
SDL_Event Game::event;
auto& player(manager.addEntity());
auto& platform(manager.addEntity());

auto& tile0(manager.addEntity());
auto& tile1(manager.addEntity());
auto& tile2(manager.addEntity());



Game::Game()
{}
Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING)==0)
	{
		std::cout << "SDL initialised" << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window Created" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			std::cout << "Renderer Created" << std::endl;
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	map = new Map();
	//Aici implementez ESC

	//tile0.addComponent<TileComponent>(0, 0, 600, 600, 0);
	tile1.addComponent<TileComponent>(250, 450, 30, 100, 1);
	tile1.addComponent<ColliderComponent>("Platform");
	tile2.addComponent<TileComponent>(0.f, 600.f, 40, 800, 2);
	tile2.addComponent <ColliderComponent>("Grass");

	player.addComponent<TransformComponent>(20.f,560.f);
	player.addComponent<SpriteComponent>("assets/player.png");
	player.addComponent<KeyboardControl>();
	player.addComponent<ColliderComponent>("player");

	platform.addComponent<TransformComponent>(450, 300, 30, 100, 1);
	platform.addComponent<SpriteComponent>("assets/platform.png");
	platform.addComponent<ColliderComponent>("wall");
}

void Game::handleEvents()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;

	default:
		break;
	}
}

void Game::update()
{	
	manager.refresh();
	manager.update();
	/*for (auto cc : colliders)
	{
		(Collision::AABB(player.getComponent<ColliderComponent>(), *cc));
	}*/
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		tile2.getComponent<ColliderComponent>().collider))
	{
		player.getComponent<TransformComponent>().velocity.y = 0;
		player.getComponent<TransformComponent>().position.y = player.getComponent<TransformComponent>().position.y - 3;
	}
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		tile1.getComponent<ColliderComponent>().collider))
	{
		player.getComponent<TransformComponent>().velocity.y = 0;
		player.getComponent<TransformComponent>().position.y = player.getComponent<TransformComponent>().position.y - 3;
	}
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		platform.getComponent<ColliderComponent>().collider))
	{
		player.getComponent<TransformComponent>().velocity.y = 0;
		player.getComponent<TransformComponent>().position.y = player.getComponent<TransformComponent>().position.y - 3;
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);
	//map->DrawMap();
	manager.draw();
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned"<<std::endl;
}

bool Game::running()
{
	return isRunning;
}