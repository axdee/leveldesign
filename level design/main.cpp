#include<SFML/Graphics.hpp>
#include<box2d/box2d.h>
#include"values.h"
b2World world(b2Vec2(0,-9));

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
	void drawcircle(sf::RenderWindow& w, float x, float y, float radius, float density, float friction, sf::Color color);
	void rendercircle(sf::RenderWindow& w, std::vector<Circle>& circles);
	void render(sf::RenderWindow& w, std::vector<Box>& boxes);
	void fixedpart(sf::RenderWindow& w);
	void rectangledraw(sf::RenderWindow& w,float x, float y, float width, float height, float density, float friction, sf::Color color);
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
	circle.m_p(radius / PPM);
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

void level::drawcircle(sf::RenderWindow& w, float x, float y, float radius, float density, float friction, sf::Color color)
{
	std::vector<Circle> circles;
	auto&& circi = createcircle(x, y, radius, density, friction, color);
	circles.push_back(circi);

	rendercircle(w, circles);
}



void level::rendercircle(sf::RenderWindow& w, std::vector<Circle>& circles)
{
	for (const auto& circle : circles)
	{
		sf::CircleShape circ;


		circ.setPosition(circle.body->GetPosition().x * PPM, WINDOW_HEIGHT - (circle.body->GetPosition().y * PPM));


		circ.setOrigin(circle.radius , circle.radius );

		circ.setRadius(circle.radius);

		circ.setFillColor(circle.color);
		w.draw(circ);
	}
	
}

void level::render(sf::RenderWindow& w, std::vector<Box>& boxes)
{

	
	for (const auto& box : boxes)
	{
		sf::RectangleShape rect;

	
		rect.setPosition(box.body->GetPosition().x * PPM, WINDOW_HEIGHT - (box.body->GetPosition().y * PPM));

		
		rect.setOrigin(box.width / 2, box.height / 2);

		rect.setSize(sf::Vector2f(box.width, box.height));


		rect.setFillColor(box.color);
		w.draw(rect);
	}	
}

void level::fixedpart(sf::RenderWindow& w)
{
	std::vector<Box> boxes;
	boxes.push_back(createGround(WINDOW_WIDTH / 2, 10, WINDOW_WIDTH, 100, sf::Color::Magenta));
	boxes.push_back(createGround(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 10, WINDOW_WIDTH, 100, sf::Color::Magenta));
	boxes.push_back(createGround(10, 300, 100, WINDOW_HEIGHT, sf::Color::Magenta));
	boxes.push_back(createGround(WINDOW_WIDTH - 10, 300, 100, WINDOW_WIDTH, sf::Color::Magenta));

	boxes.push_back(createGround(450, 300, 175, 20, sf::Color::Magenta));
	render(w, boxes);
}

void level::rectangledraw(sf::RenderWindow& w,float x, float y, float width, float height, float density, float friction, sf::Color color)
{
	std::vector<Box> boxes;
	auto&& box = createBox(x, y, width, height, density, friction, color);
	boxes.push_back(box);
	render(w, boxes);
}

int main()
{
	// Setup SFML window
	level l1;

	sf::RenderWindow w(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML + Box2D", sf::Style::Close);
	w.setFramerateLimit(60);
	

	
	std::vector<Box> boxes;
	std::vector<Circle> circles;

	auto&& box = l1.createBox(400,400, 15, 15, 1.f, 0.4f, sf::Color::Red);
	boxes.push_back(box);
	
	/*boxes.push_back(l1.createGround(WINDOW_WIDTH / 2, 10, WINDOW_WIDTH, 100, sf::Color::Magenta));
	boxes.push_back(l1.createGround(WINDOW_WIDTH / 2, WINDOW_HEIGHT-10, WINDOW_WIDTH, 100, sf::Color::Magenta));
	boxes.push_back(l1.createGround( 10, 300, 100, WINDOW_HEIGHT, sf::Color::Magenta));
	boxes.push_back(l1.createGround(WINDOW_WIDTH-10, 300, 100, WINDOW_WIDTH, sf::Color::Magenta));
	boxes.push_back(l1.createGround(450, 300, 175, 20, sf::Color::Magenta));*/
	
	//auto&& circi = l1.createcircle(440,440,20, 1.f, 0.7f, sf::Color::Red);
	//circles.push_back(circi);
	

	while (w.isOpen())
	{
		sf::Event evnt;

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
		//world.Step(1 / 60.f, 6, 3);
		
		// Render everything
		
		//l1.render(w, boxes);
		l1.fixedpart(w);
		l1.rectangledraw(w, 1200, 400, 80, 80, 1.f, 0.7f, sf::Color::Red);
		l1.drawcircle(w,1200,200,40,1.f,0.7f,sf::Color::Red);
		l1.drawcircle(w,1200,300,40,1.f,0.7f,sf::Color::Red);
		
		//l1.rendercircle(w, circles);
		w.display();


	}

	return 0;
}