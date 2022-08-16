#pragma once

#include "Common.h"
#include "Entity.h"
#include "EntityManager.h"

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float	SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };
struct WindowConfig { int W, H, FL, FS; };
struct FontConfig { int S, R, G, B; std::string F = ""; };


class Game {
	sf::RenderWindow m_window;
	EntityManager m_entities;
	sf::Font m_font;
	sf::Text m_text;
	PlayerConfig m_playerConfig;
	EnemyConfig m_enemyConfig;
	BulletConfig m_bulletConfig;
	WindowConfig m_windowConfig;
	FontConfig m_fontConfig;
	int m_score = 0;
	int m_currentFrame = 0;
	int m_lastEnemySpawnTime = 0;
	bool m_paused = false;
	bool m_running = true;

	ptr<Entity> m_player;

	Vec2 m_previousPosition;
	bool m_specialPower = false;


	void init(const std::string& config);
	void setPaused(bool paused);

	void sMovemet();			// system: Entity position/movement update
	void sUserInput();			// system: user input
	void sLifespan();			// system: lifespan
	void sRender();				// system: render/draw
	void sEnemySpawner();		// system: spawn enemys
	void sCollision();			// system: collisions

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(ptr<Entity> entity);
	void spawnBullet(ptr<Entity> entity, const sf::Vector2i& mousePos);
	void spawnSpecialWeapon(ptr<Entity> entity);
	void spawnMark(const Vec2& position);
	void destroyMark();
	void spawArrivalBullets(const ptr<Entity> e);

public:
	Game(const std::string& config);
	void run();
};