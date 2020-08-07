#include "Map.hpp"

// private methods

void Map::initEntities(const unsigned& eamount) {

	float startY = this->area.getPosition().y;

	for (unsigned i = 0; i < eamount; i++) {

		if (i % 7 != 1) {

			this->entities.emplace_back(new Enemy(
				this->uirand(this->mt) % 2000 + 500, 5, this->frand(this->mt), this->frand(this->mt) + 30.f,
				this->centerX, startY + static_cast<float>(i) * 65.f + 20.f
			));

		} else {

			this->entities.emplace_back(new Box(
				this->uirand(this->mt) % 2000 + 500, this->uirand(this->mt) % 3 + 1, this->frand(this->mt),
				this->centerX, startY + static_cast<float>(i) * 65.f + 20.f
			));

		}

	}

}

// constructor / destructor

Map::Map(const StateData& sdata, const unsigned& eamount, const float& width, const float& height)
	: mt(std::clock()) {

	this->uirand = std::uniform_int_distribution<int>(0, 10000);
	this->frand = std::uniform_real_distribution<float>(20, 45);
	this->centerX = static_cast<float>(sdata.videoMode.width) / 2.f;

	this->area.setSize(sf::Vector2f(width, height));
	this->area.setPosition(
		centerX - width / 2.f,
		static_cast<float>(sdata.videoMode.height) - height
	);
	this->area.setFillColor(sf::Color(255, 255, 255, 30));

	this->initEntities(eamount);

}

Map::~Map() {

	for (auto& entity : this->entities)
		delete entity;

}

// public methods

bool Map::updatePlayerCollision(Player* player) const {

	// local variables
	Bullet* cbullet;															// current bullet

	sf::FloatRect pbounds = player->getGlobalBounds();							// player bounds
	sf::FloatRect bbounds;														// cbullet bounds

	sf::Vector2f ppos = player->getPosition();									// player position
	sf::Vector2f bpos;															// cbullet position

	// update collision with entities
	for (auto& entity : this->entities) {

		if (this->entities[this->entities.size() - 1]->getPosition().y < ppos.y &&
			this->entities[0]->getPosition().y > ppos.y + pbounds.height)
			break;

		if (entity->getGlobalBounds().intersects(pbounds)) {

			player->loseHp(entity->getDamage());
			player->takeDamage();

			if (player->getHp() <= 0)
				return false;

		}

	}

	// update collision with entity bullets
	for (auto& entity : this->entities) {

		cbullet = entity->getBullet();

		if (cbullet && player->getGlobalBounds().intersects(cbullet->getGlobalBounds())) {

			// init local variables
			bpos = cbullet->getPosition();
			bbounds = cbullet->getGlobalBounds();

			// update player
			player->loseHp(entity->getBullet()->getDamage());
			player->takeDamage();

			// delete cbullet
			entity->deleteBullet();

			// end of the game
			if (player->getHp() <= 0)
				return false;

		}

		if (cbullet && bpos.y + bbounds.height >= this->area.getPosition().y + this->area.getGlobalBounds().height)
			entity->deleteBullet();

	}

	return true;

}

