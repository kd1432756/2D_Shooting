#pragma	once

class BaseObject
{
public:
	BaseObject() {}
	virtual ~BaseObject() {}

	void Init();
	void Update();
	void Draw();
	void Release();

	void SetPosition(Math::Vector2& pos) { m_pos = pos; }
	void SetAngle(float angle) { m_angleRad = angle; }
	void SetTexture(KdTexture* tex) { m_tex = tex; }
	void SetActive(bool active) { m_isActive = active; }
	void SetAlive(bool alive) { m_isAlive = alive; }
	void SetHP(int hp) { m_hp = hp; }

	bool IsActive() { return m_isActive; }
	bool IsAlive() { return m_isAlive; }
	Math::Vector2 GetPos() { return m_pos; }

protected:

	KdTexture* m_tex;
	bool m_isActive = false;
	bool m_isAlive = false;
	Math::Vector2 m_pos;
	float m_angleRad = 0.0f;	
	int m_hp = 1;
};