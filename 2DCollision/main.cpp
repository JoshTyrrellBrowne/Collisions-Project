/// <summary>
/// Author:Josh Tyrrell Browne
/// Date: Nov 2018
/// </summary>

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

	sf::Vector2f offScreen{ -100,-100 };
	int playerType{ 0 };
	float polyMovement{ 0.05f };

	//player sf::rectangle
	sf::RectangleShape playerSquare;
	playerSquare.setFillColor(sf::Color::Green);
	playerSquare.setSize(sf::Vector2f(40, 40));

	//player sf::circle
	sf::CircleShape playerCircle;
	playerCircle.setFillColor(sf::Color::Green);
	playerCircle.setRadius(20);
	playerCircle.setOrigin(playerCircle.getRadius(), playerCircle.getRadius());
	playerCircle.setPosition(offScreen);
	c2Circle cPlayerCircle;
	cPlayerCircle.r = playerCircle.getRadius();

	//player capsule
	sf::CircleShape playerCapsuleCircleOne;
	playerCapsuleCircleOne.setPosition(offScreen);
	playerCapsuleCircleOne.setRadius(25);
	playerCapsuleCircleOne.setOrigin(playerCapsuleCircleOne.getRadius(), playerCapsuleCircleOne.getRadius());
	playerCapsuleCircleOne.setFillColor(sf::Color::Green);

	sf::CircleShape playerCapsuleCircleTwo;
	playerCapsuleCircleTwo.setPosition(offScreen);
	playerCapsuleCircleTwo.setRadius(playerCapsuleCircleOne.getRadius());
	playerCapsuleCircleTwo.setOrigin(playerCapsuleCircleOne.getRadius(), playerCapsuleCircleOne.getRadius());
	playerCapsuleCircleTwo.setFillColor(sf::Color::Green);

	sf::RectangleShape playerCapsuleRectangle;
	playerCapsuleRectangle.setFillColor(sf::Color::Green);
	playerCapsuleRectangle.setSize(sf::Vector2f(70, playerCapsuleCircleOne.getRadius() * 2));

	c2Capsule playerC2Capsule;
	playerC2Capsule.a = c2V(playerCapsuleCircleOne.getPosition().x, playerCapsuleCircleOne.getPosition().y);
	playerC2Capsule.b = c2V(playerCapsuleCircleTwo.getPosition().x, playerCapsuleCircleTwo.getPosition().y);
	playerC2Capsule.r = playerCapsuleCircleOne.getRadius();

	//player ray 
	sf::VertexArray vertArray;
	sf::Vertex vertOne{sf::Vector2f(20,20)};
	sf::Vertex vertTwo;
	vertArray.append(vertOne);
	vertArray.append(vertTwo);
	vertArray.setPrimitiveType(sf::Lines);

	c2Ray cRay;

	//Setup NPC AABB
	c2AABB aabb_npc;

	//Setup Player AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(playerSquare.getPosition().x, playerSquare.getPosition().y);
	aabb_player.max = c2V(playerSquare.getGlobalBounds().width, playerSquare.getGlobalBounds().width);


	// Initialize Input
	Input input;

	// Collision results for playerSquare..
	int resultOneSquare = 0;
	int resultTwoSquare = 0;		// for circle
	int resultThreeSquare = 0;	// for polygon
	int resultFourSquare = 0;		// for capsule

	// Collision reults for playerCircle..
	int resultOneCircle = 0;		// for square
	int resultTwoCircle = 0;		// for capsule
	int resultThreeCircle = 0;	// for circle
	int resultFourCircle = 0;		// for poly

	// Collision reults for playerCapsule..
	int resultOneCapsule = 0;		// for square
	int resultTwoCapsule = 0;		// for capsule
	int resultThreeCapsule = 0;	// for circle
	int resultFourCapsule = 0;		// for poly

	// Collision reults for playerRay..
	int resultOneRay = 0;		// for square
	int resultTwoRay = 0;		// for capsule
	int resultThreeRay = 0;	// for circle
	int resultFourRay = 0;		// for poly


	// Direction of movement of NPC
	sf::Vector2f direction(0.01f, 0.02f);
	
	//Square========================================
	sf::RectangleShape square;
	square.setFillColor(sf::Color::Blue);
	square.setSize(sf::Vector2f(80, 80));
	square.setPosition(0, 0);

	//Circle========================================
	sf::CircleShape circle;
	circle.setPosition(0, 50);
	circle.setRadius(40);
	circle.setOrigin(40, 40);	// set origin because c2Circle works with center position
	circle.setFillColor(sf::Color::Blue);
	c2Circle cCircle;
	c2v cCirclePos;
	cCircle.r = circle.getRadius();

	//Polygon=====================================
	sf::ConvexShape polygon;
	polygon.setPointCount(3);
	polygon.setOrigin(0, 0);
	polygon.setFillColor(sf::Color::Blue);
	polygon.setPoint(0, sf::Vector2f(0, 0));
	polygon.setPoint(1, sf::Vector2f(0, 80));
	polygon.setPoint(2, sf::Vector2f(80, 0));
	
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
		switch (playerType) {
		case 0:
			playerSquare.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			playerCircle.setPosition(offScreen);
			//capsule..
			playerCapsuleCircleOne.setPosition(offScreen);
			playerCapsuleCircleTwo.setPosition(offScreen);
			playerCapsuleRectangle.setPosition(offScreen);
			//ray..
			vertOne.position = offScreen;
			vertTwo.position = offScreen;
			break;
		case 1:
			playerCircle.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			playerSquare.setPosition(offScreen);
			//capsule..
			playerCapsuleCircleOne.setPosition(offScreen);
			playerCapsuleCircleTwo.setPosition(offScreen);
			playerCapsuleRectangle.setPosition(offScreen);
			//ray..
			vertOne.position = offScreen;
			vertTwo.position = offScreen;
			break;
		case 2:
			playerCapsuleCircleOne.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			playerCapsuleRectangle.setPosition(playerCapsuleCircleOne.getPosition().x, playerCapsuleCircleOne.getPosition().y - playerCapsuleCircleOne.getRadius());
			playerCapsuleCircleTwo.setPosition(playerCapsuleRectangle.getPosition().x + playerCapsuleRectangle.getSize().x,
				playerCapsuleRectangle.getPosition().y + playerCapsuleCircleOne.getRadius());

			playerCircle.setPosition(offScreen);
			playerSquare.setPosition(offScreen);
			//ray..
			vertOne.position = offScreen;
			vertTwo.position = offScreen;
			break;
		case 3:
			// ray
			vertOne.position = sf::Vector2f{ 600,500 };
			vertTwo.position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

			playerCircle.setPosition(offScreen);
			playerSquare.setPosition(offScreen);

			//capsule..
			playerCapsuleCircleOne.setPosition(offScreen);
			playerCapsuleCircleTwo.setPosition(offScreen);
			playerCapsuleRectangle.setPosition(offScreen);
			
		}

		
		// Move The NPC
		sf::Vector2f move_to(square.getPosition().x + direction.x, square.getPosition().y + direction.y);

		if (move_to.x < 0) {
			direction.x *= -1;
			move_to.x = 0; // +npc.getAnimatedSprite().getGlobalBounds().width;
		}
		else if (move_to.x + square.getGlobalBounds().width >= 800) { 
			direction.x *= -1;
			move_to.x = 800 - square.getGlobalBounds().width;
		}
		else if (move_to.y < 0) { 
			direction.y *= -1;
			move_to.y = 0; // +npc.getAnimatedSprite().getGlobalBounds().height;
		}
		else if (move_to.y + square.getGlobalBounds().height >= 600) {
			direction.y *= -1;
			move_to.y = 600 - square.getGlobalBounds().height;
		}
		
		square.setPosition(move_to);
		circle.setPosition(move_to + sf::Vector2f(130, -40));
		//polygon.setPosition(move_to + sf::Vector2f(190, -40));  -> this way wont work i think
		if (polygon.getPoint(1).y > window.getSize().y || polygon.getPoint(0).y < 0)
		{
			polyMovement = polyMovement * -1;
		}
		//p1
		cPolygon.verts[0].x = cPolygon.verts[0].x + polyMovement; //polyMovement is 0.01 initially
		cPolygon.verts[0].y = cPolygon.verts[0].y + polyMovement;
		//p2
		cPolygon.verts[1].x = cPolygon.verts[1].x + polyMovement;
		cPolygon.verts[1].y = cPolygon.verts[1].y + polyMovement;
		//p3
		cPolygon.verts[2].x = cPolygon.verts[2].x + polyMovement;
		cPolygon.verts[2].y = cPolygon.verts[2].y + polyMovement;
		//set sf::polygon points
		polygon.setPoint(0, sf::Vector2f(cPolygon.verts[0].x, cPolygon.verts[0].y));
		polygon.setPoint(1, sf::Vector2f(cPolygon.verts[1].x, cPolygon.verts[1].y));
		polygon.setPoint(2, sf::Vector2f(cPolygon.verts[2].x, cPolygon.verts[2].y));

		// set capsule parts
		capsuleCircleOne.setPosition(move_to + sf::Vector2f(210, -40));
		capsuleRectangle.setPosition(capsuleCircleOne.getPosition().x, capsuleCircleOne.getPosition().y - capsuleCircleOne.getRadius());
		capsuleCircleTwo.setPosition(capsuleRectangle.getPosition().x + capsuleRectangle.getSize().x,
			capsuleRectangle.getPosition().y + capsuleCircleOne.getRadius());

		// Update NPC AABB set x and y
		aabb_npc.min = c2V(square.getPosition().x, square.getPosition().y);

		aabb_npc.max = c2V(square.getPosition().x + square.getGlobalBounds().width,
							square.getPosition().y + square.getGlobalBounds().height);

		// Update Player AABB
		aabb_player.min = c2V(playerSquare.getPosition().x,playerSquare.getPosition().y);
		aabb_player.max = c2V(playerSquare.getPosition().x + playerSquare.getGlobalBounds().width,
								playerSquare.getPosition().y + playerSquare.getGlobalBounds().width);

		//update player Ray..
		cRay.d = c2V(vertTwo.position.x - vertOne.position.x, vertTwo.position.y - vertOne.position.y);
		cRay.d = c2Norm(cRay.d);
		cRay.p = c2V(vertOne.position.x, vertOne.position.y);
		cRay.t = sqrt(((vertOne.position.x * vertOne.position.x) - (vertTwo.position.y * vertTwo.position.y)) + ((vertOne.position.y * vertOne.position.y) - (vertTwo.position.y * vertTwo.position.y)));
		
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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					if (playerType < 3)
					{
						playerType++;
					}
					else
					{
						playerType = 0;
					}
				}
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


		// cPlayerCircle updates..
		cPlayerCircle.p = c2V(playerCircle.getPosition().x, playerCircle.getPosition().y);
		// collision checks for playerCircle..
		resultOneCircle = c2CircletoAABB(cPlayerCircle, aabb_npc);
		resultTwoCircle = c2CircletoCapsule(cPlayerCircle, c2capsule);
		resultThreeCircle = c2CircletoCircle(cPlayerCircle, cCircle);
		resultFourCircle = c2CircletoPoly(cPlayerCircle, &cPolygon, NULL);

		// collision checks for playerCapsule..
		playerC2Capsule.a = c2V(playerCapsuleCircleOne.getPosition().x, playerCapsuleCircleOne.getPosition().y);
		playerC2Capsule.b = c2V(playerCapsuleCircleTwo.getPosition().x, playerCapsuleCircleTwo.getPosition().y);
		resultOneCapsule = c2AABBtoCapsule(aabb_npc, playerC2Capsule);
		resultTwoCapsule = c2CapsuletoCapsule(playerC2Capsule, c2capsule);
		resultThreeCapsule = c2CircletoCapsule(cCircle, playerC2Capsule);
		resultFourCapsule = c2CapsuletoPoly(playerC2Capsule, &cPolygon, NULL);

		// collision checks for playerRay..
		c2Raycast rayCast;
		resultOneRay = c2RaytoAABB(cRay, aabb_npc, &rayCast);
		resultTwoRay = c2RaytoCircle(cRay, cCircle, &rayCast);
		resultThreeRay = c2RaytoCapsule(cRay, c2capsule, &rayCast);
		resultFourRay = c2RaytoPoly(cRay, &cPolygon, NULL, &rayCast);

		// Check for collisions ==============================================
		// square to square
		resultOneSquare = c2AABBtoAABB(aabb_player, aabb_npc);
		// square to circle
		cCirclePos.x = circle.getPosition().x;
		cCirclePos.y = circle.getPosition().y;
		cCircle.p = cCirclePos;
		resultTwoSquare = c2CircletoAABB(cCircle, aabb_player);
		// square to poly
		resultThreeSquare = c2AABBtoPoly(aabb_player, &cPolygon, NULL);
		// square to capsule
		//capsule workings..
		c2capsule.a = c2V(capsuleCircleOne.getPosition().x, capsuleCircleOne.getPosition().y);
		c2capsule.b = c2V(capsuleCircleTwo.getPosition().x, capsuleCircleTwo.getPosition().y);
		c2capsule.r = capsuleCircleOne.getRadius();
		resultFourSquare = c2AABBtoCapsule(aabb_player, c2capsule);

		cout << ((resultOneSquare != 0) ? ("Collision") : "") << endl;
		
		if (resultOneSquare || resultTwoSquare || resultThreeSquare || resultFourSquare 
			|| resultOneCircle || resultTwoCircle || resultThreeCircle || resultFourCircle 
			|| resultOneCapsule || resultTwoCapsule || resultThreeCapsule|| resultFourCapsule
			|| resultOneRay || resultTwoRay || resultThreeRay)// || resultFourRay)
		{
			playerSquare.setFillColor(sf::Color(255, 0, 0));
			playerCircle.setFillColor(sf::Color(255, 0, 0));
			vertOne.color = sf::Color(255, 0, 0);
			vertTwo.color = sf::Color(255, 0, 0);
			//capsule..
			playerCapsuleCircleOne.setFillColor(sf::Color(255, 0, 0));
			playerCapsuleCircleTwo.setFillColor(sf::Color(255, 0, 0));
			playerCapsuleRectangle.setFillColor(sf::Color(255, 0, 0));
			std::cout << "Collision" << std::endl;
		}
		else {
			playerSquare.setFillColor(sf::Color(0, 255, 0));
			playerCircle.setFillColor(sf::Color(0, 255, 0));
			vertOne.color = sf::Color(0, 255, 0);
			vertTwo.color = sf::Color(0, 255, 0);
			//capsule..
			playerCapsuleCircleOne.setFillColor(sf::Color(0, 255, 0));
			playerCapsuleCircleTwo.setFillColor(sf::Color(0, 255, 0));
			playerCapsuleRectangle.setFillColor(sf::Color(0, 255, 0));
		}

		// Clear screen
		window.clear();

		//draw circle
		window.draw(circle);

		// Draw the Players Current Animated Sprite
		window.draw(playerSquare);
		window.draw(playerCircle);
		//capsule..
		window.draw(playerCapsuleCircleOne);
		window.draw(playerCapsuleCircleTwo);
		window.draw(playerCapsuleRectangle);

		// Draw the NPC's Current Animated Sprite
		window.draw(square);

		window.draw(polygon);

		window.draw(capsuleCircleOne);
		window.draw(capsuleCircleTwo);
		window.draw(capsuleRectangle);

		vertArray.clear();
		vertArray.append(vertOne);
		vertArray.append(vertTwo);
		window.draw(vertArray);

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};