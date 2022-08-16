#include "Game.h"

Game::Game(const std::string& config) {
	init(config);
}

void Game::init(const std::string& path) {
	std::string id;
	std::ifstream fin(path);
	srand(time(NULL));

	while (fin >> id) {

		if (id == "Window") {
			fin >> m_windowConfig.W >> m_windowConfig.H >> m_windowConfig.FL >> m_windowConfig.FS;
		}
		else if (id == "Font") {
			fin >> m_fontConfig.F >> m_fontConfig.S >> m_fontConfig.R >> m_fontConfig.G >> m_fontConfig.B;
		}
		else if (id == "Player") {
			fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR
				>> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;
		}
		else if (id == "Enemy") {
			fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB
				>> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;
		}
		else if (id == "Bullet") {
			fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR
				>> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
		}
	}

	//set up default window parameters
	m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), "Assignment 2");
	m_window.setFramerateLimit(m_windowConfig.FL);

	if (!m_font.loadFromFile(m_fontConfig.F)) {

		// if we cant load the font prnt an error to the console and exit
		std::cerr << "Could not laod the font" << std::endl;
		exit(-1);
	}

	m_text.setString("Score: " + std::to_string(m_score));
	m_text.setFont(m_font);
	m_text.setCharacterSize(m_fontConfig.S);
	m_text.setFillColor(sf::Color(m_fontConfig.R, m_fontConfig.G, m_fontConfig.B));
	m_text.setPosition(sf::Vector2f(0,0));


	spawnPlayer();
}

void Game::run() {
	while (m_running) {

		if (m_paused) {
			sUserInput();
			sRender();
		}
		else {
			m_entities.update();

			sLifespan();
			sEnemySpawner();
			sMovemet();
			sCollision();
			sUserInput();
			sRender();

			//incerement the frame count
			m_currentFrame++;
		}
	}
}

void Game::setPaused(bool paused) {
	m_paused = paused;
}

void Game::spawnPlayer() {
	//create evry entity by calling entityManager.addEntity(tag)
	auto entity = m_entities.addEntity("player");

	//give this entity his values
	entity->cTransform = std::make_shared<Ctransform>(Vec2((float)m_windowConfig.W / 2, (float)m_windowConfig.H / 2), Vec2(m_playerConfig.S, m_playerConfig.S), 0.0f);
	entity->cShape = std::make_shared<Cshape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
		sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
	entity->cInput = std::make_shared<CInput>();
	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

	//since we want this entity to be our player, set our game player variable to it
	m_player = entity;
}

