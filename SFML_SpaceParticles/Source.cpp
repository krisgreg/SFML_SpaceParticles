#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Particle.h"
#include "Consts.h"

int main()
{
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Space Particles");
	sf::View mainView(sf::Vector2f(0, 0), sf::Vector2f(1280 * SCREENSPACE_MULTIPLIER.x, 720 * SCREENSPACE_MULTIPLIER.y));
	window.setView(mainView);

	std::vector<Particle*> Particles;
	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		Particles.push_back(new Particle());
	}

	//4 Particles in a pluss formation
	//Particles.push_back(new Particle(glm::vec2(1280 / 2 - 200, 720 / 2), glm::vec2(0, 0), 10000.f, 1.f));
	//Particles.push_back(new Particle(glm::vec2(1280 / 2 + 200, 720 / 2), glm::vec2(0, 0), 10000.f, 1.f));
	//Particles.push_back(new Particle(glm::vec2(1280 / 2, 720 / 2 + 200), glm::vec2(0, 0), 10000.f, 1.f));
	//Particles.push_back(new Particle(glm::vec2(1280 / 2, 720 / 2 - 200), glm::vec2(0, 0), 10000.f, 1.f));

	//Density test
	//Particles.push_back(new Particle(glm::vec2(1280 / 2 - 200, 720 / 2), glm::vec2(0, 0), 10000.f, 10.f, sf::Color::Yellow));
	//Particles.push_back(new Particle(glm::vec2(1280 / 2 + 200, 720 / 2), glm::vec2(0, 0), 10000.f, 1.f));

	//Solar system test
	//Particles.push_back(new Particle(glm::vec2(1280 / 2, 720 / 2), glm::vec2(0, 0), 100000.f, 50.f, sf::Color::Yellow));
	//Particles.push_back(new Particle(glm::vec2(1280 / 2 + 300, 720 / 2), glm::vec2(0, 700), 10000.f, 1.f));
	//Particles.push_back(new Particle(glm::vec2(1280 / 2 - 300, 720 / 2), glm::vec2(0, -1000), 10000.f, 1.f));

	/*
	Particles.push_back(new Particle(glm::vec2(640, 360), glm::vec2(0, 0), 10.f));
	Particles.front()->markForDeletion();

	auto i = Particles.begin();
	while (i != Particles.end()) {
		// Do some stuff
		if ((*i)->GetbMarkedForDeletion())
			i = Particles.erase(i);
		else
			++i;
	}

	Particles.push_back(new Particle(glm::vec2(640, 180), glm::vec2(0, 0), 10.f));
	//*/

	std::vector<Particle*> newParticles;

	//Particles.front()->CalculateGravityForceFrom(*Particles.back());

	// Two close particle testcase
	//Particle p1;
	//p1.SetPos(glm::vec2(500.0f, 500.0f));
	//Particle p2;
	//p2.SetPos(glm::vec2(512.0f, 500.0f));
	//p1.CalculateGravityForceFrom(p2);
	// add this in the game loop to test:
	//p1.Draw(&window);
	//p2.Draw(&window);

	sf::Clock clock;
	sf::Clock ticker;
	int frameCounter = 0;
	while (window.isOpen())
	{
		frameCounter++;
		sf::Time dt = clock.restart();
		//old fps counter (calculates fps each frame)
		//if (dt.asMicroseconds() != 0)
		//{
		//	std::cout << 1000000 / dt.asMicroseconds() << std::endl;
		//}

		//new fps counter (calculates fps each second)
		if (ticker.getElapsedTime().asSeconds() >= 1.0f)
		{
			std::cout << "fps=" << frameCounter << std::endl;
			frameCounter = 0;
			ticker.restart();

			std::cout << "numParticles=" << Particles.size() << std::endl;
			//std::cout << "particle 1: x=" << Particles.front()->GetPos().x << " y=" << Particles.front()->GetPos().y << std::endl;

			double highestVolume = 0;
			double highestRadius = 0;
			for (auto i : Particles)
			{
				if (i->GetVolume() > highestVolume)
				{
					highestVolume = i->GetVolume();
				}

				if (i->GetRadius() > highestRadius)
				{
					highestRadius = i->GetRadius();
				}
			}

			//std::cout << "highestVolume=" << highestVolume << std::endl;
			//std::cout << "highestRadius=" << highestRadius << std::endl;

			// spawn NUM_PARTICLES particles per sec
			if (KEEP_PRODUCING_PARTICLES_PER_TICK)
			{
				for (int i = 0; i < NUM_PARTICLES; i++)
				{
					Particles.push_back(new Particle());
				}
			}
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		if (KEEP_PRODUCING_PARTICLES)
		{
			for (int i = Particles.size(); i < NUM_PARTICLES; i++)
			{
				Particles.push_back(new Particle());
			}
		}

		for (auto &i : Particles)
		{
			for (auto &j : Particles)
			{
				i->CalculateGravityForceFrom(*j);
			}
		}

		for (auto i : Particles)
		{
			i->Update(dt);
		}

		// Options:
		// handle collisions
		// merge
		std::string Collision = "handle collision";
		
		if (Collision == "handle collision")
		{
			for (auto &i : Particles)
			{
				for (auto &j : Particles)
				{
					if (i != j && i->GetbMarkedForDeletion() != true && j->GetbMarkedForDeletion() != true)
					{
						if (i->OverlapsWithParticle(*j))
						{
							//std::cout << "frame: " << frameCounter << std::endl;
							double distanceBetween = i->GetDistanceTo(j->GetPos());
							double overlap = 0.5f * (distanceBetween - i->GetRadius() - j->GetRadius());

							glm::vec2 newPos = i->GetPos();
							newPos.x -= overlap * (i->GetPos().x - j->GetPos().x) / distanceBetween;
							newPos.y -= overlap * (i->GetPos().y - j->GetPos().y) / distanceBetween;

							glm::vec2 newOtherPos = j->GetPos();
							newOtherPos.x += overlap * (i->GetPos().x - j->GetPos().x) / distanceBetween;
							newOtherPos.y += overlap * (i->GetPos().y - j->GetPos().y) / distanceBetween;

							i->SetPos(newPos);
							j->SetPos(newOtherPos);

							double IParticleMass = i->GetMass();
							double JParticleMass = j->GetMass();
							double TotalMass = IParticleMass + JParticleMass;

							glm::vec2 newParticlePos(i->GetPos().x * (IParticleMass / TotalMass) + j->GetPos().x * (JParticleMass / TotalMass), i->GetPos().y * (IParticleMass / TotalMass) + j->GetPos().y * (JParticleMass / TotalMass));
							glm::vec2 newParticleVel(i->GetVelocity().x * (IParticleMass / TotalMass) + j->GetVelocity().x * (JParticleMass / TotalMass), i->GetVelocity().y * (IParticleMass / TotalMass) + j->GetVelocity().y * (JParticleMass / TotalMass));
							//double newRadius = pow(((i->GetVolume() + j->GetVolume()) / PI) * (3.f / 4.f), (1.f / 3.f));
							//if (i->GetVolume() < 1000.f || j->GetVolume() < 1000.f)
							//{
							//	std::cout << "Volumes: I=" << i->GetVolume() << " J=" << j->GetVolume() << std::endl;
							//}
							double newVolume = i->GetVolume() + j->GetVolume();
							//std::cout << "newVolume=" << newVolume << std::endl;
							//std::cout << "newRadius=" << newRadius << std::endl;
							//std::cout << "newParticlePos: x=" << newParticlePos.x << " y=" << newParticlePos.y << std::endl;
							
							//glm::vec2 OldIVelocity = i->GetVelocity();
							//glm::vec2 OldJVelocity = j->GetVelocity();
							//i->AddVelocity(OldJVelocity);
							//j->AddVelocity(OldIVelocity);
							
							newParticles.push_back(new Particle(newParticlePos, newParticleVel, newVolume, 1.f));
							i->markForDeletion();
							j->markForDeletion();

							//std::cout << "1: newParticleVolume=" << newParticles.back()->GetVolume() << std::endl;
						}
					}
				}
			}
		}
		//if (newParticles.size() != 0)
		//	std::cout << "2: newParticleVolume=" << newParticles.back()->GetVolume() << std::endl;
		/*
		else if (Collision == "merge")
		{
			for (auto &i : Particles)
			{
				for (auto &j : Particles)
				{
					if (i != j)
					{
						if (i->OverlapsWithParticle(*j))
						{
							glm::vec2 newPos((i->GetPos().x + j->GetPos().x) / 2, (i->GetPos().y + j->GetPos().y) / 2);
							glm::vec2 newVel((i->GetVelocity().x + j->GetVelocity().x) / 2, (i->GetVelocity().y + j->GetVelocity().y) / 2);
							double newRadius = pow(((i->GetVolume() + j->GetVolume()) / PI) * (3.f / 4.f), (1.f / 3.f));
							Particles.push_back(new Particle(newPos, newVel, newRadius));
							i->markForDeletion();
							j->markForDeletion();
						}
					}
				}
			}
		}
		*/
		

		/*
		Particles.erase(
			std::remove_if(
				Particles.begin(),
				Particles.end(),
				[](Particle* element) -> bool 
				{
					if (element->GetbMarkedForDeletion())
					{
							std::cout << " :marked for deletion" << std::endl;
							return true;
					}
					else
					{
						return false;
					}
				}
			),
			Particles.end()
		);
		//*/
		
		///*
		auto i = Particles.begin();
		while (i != Particles.end()) {
			// Do some stuff
			if ((*i)->GetbMarkedForDeletion())
				i = Particles.erase(i);
			else
				++i;
		}
		//*/

		for (auto &i : newParticles)
		{
			Particles.push_back(new Particle(i->GetPos(), i->GetVelocity(), i->GetVolume(), 1.f));
		}
		newParticles.clear();

		window.clear();
		
		for (auto i : Particles)
		{
			i->Draw(&window);
		}

		window.display();
	}

	return 0;
}