bool Map::updateCollision(const float& deltatime, Player* player) {

	// local variables
	static std::map<int, std::vector<Bullet*>> collisions;				// save which button hit which entity

	sf::FloatRect abounds = this->area.getGlobalBounds();					// area bounds
	sf::FloatRect pbounds = player->getGlobalBounds();						// player bounds
	sf::FloatRect ebounds;													// entity bounds
	sf::FloatRect bbounds; 													// bullet bounds

	sf::Vector2f apos = this->area.getPosition();							// area position
	sf::Vector2f ppos = player->getPosition();								// player position
	sf::Vector2f epos;														// entity position
	sf::Vector2f bpos;														// bullet position

	Entity* centity;														// current entity
	Bullet* cbullet;														// current bullet

	std::vector<Bullet*>& pbullets = player->bullets;						// player bullets

	bool dentity = false;													// tracks entity death
	bool dbullet = false;													// tracks bullet death

	// update player collision
	if (ppos.y + pbounds.height >= apos.y + abounds.height) {

		player->stopVelocityY();
		player->setPositionY(apos.y + abounds.height - pbounds.height);

	}

	if (ppos.y <= apos.y) {

		player->stopVelocityY();
		player->setPositionY(apos.y);

	}

	// check player and extra lives collision
	for (int i = 0; i < this->elives.size(); i++) {

		if (player->intersects(this->elives[i]->getGlobalBounds())) {

			player->addHp(this->elives[i]->getHp());

			delete this->elives[i];
			this->elives.erase(this->elives.begin() + i);
			i--;

		}

	}

	// check player bullets and entity bullets collision
	for (int i = 0; i < pbullets.size(); i++) {

		bbounds = pbullets[i]->getGlobalBounds();

		for (auto& entity : this->entities) {

			if (entity->getBullet() &&
				entity->getBullet()->getGlobalBounds().intersects(bbounds)) {

				entity->deleteBullet();
				pbullets[i]->loseStrength();

				if (pbullets[i]->getStrength() <= 0) {

					delete pbullets[i];
					pbullets.erase(pbullets.begin() + i);
					i--;
					break;

				}

			}

			if (entity->getBullet() &&
				entity->getBullet()->getPosition().y + entity->getBullet()->getGlobalBounds().height >=
				this->area.getPosition().y + this->area.getGlobalBounds().height)
				entity->deleteBullet();

		}

	}

	// check player bullets and entities collision
	for (int j = 0; j < pbullets.size(); j++) 	{

		cbullet = pbullets[j];
		bbounds = pbullets[j]->getGlobalBounds();
		bpos = pbullets[j]->getPosition();

		for (int i = 0; i < this->entities.size(); i++) {

			centity = this->entities[i];

			ebounds = centity->getGlobalBounds();
			epos = centity->getPosition();

			// check bullet and entity collision
			if (centity->intersects(bbounds) && !cbullet->getHit()) {

				// update entity.hp
				centity->loseHp(cbullet->getDamage());
				centity->takeDamage();

				// update bullet.strength and bullet.hit
				cbullet->loseStrength();
				cbullet->setHit(true);

				// delete bullet
				if (cbullet->getStrength() == 0) {

					delete cbullet;
					pbullets.erase(pbullets.begin() + j);
					j--;
					dbullet = true;

				} else {

					collisions[i].emplace_back(cbullet);

				}

				// delete entity
				if (centity->getHp() <= 0) {

					for (auto& bullet : collisions[i])
						bullet->setHit(false);
					collisions[i].clear();

					if (centity->getType() == "box") {

						this->tfxrand = std::uniform_real_distribution<float>(epos.x, epos.x + ebounds.width);
						this->tfyrand = std::uniform_real_distribution<float>(epos.y, epos.y + ebounds.height);

						for (unsigned k = 0; k < centity->getBamount(); k++)
							this->elives.emplace_back(new LifeCircle(
								this->uirand(this->mt) % 50 + 25, 10.f,
								this->tfxrand(this->mt),
								this->tfyrand(this->mt)
							));

					} else if (centity->getType() == "enemy") {

						player->addBamount(2);

					}

					std::cout << "centity hp = " << centity->getHp() << std::endl;

					delete centity;
					this->entities.erase(this->entities.begin() + i);
					i--;
					dentity = true;

					std::cout << "entity die" << std::endl;


				}

			}

			// reset bullet.hit
			if (dentity) {

				collisions[i + 1].clear();

			} else {

				for (int k = 0; k < collisions[i].size(); k++) {

					if (epos.y > collisions[i][k]->getPosition().y + collisions[i][k]->getGlobalBounds().height &&
						collisions[i][k]->getHit()) {

						collisions[i][k]->setHit(false);
						collisions[i].erase(collisions[i].begin() + k);
						k--;

					}

				}

			}

			dentity = false;

		}

		// delete pullet in the end of map
		if (bpos.y <= apos.y && !dbullet) {

			pbullets.erase(pbullets.begin() + j);
			delete cbullet;
			j--;

		}

		dbullet = false;

	}

	return !this->entities.empty();

}

void Map::update(const float& deltatime) {

	for (auto& entity : this->entities)
		entity->update(deltatime);

}

void Map::render(sf::RenderTarget& target) {

	target.draw(this->area);

	for (auto& entity : this->entities)
		entity->render(target);

	for (auto& lcircle : this->elives)
		lcircle->render(target);

}