void Game::spawnEnemy() {

	//create evry entity by calling entityManager.addEntity(tag)
	auto entity = m_entities.addEntity("enemy");

	//generate random position
	float x = (float)m_enemyConfig.CR + (rand() % (1 + (m_windowConfig.W - m_enemyConfig.CR) - m_enemyConfig.CR));
	float y = (float)m_enemyConfig.CR + (rand() % (1 + (m_windowConfig.H - m_enemyConfig.CR) - m_enemyConfig.CR));

	//generate random vertices
	float v = (float)m_enemyConfig.VMIN + (rand() % (1 + m_enemyConfig.VMAX - m_enemyConfig.VMIN));

	//generate random velocity
	float s = (float)m_enemyConfig.SMIN + (rand() % (int)(1 + m_enemyConfig.SMAX - m_enemyConfig.SMIN));

	//generate fill color
	sf::Uint8 cr = rand() % 255;
	sf::Uint8 cg = rand() % 255;
	sf::Uint8 cb = rand() % 255;

	//give this entity his values
	entity->cTransform = std::make_shared<Ctransform>(Vec2(x,y), Vec2(s,s), 0.0f);
	entity->cShape = std::make_shared<Cshape>(m_enemyConfig.SR, v, sf::Color(cr,cg,cb), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
	entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
	entity->cScore = std::make_shared<CScore>(v*100);

	// record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(ptr<Entity> e) {
	for (size_t i = 0; i < e->cShape->circle.getPointCount(); i++)
	{
		//create evry entity by calling entityManager.addEntity(tag)
		auto entity = m_entities.addEntity("smallenemy");
		float angle = (float)((((360 / e->cShape->circle.getPointCount()) * i) * 3.14159f) / 180);

		entity->cTransform = std::make_shared<Ctransform>(e->cTransform->pos, Vec2(cos(angle), sin(angle)), 0);
		entity->cShape = std::make_shared<Cshape>((e->cShape->circle.getRadius()/2), e->cShape->circle.getPointCount(), e->cShape->circle.getFillColor(),
			e->cShape->circle.getOutlineColor(), e->cShape->circle.getOutlineThickness());
		entity->cCollision = std::make_shared<CCollision>(e->cCollision->radious);
		entity->cScore = std::make_shared<CScore>(e->cScore->score * 2);
		entity->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
	}
}

void Game::spawnBullet(ptr<Entity> e, const sf::Vector2i& target) {

	//create evry entity by calling entityManager.addEntity(tag)
	auto entity = m_entities.addEntity("bullet");

	//calculate distance
	Vec2 mouse((float)target.x, (float)target.y);
	Vec2 distance = mouse - e->cTransform->pos;

	//normalize distance
	distance.normalize();

	//calculate velocity
	Vec2 velocity = distance * m_bulletConfig.S;

	//give this entity his values
	entity->cTransform = std::make_shared<Ctransform>(e->cTransform->pos, velocity, 0.0f);
	entity->cShape = std::make_shared<Cshape>(m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
		sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
	entity->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
	entity->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
}

void Game::spawnMark(const Vec2& position) {
	//create evry entity by calling entityManager.addEntity(tag)
	auto entity = m_entities.addEntity("mark");

	//give this entity his values
	entity->cTransform = std::make_shared<Ctransform>(position, Vec2(0.0f,0.0f), 0.0f);
	entity->cShape = std::make_shared<Cshape>(1, 4, sf::Color(255,255,255),sf::Color(255,255,255), 1);
}

void Game::destroyMark() {
	for (auto& e : m_entities.getEntities("mark")) {
		e->destroy();
	}
}

void Game::spawArrivalBullets(const ptr<Entity> e) {
	for (size_t i = 0; i < e->cShape->circle.getPointCount(); i++)
	{
		auto entity = m_entities.addEntity("bullet");
		float angle = (float)((((360 / e->cShape->circle.getPointCount()) * i) * 3.14159f) / 180);

		entity->cTransform = std::make_shared<Ctransform>(e->cTransform->pos, Vec2(cos(angle) * m_bulletConfig.S, sin(angle) * m_bulletConfig.S), 0);
		entity->cShape = std::make_shared<Cshape>(m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
			sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
		entity->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
		entity->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
	}
}

void Game::spawnSpecialWeapon(ptr<Entity> e) {
	if (!m_specialPower) {
		m_specialPower = true;
		m_previousPosition = e->cTransform->pos;
		spawnMark(m_previousPosition);
	}
	else {
		m_specialPower = false;
		e->cTransform->pos = m_previousPosition;
		spawArrivalBullets(e);
		destroyMark();
	}
}

void Game::sMovemet() {

	Vec2 playerSpeed;
	if (m_player->cInput->left) {
		playerSpeed.x -= 5.0f;
	}
	if (m_player->cInput->right) {
		playerSpeed.x += 5.0f;
	}
	if (m_player->cInput->up) {
		playerSpeed.y -= 5.0f;
	}
	if (m_player->cInput->down) {
		playerSpeed.y += 5.0f;
	}
	m_player->cTransform->speed = playerSpeed;


	for (auto& e : m_entities.getEntities()) {
		if (e->tag() != "player") {
			if (e->cTransform->pos.x - e->cShape->circle.getRadius() < 0 || (e->cTransform->pos.x + e->cShape->circle.getRadius() > 1280)) {
				e->cTransform->speed.x *= -1.0f;
			}
			else if (e->cTransform->pos.y - e->cShape->circle.getRadius() < 0 || (e->cTransform->pos.y + e->cShape->circle.getRadius() > 720)) {
				e->cTransform->speed.y *= -1.0f;
			}
		}
		else {
			if (e->cTransform->pos.x - e->cShape->circle.getRadius() < 0) {
				e->cTransform->pos = Vec2(0.1f + e->cShape->circle.getRadius(), e->cTransform->pos.y);
			}
			else if (e->cTransform->pos.x + e->cShape->circle.getRadius() > 1280) {
				e->cTransform->pos = Vec2(1279.0f - e->cShape->circle.getRadius(), e->cTransform->pos.y);
			}
			else if (e->cTransform->pos.y - e->cShape->circle.getRadius() < 0) {
				e->cTransform->pos = Vec2(e->cTransform->pos.x, 0.1f + e->cShape->circle.getRadius());
			}
			else if (e->cTransform->pos.y + e->cShape->circle.getRadius() > 720) {
				e->cTransform->pos = Vec2(e->cTransform->pos.x, 719.0f - e->cShape->circle.getRadius());
			}
		}
		e->cTransform->pos += e->cTransform->speed;
	}
}

void Game::sLifespan() {
	for (auto& e : m_entities.getEntities()) {
		if (e->cLifespan) {
			if (e->cLifespan->remaining > 0) {
				e->cLifespan->remaining--;
				sf::Color cf = e->cShape->circle.getFillColor();
				sf::Color co = e->cShape->circle.getOutlineColor();
				cf.a = (sf::Uint8)(((float)e->cLifespan->remaining / (float)e->cLifespan->total)*255);
				co.a = (sf::Uint8)(((float)e->cLifespan->remaining / (float)e->cLifespan->total)*255);
				e->cShape->circle.setFillColor(cf);
				e->cShape->circle.setOutlineColor(co);
			}
			else {
				e->destroy();
			}
		}
	}
}

void Game::sCollision() {
	float distance = 0.0f;

	if (m_entities.verifyTag("enemy") != NULL) {
		for (auto& e : m_entities.getEntities("enemy")) {
			// collision player enemy
			distance = m_player->cTransform->pos.dist(e->cTransform->pos);
			if (distance < m_player->cCollision->radious + e->cCollision->radious) {
				m_player->cTransform->pos = Vec2((float)m_windowConfig.W / 2, (float)m_windowConfig.H / 2);
				spawnSmallEnemies(e);
				e->destroy();
			}
		}
	}

	if (m_entities.verifyTag("smallenemy") != NULL) {
		for (auto& e : m_entities.getEntities("smallenemy")) {
			// collision player small enemy
			distance = m_player->cTransform->pos.dist(e->cTransform->pos);
			if (distance < m_player->cCollision->radious + e->cCollision->radious) {
				m_player->cTransform->pos = Vec2((float)m_windowConfig.W / 2, (float)m_windowConfig.H / 2);
				e->destroy();
			}
		}
	}

	if (m_entities.verifyTag("enemy") != NULL && m_entities.verifyTag("bullet") != NULL) {
		for (auto& e : m_entities.getEntities("enemy")) {
			for (auto& b : m_entities.getEntities("bullet")) {
				// collision bullet enemy
				distance = b->cTransform->pos.dist(e->cTransform->pos);
				if (distance < e->cCollision->radious + b->cCollision->radious) {
					spawnSmallEnemies(e);
					m_score += e->cScore->score;
					m_text.setString("Score: " + std::to_string(m_score));
					b->destroy();
					e->destroy();
				}
			}
		}
	}

	if (m_entities.verifyTag("smallenemy") != NULL && m_entities.verifyTag("bullet") != NULL) {
		for (auto& e : m_entities.getEntities("smallenemy")) {
			for (auto& b : m_entities.getEntities("bullet")) {
				// collision bullet enemy
				distance = b->cTransform->pos.dist(e->cTransform->pos);
				if (distance < e->cCollision->radious + b->cCollision->radious) {
					m_score += e->cScore->score;
					m_text.setString("Score: " + std::to_string(m_score));
					b->destroy();
					e->destroy();
				}
			}
		}
	}
}

void Game::sEnemySpawner() {
	if (m_currentFrame - m_lastEnemySpawnTime > m_enemyConfig.SI) {
		spawnEnemy();
	}
}

void Game::sRender() {
	m_window.clear();

	for (auto& e : m_entities.getEntities()) {
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);
		m_window.draw(e->cShape->circle);
	}

	m_window.draw(m_text);
	m_window.display();
}

void Game::sUserInput() {

	sf::Event event;
	while (m_window.pollEvent(event)) {

		//this event triggers if window is closed
		if (event.type == sf::Event::Closed) {
			m_running = false;
		}

		//this event triggers if a key is pressed
		if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
			case sf::Keyboard::W:
				//Set player input components to true
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::A:
				//Set player input components to true
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::S:
				//Set player input components to true
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::D:
				//Set player input components to true
				m_player->cInput->right = true;
				break;
			case sf::Keyboard::P:
				//Set paused flag to true
				if (m_paused) {
					m_paused = false;
				}
				else {
					m_paused = true;
				}
				break;
			case sf::Keyboard::Escape:
				//Exit application
				std::exit(0);
				break;
			}
		}

		//this event triggers if key is released
		if (event.type == sf::Event::KeyReleased) {
			switch (event.key.code) {
			case sf::Keyboard::W:
				//Set player input components to false
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::A:
				//Set player input components to false
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::S:
				//Set player input components to false
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::D:
				//Set player input components to false
				m_player->cInput->right = false;
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				spawnBullet(m_player, sf::Mouse::getPosition(m_window));
			}

			if (event.mouseButton.button == sf::Mouse::Right) {
				spawnSpecialWeapon(m_player);
			}
		}
	}
}