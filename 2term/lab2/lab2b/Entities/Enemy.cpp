#include "Enemy.hpp"

// constructor / destructor

Enemy::Enemy(const int& hp, const int& damage, const float& size,
	const float& btimeMax, const float& x, const float& y) {

	this->hp = hp;
	this->mhp = hp;
	this->damage = damage;
	this->btime = btimeMax;
	this->btimeMax = btimeMax;
	this->bamount = 1;
	this->type = "enemy";

	this->rect.setSize(sf::Vector2f(size, size));
	this->rect.setFillColor(sf::Color::Red);
	this->rect.setPosition(x - size / 2.f, y);
//	this->rect.setOutlineThickness(-2.f);
//	this->rect.setOutlineColor(sf::Color::White);

	this->initAnimationComponent();

	this->initHpBar(this->mhp, sf::Color(200, 200, 200, 200), x, y - 9.f, size, 5.f);

}

Enemy::~Enemy() {

	delete this->bullet;

}

// accessors

bool Enemy::getBtime() {

	if (this->btime >= this->btimeMax) {

		this->btime = 0.f;
		this->bamount++;
		return true;

	}

	return false;

}

// methods

void Enemy::attack() {

	this->bullet = new Bullet(
		6.f, 60.f, 100, 1, 1.f, this->rect.getFillColor(),
		this->rect.getPosition().x + this->rect.getGlobalBounds().width / 2.f,
		this->rect.getPosition().y
	);

	this->bamount--;

}

void Enemy::updateBtime(const float& deltatime) {

	if (this->btime < this->btimeMax)
		this->btime += 40.f * deltatime;

}

void Enemy::update(const float& deltatime) {

	this->updateBtime(deltatime);

	if (this->getBtime() && !this->bullet)
		this->attack();

	this->animationComponent->update(deltatime);
	this->hpBar->update(this->hp);

	if (this->bullet)
		this->bullet->update(deltatime);

}

void Enemy::render(sf::RenderTarget& target) {

	target.draw(this->rect);

	this->hpBar->render(target);

	if (this->bullet)
		this->bullet->render(target);

}