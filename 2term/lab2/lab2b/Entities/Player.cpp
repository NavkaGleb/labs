#include "Player.hpp"

// constructor / destructor

Player::Player(const int& hp, const unsigned& bamount, const float& x) {

	this->hp = hp;
	this->mhp = hp;
	this->bamount = bamount;
	this->mbamount = bamount;
	this->bstrength = 5;
	this->type = "player";

	this->btime = 0.f;
	this->mbtime = 10.f;

	float width = 50.f;
	float height = 50.f;

	this->rect.setSize(sf::Vector2f(width, height));
	this->rect.setFillColor(sf::Color::Blue);
	this->rect.setPosition(x - width / 2.f, 800.f);

	this->initMovementComponent(200.f, 1500.f, 500.f);
	this->initAnimationComponent();

	this->initHpBar(this->mhp, sf::Color(200, 0, 0, 200), 55.f, 10.f, 90.f, 5.f);
	this->bulletBar = new gui::Bar(static_cast<int>(this->mbamount), sf::Color::Blue, 55.f, 25.f, 90.f, 5.f);

}

Player::~Player() {

	delete this->movementComponent;

}

// accessors

bool Player::getBtime() {

	if (this->btime > this->mbtime) {

		this->btime = 0.f;
		return true;

	}

	return false;

}

// methods

void Player::addBamount(const int& amount) {

	this->bamount += amount;

	if (this->bamount > this->mbamount)
		this->bamount = this->mbamount;

}

void Player::addHp(const int& hp) {

	this->hp += hp;

	if (this->hp > this->mhp)
		this->hp = this->mhp;

}

void Player::move(const float dirX, const float dirY, const float& deltatime) {

	this->movementComponent->move(dirX, dirY, deltatime);

}

void Player::attack() {

	if (this->bamount > 0) {

		this->bullets.emplace_back(new Bullet(
			7.f, 350.f, 300, this->bstrength, -1.f, this->rect.getFillColor(),
			this->rect.getPosition().x + this->rect.getGlobalBounds().width / 2.f,
			this->rect.getPosition().y
		));

		this->bamount--;

	}

}

void Player::updateBtime(const float& deltatime) {

	if (this->bamount == 0 && this->btime < this->mbtime)
		this->btime += 6.f * deltatime;

	if (this->bamount == 0 && this->getBtime())
		this->bamount++;

}

void Player::updateBullets(const float& deltatime) {

	for (auto& bullet : this->bullets)
		bullet->update(deltatime);

}

void Player::update(const float& deltatime) {

	this->updateBtime(deltatime);

	this->movementComponent->update(deltatime);
	this->animationComponent->update(deltatime);

	this->updateBullets(deltatime);

	this->hpBar->update(this->hp);
	this->bulletBar->update(static_cast<int>(this->bamount));

}

void Player::renderGui(sf::RenderTarget& target) {

	this->hpBar->render(target);
	this->bulletBar->render(target);

}

void Player::render(sf::RenderTarget& target) {

	target.draw(this->rect);

	for (auto& bullet : this->bullets)
		bullet->render(target);

}