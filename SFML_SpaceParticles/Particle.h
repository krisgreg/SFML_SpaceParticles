#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <SFML/Graphics.hpp>

class Particle
{
private:
	glm::vec2 Pos;
	glm::vec2 Velocity;
	glm::vec2 Acceleration;
	sf::CircleShape Body;
	sf::Vertex VelocityLine[2];
	double Radius;
	double Volume;
	double Density;
	double Mass;
	bool bMarkedForDeletion = false;
public:
	Particle();
	Particle(glm::vec2 Pos, glm::vec2 Vel, double Radius);
	Particle(glm::vec2 Pos, glm::vec2 Vel, double Volume, double density, sf::Color newColor = sf::Color::White);
	~Particle();

	void Draw(sf::RenderWindow *window);
	void Update(sf::Time dt);
	void CalculateGravityForceFrom(Particle &other);
	double GetDistanceTo(glm::vec2 otherPos);
	void AddAcceleration(glm::vec2 newAcceleration);
	bool OverlapsWithParticle(Particle &other);

	void markForDeletion();
	bool GetbMarkedForDeletion();
	void SetPos(glm::vec2 newPos);
	glm::vec2 GetPos();
	void SetVelocity(glm::vec2 newVelocity);
	void AddVelocity(glm::vec2 newVelocity);
	glm::vec2 GetVelocity();
	double GetRadius();
	double GetVolume();
	double GetMass();
};

