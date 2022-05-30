#include<SFML/Graphics.hpp>
#include<box2d/box2d.h>
#include"values.h"
b2World world(b2Vec2(0, -9.8));

const struct Box
{
	float width;
	float height;
	sf::Color color;
	b2Body* body;
};

const struct Circle
{
	
	float radius;
	sf::Color color;
	b2Body* body;
};


class level
{
	
public:
	Box createGround(float x, float y, float width, float height, sf::Color color);
	Box createBox(float x, float y, float width, float height, float density, float friction, sf::Color color);
	Circle createcircle(float x, float y, float radius, float density, float friction, sf::Color color);
	void rendercircle(sf::RenderWindow& w, std::vector<Box>& circles);
	void render(sf::RenderWindow& w, std::vector<Box>& boxes);
};	

Box level::createBox(float x, float y, float width, float height, float density, float friction, sf::Color color)
{
	// Body definition
	b2BodyDef boxBodyDef;
	boxBodyDef.position.Set(x / PPM, y / PPM);
	boxBodyDef.type = b2_dynamicBody;

	// Shape definition
	b2PolygonShape boxShape;
	boxShape.SetAsBox(width / 2 / PPM, height / 2 / PPM);

	// Fixture definition
	b2FixtureDef fixtureDef;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.shape = &boxShape;

	// Now we have a body for our Box object
	b2Body* boxBody = world.CreateBody(&boxBodyDef);
	// Lastly, assign the fixture
	boxBody->CreateFixture(&fixtureDef);

	return Box{ width, height, color, boxBody };
}

Box level::createGround(float x, float y, float width, float height, sf::Color color)
{
	// Static body definition
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(x / PPM, y / PPM);

	// Shape definition
	b2PolygonShape groundBox;
	groundBox.SetAsBox(width / 2 / PPM, height / 2 / PPM);

	// Now we have a body for our Box object
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	// For a static body, we don't need a custom fixture definition, this will do:
	groundBody->CreateFixture(&groundBox, 0.0f);

	return Box{ width, height, color, groundBody };
}


Circle level::createcircle(float x, float y, float radius, float density, float friction, sf::Color color)
{
	// Body definition
	b2BodyDef boxBodyDef;
	boxBodyDef.position.Set(x / PPM, y / PPM);
	boxBodyDef.type = b2_dynamicBody;

	// Shape definition
	b2CircleShape circle;
	circle.m_p(radius / 2 / PPM);
	circle.m_radius = radius;

	// Fixture definition
	b2FixtureDef fixtureDef;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.shape = &circle;

	// Now we have a body for our Box object
	b2Body* boxBody = world.CreateBody(&boxBodyDef);
	// Lastly, assign the fixture
	boxBody->CreateFixture(&fixtureDef);

	return Circle{ radius, color, boxBody };
}

void level::rendercircle(sf::RenderWindow& w, std::vector<Box>& circles)
{
	for (const auto& circle : circles)
	{
		sf::CircleShape circ;


		circ.setPosition(circle.body->GetPosition().x * PPM, WINDOW_HEIGHT - (circle.body->GetPosition().y * PPM));


		circ.setOrigin(circle.width / 2, circle.width / 2);




		circ.setRotation(-1 * circle.body->GetAngle() * DEG_PER_RAD);

		circ.setFillColor(circle.color);
		w.draw(circ);
	}
	w.display();
}

void level::render(sf::RenderWindow& w, std::vector<Box>& boxes)
{

	w.clear();
	for (const auto& box : boxes)
	{
		sf::RectangleShape rect;

	
		rect.setPosition(box.body->GetPosition().x * PPM, WINDOW_HEIGHT - (box.body->GetPosition().y * PPM));

		
		rect.setOrigin(box.width / 2, box.height / 2);

		rect.setSize(sf::Vector2f(box.width, box.height));

		
		rect.setRotation(-1 * box.body->GetAngle() * DEG_PER_RAD);

		rect.setFillColor(box.color);
		w.draw(rect);
	}



	w.display();
}



int main()
{
	// Setup SFML window
	level l1;

	sf::RenderWindow w(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML + Box2D", sf::Style::Close);
	w.setFramerateLimit(60);
	

	
	std::vector<Box> boxes;
	std::vector<Box> circles;

	
	boxes.push_back(l1.createGround(300, 20, 1000, 100, sf::Color::Magenta));
	auto&& circi = l1.createcircle(100,200,20, 1.f, 0.7f, sf::Color::Red);
	circles.push_back(circi);

	while (w.isOpen())
	{
		sf::Event evnt;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{

			float localPositionx = sf::Mouse::getPosition(w).x;
			float localPositiony = sf::Mouse::getPosition(w).y;

			auto&& box = l1.createBox(localPositionx, WINDOW_HEIGHT - localPositiony, 15.f, 15.f, 1.f, 0.7f, sf::Color::Red);
			boxes.push_back(box);

		}
		while (w.pollEvent(evnt))
		{
			switch (evnt.type) {
			case sf::Event::Closed:
				w.close();
			case sf::Event::TextEntered:
				if (evnt.text.unicode < 128) {
					printf("%c", evnt.text.unicode);
				}

			}

		}

		// Update the world, standard arguments
		world.Step(1 / 60.f, 6, 3);
		
		// Render everything
		l1.render(w, boxes);
		l1.rendercircle(w, circles);
		


	}

	return 0;
}