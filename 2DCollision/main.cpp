#include <iostream>
#include <SFML/Graphics.hpp>
#define TINYC2_IMPL
#include <tinyc2.h>
#include <AnimatedSprite.h>
#include <GameObject.h>
#include <Player.h>
#include <NPC.h>
#include <Input.h>
#include <Debug.h>

using namespace std;

int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	// Load a NPC's sprites to display
	sf::Texture npc_texture;
	if (!npc_texture.loadFromFile("assets\\grid.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Load a mouse texture to display
	sf::Texture player_texture;
	if (!player_texture.loadFromFile("assets\\player.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Setup NPC's Default Animated Sprite
	AnimatedSprite npc_animated_sprite(npc_texture);
	npc_animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup Players Default Animated Sprite
	AnimatedSprite player_animated_sprite(player_texture);
	player_animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup the NPC
	GameObject &npc = NPC(npc_animated_sprite);

	// Setup the Player
	GameObject &player = Player(player_animated_sprite);

	//Setup NPC AABB
	c2AABB aabb_npc;
	aabb_npc.min = c2V(npc.getAnimatedSprite().getPosition().x, npc.getAnimatedSprite().getPosition().y);
	aabb_npc.max = c2V(
		npc.getAnimatedSprite().getPosition().x +
		npc.getAnimatedSprite().getGlobalBounds().width, 
		npc.getAnimatedSprite().getPosition().y +
		npc.getAnimatedSprite().getGlobalBounds().height);

	//Setup Player AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(player.getAnimatedSprite().getPosition().x, player.getAnimatedSprite().getPosition().y);
	aabb_player.max = c2V(player.getAnimatedSprite().getGlobalBounds().width / 6, player.getAnimatedSprite().getGlobalBounds().width / 6);


	// Initialize Input
	Input input;

	// Collision result
	int resultOne = 0;
	int resultTwo = 0;		// for circle
	int resultThree = 0;	// for polygon
	int resultFour = 0;		// for capsule


	// Direction of movement of NPC
	sf::Vector2f direction(0.01f, 0.02f);
	
	//Circle========================================
	sf::CircleShape circle;
	circle.setPosition(0, 50);
	circle.setRadius(40);
	circle.setOrigin(40, 40);	// set origin because c2Circle works with center position
	circle.setFillColor(sf::Color::Green);
	c2Circle cTwoCircle;
	c2v cCirclePos;
	cTwoCircle.r = circle.getRadius();

	//Polygon=====================================
	sf::ConvexShape polygon;
	polygon.setPointCount(3);
	polygon.setOrigin(0, 0);
	polygon.setFillColor(sf::Color::Green);
	polygon.setPoint(0, sf::Vector2f(40, 40));
	polygon.setPoint(1, sf::Vector2f(40, 80));
	polygon.setPoint(2, sf::Vector2f(80, 40));
	
	c2Poly cPolygon;
	cPolygon.count = polygon.getPointCount();
	//p1
	cPolygon.verts[0] = c2V(polygon.getPoint(0).x, polygon.getPoint(0).y);
	//p2
	cPolygon.verts[1] = c2V(polygon.getPoint(1).x, polygon.getPoint(1).y);
	//p3
	cPolygon.verts[2] = c2V(polygon.getPoint(2).x, polygon.getPoint(2).y);


	//Capsule====================================
	sf::CircleShape capsuleCircleOne;
	capsuleCircleOne.setPosition(50, 50);
	capsuleCircleOne.setRadius(25);
	capsuleCircleOne.setOrigin(capsuleCircleOne.getRadius(), capsuleCircleOne.getRadius());	
	capsuleCircleOne.setFillColor(sf::Color::Blue);

	sf::CircleShape capsuleCircleTwo;
	//capsuleCircleTwo.setPosition(0, 50);
	capsuleCircleTwo.setRadius(capsuleCircleOne.getRadius());
	capsuleCircleTwo.setOrigin(capsuleCircleOne.getRadius(), capsuleCircleOne.getRadius());
	capsuleCircleTwo.setFillColor(sf::Color::Blue);

	sf::RectangleShape capsuleRectangle;
	capsuleRectangle.setFillColor(sf::Color::Blue);
	capsuleRectangle.setSize(sf::Vector2f(70, capsuleCircleOne.getRadius() * 2));
	
	c2Capsule c2capsule;
	c2capsule.a = c2V(capsuleCircleOne.getPosition().x, capsuleCircleOne.getPosition().y);
	c2capsule.b = c2V(capsuleCircleTwo.getPosition().x, capsuleCircleTwo.getPosition().y);
	c2capsule.r = capsuleCircleOne.getRadius();

	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		player.getAnimatedSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		
		// Move The NPC
		sf::Vector2f move_to(npc.getAnimatedSprite().getPosition().x + direction.x, npc.getAnimatedSprite().getPosition().y + direction.y);

		if (move_to.x < 0) {
			direction.x *= -1;
			move_to.x = 0; // +npc.getAnimatedSprite().getGlobalBounds().width;
		}
		else if (move_to.x + npc.getAnimatedSprite().getGlobalBounds().width >= 800) { 
			direction.x *= -1;
			move_to.x = 800 - npc.getAnimatedSprite().getGlobalBounds().width;
		}
		else if (move_to.y < 0) { 
			direction.y *= -1;
			move_to.y = 0; // +npc.getAnimatedSprite().getGlobalBounds().height;
		}
		else if (move_to.y + npc.getAnimatedSprite().getGlobalBounds().height >= 600) {
			direction.y *= -1;
			move_to.y = 600 - npc.getAnimatedSprite().getGlobalBounds().height;
		}
		
		npc.getAnimatedSprite().setPosition(move_to);
		circle.setPosition(move_to + sf::Vector2f(130, -40));
		//polygon.setPosition(move_to + sf::Vector2f(190, -40));
		//p1
		cPolygon.verts[0].x = cPolygon.verts[0].x + 0.01;
		cPolygon.verts[0].y = cPolygon.verts[0].y + 0.01;
		//p2
		cPolygon.verts[1].x = cPolygon.verts[1].x + 0.01;
		cPolygon.verts[1].y = cPolygon.verts[1].y + 0.01;
		//p3
		cPolygon.verts[2].x = cPolygon.verts[2].x + 0.01;
		cPolygon.verts[2].y = cPolygon.verts[2].y + 0.01;
		//set sf::polygon points
		polygon.setPoint(0, sf::Vector2f(cPolygon.verts[0].x, cPolygon.verts[0].y));
		polygon.setPoint(1, sf::Vector2f(cPolygon.verts[1].x, cPolygon.verts[1].y));
		polygon.setPoint(2, sf::Vector2f(cPolygon.verts[2].x, cPolygon.verts[2].y));

		// set capsule parts

		capsuleRectangle.setPosition(capsuleCircleOne.getPosition().x, capsuleCircleOne.getPosition().y - capsuleCircleOne.getRadius());
		capsuleCircleTwo.setPosition(capsuleRectangle.getPosition().x + capsuleRectangle.getSize().x,
			capsuleRectangle.getPosition().y + capsuleCircleOne.getRadius());

		// Update NPC AABB set x and y
		aabb_npc.min = c2V(
			npc.getAnimatedSprite().getPosition().x, 
			npc.getAnimatedSprite().getPosition().y
		);

		aabb_npc.max = c2V(
			npc.getAnimatedSprite().getPosition().x +
			npc.getAnimatedSprite().getGlobalBounds().width,
			npc.getAnimatedSprite().getPosition().y +
			npc.getAnimatedSprite().getGlobalBounds().height
		);

		// Update Player AABB
		aabb_player.min = c2V(
			player.getAnimatedSprite().getPosition().x, 
			player.getAnimatedSprite().getPosition().y
		);
		aabb_player.max = c2V(
			player.getAnimatedSprite().getPosition().x +
			player.getAnimatedSprite().getGlobalBounds().width, 
			player.getAnimatedSprite().getPosition().y + 
			player.getAnimatedSprite().getGlobalBounds().height
		);

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				// Close window : exit
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					input.setCurrent(Input::Action::LEFT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					input.setCurrent(Input::Action::RIGHT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					input.setCurrent(Input::Action::UP);
				}
				break;
			default:
				input.setCurrent(Input::Action::IDLE);
				break;
			}
		}

		// Handle input to Player
		player.handleInput(input);

		// Update the Player
		player.update();

		// Update the Player
		npc.update();

		// Check for collisions
		resultOne = c2AABBtoAABB(aabb_player, aabb_npc);
		
		cCirclePos.x = circle.getPosition().x;
		cCirclePos.y = circle.getPosition().y;
		cTwoCircle.p = cCirclePos;
		resultTwo = c2CircletoAABB(cTwoCircle, aabb_player);

		resultThree = c2AABBtoPoly(aabb_player, &cPolygon, NULL);

		//capsule workings..
		c2capsule.a = c2V(capsuleCircleOne.getPosition().x, capsuleCircleOne.getPosition().y);
		c2capsule.b = c2V(capsuleCircleTwo.getPosition().x, capsuleCircleTwo.getPosition().y);
		c2capsule.r = capsuleCircleOne.getRadius();
		resultFour = c2AABBtoCapsule(aabb_player, c2capsule);

		cout << ((resultOne != 0) ? ("Collision") : "") << endl;
		if (resultOne || resultTwo || resultThree || resultFour){
			player.getAnimatedSprite().setColor(sf::Color(255,0,0));
		}
		else {
			player.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
		}

		// Clear screen
		window.clear();

		//draw circle
		window.draw(circle);

		// Draw the Players Current Animated Sprite
		window.draw(player.getAnimatedSprite());

		// Draw the NPC's Current Animated Sprite
		window.draw(npc.getAnimatedSprite());

		window.draw(polygon);

		window.draw(capsuleCircleOne);
		window.draw(capsuleCircleTwo);
		window.draw(capsuleRectangle);

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};