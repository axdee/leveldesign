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
	circle.m_p(radius / 2 / PPM, radius / 2 / PPM);
	circle.m_radius(radius);

	// Fixture definition
	b2FixtureDef fixtureDef;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.shape = &circle;

	// Now we have a body for our Box object
	b2Body* boxBody = world.CreateBody(&boxBodyDef);
	// Lastly, assign the fixture
	boxBody->CreateFixture(&fixtureDef);

	return Circle{ radius, color, circle };
}

