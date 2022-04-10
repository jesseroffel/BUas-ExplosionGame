#include "entity.h"


Entity::Entity()
{
}

Entity::Entity(std::string path, SDL_Renderer* rRend)
{

}


Entity::~Entity()
{
}



void Entity::Update()
{

}
void Entity::Render(SDL_Renderer* rRend)
{

}

bool Entity::checkCollision(SDL_Rect a, SDL_Rect b)
{
	//the insides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//calculate the sides of rect b
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//if any sides from a are outside b
	if (bottomA <= topB) { return false; }
	if (topA >= bottomB) { return false; }
	if (rightA <= leftB) { return false; }
	if (leftA >= rightB) { return false; }

	//if A and B collides
	return true;
}

bool Entity::checkCollision(Circle& a, SDL_Rect b)
{
	//closest point to collision box
	int cX, cY;

	//if closest x offset
	if (a.x < b.x) { cX = b.x; }
	else if (a.x > b.x + b.w) { cX = b.x + b.w; }
	else { cX = a.x; }

	//find closes y offset
	if (a.y < b.y) { cY = b.y; }
	else if (a.y > b.y + b.h) { cY = b.y + b.h; }
	else { cY = a.y; }

	//if closest point is inside the circle
	if (distanceSquared(a.x, a.y, cX, cY) < a.r * a.r) { return true; }
	return false;
}


bool Entity::checkCollision(Circle& a, Circle& b)
{
	//calculate total radius squared
	int totalRadiusSquared = a.r + b.r;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;
	//if the distance btween the centers of the circles is less than the sum of their radius
	if (distanceSquared(a.x, a.y, b.x, b.y) < (totalRadiusSquared)) { return true; }
	return false;
}


double Entity::distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}

bool Entity::getActive() { return mActive; }

int Entity::getPosX() { return mPosX; }
int Entity::getPosY() { return mPosY; }