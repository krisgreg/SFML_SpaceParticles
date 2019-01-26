#include "Particle.h"

#include <iostream>
#include <cmath>

#include "Consts.h"

Particle::Particle()
{
	Pos.x = rand() % 1280 * SCREENSPACE_MULTIPLIER.x - (1280 * SCREENSPACE_MULTIPLIER.x) / 2;
	Pos.y = rand() % 1280 * SCREENSPACE_MULTIPLIER.y - (1280 * SCREENSPACE_MULTIPLIER.y) / 2;
	//if (Pos.x <= -10000.f)
	//{
	//	std::cout << "ERROR! Pos.x=" << Pos.x << std::endl;
	//}
	//if (Pos.y <= -10000.f)
	//{
	//	std::cout << "ERROR! Pos.y=" << Pos.y << std::endl;
	//}
	Velocity = glm::vec2(0.0f, 0.0f);
	Acceleration = glm::vec2(0.0f, 0.0f);
	Radius = 7.0f;
	Body.setRadius(Radius);
	Body.setFillColor(sf::Color::White);
	Body.setPosition(Pos.x, Pos.y);
	Body.setOrigin(Radius, Radius);
	Volume = (4 / 3) * PI * (pow(Radius, 3));
	Density = 1;
	Mass = Volume * Density;
}

Particle::Particle(glm::vec2 Pos, glm::vec2 Vel, double Radius)
{
	this->Pos = Pos;
	this->Velocity = Vel;
	this->Radius = Radius;
	Acceleration = glm::vec2(0.0f, 0.0f);
	Body.setRadius(Radius);
	Body.setFillColor(sf::Color::White);
	Body.setPosition(Pos.x, Pos.y);
	Body.setOrigin(Radius, Radius);
	Volume = (4 / 3) * PI * (pow(Radius, 3));
	Density = 1;
	Mass = Volume * Density;
}

Particle::Particle(glm::vec2 Pos, glm::vec2 Vel, double Volume, double Density, sf::Color newColor)
{
	this->Pos = Pos;
	this->Velocity = Vel;
	Acceleration = glm::vec2(0.0f, 0.0f);
	Radius = pow((Volume / PI) * (3.f / 4.f), (1.f / 3.f));
	if (Radius <= 0.f)
	{
		std::cout << "RADIUS ERROR! RADIUS=" << Radius << std::endl;
	}
	Body.setRadius(Radius);
	Body.setFillColor(newColor);
	Body.setPosition(Pos.x, Pos.y);
	Body.setOrigin(Radius, Radius);
	this->Volume = Volume;
	this->Density = Density;
	Mass = Volume * Density;
}


Particle::~Particle()
{
}

void Particle::Draw(sf::RenderWindow *window)
{
	window->draw(Body);
	if (DRAW_VELOCITY_LINES)
	{
		window->draw(VelocityLine, 2, sf::Lines);
	}
}

void Particle::Update(sf::Time dt)
{
	Velocity += Acceleration * (1.f * dt.asSeconds());
	Pos += Velocity * (1.f * dt.asSeconds());
	Body.setPosition(Pos.x, Pos.y);
	
	VelocityLine[0] = sf::Vertex(sf::Vector2f(Pos.x, Pos.y), sf::Color::Red);
	VelocityLine[1] = sf::Vertex(sf::Vector2f(Pos.x + (Velocity.x)*1, Pos.y + (Velocity.y)*1), sf::Color::Red);

	Acceleration = glm::vec2(0, 0);
}

void Particle::CalculateGravityForceFrom(Particle &other)
{
	if (Pos != other.Pos)
	{
		double distanceToOther = GetDistanceTo(other.Pos);

		double forceDueToGravity = (GRAVITATIONAL_CONSTANT * Mass * other.Mass) / pow(distanceToOther, 2);
		//std::cout << "force: " << forceDueToGravity << "N" << std::endl;
		double accelerationDueToGravity = forceDueToGravity / Mass;
		//std::cout << "acceleration: " << accelerationDueToGravity << "Units/s^2" << std::endl;

		//std::cout << "force=" << forceDueToGravity << " Mass=" << Mass << " Acc=" << accelerationDueToGravity << std::endl;
		
		//double angle = atan2(Pos.y - other.Pos.y, Pos.x - other.Pos.x) * (180.0 / 3.141592653589793238463f);
 		double angle = atan2(Pos.y - other.Pos.y, Pos.x - other.Pos.x);

		//std::cout << "Angle=" << angle << std::endl;

		glm::vec2 newAcceleration;
		newAcceleration.y = accelerationDueToGravity * sin(angle) * (-1);
		newAcceleration.x = accelerationDueToGravity * cos(angle) * (-1);

		//std::cout << "Total acceleration=" << accelerationDueToGravity << "\t xAcc=" << newAcceleration.x << "\t yAcc=" << newAcceleration.y << std::endl;

		AddAcceleration(newAcceleration);
	}
}

double Particle::GetDistanceTo(glm::vec2 otherPos)
{
	return sqrt(pow(abs(Pos.x - otherPos.x), 2) + pow(abs(Pos.y - otherPos.y), 2));
}

void Particle::AddAcceleration(glm::vec2 newAcceleration)
{
	Acceleration += newAcceleration;
}

bool Particle::OverlapsWithParticle(Particle & other)
{
	return abs(pow(Pos.x - other.Pos.x, 2) + pow(Pos.y - other.Pos.y, 2)) <= pow(Radius + other.Radius, 2);
}

void Particle::markForDeletion()
{
	bMarkedForDeletion = true;
}

bool Particle::GetbMarkedForDeletion()
{
	return bMarkedForDeletion;
}

void Particle::SetPos(glm::vec2 newPos)
{
	Pos = newPos;
	Body.setPosition(Pos.x, Pos.y);
}

glm::vec2 Particle::GetPos()
{
	return Pos;
}

void Particle::SetVelocity(glm::vec2 newVelocity)
{
	Velocity = newVelocity;
}

void Particle::AddVelocity(glm::vec2 newVelocity)
{
	Velocity += newVelocity;
}

glm::vec2 Particle::GetVelocity()
{
	return Velocity;
}

double Particle::GetRadius()
{
	return Radius;
}

double Particle::GetVolume()
{
	return Volume;
}

double Particle::GetMass()
{
	return Mass;
}
